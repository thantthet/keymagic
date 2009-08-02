#include "InputProcessor.h"

Kmklf klf;
static int DontEatBackspace = 0;
static bool do_delete = true;
static int keys_length = 0;

static boost::wregex slash(L"[()\\[\\]{}|.\\\\]");
static std::wstring r(L"\\\\$0");

typedef std::vector<structRule> RULES;

struct ModKeysStatus
{
	bool CTRL;
	bool ALT;
	bool SHIFT;
};

static ModKeysStatus old_status;

struct InputRuleInfo
{
	std::wstring expression;
	int context_length;
	int keys_length;
	boost::wregex regex;
	RULES::iterator it;
	bool do_delete;
};

bool AppendVariableValue(std::vector<wchar_t*> vTstr, int index, std::wstring * s)
{
	wchar_t * str = vTstr.at(--index);
	int len = wcslen(str);
	for (int i = 0; i < len; i++)
	{
		if (str[i] < vTstr.size())
		{
			if (!AppendVariableValue(vTstr, index, s))
				return false;
		}
		else
		{
			s->append(boost::regex_replace(std::wstring((wchar_t*)str), slash, r));
			return true;
		}
	}
	return false;
}

int findLastOpenBracket(std::wstring * s)
{
	for ( int rit = s->length() - 1; rit >= 0; rit-- )
	{
		if (s->at(rit) == '(')
		{
			if (rit && s->at(rit-1) == '\\')
				break;

			return rit;
		}
	}
	return -1;
}

struct structCClass
{
	int idx;
	const wchar_t * start;
	const wchar_t * end;
} ;

typedef std::vector<structCClass> charClasses;

int getindextoreplace(int sub_index, boost::wcmatch matches, charClasses * cc)
{
	charClasses::iterator it;
	for ( it=cc->begin() ; it < cc->end(); it++ )
		if (it->idx == sub_index)
		{
			static boost::wregex e(L"\\\\(.)");
			std::wstring ws = boost::regex_replace(std::wstring((*it).start, 1 + (*it).end - (*it).start ), e, std::wstring(L"$1"));
			const wchar_t * w = wcschr( ws.c_str(), *matches[sub_index].first );
			if (w > ws.c_str() + ws.length())
				return -1;
			return w - ws.c_str();
		}
	return -1;
}

std::wstring * makeRegex(WORD * raw_str, int len, WORD wVk, LPBYTE KeyStatus, 
						 bool genCapture = true, 
						 boost::wcmatch * matches = NULL,
						 charClasses * cc = NULL)
{
	bool matched;
	std::wstring * s = new std::wstring(L"");
	std::vector<wchar_t*> vTstr = klf.getStrings();

	ModKeysStatus m_status;
	memset(&m_status, 0, sizeof(m_status));

	while (len > 0)
	{
		short index, mod;
		wchar_t dec[20];
		size_t size;
		int lastB;
		//WORD * last_raw;
		static int lastLength;

		len--;
		switch (*raw_str++)
		{
		case opSTRING:
			size = *raw_str++; len--;

			if (genCapture)
				s->append(L"(");

			s->append(boost::regex_replace(std::wstring((wchar_t*)raw_str, size), slash, r));
			if (genCapture)
				s->append(L")");

			len -= size; raw_str += size;
			break;
		case opVARIABLE:
			index = (short)*raw_str++; len--;

			lastLength = s->length();

			if (genCapture)
				s->append(L"(");
			AppendVariableValue(vTstr, index, s);
			if (genCapture)
				s->append(L")");

			break;
		case opMODIFIER:
			mod = (short)*raw_str++; len--;

			switch (mod)
			{
			case opANYOF:
				if ((lastB = findLastOpenBracket(s)) != -1)
				{
					s->insert(lastB+1, 1, L'[');
					s->insert(s->length()-1, 1, L']');
					break;
				}
				break;
			default:
				if (!matches)
					return false;
				if (mod > matches->size())
					return false;
				int index = getindextoreplace(mod, *matches, cc);
				if (lastLength + index >= s->length())
					return false;
				wchar_t wc = s->at(lastLength + index);
				s->erase(lastLength);
				s->append(1, wc);
				break;
			}

			break;
		case opPREDEFINED:
			if (genCapture)
				s->append(L"(");
			if (structPREdef * structPd = getPreDef((emPreDef)*raw_str++))
			{
				s->append(boost::regex_replace(std::wstring((wchar_t*)structPd->value), slash, r));
				len --;
			}
			if (genCapture)
				s->append(L")");
			break;
		case opREFERENCE:
			index = (short)*raw_str++; len--;

			wchar_t buffer [33];
			swprintf(buffer,L"%d",index);
			s->append(L"$");
			s->append(buffer);
			break;
		case opAND:
			matched = false;

			if (*raw_str++ != opPREDEFINED)
			{
				//error
				break;
			}
			len--;
			structPREdef * preDef = getPreDef((emPreDef)*raw_str);
			switch (preDef->value[0])
			{
			case VK_MENU:
				m_status.ALT = true;
				break;
			case VK_CONTROL:
				m_status.CTRL = true;
				break;
			case VK_SHIFT:
				m_status.SHIFT = true;
				break;
			}
			if (preDef->value[0] == wVk)
			{
				matched = true;
			}
			if (!(KeyStatus[(BYTE)preDef->value[0]] & 0x80))
			{
				delete s;
				return NULL;
			}

			keys_length++;
			raw_str++;
			len--;

			while(len)
			{
				if (*raw_str++ != opPREDEFINED)
				{
					//error
					break;
				}
				len--;
				preDef = getPreDef((emPreDef)*raw_str++);
				switch (preDef->value[0])
				{
				case VK_MENU:
					m_status.ALT = true;
					break;
				case VK_CONTROL:
					m_status.CTRL = true;
					break;
				case VK_SHIFT:
					m_status.SHIFT = true;
					break;
				}
				if (preDef->value[0] == wVk)
				{
					matched = true;
				}
				if (!(KeyStatus[(BYTE)preDef->value[0]] & 0x80))
				{
					delete s;
					return NULL;
				}
				len--;
				keys_length++;
			}
			
			if (
				!matched || 
				m_status.CTRL ^ old_status.CTRL || 
				m_status.ALT ^ old_status.ALT || 
				m_status.SHIFT ^ old_status.SHIFT
				)
			{
				delete s;
				return NULL;
			}

			InternalEditor.Delete();
			do_delete = false;
			return s;
			break;
		}
	}
	return s;
}

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
		SendInput(1, &ip, sizeof(INPUT));
	}

	InternalEditor.AppendText(s->c_str(), s->length());

}

void backspace(int count)
{
	if (!GetFocus() || count < 1)
		return ;

	DontEatBackspace += count;
	
	/*while(count--)
	{
		keybd_event(VK_BACK, 255, 0, 0);
		keybd_event(VK_BACK, 2, KEYEVENTF_KEYUP, 0);
		InternalEditor.Delete();
	}*/
	INPUT * ip = new INPUT[count+1];
	int i;
	for(i=0; i < count; i++){
		ip[i].type = INPUT_KEYBOARD;
		ip[i].ki.wScan = 0x0e;
		ip[i].ki.dwExtraInfo = 0;
		ip[i].ki.dwFlags = 0;
		ip[i].ki.wVk = VK_BACK;
		ip[i].ki.time = 0;
	}

	ip[i].type = INPUT_KEYBOARD;
	ip[i].ki.wScan = 0x0e;
	ip[i].ki.dwExtraInfo = 0;
	ip[i].ki.dwFlags = KEYEVENTF_KEYUP;
	ip[i].ki.wVk = VK_BACK;
	ip[i].ki.time = 0;

	SendInput(count, ip, sizeof(INPUT));
	InternalEditor.Delete(count);
	delete[] ip;
}

int estimate_len (const wchar_t * pattern)
{
	int length = 0;
	bool opened_square_bracket = false;

	while (*pattern)
	{
		switch (*pattern++)
		{
		case '[':
			if (!opened_square_bracket)
			{
				opened_square_bracket = true;
				length ++ ;
			}
			break;

		case ']':
			if (opened_square_bracket)
				opened_square_bracket = false;
			else
				length ++ ;
			break;

		case '(':
		case ')':
			break;
		
		case '\\':
			pattern ++;
			if (!opened_square_bracket)
				length++;
			break;
		default:
			if (!opened_square_bracket)
				length ++ ;
		}
	}
	return length;
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

void extractCharClasses(const wchar_t * e, charClasses * cc)
{
	bool opened_square_bracket = false;
	int index = 0;
	structCClass scc;

	while (*e)
	{
		switch (*e++)
		{
		case '[':
			if (!opened_square_bracket)
			{
				scc.start = e;
				scc.idx = index;

				opened_square_bracket = true;
			}
			break;

		case ']':
			if (opened_square_bracket)
			{
				scc.end = e - 2 ;
				cc->push_back(scc);

				opened_square_bracket = false;
			}
			break;

		case '(':
			index ++ ;
			break;
		case ')':
			if (!opened_square_bracket)
			{
				//error;
			}
			break;
		
		case '\\':
			e ++;
			break;
		}
	}
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

bool get_output_and_send(InputRuleInfo * ir_info, WORD wVk, LPBYTE KeyStatus)
{
	boost::wcmatch matches;
	charClasses cc;

	wchar_t * src = InternalEditor.GetTextBackward(ir_info->context_length);
	if (!src)
		return false;
	extractCharClasses(ir_info->expression.c_str(), &cc);

	boost::regex_match(src, matches, ir_info->regex);

	int len = wcslen((wchar_t*)ir_info->it->strOutRule);
	std::wstring * s_out = makeRegex(ir_info->it->strOutRule, len, wVk, KeyStatus, false, &matches, &cc);

	if (!s_out)
		return false;

	SendKey(VK_CONTROL, KEYEVENTF_KEYUP);
	SendKey(VK_MENU, KEYEVENTF_KEYUP);
	SendKey(VK_SHIFT, KEYEVENTF_KEYUP);

	if (!s_out->length())
	{
		if (ir_info->do_delete)
		{
			// Delete recent added temporary input
			InternalEditor.Delete();
		}

		backspace(wcslen(src));

		if (old_status.CTRL)
			SendKey(VK_CONTROL, 0);
		if (old_status.CTRL)
			SendKey(VK_MENU, 0);
		if (old_status.SHIFT)
			SendKey(VK_SHIFT, 0);

		delete s_out;

		return true;
	}

	std::wstring replaced_str;

	replaced_str = boost::regex_replace(std::wstring(src), ir_info->regex, *s_out);

	if (!replaced_str.size())
	{
		//rise error
		//continue;
		return false;
	}

	if (ir_info->do_delete)
	{
		// Delete recent added temporary input
		InternalEditor.Delete();
	}

	int src_len = wcslen(src);

	s_out->assign(replaced_str);

	if (src_len)
	{
		int match_len = get_match_len(s_out->c_str(), src);

		if (match_len < 0)
		{
			backspace(0 - match_len);
			match_len = get_match_len(s_out->c_str(), src);
			backspace(s_out->length() - match_len);
		}
		else if (src_len > match_len)
		{
			backspace(src_len - match_len);
		}
		s_out->erase(0, match_len);
	}

	SendStrokes(s_out);

	MatchRules(0, 0, KeyStatus);

	if (old_status.CTRL)
		SendKey(VK_CONTROL, 0);
	if (old_status.ALT)
		SendKey(VK_MENU, 0);
	if (old_status.SHIFT)
		SendKey(VK_SHIFT, 0);

	delete s_out;

	return true;
}

bool MatchRules(wchar_t wcInput, WORD wVk, LPBYTE KeyStatus)
{
	InputRuleInfo ir_info;
	bool have_match = false;

	ir_info.keys_length = 0;
#ifdef _DEBUG
	Debug(L"Before:");
#endif
	// Add temporarily
	InternalEditor.AddInput(wcInput);
	RULES Rules = klf.getRules();

	for (RULES::iterator it = Rules.begin(); it != Rules.end(); it++)
	{
		do_delete = true;

		size_t len = wcslen((wchar_t*)it->strInRule);
		keys_length = 0xDEAD0000;
		std::wstring * s_in = makeRegex(it->strInRule, len, wVk, KeyStatus, true);

		if (!s_in)
			continue;

		int context_length = estimate_len(s_in->c_str());

		wchar_t * src = InternalEditor.GetTextBackward(context_length);
		if (src)
		{
			boost::wregex e(s_in->c_str());
			boost::wcmatch matches;
			if (boost::regex_match(src, matches, e))
			{
				have_match = true;
				if (
					(ir_info.keys_length < keys_length) 
					|| 
					(ir_info.expression.length() + ir_info.keys_length < s_in->length() + keys_length)
					)
				{
					ir_info.expression = *s_in;
					ir_info.context_length = context_length;
					ir_info.keys_length = keys_length;
					ir_info.regex = e;
					ir_info.it = it;
					ir_info.do_delete = do_delete;
				}
			}
		}
		if (do_delete == false)
				InternalEditor.AddInput(wcInput);
		delete s_in;
	}

	if (have_match == true)
	{
		if (ir_info.do_delete == false)
			InternalEditor.Delete();
		return get_output_and_send(&ir_info, wVk, KeyStatus);
	}

	if (wcInput)
	{
		//Delete recent added temporary input
		InternalEditor.Delete();
	}

	return false;
}

//Param  -	wVk (WORD) : Input to process : NOT NULL
//return -	true : Input was eatten.
//			false : Input was not processed.
bool ProcessInput(WORD wVk, LPARAM lParam)
{
	if (wVk == VK_BACK && DontEatBackspace > 0)
	{
		DontEatBackspace -= lParam & 0xFF;
		if (DontEatBackspace < 0)
		{
			DontEatBackspace=0;
			backspace (0 - DontEatBackspace);
		}
		return false;
	}

	if (wVk == VK_CONTROL || wVk == VK_MENU)
		return false;

	BYTE KeyStatus[256];
	GetKeyboardState(KeyStatus);

	old_status.CTRL = KeyStatus[VK_CONTROL] & 0x80;
	old_status.ALT = KeyStatus[VK_MENU] & 0x80;
	old_status.SHIFT = KeyStatus[VK_SHIFT] & 0x80;

	Debug(L"CTRL = %x ALT = %x\n", KeyStatus[VK_CONTROL], KeyStatus[VK_MENU]);

	wchar_t wcInput = wVk;
	if (TranslateToAscii((WORD*)&wcInput))
	{
		if (MatchRules(wcInput, wVk, KeyStatus ))
		{
			return true;
		}
		else if (wVk == VK_BACK)
		{
			InternalEditor.Delete(lParam & 0xFF);
		}
		else
			InternalEditor.AddInput(wcInput);
	}

	if (old_status.CTRL ^ old_status.ALT)
	{
		Debug(L"(isCTRL ^ isALT)\n");
		InternalEditor.Restart();
		return false;
	}
	return false;
}