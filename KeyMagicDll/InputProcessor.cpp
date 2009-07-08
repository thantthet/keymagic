#include "InputProcessor.h"

Kmklf klf;
static int DontEatBackspace = 0;

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
			s->append(str);
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

std::wstring * makeRegex(WORD * raw_str, int len, LPBYTE KeyStatus)
{
	std::wstring * s = new std::wstring;
	std::vector<wchar_t*> vTstr = klf.getStrings();

	while (len > 0)
	{
		short index, mod;
		wchar_t dec[20];
		size_t size;
		int lastB;
		WORD * last_raw;

		len--;
		switch (*raw_str++)
		{
		case opSTRING:
			size = *raw_str++; len--;

			s->append(L"(");
			s->append((wchar_t*)raw_str, size);	
			s->append(L")");

			len -= size; raw_str += size;
			break;
		case opVARIABLE:
			index = (short)*raw_str++; len--;

			s->append(L"(");
			AppendVariableValue(vTstr, index, s);
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
				// error occur
				break;
			default:
				break;
			}

			break;
		case opPREDEFINED:
			s->append(L"(");
			if (structPREdef * structPd = getPreDef((emPreDef)*raw_str++))
			{
				s->append(structPd->value);
				len --;
			}
			s->append(L")");
			break;
		case opREFERENCE:
			index = (short)*raw_str++; len--;
			s->push_back(index);
			break;
		case opAND:

			if ((lastB = findLastOpenBracket(s)) != -1)
			{
				last_raw = raw_str - 3;
				if (*last_raw++ != opPREDEFINED)
				{
					//error
					break;
				}
				structPREdef * preDef = getPreDef((emPreDef)*last_raw);
				if (!(KeyStatus[(BYTE)preDef->value[0]] & 0x80))
				{
					delete s;
					return NULL;
				}

				raw_str++;

				for (int i = 1; 0 < len; i++, len -= 3, raw_str += 3)
				{
					preDef = getPreDef((emPreDef)*raw_str);
					if (!(KeyStatus[(BYTE)preDef->value[0]] & 0x80))
					{
						delete s;
						return NULL;
					}
				}
				s->erase(lastB, std::string::npos);
				return s;
			}
			break;
		}
	}
	return s;
}

void SendStrokes (std::wstring * s)//Send Keys Strokes
{
	HWND hwnd = GetFocus();
	int i;

	INPUT ip;
	ip.type = INPUT_KEYBOARD;
	ip.ki.dwExtraInfo = 0;
	ip.ki.dwFlags = KEYEVENTF_UNICODE;
	ip.ki.wVk = 0;
	ip.ki.time = 0;

	for(i=0; i < s->length(); i++){
		ip.ki.wScan = s->at(i);
		SendInput(1, &ip, sizeof(INPUT));
	}

	InternalEditor.AppendText(s->c_str(), s->length());

}

void backspace(int count)
{
	if (!GetFocus() || count < 1)
		return ;

	DontEatBackspace = count;
	
	while(count--)
	{
		keybd_event(VK_BACK, 255, 0, 0);
		keybd_event(VK_BACK, 2, KEYEVENTF_KEYUP, 0);
		InternalEditor.Delete();
	}

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
			opened_square_bracket = true;
			length ++ ;
			break;

		case ']':
			opened_square_bracket = false;
			break;

		case '(':
		case ')':
			break;
		
		case '\\':
			pattern += 2;
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
bool MatchRules(wchar_t wcInput, LPBYTE KeyStatus)
{
#ifdef _DEBUG
	OutputDebugString("Before:");
#endif
	// Add temporarily
	InternalEditor.AddInput(wcInput);
	std::vector<structRule> Rules = klf.getRules();

	for (std::vector<structRule>::iterator it = Rules.begin(); it != Rules.end(); it++)
	{
		size_t len = wcslen((wchar_t*)(*it).strInRule);
		std::wstring * s_in = makeRegex((*it).strInRule, len, KeyStatus);

		if (!s_in)
			continue;

		wchar_t * src = InternalEditor.GetTextBackward(estimate_len(s_in->c_str()));		
		if (s_in->size() && src)
		{
			Regex * r = new Regex(s_in->c_str());

			if (r->test(src))
			{

				len = wcslen((wchar_t*)(*it).strOutRule);
				std::wstring * s_out = makeRegex((*it).strOutRule, len, KeyStatus);

				if (!s_out->length())
				{
					//Delete recent added temporary input
					InternalEditor.Delete();

					backspace(wcslen(src));

					delete s_in;
					delete s_out;

#ifdef _DEBUG
					OutputDebugString("After\n");
#endif
					return true;
				}

				wchar_t replaced_str[100];
				r->sub(src, s_out->c_str(), replaced_str);

				if (*replaced_str == NULL)
				{
					//rise error
					continue;
				}

				int src_len = wcslen(src);

				s_out->assign(replaced_str);

				if (src_len)
				{
					int match_len = get_match_len(s_out->c_str(), src);
					// Delete recent added temporary input
					InternalEditor.Delete();

					if (match_len < 0)
					{
						match_len = 0;
						backspace(src_len);
					}
					else
						backspace((src_len-1) - match_len);
					s_out->erase(0, match_len);
				}

				SendStrokes(s_out);

				delete s_in;
				delete s_out;
#ifdef _DEBUG
				OutputDebugString("After\n");
#endif
				return true;
			}
		}
		delete s_in;
	}
	//Delete recent added temporary input
	InternalEditor.Delete();

	if (wcInput == VK_BACK)
		InternalEditor.Delete();
	else
		InternalEditor.AddInput(wcInput);

#ifdef _DEBUG
	OutputDebugString("After\n");
#endif

	return false;
}

//Param  -	wcInput (wchar_t) : Input to process : NOT NULL
//return -	true : Input was eatten.
//			false : Input was not processed.
bool ProcessInput(wchar_t wcInput)
{
	bool isCTRL, isALT;

	if (wcInput == VK_CONTROL || wcInput == VK_MENU)
		return false;

	BYTE KeyStatus[256];
	GetKeyboardState(KeyStatus);

/*	isCTRL = KeyStatus[VK_CONTROL] & 0x80;
	isALT = KeyStatus[VK_MENU] & 0x80;

	if (isCTRL || isALT)
		return false;*/

	wchar_t str[30];
	swprintf(str, L"CTRL = %x ALT = %x\n", KeyStatus[VK_CONTROL], KeyStatus[VK_MENU]);
	Debug(str);

	if (TranslateToAscii((UINT*)&wcInput))
	{
		if (wcInput == VK_BACK && DontEatBackspace)
		{
			DontEatBackspace--;
			return false;
		}
		else if (MatchRules(wcInput, KeyStatus ))
		{
			return true;
		}
	}
	return false;
}