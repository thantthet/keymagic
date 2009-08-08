#include "InputProcessor.h"

// Backsapce counts that won't eat
static int DontEatBackspace = 0;
// If context is matching with VK_*, do delete is false
//static bool do_delete = true;
// < VK_* & VK_* & ... > length
static int keys_length = 0;

struct ModKeysStatus
{
	bool CTRL;
	bool ALT;
	bool SHIFT;
	bool CAPS;
};

static ModKeysStatus old_status;

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

	INPUT ip;
	ip.type = INPUT_KEYBOARD;
	ip.ki.dwExtraInfo = 0;
	ip.ki.dwFlags = KEYEVENTF_UNICODE;
	ip.ki.wVk = 0;
	ip.ki.time = 0;

	for(int i=0; i < s->length(); i++){
		ip.ki.wScan = s->at(i);
		ip.ki.dwFlags = KEYEVENTF_UNICODE;
		SendInput(1, &ip, sizeof(INPUT));

		ip.ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_UNICODE;
		SendInput(1, &ip, sizeof(INPUT));
	}

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
	if (!makeRegex(&str_out, &vks, ir_info->it->rule_it->strOutRule, len, false, &matches, &ir_info->it->cc))
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

		if (old_status.CTRL)
			SendKey(VK_CONTROL, 0);
		if (old_status.CTRL)
			SendKey(VK_MENU, 0);
		//if (old_status.SHIFT)
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

	// Do match again
	MatchRules(0, 0, KeyStates, false);

	if (old_status.CTRL)
		SendKey(VK_CONTROL, 0);
	if (old_status.ALT)
		SendKey(VK_MENU, 0);
	//if (old_status.SHIFT)
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

//		size_t len = wcslen((wchar_t*)it->strInRule);
//		keys_length = 0xDEAD0000;// just a prefix value ;D
//		std::wstring * s_in = makeRegex(it->strInRule, len, wVk, KeyStates, true);
//
//		if (!s_in)
//			continue;
//
//		int context_length = estimate_len(s_in->c_str());
		ModKeysStatus mks;
		memset(&mks, 0, sizeof(ModKeysStatus));
		for (VIRTUALKEYS::iterator vk_it = it->vk->begin(); vk_it != it->vk->end(); vk_it++)
		{
			if (KeyStates[*vk_it] & 0x80)
			{
				switch (*vk_it)
				{
				case VK_CONTROL:
					mks.CTRL = true;
					break;
				case VK_MENU:
					mks.ALT = true;
					break;
				case VK_SHIFT:
					mks.SHIFT = true;
					break;
				default:
					if (*vk_it == wVk)
						vk_matched = true;
					break;
				}
			}
			else
			{
				if (*vk_it == VK_SHIFT)
				{
					if (KeyStates[VK_CAPITAL] & 0x81)
						mks.SHIFT = true;
					else 
					{
						vk_matched = false;
						break;
					}
				}
				else
				{
					vk_matched = false;
					break;
				}
			}
		}

		if (it->vk->size() && ( (vk_matched == false) || (old_status.CTRL ^ mks.CTRL) || (old_status.ALT ^ mks.ALT) || ( (old_status.SHIFT ^ old_status.CAPS) ^ mks.SHIFT) ) )
			continue;
		else if (it->vk->size())
		{
			InternalEditor.Delete();
			deleted = true;
		}
		else if (user_input && (old_status.CTRL || old_status.ALT))
		{
			continue;
		}

		wchar_t * src = InternalEditor.GetTextBackward(it->estimated_length);
		if (src) // Is there enough length to do a match?
		{
//			boost::wregex e(s_in->c_str());
			boost::wcmatch matches;
			if (boost::regex_match(src, matches, *it->regex))
			{
				have_match = true; // Got a match
				if (ir_info.matched == false)
				{
					ir_info.matched = true;
					ir_info.it = it;
					ir_info.deleted = user_input ? deleted : true;
				}

				if (ir_info.it->vk->size() < it->vk->size()) // compare < VK_* & .. > counts)
				{
					//store
					ir_info.it = it;
					ir_info.deleted = user_input ? deleted : true;
				}
				else if (ir_info.it->estimated_length + ir_info.it->vk->size() < it->estimated_length + it->vk->size()) // compare pattern counts
				{
					//store
					ir_info.it = it;
					ir_info.deleted = user_input ? deleted : true;
				}
				//continue matching
			}
		}
		if (deleted == true)
			InternalEditor.AddInput(wcInput);
//		delete s_in;
	}
//
	if (have_match == true)
	{
		if (ir_info.deleted == true && user_input == true)
			InternalEditor.Delete();
		return get_output_and_send(&ir_info, wVk, KeyStates);
	}
//
	if (wcInput)
	{
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
	// If VK_BACK and there is any DontEatBackspace, reduce the DontEatBackspace count from lParam
	if (wVk == VK_BACK && DontEatBackspace > 0 && ((BYTE)scancode == 0xff))
	{
		DontEatBackspace -= lParam & 0xFF;
		if (DontEatBackspace < 0) // lParam is greater then DontEatBackspace
		{ // Eat only previous DontEatBackspace count
			InternalEditor.Delete (0 - DontEatBackspace); // And delete additional backspace (Hack for internal editor)
			DontEatBackspace=0;
		}
		return false;
	}

	// Do not need to do anything, just pass it
	if (wVk == VK_CONTROL || wVk == VK_MENU)
		return false;

	BYTE KeyStates[256];
	GetKeyboardState(KeyStates); // Get States

	// Store mod keys to use it later
	old_status.CTRL = KeyStates[VK_CONTROL] & 0x80;
	old_status.ALT = KeyStates[VK_MENU] & 0x80;
	old_status.SHIFT = KeyStates[VK_SHIFT] & 0x80;
	old_status.CAPS = KeyStates[VK_CAPITAL] & 0x81;

	Debug(L"CTRL = %x ALT = %x\n", KeyStates[VK_CONTROL], KeyStates[VK_MENU]);

	wchar_t wcInput = wVk;
	if (TranslateToAscii((WORD*)&wcInput)) // Get Ascii Value
	{ // If there is any ascii value
		if (MatchRules(wcInput, wVk, KeyStates, true )) // Match for the input
		{// Found matched
			return true; // Eaten
		}
		else if (wVk == VK_BACK)
		{// If VK_BACK
			InternalEditor.Delete(lParam & 0xFF); // Delete from internal editor
		}
		else
			InternalEditor.AddInput(wcInput); // Not matched? Just store the input
	}
	//If only one of these two key are pressed
	if (old_status.CTRL ^ old_status.ALT)
	{
		Debug(L"(isCTRL ^ isALT)\n");
		InternalEditor.Restart();// restart the internal editor's buffer
		return false;
	}
	return false;
}