//////////////////////////////////////////////////////////////////////
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
//  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//  PARTICULAR PURPOSE.
//
//  Copyright (C) 2003  Microsoft Corporation.  All rights reserved.
//
//  KeyEventSink.cpp
//
//          ITfKeyEventSink implementation.
//
//////////////////////////////////////////////////////////////////////

#include "Globals.h"
#include "TextService.h"

//
// GUID for the preserved keys.
//
// {F5E1FC24-B000-4d26-B011-65020E9DBFB1}
static const GUID GUID_PRESERVEDKEY_NEXTKB = 
{ 0xf5e1fc24, 0xb000, 0x4d26, { 0xb0, 0x11, 0x65, 0x2, 0xe, 0x9d, 0xbf, 0xb1 } };
// {3F31D1DA-B70A-4df4-B5E5-41607A5CC115}
static const GUID GUID_PRESERVEDKEY_PREVKB = 
{ 0x3f31d1da, 0xb70a, 0x4df4, { 0xb5, 0xe5, 0x41, 0x60, 0x7a, 0x5c, 0xc1, 0x15 } };


//
// the preserved keys declaration
//
static const TF_PRESERVEDKEY c_pkeyNextKb = { VK_SPACE, TF_MOD_CONTROL };
static const TF_PRESERVEDKEY c_pkeyPrevKb = { VK_SPACE, TF_MOD_CONTROL | TF_MOD_SHIFT };

//
// the description for the preserved keys
//
static const WCHAR c_szPKeyNextKb[] = L"Next Keyboard";
static const WCHAR c_szPKeyPrevKb[] = L"Prev Keyboard";

//+---------------------------------------------------------------------------
//
// _IsKeyEaten
//
//----------------------------------------------------------------------------

BOOL CTextService::_IsKeyEaten(WPARAM wParam)
{
    // if the keyboard is disabled, the keys are not consumed.
    if (_IsKeyboardDisabled())
        return FALSE;

    // if the keyboard is closed, the keys are not consumed.
    if (!_IsKeyboardOpen())
        return FALSE;

	return TRUE; // handle all keys
}


//+---------------------------------------------------------------------------
//
// OnSetFocus
//
// Called by the system whenever this service gets the keystroke device focus.
//----------------------------------------------------------------------------

STDAPI CTextService::OnSetFocus(BOOL fForeground)
{
    return S_OK;
}

//+---------------------------------------------------------------------------
//
// OnTestKeyDown
//
// Called by the system to query this service wants a potential keystroke.
//----------------------------------------------------------------------------

STDAPI CTextService::OnTestKeyDown(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pfEaten)
{
	if ((lParam >> 16 & 0xff) == 255) {
		*pfEaten = FALSE;
	}
	else {
		*pfEaten = _IsKeyEaten(wParam);
	}
	return S_OK;
}

//+---------------------------------------------------------------------------
//
// OnKeyDown
//
// Called by the system to offer this service a keystroke.  If *pfEaten == TRUE
// on exit, the application will not handle the keystroke.
//----------------------------------------------------------------------------

STDAPI CTextService::OnKeyDown(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pfEaten)
{
    *pfEaten = _IsKeyEaten(wParam);

    if (*pfEaten)
    {
		_InvokeKeyHandler(pContext, wParam, lParam);
    }
    return S_OK;
}

//+---------------------------------------------------------------------------
//
// OnTestKeyUp
//
// Called by the system to query this service wants a potential keystroke.
//----------------------------------------------------------------------------

STDAPI CTextService::OnTestKeyUp(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pfEaten)
{
    *pfEaten = _IsKeyEaten(wParam);
    return S_OK;
}

//+---------------------------------------------------------------------------
//
// OnKeyUp
//
// Called by the system to offer this service a keystroke.  If *pfEaten == TRUE
// on exit, the application will not handle the keystroke.
//----------------------------------------------------------------------------

STDAPI CTextService::OnKeyUp(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pfEaten)
{
    *pfEaten = _IsKeyEaten(wParam);
    return S_OK;
}

//+---------------------------------------------------------------------------
//
// OnPreservedKey
//
// Called when a hotkey (registered by us, or by the system) is typed.
//----------------------------------------------------------------------------

STDAPI CTextService::OnPreservedKey(ITfContext *pContext, REFGUID rguid, BOOL *pfEaten)
{

    if (IsEqualGUID(rguid, GUID_PRESERVEDKEY_NEXTKB))
    {
		_SwitchNextLayout();
        *pfEaten = TRUE;
    }
	else if (IsEqualGUID(rguid, GUID_PRESERVEDKEY_PREVKB))
    {
		_SwitchPrevLayout();
        *pfEaten = TRUE;
    }
    else
    {
        *pfEaten = FALSE;
    }

    return S_OK;
}

//+---------------------------------------------------------------------------
//
// _InitKeyEventSink
//
// Advise a keystroke sink.
//----------------------------------------------------------------------------

BOOL CTextService::_InitKeyEventSink()
{
    ITfKeystrokeMgr *pKeystrokeMgr;
    HRESULT hr;

    if (_pThreadMgr->QueryInterface(IID_ITfKeystrokeMgr, (void **)&pKeystrokeMgr) != S_OK)
        return FALSE;

    hr = pKeystrokeMgr->AdviseKeyEventSink(_tfClientId, (ITfKeyEventSink *)this, TRUE);

    pKeystrokeMgr->Release();

    return (hr == S_OK);
}

//+---------------------------------------------------------------------------
//
// _UninitKeyEventSink
//
// Unadvise a keystroke sink.  Assumes a sink has benn advised already.
//----------------------------------------------------------------------------

void CTextService::_UninitKeyEventSink()
{
    ITfKeystrokeMgr *pKeystrokeMgr;

    if (_pThreadMgr->QueryInterface(IID_ITfKeystrokeMgr, (void **)&pKeystrokeMgr) != S_OK)
        return;

    pKeystrokeMgr->UnadviseKeyEventSink(_tfClientId);

    pKeystrokeMgr->Release();
}

//+---------------------------------------------------------------------------
//
// _InitPreservedKey
//
// Register a hot key.
//----------------------------------------------------------------------------

BOOL CTextService::_InitPreservedKey()
{
    ITfKeystrokeMgr *pKeystrokeMgr;
    HRESULT hr;

    if (_pThreadMgr->QueryInterface(IID_ITfKeystrokeMgr, (void **)&pKeystrokeMgr) != S_OK)
        return FALSE;

    // register CTRL + SPACE key
    hr = pKeystrokeMgr->PreserveKey(_tfClientId, 
                                    GUID_PRESERVEDKEY_NEXTKB,
                                    &c_pkeyNextKb,
                                    c_szPKeyNextKb,
                                    wcslen(c_szPKeyNextKb));

	// register CTRL + SHIFT + SPACE key
    hr = pKeystrokeMgr->PreserveKey(_tfClientId, 
                                    GUID_PRESERVEDKEY_PREVKB,
                                    &c_pkeyPrevKb,
                                    c_szPKeyPrevKb,
                                    wcslen(c_szPKeyPrevKb));

    pKeystrokeMgr->Release();

    return (hr == S_OK);
}

//+---------------------------------------------------------------------------
//
// _UninitPreservedKey
//
// Uninit a hot key.
//----------------------------------------------------------------------------

void CTextService::_UninitPreservedKey()
{
    ITfKeystrokeMgr *pKeystrokeMgr;

    if (_pThreadMgr->QueryInterface(IID_ITfKeystrokeMgr, (void **)&pKeystrokeMgr) != S_OK)
        return;

    pKeystrokeMgr->UnpreserveKey(GUID_PRESERVEDKEY_NEXTKB, &c_pkeyNextKb);
	pKeystrokeMgr->UnpreserveKey(GUID_PRESERVEDKEY_PREVKB, &c_pkeyPrevKb);

    pKeystrokeMgr->Release();
}

