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

static ModKeysState old_state;

struct InputRuleInfo
{
	//std::wstring expression;
	//int context_length;
	//int keys_length;
	//boost::wregex regex;
	bool matched;
	EXPENDEDRULES::iterator it;
	bool deleted;
};

void SendStrokes (std::wstring * s)//Send Keys Strokes
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
	InternalEditor.AppendText(s->c_str(), s->length());

}

void backspace(int count)
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
	InternalEditor.Delete(count);
}

signed int get_match_len(const wchar_t * s1, const wchar_t * s2)
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

void SendKey(WORD wVk, DWORD dwFlags)
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

bool get_output_and_send(InputRuleInfo * ir_info, WORD wVk, LPBYTE KeyStates)
{
	boost::wcmatch matches;

	wchar_t * src = InternalEditor.GetTextBackward(ir_info->it->estimated_length);
	if (!src)
		return false;

	boost::regex_match(src, matches, *ir_info->it->regex);

	int len = wcslen((wchar_t*)ir_info->it->rule_it->strOutRule);
	std::wstring str_out ;
	VIRTUALKEYS vks;
	SWITCHES sws;

	/*for (SWITCHES::iterator sws_it = ir_info->it->switches->begin();
		sws_it != ir_info->it->switches->end();
		sws_it++) {
			InternalEditor.invertSwitch(*sws_it);
	}*/

	if (!makeRegex(&str_out, &vks, &sws, ir_info->it->rule_it->strOutRule, len, false, &matches, &ir_info->it->cc))
		return false;

	SendKey(VK_CONTROL, KEYEVENTF_KEYUP);
	SendKey(VK_MENU, KEYEVENTF_KEYUP);
	//SendKey(VK_SHIFT, KEYEVENTF_KEYUP);

	if (!str_out.length())
	{
		if (ir_info->deleted == false)
			// Delete recent added temporary input
			InternalEditor.Delete();

		backspace(wcslen(src));

		if (old_state.CTRL)
			SendKey(VK_CONTROL, 0);
		if (old_state.CTRL)
			SendKey(VK_MENU, 0);
		//if (old_state.SHIFT)
		//	SendKey(VK_SHIFT, 0);

		return true;
	}

	std::wstring replaced_str;

	replaced_str = boost::regex_replace(std::wstring(src), *ir_info->it->regex, str_out);

	if (!replaced_str.size())
	{
		//rise error
		//continue;
		return false;
	}
	if (ir_info->deleted == false)
		// Delete recent added temporary input
		InternalEditor.Delete();

	int src_len = wcslen(src);

	str_out.assign(replaced_str);

	if (src_len)
	{
		int match_len = get_match_len(str_out.c_str(), src);

		if (match_len < 0)
		{
			backspace(0 - match_len);
			match_len = get_match_len(str_out.c_str(), src);
			backspace(str_out.length() - match_len);
		}
		else if (src_len > match_len)
		{
			backspace(src_len - match_len);
		}
		str_out.erase(0, match_len);
	}

	SendStrokes(&str_out);

	if (str_out.length() > 0 && (str_out.at(0) < 0x20 || str_out.at(0) > 0x7F)){
		// Do match again
		MatchRules(0, 0, KeyStates, false);
	}

	if (old_state.CTRL)
		SendKey(VK_CONTROL, 0);
	if (old_state.ALT)
		SendKey(VK_MENU, 0);
	//if (old_state.SHIFT)
	//	SendKey(VK_SHIFT, 0);

	return true;
}

bool MatchRules(wchar_t wcInput, WORD wVk, LPBYTE KeyStates, bool user_input)
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
//	RULES Rules = klf.getRules();
//
//	for (RULES::iterator it = Rules.begin(); it != Rules.end(); it++)
	for (EXPENDEDRULES::iterator it = vtERs.begin(); it != vtERs.end(); it++)
	{
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
				case VK_CONTROL:
					mks.CTRL = true;
					vk_matched = true;
					break;
				case VK_MENU:
					mks.ALT = true;
					vk_matched = true;
					break;
				case VK_SHIFT:
					mks.SHIFT = true;
					vk_matched = true;
					break;
				default:
					if (*vk_it == wVk)
						vk_matched = true;
					break;
				}
			}
			else{
				if (*vk_it == VK_SHIFT){
					if (KeyStates[VK_CAPITAL] & 0x81)
						mks.SHIFT = true;
					else {vk_matched=false;break;}
				}
				else {vk_matched=false;break;}
			}
		}

		if (klf.layout.trackCaps ?
			(it->vk->size() && ( (vk_matched==false) || (old_state.CTRL ^ mks.CTRL) || (old_state.ALT ^ mks.ALT) || ( old_state.SHIFT ^ old_state.CAPS)^ mks.SHIFT ) ):
			(it->vk->size() && ( (vk_matched==false) || (old_state.CTRL ^ mks.CTRL) || (old_state.ALT ^ mks.ALT) || ( old_state.SHIFT ^ mks.SHIFT) ) )
				)
				continue;
		else if (it->vk->size() && wcInput){
			InternalEditor.Delete();
			deleted = true;
		}
		else if (user_input && (old_state.CTRL || old_state.ALT)){continue;}

		wchar_t * src = InternalEditor.GetTextBackward(it->estimated_length);
		if (src) {// Is there enough length to do a match?

			if (wcslen(src) && !wcInput && user_input){continue;}

			boost::wcmatch matches;
			if (boost::regex_match(src, matches, *it->regex)){
				have_match = true; // Got a match
				if (ir_info.matched == false){
					ir_info.matched = true;
					ir_info.it = it;
					ir_info.deleted = (wcInput && user_input) ? deleted : true;
				}
				if (ir_info.it->vk->size() < it->vk->size()) {// compare < VK_* & .. > counts)
					//store
					ir_info.it = it;
					ir_info.deleted = (wcInput && user_input) ? deleted : true;
				}
				else if (ir_info.it->estimated_length + ir_info.it->vk->size() < it->estimated_length + it->vk->size()) {// compare pattern counts
					//store
					ir_info.it = it;
					ir_info.deleted = (wcInput && user_input) ? deleted : true;
				}
				if (ir_info.it->switches->size() < it->switches->size()){
					ir_info.it = it;
					ir_info.deleted = (wcInput && user_input) ? deleted : true;
				}
				//continue matching
			}
		}
		if (deleted == true && wcInput)
			InternalEditor.AddInput(wcInput);
end_of_loop:;
//		delete s_in;
	}
//
	if ((old_state.CTRL ^ old_state.ALT)==false && wcInput > 0x20 && wcInput < 0x7F)
		InternalEditor.setAllSwitchOff();

	if (have_match == true){
		if (ir_info.deleted == true && user_input == true && wcInput)
			InternalEditor.Delete();
		return get_output_and_send(&ir_info, wVk, KeyStates);
	}
//
	if (wcInput){
		//Delete recent added temporary input
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
	WORD scancode = (WORD)(lParam >> 16);
	DWORD dwExtraInfo = GetMessageExtraInfo();
	// If VK_BACK and there is any DontEatBackspace, subtract the DontEatBackspace count from lParam
	if (wVk == VK_BACK && DontEatBackspace > 0 && ((BYTE)scancode == 0xff))
	{
		DontEatBackspace -= lParam & 0xFF;
		if (DontEatBackspace < 0) // lParam is larger then DontEatBackspace
		{ // Eat only previous DontEatBackspace count
			InternalEditor.Delete (0 - DontEatBackspace); // And delete additional backspace (Hack for internal editor)
			DontEatBackspace=0;
		}
		return false;
	}

	// Do not need to do anything, just pass it
	if (wVk == VK_CONTROL || wVk == VK_MENU)
		return false;

	BYTE GlobalKeyStates[256];
	GetKeyboardState(GlobalKeyStates); // Get States

	// Store mod keys to use it later
	old_state.CTRL = GlobalKeyStates[VK_CONTROL] & 0x80;
	old_state.ALT = GlobalKeyStates[VK_MENU] & 0x80;
	old_state.SHIFT = GlobalKeyStates[VK_SHIFT] & 0x80;
	old_state.CAPS = GlobalKeyStates[VK_CAPITAL] & 0x81;

	Debug(L"CTRL = %x ALT = %x\n", GlobalKeyStates[VK_CONTROL], GlobalKeyStates[VK_MENU]);

	wchar_t wcInput = wVk;UINT usVk;
	if (usVk = TranslateToUnicode((WORD*)&wcInput, GlobalKeyStates)) // Get Ascii Value
	{ // If there is any ascii value
		if (MatchRules(wcInput, usVk, GlobalKeyStates, true )) {// Match for the input
			// Found matched
			return true; // Eaten
		}
		else if (wVk == VK_BACK){
			// If VK_BACK
			InternalEditor.Delete(lParam & 0xFF); // Delete from internal editor
		}
		else if ((old_state.CTRL ^ old_state.ALT)==false && wcInput > 0x20 && wcInput < 0x7F){
			return true;
		}
		else {
			InternalEditor.AddInput(wcInput); // Not matched? Just store the input
			return false;
		}
	} else {
		if (MatchRules(NULL, wVk, GlobalKeyStates, true )) {// Match for the input
			// Found matched
			return true; // Eaten
		}
	}
	//If only one of these two keys are pressed
	if (old_state.CTRL ^ old_state.ALT)
	{
		Debug(L"(isCTRL ^ isALT)\n");
		InternalEditor.Restart();// restart the internal editor's buffer
		return false;
	}
	return false;
}