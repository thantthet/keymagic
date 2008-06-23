#pragma once

#include "resource.h"

#define KM_MESSAGE WM_USER+100
#define KM_SETKBID KM_MESSAGE+1
#define KM_KILLFOCUS KM_MESSAGE+2
#define KM_GETFOCUS KM_MESSAGE+3
#define KM_RESCAN KM_MESSAGE+3

struct KeyFileData{
	bool isNew;
	char Name[30];
	char Display[30];
	char Path[MAX_PATH];
	WORD wHotkey;
};

// Structure associated with menu items 
 
typedef struct tagMYITEM 
{ 
    int   cchItemText; 
    char  szItemText[1]; 
} MYITEM, NEAR *PMYITEM, FAR *LPMYITEM;
 
#define CCH_MAXITEMTEXT 256