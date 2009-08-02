#ifndef SHORTCUT_H_
#define SHORTCUT_H_

#pragma once

static bool isActive = false;
//UINT	NumOfShortCut = 0;
static UINT ActiveIndex = -1;

int ShortCutCheck (UINT uvKey){
//	Logger("ShortCutCheck Entry");
	bool isCTRL, isALT, isSHIFT;

	BYTE KeyStatus[256];
	GetKeyboardState(KeyStatus);

	isCTRL = KeyStatus[VK_CONTROL] & 0x80;
	isALT = KeyStatus[VK_MENU] & 0x80;
	isSHIFT = KeyStatus[VK_SHIFT] & 0x80;

	BYTE MOD_KEY = (isALT ? 4 : 0) + (isCTRL ? 2 : 0) + (isSHIFT ? 1 : 0);

//	Logger("ShortCutCheck StartChecking");
/*	for (int i=0; i < NumOfShortCut; i++){
//		Logger("ShortCutCheck : Not Found Yet i = %X",i);
		if (uvKey == vtSC[i].ukey && MOD_KEY == vtSC[i].modkey){
			return TRUE;
		}
	}
*/
	vector<KM_ShortCut>::iterator it;
	for (it=vtSC.begin(); it < vtSC.end(); it++ )
	{
		if (uvKey == ((KM_ShortCut)*it).ukey && MOD_KEY == ((KM_ShortCut)*it).modkey){
			return TRUE;
		}
	}
//	Logger("ShortCutCheck : return -1");

	return -1;
}

#endif