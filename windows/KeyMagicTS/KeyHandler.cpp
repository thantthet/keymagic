//////////////////////////////////////////////////////////////////////
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
//  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//  PARTICULAR PURPOSE.
//
//  Copyright (C) 2003  Microsoft Corporation.  All rights reserved.
//
//  KeyHandler.cpp
//
//          the handler routines for key events
//
//////////////////////////////////////////////////////////////////////

#include "Globals.h"
#include "EditSession.h"
#include "TextService.h"
#include "KeyMagicUtil.h"

//+---------------------------------------------------------------------------
//
// CKeyHandlerEditSession
//
//----------------------------------------------------------------------------

class CKeyHandlerEditSession : public CEditSessionBase
{
public:
    CKeyHandlerEditSession(CTextService *pTextService, ITfContext *pContext, WPARAM wParam) : CEditSessionBase(pTextService, pContext)
    {
        _wParam = wParam;
    }

    // ITfEditSession
    STDMETHODIMP DoEditSession(TfEditCookie ec);

private:
    WPARAM _wParam;
};

//+---------------------------------------------------------------------------
//
// DoEditSession
//
//----------------------------------------------------------------------------

STDAPI CKeyHandlerEditSession::DoEditSession(TfEditCookie ec)
{
	UINT vk, _char, modifier = 0;
	MapVK(_wParam, &vk, &_char);
	if (GetKeyState(VK_CONTROL) & 0x80) {
		modifier |= KeyMagicEngine::CTRL_MASK;
	}
	if (GetKeyState(VK_SHIFT) & 0x80) {
		modifier |= KeyMagicEngine::SHIFT_MASK;
	}
	if (GetKeyState(VK_MENU) & 0x80) {
		modifier |= KeyMagicEngine::ALT_MASK;
	}
	if (GetKeyState(VK_CAPITAL) & 0x81) {
		modifier |= KeyMagicEngine::CAPS_MASK;
	}

	if (_pTextService->_ProcessKeyEvent(ec, _pContext, _char, vk, modifier) == S_FALSE) {

		switch (_wParam)
		{
		case VK_CONTROL:
		case VK_MENU:
		case VK_SHIFT:
		case VK_CAPITAL:
			break;
		default:
			_pTextService->_TerminateComposition(ec, _pContext);
			
			INPUT ip;

			ip.type = INPUT_KEYBOARD;
			ip.ki.wScan = 255;
			ip.ki.dwExtraInfo = 0;
			ip.ki.dwFlags = 0;
			ip.ki.time = 0;
			ip.ki.wVk = _wParam;

			SendInput(1, &ip, sizeof(INPUT));
		break;
		}
	}

    /*switch (_wParam)
    {
        case VK_LEFT:
		case VK_RIGHT:
		case VK_UP:
		case VK_DOWN:
			_pTextService->_TerminateComposition(ec, _pContext);
			return S_OK;

        case VK_RETURN:
            return _pTextService->_HandleReturnKey(ec, _pContext);

        default:
			return _pTextService->_ProcessKeyEvent(ec, _pContext, _char, vk, modifier);
            break;
    }*/

    return S_OK;

}

//+---------------------------------------------------------------------------
//
// IsRangeCovered
//
// Returns TRUE if pRangeTest is entirely contained within pRangeCover.
//
//----------------------------------------------------------------------------

BOOL IsRangeCovered(TfEditCookie ec, ITfRange *pRangeTest, ITfRange *pRangeCover)
{
    LONG lResult;

    if (pRangeCover->CompareStart(ec, pRangeTest, TF_ANCHOR_START, &lResult) != S_OK ||
        lResult > 0)
    {
        return FALSE;
    }

    if (pRangeCover->CompareEnd(ec, pRangeTest, TF_ANCHOR_END, &lResult) != S_OK ||
        lResult < 0)
    {
        return FALSE;
    }

    return TRUE;
}

void sendBackspace(int count)
{
	if (!GetFocus() || count < 1)
		return ;

	INPUT ip;
	ip.type = INPUT_KEYBOARD;
	ip.ki.dwExtraInfo = 0;
	ip.ki.time = 0;

	for(int i=0; i < count; i++){
		
		ip.ki.wScan = 255;
		ip.ki.dwFlags = 0;
		ip.ki.wVk = VK_BACK;
		SendInput(1, &ip, sizeof(INPUT));

		ip.ki.wScan = 0;
		ip.ki.dwFlags = KEYEVENTF_KEYUP;
		ip.ki.wVk = VK_BACK;
		SendInput(1, &ip, sizeof(INPUT));
	}
}

void sendSingleKey(WORD wVk, DWORD dwFlags)
{
	INPUT ip;

	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0;
	ip.ki.dwExtraInfo = 0;
	ip.ki.dwFlags = dwFlags;
	ip.ki.time = 0;
	ip.ki.wVk = wVk;

	SendInput(1, &ip, sizeof(INPUT));
}

int sendKeyStrokes (const std::wstring& s)
{
	int cInputs = s.length() * 2; // need twice for up and down

	INPUT * ip = new INPUT[cInputs];

	for(int i=0, ii=0; i < s.length(); i++, ii++){
		ip[ii].type = INPUT_KEYBOARD;
		ip[ii].ki.dwExtraInfo = 255;
		ip[ii].ki.dwFlags = KEYEVENTF_UNICODE;
		ip[ii].ki.time = 0;
		ip[ii].ki.wScan = s.at(i);
		ip[ii].ki.wVk = 0;

		ii++;

		ip[ii].type = INPUT_KEYBOARD;
		ip[ii].ki.dwExtraInfo = 0;
		ip[ii].ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_UNICODE;
		ip[ii].ki.time = 0;
		ip[ii].ki.wScan = s.at(i);
		ip[ii].ki.wVk = 0;
	}

	int cSent = SendInput(cInputs, ip, sizeof(INPUT));

	return cSent;
}

HRESULT CTextService::_ProcessKeyEvent(TfEditCookie ec, ITfContext *pContext, UINT keyval, UINT keycode, UINT modifier)
{
	ITfRange *pRangeComposition;
    TF_SELECTION tfSelection;
    ULONG cFetched;
    BOOL fCovered;
	BOOL fSuccess;

    // Start the new compositon if there is no composition.
    if (!_IsComposing())
        _StartComposition(pContext);

	byte keyboardState[256] = {0};

	_engine.setKeyStates(keyboardState);

	GetKeyboardState(keyboardState);
	
	int modifierList[] = {VK_CONTROL, VK_MENU, VK_SHIFT, VK_LCONTROL, VK_RCONTROL, VK_LMENU, VK_RMENU, VK_LSHIFT, VK_RSHIFT, NULL};

	for (int i = 0; modifierList[i] != NULL; i++) {
		int key = modifierList[i];
		_engine.setKeyState(key, keyboardState[key]);
	}

	KeyMagicString before = _engine.getContextText()->c_str();

	fSuccess = _engine.processKeyEvent(keyval, keycode, modifier);

	// first, test where a keystroke would go in the document if an insert is done
    if (pContext->GetSelection(ec, TF_DEFAULT_SELECTION, 1, &tfSelection, &cFetched) != S_OK || cFetched != 1)
        return S_FALSE;

    // is the insertion point covered by a composition?
    if (_pComposition->GetRange(&pRangeComposition) == S_OK)
    {
        fCovered = IsRangeCovered(ec, tfSelection.range, pRangeComposition);

        pRangeComposition->Release();

        if (!fCovered)
        {
            goto Exit;
        }
    }

    // insert the text
    // use SetText here instead of InsertTextAtSelection because a composition was already started
    //Don't allow to the app to adjust the insertion point inside the composition
	KeyMagicString * context = _engine.getContextText();

    if (tfSelection.range->SetText(ec, 0, context->c_str(), context->length()) != S_OK)
        goto Exit;
    //
    // set the display attribute to the composition range.
    //
    _SetCompositionDisplayAttributes(ec, pContext, _gaDisplayAttributeInput);

Exit:
    tfSelection.range->Release();
	return fSuccess ? S_OK : S_FALSE;
}

//+---------------------------------------------------------------------------
//
// _HandleCharacterKey
//
// If the keystroke happens within a composition, eat the key and return S_OK.
//
//----------------------------------------------------------------------------

HRESULT CTextService::_HandleCharacterKey(TfEditCookie ec, ITfContext *pContext, WPARAM wParam)
{
    ITfRange *pRangeComposition;
    TF_SELECTION tfSelection;
    ULONG cFetched;
    WCHAR ch;
    BOOL fCovered;

    // Start the new compositon if there is no composition.
    if (!_IsComposing())
        _StartComposition(pContext);

    //
    // Assign VK_ value to the char. So the inserted the character is always
    // uppercase.
    //
    ch = (WCHAR)wParam;

    // first, test where a keystroke would go in the document if an insert is done
    if (pContext->GetSelection(ec, TF_DEFAULT_SELECTION, 1, &tfSelection, &cFetched) != S_OK || cFetched != 1)
        return S_FALSE;

    // is the insertion point covered by a composition?
    if (_pComposition->GetRange(&pRangeComposition) == S_OK)
    {
        fCovered = IsRangeCovered(ec, tfSelection.range, pRangeComposition);

        pRangeComposition->Release();

        if (!fCovered)
        {
            goto Exit;
        }
    }

    // insert the text
    // use SetText here instead of InsertTextAtSelection because a composition was already started
    //Don't allow to the app to adjust the insertion point inside the composition
    if (tfSelection.range->SetText(ec, 0, &ch, 1) != S_OK)
        goto Exit;

    // update the selection, make it an insertion point just past
    // the inserted text.
//    tfSelection.range->Collapse(ec, TF_ANCHOR_END);
//    pContext->SetSelection(ec, 1, &tfSelection);

    //
    // set the display attribute to the composition range.
    //
    _SetCompositionDisplayAttributes(ec, pContext, _gaDisplayAttributeInput);

Exit:
    tfSelection.range->Release();
    return S_OK;
}

//+---------------------------------------------------------------------------
//
// _HandleReturnKey
//
//----------------------------------------------------------------------------

HRESULT CTextService::_HandleReturnKey(TfEditCookie ec, ITfContext *pContext)
{
    // just terminate the composition
    _TerminateComposition(ec, pContext);
    return S_OK;
}

//+---------------------------------------------------------------------------
//
// _HandleSpaceKey
//
//----------------------------------------------------------------------------

HRESULT CTextService::_HandleSpaceKey(TfEditCookie ec, ITfContext *pContext)
{
    //
    // set the display attribute to the composition range.
    //
    // The real text service may have linguistic logic here and set 
    // the specific range to apply the display attribute rather than 
    // applying the display attribute to the entire composition range.
    //
    //_SetCompositionDisplayAttributes(ec, pContext, _gaDisplayAttributeConverted);

    return S_OK;
}

//+---------------------------------------------------------------------------
//
// _HandleArrowKey
//
// Update the selection within a composition.
//
//----------------------------------------------------------------------------

HRESULT CTextService::_HandleArrowKey(TfEditCookie ec, ITfContext *pContext, WPARAM wParam)
{
    ITfRange *pRangeComposition;
    LONG cch;
    BOOL fEqual;
    TF_SELECTION tfSelection;
    ULONG cFetched;

    // get the selection
    if (pContext->GetSelection(ec, TF_DEFAULT_SELECTION, 1, &tfSelection, &cFetched) != S_OK ||
        cFetched != 1)
    {
        // no selection?
        return S_OK; // eat the keystroke
    }

    // get the composition range
    if (_pComposition->GetRange(&pRangeComposition) != S_OK)
        goto Exit;

    // adjust the selection
    if (wParam == VK_LEFT)
    {
        if (tfSelection.range->IsEqualStart(ec, pRangeComposition, TF_ANCHOR_START, &fEqual) == S_OK &&
            !fEqual)
        {
            tfSelection.range->ShiftStart(ec, -1, &cch, NULL);
        }
        tfSelection.range->Collapse(ec, TF_ANCHOR_START);
    }
    else
    {
        // VK_RIGHT
        if (tfSelection.range->IsEqualEnd(ec, pRangeComposition, TF_ANCHOR_END, &fEqual) == S_OK &&
            !fEqual)
        {
            tfSelection.range->ShiftEnd(ec, +1, &cch, NULL);
        }
        tfSelection.range->Collapse(ec, TF_ANCHOR_END);
    }

    pContext->SetSelection(ec, 1, &tfSelection);

    pRangeComposition->Release();

Exit:
    tfSelection.range->Release();
    return S_OK; // eat the keystroke
}

//+---------------------------------------------------------------------------
//
// _InvokeKeyHandler
//
// This text service is interested in handling keystrokes to demonstrate the
// use the compositions. Some apps will cancel compositions if they receive
// keystrokes while a compositions is ongoing.
//
//----------------------------------------------------------------------------

HRESULT CTextService::_InvokeKeyHandler(ITfContext *pContext, WPARAM wParam, LPARAM lParam)
{
    CKeyHandlerEditSession *pEditSession;
    HRESULT hr = E_FAIL;

    // Insert a char in place of this keystroke
    if ((pEditSession = new CKeyHandlerEditSession(this, pContext, wParam)) == NULL)
        goto Exit;

    // A lock is required.
    // nb: this method is one of the few places where it is legal to use
    // the TF_ES_SYNC flag
    hr = pContext->RequestEditSession(_tfClientId, pEditSession, TF_ES_SYNC | TF_ES_READWRITE, &hr);

    pEditSession->Release();

Exit:
    return hr;
}

