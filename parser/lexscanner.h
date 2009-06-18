#include "global.h"

class lexscanner
{

public:
	lexscanner()
	{
		init_vars();
	}
	lexscanner(script s_input)
	{
		init_vars();

		s = s_input;
		scan();
	}
	vector<tyObject> getObjects()
	{
		return objects;
	}

private:
	vector<tyObject> objects;
	wchar_t * wPlus, * wASSIGN, * wPRINT, * wAND , * wStar;
	script s;

	void init_vars()
	{
		wPlus = L"+";
		wASSIGN = L"=";
		wPRINT = L"=>";
		wAND = L"&";
		wStar = L"*";
	}

	bool scan ()
	{
		int scannedIndex = 0, lineNum=1;

		while (s.wCharAt(scannedIndex) != EOS)
		{
			tyObject kObject;
			int retlen;

			if (s.wCharAt(scannedIndex) == '\r' || s.wCharAt(scannedIndex) == '\n')
			{
				kObject.iLength = 2;
				kObject.iStartIndex = scannedIndex;
				kObject.Type = T_NEWLINE;
				kObject.Value = _T("\r\n");
				objects.push_back(kObject);

				DumpObject(_T("New Object Assigned:"), kObject);

				scannedIndex+=2;
				continue;
			}

			while (s.wCharAt(scannedIndex) == ' ' || s.wCharAt(scannedIndex) == '\t') {scannedIndex++; }

			switch (s.wCharAt(scannedIndex))
			{
			case '[':
				if (s.wCharAt(scannedIndex+2) == ']')
				{
					wchar_t* wModer = new wchar_t[2];
					wModer[1]=NULL;
					wModer[0]=s.wCharAt(scannedIndex+1);

					kObject.iLength = 3;
					kObject.iStartIndex = scannedIndex;
					kObject.Type = T_MODIFIER;
					kObject.Value = wModer;
					objects.push_back(kObject);

					scannedIndex += 3;
				}
				else
				{
					wchar_t str[50];
					swprintf(str, _T("ERROR: Parse error => Line: %d Pos: %d\n"), s.getLineNum(scannedIndex), s.getPosLine(scannedIndex));
					Debug(str);
					Exit(0);
				}
				break;
			case '$':
				retlen = reference(s, scannedIndex);
				if (retlen > 0)
					scannedIndex += retlen;
				else
				{
					wchar_t * sEnd = WholeWord(s.lpwCharAt(scannedIndex+1));
					if (sEnd == s.lpwCharAt(scannedIndex))
						return false;
					scannedIndex++;
					int wlen = sEnd - s.lpwCharAt(scannedIndex);
					wchar_t * wNew = new wchar_t[wlen+1];
					wcsncpy(wNew, s.lpwCharAt(scannedIndex), wlen);
					wNew[wlen] = NULL;

					kObject.iLength = wlen;
					kObject.iStartIndex = scannedIndex;
					kObject.Type = T_IDENTIFIER;
					kObject.Value = wNew;

					objects.push_back(kObject);
					scannedIndex += wlen;

					DumpObject(_T("New Object Assigned:"), kObject);
				}
				break;
			case '+':
			case '&':
				kObject.iLength = 1;
				kObject.iStartIndex = scannedIndex;
				kObject.Type = T_ANDOP;
				kObject.Value = wPlus;
				objects.push_back(kObject);

				DumpObject(_T("New Object Assigned:"), kObject);

				scannedIndex++;
				break;
			case '=':
				if (s.wCharAt(scannedIndex+1) == '>')
				{
					kObject.iLength = 2;
					kObject.iStartIndex = scannedIndex;
					kObject.Type = T_PRINT;
					kObject.Value = wPRINT;
					objects.push_back(kObject);

					scannedIndex++;
				}
				else
				{
					kObject.iLength = 1;
					kObject.iStartIndex = scannedIndex;
					kObject.Type = T_ASSIGN;
					kObject.Value = wASSIGN;
					objects.push_back(kObject);
				}

				scannedIndex++;

				DumpObject(_T("New Object Assigned:"), kObject);
				
				break;
			case 'u':
				retlen = unicode(s, scannedIndex);
				if (retlen > 0)
					scannedIndex += retlen;
				else
					return false;
				break;
			case '"':
				retlen = parse_string (s, scannedIndex);
				if (retlen > 0)
					scannedIndex += retlen;
				else
					return false;
				break;
			case '/':
				if (!isComment(s, &scannedIndex))
					return false;
				break;
			default:
				wchar_t str[50];
				swprintf(str, _T("ERROR: Parse error => Line: %d Pos: %d\n"), s.getLineNum(scannedIndex), s.getPosLine(scannedIndex));
				Debug(str);
				/*wchar_t * sEnd = wcschr(s.lpwCharAt(scannedIndex), ' ');
				wchar_t * sEnd = WholeWord(s.lpwCharAt(scannedIndex));
				if (!sEnd)
					return false;
				int wlen = sEnd - s.lpwCharAt(scannedIndex);
				wchar_t * wNew = new wchar_t[wlen+1];
				wcsncpy(wNew, s.lpwCharAt(scannedIndex), wlen);
				wNew[wlen] = NULL;

				if (isModedVar(wNew, wlen, &scannedIndex))
					break;

				kObject.iLength = wlen;
				kObject.iStartIndex = scannedIndex;
				kObject.Type = T_IDENTIFIER;
				kObject.Value = wNew;

				objects.push_back(kObject);
				scannedIndex += wlen;

				DumpObject(_T("New Object Assigned:"), kObject);*/
				break;
			}
		}

		return true;
	}

	wchar_t * WholeWord(wchar_t * s)
	{
		//while (*s != EOS)
		//{
			//if (*s == ' ' || *s == '\r' || *s == '\n' || *s == '\t' || *s == '/')
			//	break;
			//s++;
		//}
		while (*s != EOS){
			if ((*s < '0' || *s > '9') && (*s < 'A' || *s > 'Z') && (*s < 'a' || *s > 'z'))
				break;
			s++;
		}
		
		return s;
		//return wcspbrk(s, _T("\r\n \t\uFFFF/"));
	}

	int reference (script s,  int scannedIndex)
	{
		scannedIndex++;

		//wchar_t * sEnd = wcschr(s.lpwCharAt(scannedIndex), ' ');
		wchar_t * sEnd = WholeWord(s.lpwCharAt(scannedIndex));
		if (!sEnd)
			return false;

		for (wchar_t * w = sEnd-1; w >= s.lpwCharAt(scannedIndex); w--)
		{
			if (!isdigit(*w))
			{
				return false;
			}
		}
		
		int wlen = sEnd - s.lpwCharAt(scannedIndex);
		wchar_t * wNew = new wchar_t[wlen+1];
		wcsncpy(wNew, s.lpwCharAt(scannedIndex), wlen);
		wNew[wlen] = NULL;

		tyObject kObject;

		kObject.iLength = wlen;
		kObject.iStartIndex = scannedIndex;
		kObject.Type = T_REFERENCE;
		kObject.Value = wNew;

		objects.push_back(kObject);

		DumpObject(_T("New Object Assigned:"), kObject);

		return wlen+1;
	}

	bool ishexa(wchar_t w)
	{
		if ((w >= '0' && w <= '9') || (w >= 'a' && w <= 'f') || (w >= 'A' && w <= 'F'))
			return true;
		return false;
	}

	int unicode (script s,  int scannedIndex)
	{
		scannedIndex++;

		wchar_t * sEnd = WholeWord(s.lpwCharAt(scannedIndex));
		if (!sEnd)
			return false;

		for (wchar_t * w = sEnd-1; w > s.lpwCharAt(scannedIndex); w--)
		{
			if (!isxdigit(*w))
			{
				return false;
			}
		}
		
		int wlen = sEnd - s.lpwCharAt(scannedIndex);
		wchar_t * wNew = new wchar_t[wlen+1];
		wcsncpy(wNew, s.lpwCharAt(scannedIndex), wlen);
		wNew[wlen] = NULL;

		swscanf(wNew, _T("%x"), wNew);

		tyObject kObject;

		kObject.iLength = wlen;
		kObject.iStartIndex = scannedIndex;
		//kObject.Type = T_UNICODE;
		kObject.Type = T_STRING;
		kObject.Value = wNew;

		objects.push_back(kObject);

		DumpObject(_T("New Object Assigned:"), kObject);

		return wlen+1;
	}

	bool isModedVar(wchar_t * keyword, int wlen, int * scannedIndex)
	{
		if (wlen < 5){return false;}
		
		if (keyword[wlen-1] != ']' || keyword[wlen-3] != '[')
		{
			return false;
		}

		int varlen = wlen - 3;

		wchar_t * varname = new wchar_t[varlen+1];
		wcsncpy(varname, keyword,  varlen);
		varname[varlen] = NULL;

		tyObject kObject;

		kObject.iLength = varlen;
		kObject.iStartIndex = *scannedIndex;
		kObject.Type = T_IDENTIFIER;
		kObject.Value = varname;
		objects.push_back(kObject);

		wchar_t* wModer = new wchar_t[2];
		wModer[1]=NULL;
		wModer[0]=keyword[wlen-2];

		kObject.iLength = 3;
		kObject.iStartIndex = (*scannedIndex) - varlen;
		kObject.Type = T_MODIFIER;
		kObject.Value = wModer;
		objects.push_back(kObject);

		(*scannedIndex) += wlen;

		return true;
	}

	bool isComment(script s, int * index)
	{
		int scannedIndex = *index;
		if (s.wCharAt(scannedIndex) == '/' && s.wCharAt(scannedIndex+1) == '/')
		{
			wchar_t * lineEnd = wcspbrk(s.lpwCharAt(scannedIndex), _T("\r\n"));
			//lineEnd = wcschr(&c[scannedIndex], '\n');
			(*index) = lineEnd - s.lpwCharAt(0);
			return true;
		}
		else if (s.wCharAt(scannedIndex) == '/' && s.wCharAt(scannedIndex+1) == '*')
		{
			scannedIndex++;
			wchar_t * commentEnd = wcsstr(s.lpwCharAt(scannedIndex), _T("*/"));
			if (commentEnd)
			{
				(*index) = (commentEnd+2) - s.lpwCharAt(0);
				return true;
			}
		}
		Debug(_T("/ cannot be parsed"));
		Exit(0);
		return false;
	}

	int parse_string (script s, int scannedIndex)
	{
		wchar_t * sEnd;

		scannedIndex++;

		do {
			sEnd = wcschr(s.lpwCharAt(scannedIndex), '"');
		} while (sEnd && *(sEnd-1) == '\\');
		

		if (!sEnd)
			return -1;

		int wlen = sEnd - s.lpwCharAt(scannedIndex);
		wchar_t * wNew = new wchar_t[wlen+1];
		wcsncpy(wNew, s.lpwCharAt(scannedIndex), wlen);
		wNew[wlen] = NULL;

		tyObject kObject;
		kObject.iLength = wlen;
		kObject.iStartIndex = scannedIndex;
		kObject.Type = T_STRING;
		kObject.Value = wNew;

		objects.push_back(kObject);

		DumpObject(_T("New Object Assigned:"), kObject);

		return (wlen+2);
	}
};
