#include "InputProcessor.h"

Kmklf klf;
static int DontEatBackspace = 0;
static bool Delete = true;

static boost::wregex slash(L"[\\(\\)\\[\\]\\{\\}\\|]");
static std::wstring r(L"\\\\$0");

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

int getindextoreplace(int sub_index, boost::wcmatch * matches, charClasses * cc)
{
	charClasses::iterator it;
	for ( it=cc->begin() ; it < cc->end(); it++ )
		if ((*it).idx == sub_index)
		{
			static boost::wregex e(L"\\\\(.)");
			std::wstring ws = boost::regex_replace(std::wstring((*it).start, 1 + (*it).end - (*it).start ), e, std::wstring(L"$1"));
			const wchar_t * w = wcschr(ws.c_str(), *((*matches)[sub_index].first));
			if (w > ws.c_str() + ws.length())
				return -1;
			return w - ws.c_str();
		}
	return -1;
}

std::wstring * makeRegex(WORD * raw_str, int len, LPBYTE KeyStatus, 
						 bool genCapture = true, 
						 boost::wcmatch * matches = NULL,
						 charClasses * cc = NULL)
{
	std::wstring * s = new std::wstring;
	std::vector<wchar_t*> vTstr = klf.getStrings();

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
				int index = getindextoreplace(mod, matches, cc);
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
				s->append(structPd->value);
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

			//if ((lastB = findLastOpenBracket(s)) != -1)
			//{
				//last_raw = raw_str - 4;
				if (*raw_str++ != opPREDEFINED)
				{
					//error
					break;
				}
				len--;
				structPREdef * preDef = getPreDef((emPreDef)*raw_str);
				if (!(KeyStatus[(BYTE)preDef->value[0]] & 0x80))
				{
					delete s;
					return NULL;
				}

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
					if (!(KeyStatus[(BYTE)preDef->value[0]] & 0x80))
					{
						delete s;
						return NULL;
					}
					len--;
				}
				//s->erase(lastB);
				InternalEditor.Delete();
				Delete = false;
				return s;
			//}
			//break;
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

	DontEatBackspace = count;
	
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
			e += 2;
			break;
		}
	}
}

// Release Control - Alt - Shift Keys
void ReleaseCAS()
{
	INPUT ip;

	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0;
	ip.ki.dwExtraInfo = 0;
	ip.ki.dwFlags = KEYEVENTF_KEYUP;
	ip.ki.time = 0;

	ip.ki.wVk = VK_CONTROL;
	SendInput(1, &ip, sizeof(INPUT));

	ip.ki.wVk = VK_MENU;
	SendInput(1, &ip, sizeof(INPUT));

	ip.ki.wVk = VK_SHIFT;
	SendInput(1, &ip, sizeof(INPUT));

}

bool MatchRules(wchar_t wcInput, LPBYTE KeyStatus)
{
#ifdef _DEBUG
	Debug(L"Before:");
#endif
	// Add temporarily
	InternalEditor.AddInput(wcInput);
	std::vector<structRule> Rules = klf.getRules();

	for (std::vector<structRule>::iterator it = Rules.begin(); it != Rules.end(); it++)
	{
		Delete = true;

		size_t len = wcslen((wchar_t*)(*it).strInRule);
		std::wstring * s_in = makeRegex((*it).strInRule, len, KeyStatus);

		if (!s_in)
			continue;

		wchar_t * src = InternalEditor.GetTextBackward(estimate_len(s_in->c_str()));		
		if (s_in->size() && src)
		{
			boost::wregex e(s_in->c_str());
			boost::wcmatch matches;
			if (boost::regex_match(src, matches, e))
			{
				charClasses cc;
				extractCharClasses(s_in->c_str(), &cc);

				len = wcslen((wchar_t*)(*it).strOutRule);
				std::wstring * s_out = makeRegex((*it).strOutRule, len, KeyStatus, false, &matches, &cc);

				ReleaseCAS();

				if (!s_out->length())
				{
					//Delete recent added temporary input
					InternalEditor.Delete();

					backspace(wcslen(src));

					delete s_in;
					delete s_out;

#ifdef _DEBUG
					Debug(L"After\n");
#endif
					return true;
				}

				std::wstring replaced_str;

				replaced_str = boost::regex_replace(std::wstring(src), e, *s_out);

				if (!replaced_str.size())
				{
					//rise error
					continue;
				}

				if (Delete)
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
					}
					else if (src_len > match_len)
					{
						backspace(src_len - match_len);
					}
					s_out->erase(0, match_len);
				}

				SendStrokes(s_out);

				delete s_in;
				delete s_out;
#ifdef _DEBUG
				Debug(L"After\n");
#endif
				return true;
			}
			else if (Delete == false)
			{
				InternalEditor.AddInput(wcInput);
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
	Debug(L"After\n");
#endif

	return false;
}

//Param  -	wcInput (wchar_t) : Input to process : NOT NULL
//return -	true : Input was eatten.
//			false : Input was not processed.
bool ProcessInput(wchar_t wcInput, LPARAM lParam)
{
	bool isCTRL, isALT;

	if (wcInput == VK_BACK && DontEatBackspace)
	{
		DontEatBackspace -= lParam & 0xFF;
		return false;
	}

	if (wcInput == VK_CONTROL || wcInput == VK_MENU)
		return false;

	BYTE KeyStatus[256];
	GetKeyboardState(KeyStatus);

	isCTRL = KeyStatus[VK_CONTROL] & 0x80;
	isALT = KeyStatus[VK_MENU] & 0x80;

	if ((isCTRL == true && isALT == false) || (isCTRL == false && isALT == true))
	{
		Debug(L"(isCTRL == true && isALT == false) || (isCTRL == false && isALT == true)\n");
		InternalEditor.Restart();
		return false;
	}

	wchar_t str[30];
	swprintf(str, L"CTRL = %x ALT = %x\n", KeyStatus[VK_CONTROL], KeyStatus[VK_MENU]);
	Debug(str);

	if (TranslateToAscii((UINT*)&wcInput))
	{
		if (MatchRules(wcInput, KeyStatus ))
		{
			return true;
		}
	}
	return false;
}