#include "InputProcessor.h"

// Backsapce counts that won't eat
static int DontEatBackspace = 0;
// If context is matching with VK_*, do delete is false
//static bool do_delete = true;
// < VK_* & VK_* & ... > length
static int keys_length = 0;

struct ModKeysState
{
	bool CTRL;
	bool ALT;
	bool SHIFT;
	bool CAPS;
};

static ModKeysState mksOldState;

struct InputRuleInfo
{
	//std::wstring expression;
	//int context_length;
	//int keys_length;
	//boost::wregex regex;
	bool matched;
	EXPENDEDRULELIST::iterator it;
	bool deleted;
};

void sendKeyStrokes (std::wstring * s, bool noInternalActions = false)//Send Keys Strokes
{
	HWND hwnd = GetFocus();

	int cInputs = s->length() * 2;

	INPUT * ip = new INPUT[cInputs];

	for(int i=0, ii=0; i < s->length(); i++, ii++){
		ip[ii].type = INPUT_KEYBOARD;
		ip[ii].ki.dwExtraInfo = 0xDEADC0DE;
		ip[ii].ki.dwFlags = KEYEVENTF_UNICODE;
		ip[ii].ki.time = 0;
		ip[ii].ki.wScan = s->at(i);
		ip[ii].ki.wVk = 0;
		//SendInput(1, &ip, sizeof(INPUT));

		ii++;

		ip[ii].type = INPUT_KEYBOARD;
		ip[ii].ki.dwExtraInfo = 0;
		ip[ii].ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_UNICODE;
		ip[ii].ki.time = 0;
		ip[ii].ki.wScan = s->at(i);
		ip[ii].ki.wVk = 0;
		//SendInput(1, &ip, sizeof(INPUT));
	}

	int cSent = SendInput(cInputs, ip, sizeof(INPUT));
	Debug(L"cInputs=%d, cSent=%d\n",cInputs, cSent);

	if (noInternalActions == false) {
		InternalEditor.AppendText(s->c_str(), s->length());
	}
}

void sendBackspace(int count, bool noInternalActions = false)
{
	if (!GetFocus() || count < 1)
		return ;

	DontEatBackspace += count;
	
	//while(count--)
	//{
	//	keybd_event(VK_BACK, 255, 0, 0);
	//	keybd_event(VK_BACK, 2, KEYEVENTF_KEYUP, 0);
	//	InternalEditor.Delete();
	//}

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

	if (noInternalActions == false) {
		InternalEditor.Delete(count);
	}
}

signed int getMatchLength(const wchar_t * s1, const wchar_t * s2)
{
	int length = 0;

	int s1len = wcslen(s1);
	int s2len = wcslen(s2);

	if (s1len < s2len)
		return s1len - s2len;

	while (*s1)
	{
		if (*s1++ == *s2++)
			length ++ ;
		else
			break;
	}
	return length;
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

bool getOutputAndSend(InputRuleInfo * ir_info, WORD wVk, LPBYTE KeyStates)
{
	boost::wcmatch matches;

	const wchar_t * srcString = InternalEditor.GetTextBackward(ir_info->it->estimated_length);
	if (!srcString)
		return false;

	boost::regex_match(srcString, matches, *ir_info->it->regex);

	int len = wcslen((wchar_t*)ir_info->it->rule_it->strOutRule);
	std::wstring outStr ;
	VIRTUALKEYS vks;
	SWITCHES sws;

	/*for (SWITCHES::iterator sws_it = ir_info->it->switches->begin();
		sws_it != ir_info->it->switches->end();
		sws_it++) {
			InternalEditor.invertSwitch(*sws_it);
	}*/

	if (!makeRegex(&outStr, &vks, &sws, ir_info->it->rule_it->strOutRule, len, false, &matches, &ir_info->it->cc))
		return false;

	sendSingleKey(VK_CONTROL, KEYEVENTF_KEYUP);
	sendSingleKey(VK_MENU, KEYEVENTF_KEYUP);
	//sendSingleKey(VK_SHIFT, KEYEVENTF_KEYUP);

	if (!outStr.length())
	{
		if (ir_info->deleted == false)
			// Delete recent added temporary input
			InternalEditor.Delete();

		sendBackspace(wcslen(srcString));

		if (mksOldState.CTRL)
			sendSingleKey(VK_CONTROL, 0);
		if (mksOldState.ALT)
			sendSingleKey(VK_MENU, 0);
		//if (mksOldState.SHIFT)
		//	sendSingleKey(VK_SHIFT, 0);
		return true;
	}

	std::wstring resultStr;
	//std::wstring * temp = new std::wstring(srcString);

	resultStr = boost::regex_replace((std::wstring)srcString, *ir_info->it->regex, outStr);
	//temp->clear();

	if (!resultStr.size()){
		//rise error
		//continue;
		return false;
	}
	if (ir_info->deleted == false)
		// Delete recently added temporary input
		InternalEditor.Delete();

	int srcStringStringLen = wcslen(srcString);

	outStr.assign(resultStr);

	if (srcStringStringLen) {
		int matchLength = getMatchLength(outStr.c_str(), srcString);

		if (matchLength < 0){
			sendBackspace(0 - matchLength);
			matchLength = getMatchLength(outStr.c_str(), srcString);
			sendBackspace(outStr.length() - matchLength);
		}
		else if (srcStringStringLen > matchLength){
			sendBackspace(srcStringStringLen - matchLength);
		}
		outStr.erase(0, matchLength);
	}

	sendKeyStrokes(&outStr);

	int strOutLength = outStr.length();
	if ( strOutLength == 1 && (outStr.at(0)>0x20 && outStr.at(0)<0x7F) ){} else 
	{
		// KeyUp match VKs
		for (VIRTUALKEYS::iterator vk_it = ir_info->it->vk->begin();
			vk_it != ir_info->it->vk->end();
			vk_it++){
				KeyStates[*vk_it] = 0;
		}
		// Do match again
		matchRules(0, 0, KeyStates, false);
	}

	if (mksOldState.CTRL)
		sendSingleKey(VK_CONTROL, 0);
	if (mksOldState.ALT)
		sendSingleKey(VK_MENU, 0);
	//if (mksOldState.SHIFT)
	//	sendSingleKey(VK_SHIFT, 0);

	return true;
}

bool matchRules(wchar_t wcInput, WORD wVk, LPBYTE KeyStates, bool user_input)
{
	InputRuleInfo ir_info;
	bool have_match = false, vk_matched = true;
	ir_info.matched = false;

//	ir_info.keys_length = 0;
//#ifdef _DEBUG
//	Debug(L"Before:");
//#endif
	// Add temporarily
	InternalEditor.AddInput(wcInput);
//	RULELIST Rules = klf.getRules();
//
//	for (RULELIST::iterator it = Rules.begin(); it != Rules.end(); it++)
	for (EXPENDEDRULELIST::iterator it = vtERs.begin(); it != vtERs.end(); it++)
	{
		//if ( klf.isDebugBreak(it - vtERs.begin()) & IsDebuggerPresent()){}
			 //DebugBreak();
		bool deleted = false;
		vk_matched = false;

		ModKeysState mks;
		memset(&mks, 0, sizeof(ModKeysState));
		for (SWITCHES::iterator sws_it = it->switches->begin();
			sws_it != it->switches->end();
			sws_it++) {
				if (InternalEditor.isSwitchOn(*sws_it)==false)
					goto end_of_loop;
					
		}
		for (VIRTUALKEYS::iterator vk_it = it->vk->begin(); vk_it != it->vk->end(); vk_it++){
			if (KeyStates[*vk_it] & 0x80){
				switch (*vk_it){
				case VK_LCONTROL: case VK_RCONTROL: case VK_CONTROL:
					mks.CTRL = true;
					vk_matched = true;
					break;
				case VK_LMENU: case VK_RMENU: case VK_MENU:
					mks.ALT = true;
					vk_matched = true;
					break;
				case VK_RSHIFT: case VK_LSHIFT: case VK_SHIFT:
					mks.SHIFT = true;
					vk_matched = true;
					break;
				default:
					if (*vk_it == wVk)
						vk_matched = true;
					break;
				}
			}
			else {
				if (*vk_it == VK_SHIFT){
					if ( (KeyStates[VK_CAPITAL] & 0x81) && klf.layout.trackCaps)
						mks.SHIFT = true;
					else { vk_matched=false; break; }
				}
				else { vk_matched=false; break; }
			}
		}

		if (klf.layout.trackCaps ?
			(it->vk->size() && ( (vk_matched==false) || (mksOldState.CTRL ^ mks.CTRL) || (mksOldState.ALT ^ mks.ALT) || ( mksOldState.SHIFT ^ mksOldState.CAPS)^ mks.SHIFT ) ):
			(it->vk->size() && ( (vk_matched==false) || (mksOldState.CTRL ^ mks.CTRL) || (mksOldState.ALT ^ mks.ALT) || ( mksOldState.SHIFT ^ mks.SHIFT) ) )
				)
				continue;
		else if (it->vk->size() && wcInput){
			InternalEditor.Delete();
			deleted = true;
		}
		else if (user_input && (mksOldState.CTRL || mksOldState.ALT)){continue;}

		const wchar_t * src = InternalEditor.GetTextBackward(it->estimated_length);
		if (src) {// Is there enough length to do a match?

			if (wcslen(src) && !wcInput && user_input) continue;

			boost::wcmatch matches;
			if (boost::regex_match(src, matches, *it->regex)){
				have_match = true; // Got a match
				if (ir_info.matched == false) {
					ir_info.matched = true;
					ir_info.it = it;
					ir_info.deleted = (wcInput && user_input) ? deleted : true;
				}
				break;
				//if (ir_info.it->switches->size() < it->switches->size() ) {
				//	//store
				//	ir_info.it = it;
				//	ir_info.deleted = (wcInput && user_input) ? deleted : true;
				//}
				//else if (ir_info.it->vk->size() < it->vk->size() ) {// compare < VK_* & .. > counts)
				//	//store
				//	ir_info.it = it;
				//	ir_info.deleted = (wcInput && user_input) ? deleted : true;
				//}
				//else if (ir_info.it->estimated_length + ir_info.it->vk->size() < it->estimated_length + it->vk->size()) {// compare pattern counts
				//	//store
				//	ir_info.it = it;
				//	ir_info.deleted = (wcInput && user_input) ? deleted : true;
				//}
				////continue matching
			}
		}
		if (deleted == true && wcInput)
			InternalEditor.AddInput(wcInput);
end_of_loop:;
//		delete s_in;
	}
//
	if ((mksOldState.CTRL ^ mksOldState.ALT)==false && wcInput >= 0x20 && wcInput < 0x7F)
		InternalEditor.setAllSwitchOff();

	if (have_match == true){
		//if (ir_info.deleted == true && user_input == true && wcInput)
		//	InternalEditor.Delete();
		return getOutputAndSend(&ir_info, wVk, KeyStates);
	}
//
	if (wcInput){
		//Delete recently added temporary input
		InternalEditor.Delete();
	}
//
	return false;
}

//Args	-	wVk (WORD) : Input to process : NOT NULL
//			lParam (LPARAM) : lParm passed from hooks
//return -	true : Input was eatten.
//			false : Input was not processed.
bool ProcessInput(WORD wVk, LPARAM lParam)
{
	try {

		WORD scancode = (WORD)(lParam >> 16);
		DWORD dwExtraInfo = GetMessageExtraInfo();
		// If VK_BACK and there is any DontEatBackspace, subtract the DontEatBackspace count from lParam
		if (wVk == VK_BACK && DontEatBackspace > 0 && ((BYTE)scancode == 0xff))
		{
			DontEatBackspace -= lParam & 0xFF;
			if (DontEatBackspace < 0) // lParam is larger then DontEatBackspace
			{ // Eat
				InternalEditor.Delete (0 - DontEatBackspace); // And delete additional sendBackspace (Hack for internal editor)
				DontEatBackspace = 0;
			}
			return false;
		}

		// Do not need to do anything, just pass it
		if (wVk == VK_CONTROL || wVk == VK_MENU || wVk == VK_SHIFT)
			return false;

		BYTE GlobalKeyStates[256];
		GetKeyboardState(GlobalKeyStates); // Get States

		// Store mod keys to use it later
		mksOldState.CTRL = GlobalKeyStates[VK_CONTROL] & 0x80;
		mksOldState.ALT = GlobalKeyStates[VK_MENU] & 0x80;
		mksOldState.SHIFT = GlobalKeyStates[VK_SHIFT] & 0x80;
		mksOldState.CAPS = GlobalKeyStates[VK_CAPITAL] & 0x81;

		Debug(L"CTRL = %x ALT = %x\n", mksOldState.CTRL, mksOldState.ALT);

		wchar_t wcInput = wVk;UINT usVk;

		if (usVk = TranslateToUnicode((WORD*)&wcInput, GlobalKeyStates)) // Get Ascii Value
		{ // If there is any ascii value
			if (matchRules(wcInput, usVk, GlobalKeyStates, true )) {// Match for the input
				// Found matched
				return true; // Eaten
			}
			else if (wVk == VK_BACK) {
				// If VK_BACK
				return false;
				//InternalEditor.Delete(lParam & 0xFF); // Delete from internal editor
			}
			else if (klf.layout.eat==true && //eat unused key
				mksOldState.CTRL==false && mksOldState.ALT==false &&
				wcInput > 0x20 && wcInput < 0x7F) {
					InternalEditor.undoEnd();
					return true;
			}
			else if (mksOldState.CTRL==false && mksOldState.ALT==false) {
				InternalEditor.AddInput(wcInput); // Not matched? Just store the input
				return false;
			}
		} else {
			if (wVk != 0xe7) {// unicode input
				if (matchRules(NULL, wVk, GlobalKeyStates, true )) {// Match for the input
					// Found matched
					return true; // Eaten
				}
			}
		}

		//If only one of these two keys are pressed
		if (mksOldState.CTRL ^ mksOldState.ALT){
			Debug(L"(isCTRL ^ isALT)\n");
			InternalEditor.Reset();// restart the internal editor's buffer
			return false;
		}
		
	} catch (...) {
		Debug(L"Caught exception!\n");
	}
	return false;
}

bool Undo()
{
	wchar_t * TextFrom = new wchar_t[InternalEditor.GetTextLength() + 1];
	wcscpy(TextFrom, InternalEditor.GetAll());

	if (InternalEditor.undo())
	{
		const wchar_t * TextTo = InternalEditor.GetAll();
		int lenFrom = wcslen(TextFrom);
		int lenTo = wcslen(TextTo);
		if (lenFrom > lenTo)
		{
			sendBackspace(lenFrom - lenTo, true);
			TextFrom[lenTo] = 0;
			lenFrom = lenTo;
		}
		
		int i = 0;
		while (TextTo[i] == TextFrom[i])
		{
			i++;
		}

		if (i < lenFrom) 
		{
			sendBackspace(lenFrom - i, true);
			TextFrom[i] = 0;
			lenFrom = i;
		}

		if (i < lenTo)
		{
			sendKeyStrokes(&std::wstring(&TextTo[i]), true);
		}

		return true;
	}
	return false;
}