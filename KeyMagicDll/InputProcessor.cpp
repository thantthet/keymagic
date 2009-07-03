#include "InputProcessor.h"
#include "common.h"

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

std::wstring * MakePureStr(WORD * raw_str, int len)
{
	std::wstring * s = new std::wstring;
	std::vector<wchar_t*> vTstr = klf.getStrings();

	while (len)
	{
		short index;
		wchar_t dec[20];
		size_t size;

		switch (*raw_str)
		{
		case opSTRING:
			raw_str++;
			len--;
			size = *raw_str++;
			len--;
			s->append((wchar_t*)raw_str, size);	
			len -= size;
			raw_str += size;
			break;
		case opVARIABLE:
			raw_str++;
			len--;
			index = (short)*raw_str++;
			len--;
			AppendVariableValue(vTstr, index, s);
			break;
		case opPREDEFINED:
			raw_str++;
			if (structPREdef * structPd = getPreDef((emPreDef)*raw_str++))
			{
				s->append(structPd->value);
			}
			break;
		case opREFERENCE:
			raw_str++;
			len--;
			index = (short)*raw_str++;
			len--;
			wsprintfW(dec, L"\\%d", index);
			s->append(dec);
			break;
		default:
			len--;
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
	if (!GetFocus())
		return ;

	DontEatBackspace = count;
	
	while(count--)
	{
		keybd_event(VK_BACK, 255, 0, 0);
		keybd_event(VK_BACK, 2, KEYEVENTF_KEYUP, 0);
		InternalEditor.Delete();
	}

}

bool MatchRules(wchar_t wcInput)
{
	OutputDebugString("Before\n");
	InternalEditor.dump();
	// Append temporarily
	InternalEditor.AppendText(&wcInput, 1);
	std::vector<structRule> Rules = klf.getRules();

	for (std::vector<structRule>::iterator it = Rules.begin(); it != Rules.end(); it++)
	{
		size_t len = wcslen((wchar_t*)(*it).strInRule);
		std::wstring * s_in = MakePureStr((*it).strInRule, len);

		wchar_t * src = InternalEditor.GetTextBackward(s_in->length());
		if (s_in->size() && src)
		{
			if (!wcscmp(src, s_in->c_str()))
			{	
				//Delete recent appended temporary input
				InternalEditor.Delete();

				len = wcslen((wchar_t*)(*it).strOutRule);
				std::wstring * s_out = MakePureStr((*it).strOutRule, len);

				s_in->find_first_not_of

				backspace(s_in->length()-1);

				SendStrokes(s_out);

				delete s_in;
				delete s_out;
				OutputDebugString("Before\n");
				InternalEditor.dump();
				return true;
			}
		}
		delete s_in;
	}
	//Delete recent appended temporary input
	InternalEditor.Delete();

	if (wcInput == VK_BACK)
		InternalEditor.Delete();
	else
		InternalEditor.AppendText(&wcInput, 1);

	OutputDebugString("After\n");
	InternalEditor.dump();

	return false;
}

//Param  -	wcInput (wchar_t) : Input to process : NOT NULL
//return -	true : Input was eatten.
//			false : Input was not processed.
bool ProcessInput(wchar_t wcInput)
{
	if (TranslateToAscii((UINT*)&wcInput))
	{
		if (wcInput == VK_BACK && DontEatBackspace)
		{
			DontEatBackspace--;
			return false;
		}
		else if (MatchRules(wcInput))
		{
			return true;
		}
	}
	return false;
}