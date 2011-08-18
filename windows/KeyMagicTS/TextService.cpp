//////////////////////////////////////////////////////////////////////
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
//  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//  PARTICULAR PURPOSE.
//
//  Copyright (C) 2003  Microsoft Corporation.  All rights reserved.
//
//  TextService.cpp
//
//          IUnknown, ITfTextInputProcessor implementation.
//
//////////////////////////////////////////////////////////////////////

#include "globals.h"
#include "TextService.h"

//+---------------------------------------------------------------------------
//
// CreateInstance
//
//----------------------------------------------------------------------------

/* static */
HRESULT CTextService::CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppvObj)
{
    CTextService *pCase;
    HRESULT hr;

    if (ppvObj == NULL)
        return E_INVALIDARG;

    *ppvObj = NULL;

    if (NULL != pUnkOuter)
        return CLASS_E_NOAGGREGATION;

    if ((pCase = new CTextService) == NULL)
        return E_OUTOFMEMORY;

    hr = pCase->QueryInterface(riid, ppvObj);

    pCase->Release(); // caller still holds ref if hr == S_OK

    return hr;
}

//+---------------------------------------------------------------------------
//
// ctor
//
//----------------------------------------------------------------------------

CTextService::CTextService()
{
    DllAddRef();

    //
    // Initialize the thread manager pointer.
    //
    _pThreadMgr = NULL;

    //
    // Initialize the numbers for ThreadMgrEventSink.
    //
    _dwThreadMgrEventSinkCookie = TF_INVALID_COOKIE;

    //
    // Initialize the numbers for TextEditSink.
    //
    _pTextEditSinkContext = NULL;
    _dwTextEditSinkCookie = TF_INVALID_COOKIE;

    //
    // Initialize the composition object pointer.
    //
    _pComposition = NULL;

	//
	//
	//
	_pLangBarItem = NULL;

    _cRef = 1;
}

//+---------------------------------------------------------------------------
//
// dtor
//
//----------------------------------------------------------------------------

CTextService::~CTextService()
{
    DllRelease();
}

//+---------------------------------------------------------------------------
//
// QueryInterface
//
//----------------------------------------------------------------------------

STDAPI CTextService::QueryInterface(REFIID riid, void **ppvObj)
{
    if (ppvObj == NULL)
        return E_INVALIDARG;

    *ppvObj = NULL;

    if (IsEqualIID(riid, IID_IUnknown) ||
        IsEqualIID(riid, IID_ITfTextInputProcessor))
    {
        *ppvObj = (ITfTextInputProcessor *)this;
    }
    else if (IsEqualIID(riid, IID_ITfThreadMgrEventSink))
    {
        *ppvObj = (ITfThreadMgrEventSink *)this;
    }
    else if (IsEqualIID(riid, IID_ITfTextEditSink))
    {
        *ppvObj = (ITfTextEditSink *)this;
    }
    else if (IsEqualIID(riid, IID_ITfKeyEventSink))
    {
        *ppvObj = (ITfKeyEventSink *)this;
    }
    else if (IsEqualIID(riid, IID_ITfCompositionSink))
    {
        *ppvObj = (ITfKeyEventSink *)this;
    }
    else if (IsEqualIID(riid, IID_ITfDisplayAttributeProvider))
    {
        *ppvObj = (ITfDisplayAttributeProvider *)this;
    }

    if (*ppvObj)
    {
        AddRef();
        return S_OK;
    }

    return E_NOINTERFACE;
}


//+---------------------------------------------------------------------------
//
// AddRef
//
//----------------------------------------------------------------------------

STDAPI_(ULONG) CTextService::AddRef()
{
    return ++_cRef;
}

//+---------------------------------------------------------------------------
//
// Release
//
//----------------------------------------------------------------------------

STDAPI_(ULONG) CTextService::Release()
{
    LONG cr = --_cRef;

    assert(_cRef >= 0);

    if (_cRef == 0)
    {
        delete this;
    }

    return cr;
}

//+---------------------------------------------------------------------------
//
// Activate
//
//----------------------------------------------------------------------------

STDAPI CTextService::Activate(ITfThreadMgr *pThreadMgr, TfClientId tfClientId)
{
    _pThreadMgr = pThreadMgr;
    _pThreadMgr->AddRef();
    _tfClientId = tfClientId;

	wstring path;
	if (_GetConfig(L"Active Path", path) && _engine.loadKeyboardFile(path.c_str()))
	{
		_SetKeyboardOpen(TRUE);
		_activeKeyboardPath = path;
		_LoadActiveKeyboardIcon();
		wstring * wstr;
		wstr = GetKeyboardNameOrFileTitle(_engine.getKeyboard()->getInfoList(), _activeKeyboardPath);
		_keyboardName = wstr->c_str();
		delete wstr;
		wstr = GetDescription(_engine.getKeyboard()->getInfoList());
		_keyboardDescription = wstr->c_str();
		delete wstr;
	
	} else {
		// Get KeyMagic keyboard layout list
		KEYBOARD_LIST * _keyboards = GetKeyboards();
		if (_keyboards->size()) {
			_SetActiveKeyboard(_keyboards->at(0));
		}
	}

    //
    // Initialize ThreadMgrEventSink.
    //
    if (!_InitThreadMgrEventSink())
        goto ExitError;

    // 
    //  If there is the focus document manager already,
    //  advise the TextEditSink.
    // 
    ITfDocumentMgr *pDocMgrFocus;
    if ((_pThreadMgr->GetFocus(&pDocMgrFocus) == S_OK) &&
        (pDocMgrFocus != NULL))
    {
        _InitTextEditSink(pDocMgrFocus);
        pDocMgrFocus->Release();
    }

    //
    // Initialize Language Bar.
    //
    if (!_InitLanguageBar())
        goto ExitError;

    //
    // Initialize KeyEventSink
    //
    if (!_InitKeyEventSink())
        goto ExitError;

    //
    // Initialize PreservedKeys
    //
    if (!_InitPreservedKey())
        goto ExitError;

    //
    // Initialize display guid atom
    //
    if (!_InitDisplayAttributeGuidAtom())
        goto ExitError;

    return S_OK;

ExitError:
    Deactivate(); // cleanup any half-finished init
    return E_FAIL;
}

//+---------------------------------------------------------------------------
//
// Deactivate
//
//----------------------------------------------------------------------------

STDAPI CTextService::Deactivate()
{
    //
    // Unadvise TextEditSink if it is advised.
    //
    _InitTextEditSink(NULL);

    //
    // Uninitialize ThreadMgrEventSink.
    //
    _UninitThreadMgrEventSink();

    //
    // Uninitialize Language Bar.
    //
    _UninitLanguageBar();

    //
    // Uninitialize KeyEventSink
    //
    _UninitKeyEventSink();

    //
    // Uninitialize PreservedKeys
    //
    _UninitPreservedKey();

    // Release ALL refs to _pThreadMgr in Deactivate
    if (_pThreadMgr != NULL)
    {
        _pThreadMgr->Release();
        _pThreadMgr = NULL;
    }

    _tfClientId = TF_CLIENTID_NULL;

    return S_OK;
}

void CTextService::_LoadActiveKeyboardIcon()
{
	HICON hIcon = LoadIconFromKeyboard(_engine.getKeyboard()->getInfoList());
	if (!hIcon) {
		hIcon = (HICON)LoadImage(g_hInst, TEXT("IDI_TEXTSERVICE_EMPTY"), IMAGE_ICON, 16, 16, 0);
	}
	_hIcon = hIcon;
}

void CTextService::_SetActiveKeyboard(const wstring& keyboardPath)
{
	if (_engine.loadKeyboardFile(keyboardPath.c_str())) {
		_SetConfig(L"Active Path", keyboardPath);
		_activeKeyboardPath = keyboardPath;
		_SetKeyboardOpen(TRUE);

		_LoadActiveKeyboardIcon();
		
		wstring * wstr;
		wstr = GetKeyboardNameOrFileTitle(_engine.getKeyboard()->getInfoList(), _activeKeyboardPath);
		_keyboardName = wstr->c_str();
		delete wstr;
		wstr = GetDescription(_engine.getKeyboard()->getInfoList());
		_keyboardDescription = wstr->c_str();
		delete wstr;

		if (_pLangBarItem != NULL) {
			_UninitLanguageBar();
			_InitLanguageBar();
		}
	}
}

const wstring& CTextService::_GetActiveKeyboard()
{
	return _activeKeyboardPath;
}
void CTextService::_SwitchNextLayout()
{
	KEYBOARD_LIST * keyboards = GetKeyboards();
	for (KEYBOARD_LIST::iterator i = keyboards->begin(); i != keyboards->end(); i++) 
	{
		if (*i == _activeKeyboardPath)
		{
			if ((i+1) == keyboards->end()) {
				_SetActiveKeyboard(*keyboards->begin());
			}
			else {
				_SetActiveKeyboard(*(i+1));
			}
			break;
		}
	}
	delete keyboards;
}

void CTextService::_SwitchPrevLayout()
{
	KEYBOARD_LIST * keyboards = GetKeyboards();
	for (KEYBOARD_LIST::reverse_iterator i = keyboards->rbegin(); i != keyboards->rend(); i++) 
	{
		if (*i == _activeKeyboardPath)
		{
			if ((i+1) == keyboards->rend()) {
				_SetActiveKeyboard(*keyboards->rbegin());
			}
			else {
				_SetActiveKeyboard(*(i+1));
			}
			break;
		}
	}
	delete keyboards;
}

BOOL CTextService::_SetConfig(const wstring& key, const wstring& value)
{
	DWORD dw;
	HKEY hKey, hSubKey;
	BOOL fRet;

	if (fRet = RegOpenKeyExW(HKEY_CURRENT_USER, L"Software", REG_OPTION_NON_VOLATILE, KEY_WRITE, &hKey) == ERROR_SUCCESS)
	{
		if (fRet &= RegCreateKeyExW(hKey, L"KeyMagic", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hSubKey, &dw) == ERROR_SUCCESS)
		{
			fRet &= RegSetValueExW(hSubKey, key.c_str(), 0, REG_SZ, (BYTE *)value.c_str(), value.length() * sizeof(WCHAR)) == ERROR_SUCCESS;
			RegCloseKey(hSubKey);
		}
		RegCloseKey(hKey);
	}

	return fRet;
}

BOOL CTextService::_SetConfig(const string& key, const string& value)
{
	int converted = 0;
	WCHAR *wcKey = new WCHAR[key.length() + 1];
	WCHAR *wcValue = new WCHAR[value.length() + 1];

	if (converted = MultiByteToWideChar(CP_UTF8, 0, key.c_str(), key.length(), wcKey, key.length())) {
		wcKey[converted] = '\0';
		if (converted = MultiByteToWideChar(CP_UTF8, 0, value.c_str(), value.length(), wcValue, value.length())) {
			wcKey[converted] = '\0';
			BOOL b = _SetConfig(key, value);
			delete[] wcKey;
			delete[] wcValue;
			return b;
		}
	}
	delete[] wcKey;
	delete[] wcValue;
	return false;
}

BOOL CTextService::_GetConfig(const wstring& key, wstring& value)
{
	DWORD dw, type;
	HKEY hKey, hKeyMagic;
	BOOL fRet;

	WCHAR szValue[1000];

	if (fRet = RegOpenKeyExW(HKEY_CURRENT_USER, L"Software", REG_OPTION_NON_VOLATILE, KEY_READ, &hKey) == ERROR_SUCCESS)
	{
		if (fRet = RegOpenKeyExW(hKey, L"KeyMagic", REG_OPTION_NON_VOLATILE, KEY_READ, &hKeyMagic) == ERROR_SUCCESS) {
			dw = sizeof(szValue);
			type = REG_SZ;
			if (fRet = RegQueryValueExW(hKeyMagic, key.c_str(), NULL, &type, (LPBYTE)szValue, &dw) == ERROR_SUCCESS)
			{
				value = szValue;
			}
			RegCloseKey(hKeyMagic);
		}
		RegCloseKey(hKey);
	}
	return fRet;
}

BOOL CTextService::_GetConfig(const string& key, string& value)
{
	int converted = 0;
	WCHAR *wcKey = new WCHAR[key.length() + 1];

	if (converted = MultiByteToWideChar(CP_UTF8, 0, key.c_str(), key.length(), wcKey, key.length()))
	{
		wcKey[converted] = '\0';
		wstring wvalue;

		BOOL b = _GetConfig(wcKey, wvalue);
		if (b)
		{
			int length = wvalue.length();
			char *szValue = new char[length + 1];
			converted = WideCharToMultiByte(CP_UTF8, 0, wvalue.c_str(), length, szValue, length, 0, 0);
			szValue[converted] = '\0';
			value = szValue;
			delete[] szValue;
		}
		delete[] wcKey;
		return b;
	}
	delete[] wcKey;
	return false;
}