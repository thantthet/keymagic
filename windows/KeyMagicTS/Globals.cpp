//////////////////////////////////////////////////////////////////////
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
//  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//  PARTICULAR PURPOSE.
//
//  Copyright (C) 2003  Microsoft Corporation.  All rights reserved.
//
//  Globals.cpp
//
//          Global variables
//
//////////////////////////////////////////////////////////////////////

#include "globals.h"

HINSTANCE g_hInst;

LONG g_cRefDll = -1; // -1 /w no refs, for win95 InterlockedIncrement/Decrement compat

CRITICAL_SECTION g_cs;
// {404FE037-18CC-4d23-A179-B80F152E9989}
const GUID c_clsidTextService = 
{ 0x404fe037, 0x18cc, 0x4d23, { 0xa1, 0x79, 0xb8, 0xf, 0x15, 0x2e, 0x99, 0x89 } };
// {A7614680-DF45-4cee-97A0-563AB77759E9}
const GUID c_guidProfile = 
{ 0xa7614680, 0xdf45, 0x4cee, { 0x97, 0xa0, 0x56, 0x3a, 0xb7, 0x77, 0x59, 0xe9 } };
// {9C65880C-70DE-4d8c-89CE-F5F09BB2075E}
const GUID c_guidLangBarItemButton = 
{ 0x9c65880c, 0x70de, 0x4d8c, { 0x89, 0xce, 0xf5, 0xf0, 0x9b, 0xb2, 0x7, 0x5e } };

//
//  define two guids for display attribute info. This textservice has
//  two display attribute. One is for input text and the other is for the
//  converted text.
//
//      c_guidDisplayAttributeInput 
//

// {B3EF22A3-AFFB-4c59-AF54-C537DF8BE861}
static const GUID c_guidDisplayAttributeInput = 
{ 0xb3ef22a3, 0xaffb, 0x4c59, { 0xaf, 0x54, 0xc5, 0x37, 0xdf, 0x8b, 0xe8, 0x61 } };
