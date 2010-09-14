#include "common.h"
#include "kbdext.h"

#pragma data_seg(".keymagic")
HHOOK hKeyHook = NULL;
HHOOK hWndProcHook = NULL;
HHOOK hGetMsgHook = NULL;
HWND hwndKWindows = NULL;
hwndExc hwndExceptions;
TCHAR szDir[1000] = {0};
#pragma data_seg()

//Make sure that section can READ WRITE and SHARE
#pragma comment(linker, "/SECTION:.keymagic,RWS")

SHORTCUTS vtSC;
EXPENDEDRULELIST vtERs;
classInternalEditor InternalEditor;
kmKLF klf;
HKL hkl;

void extractCharClasses(const wchar_t * e, CHARCLASSES * cc)
{
	bool opened_square_bracket = false;
	int index = 0;
	structCClass scc;

	while (*e) {
		switch (*e++) {
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

int estimate_length (const wchar_t * pattern)
{
	int length = 0;
	bool opened_square_bracket = false;

	while (*pattern) {
		switch (*pattern++) {
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

bool MakeLeftRules()
{
	RULELIST * Rules = klf.getRules();

	//vtERs.clear();
	for (EXPENDEDRULELIST::iterator it = vtERs.begin(); it != vtERs.end(); ++it) {
		structExpendedRule * r = &*it;
		delete r->match_pattern;
		delete r->vk;
		delete r->switches;
		delete r->regex;
	}

	vtERs.clear();

	for (RULELIST::iterator it = Rules->begin(); it != Rules->end(); it++) {
		std::wstring * str_in = new std::wstring;
		VIRTUALKEYS * vks = new VIRTUALKEYS;
		SWITCHES * sws = new SWITCHES;
		if (makeRegex(str_in, vks, sws, it->strInRule, wcslen((wchar_t*)it->strInRule))) {
			structExpendedRule er;

			er.match_pattern = str_in;
			er.vk = vks;
			er.switches = sws;

			er.estimated_length = estimate_length(str_in->c_str());
			extractCharClasses(str_in->c_str(), &er.cc);

			er.regex = new boost::wregex (str_in->c_str());

			er.rule_it = it;

			vtERs.push_back(er);
		}
	}
	vtERs.sort();
	vtERs.reverse();
	return true;
}

bool AppendVariableValue(int index, std::wstring * s)
{
	std::vector<const wchar_t*> * vTstr = klf.getStrings();
	
	if (--index > vTstr->size())
		return false;

	const wchar_t * str = vTstr->at(index);
	int len = wcslen(str);
	if (!len)
		return false;
	for (int i = 0; i < len; i++){
		if (str[i] == opVARIABLE){
			i++;
			if (!AppendVariableValue(str[i], s))
				return false;
		}
		else{
			s->push_back(str[i]);
		}
	}
	
	return true;
}

int findLastOpenBracket(std::wstring * s)
{
	for ( int rit = s->length() - 1; rit >= 0; rit-- ) {
		if (s->at(rit) == '(') {
			if (rit && s->at(rit-1) == '\\')
				continue;

			return rit;
		}
	}
	return -1;
}

int getindextoreplace(int sub_index, boost::wcmatch matches, CHARCLASSES * cc)
{
	CHARCLASSES::iterator it;
	for ( it=cc->begin() ; it < cc->end(); it++ )
		if (it->idx == sub_index) {
			static boost::wregex e(L"\\\\(.)");
			std::wstring ws = boost::regex_replace(std::wstring((*it).start, 1 + (*it).end - (*it).start ), e, std::wstring(L"$1"));
			const wchar_t * w = wcschr( ws.c_str(), *matches[sub_index].first );
			if (w > ws.c_str() + ws.length())
				return -1;
			return w - ws.c_str();
		}
	return -1;
}

bool makeRegex(std::wstring * output_str, // regex pattern
			   VIRTUALKEYS * ouput_vks, // virtual-keys to match
			   SWITCHES * output_sws, // switches to match
			   WORD * raw_str, // [in] binary rule
			   int len, // [in] binary rule length
			   bool genCapture, // actually, this distinguish between LHS and RHS. LHS need to do caputre
			   boost::wcmatch* matches, // needed for RHS. need for references
			   CHARCLASSES* cc) // needed for RHS. for [*]
			   // TODO: these arguments should be put into a structure
{

	while (len > 0) {
		short index, mod;
		wchar_t dec[20];
		size_t size;
		int lastB;
		static int lastLength;
		std::wstring wstemp;

		len--;
		switch (*raw_str++) {
		case opSWITCH:
			if (matches){
				InternalEditor.invertSwitch(*raw_str++), len--;
			}
			else {output_sws->push_back(*raw_str++), len--;}
			break;
		case opSTRING:
			size = *raw_str++, len--;

			if (genCapture)
				output_str->append(L"(");

			wstemp = std::wstring((wchar_t*)raw_str, size);
			output_str->append(boost::regex_replace(wstemp, slash, slash_r));
			wstemp.clear();
			if (genCapture)
				output_str->append(L")");

			len -= size; raw_str += size;
			break;
		case opVARIABLE:
			index = (short)*raw_str++; len--;

			lastLength = output_str->length();

			if (genCapture)
				output_str->append(L"(");
			wstemp = std::wstring();
			AppendVariableValue(index, &wstemp);
			output_str->append(boost::regex_replace(wstemp, slash, slash_r));
			wstemp.clear();
			if (genCapture)
				output_str->append(L")");

			break;
		case opMODIFIER:
			mod = (short)*raw_str++; len--;

			switch (mod) {
			case opANYOF:
				if ((lastB = findLastOpenBracket(output_str)) != -1){
					output_str->insert(lastB+1, 1, L'[');
					output_str->insert(output_str->length()-1, 1, L']');
					break;
				}
				break;
			case opNANYOF:
				if ((lastB = findLastOpenBracket(output_str)) != -1){
					output_str->insert(lastB+1, L"[^");
					output_str->insert(output_str->length()-1, 1, L']');
					break;
				}
				break;
			default:
				if (!matches)
					return false;
				if (mod > matches->size())
					return false;
				int index = getindextoreplace(mod, *matches, cc);
				if (lastLength + index >= output_str->length())
					return false;
				static boost::wregex e(L"\\\\(.)");
				std::wstring ws = boost::regex_replace(output_str->substr(lastLength, std::string::npos), e, std::wstring(L"$1"));
				wchar_t wc = ws.at(index);
				ws.clear();
				output_str->erase(lastLength);
				output_str->append(1, wc);
				break;
			}

			break;
		case opPREDEFINED:
			if (genCapture)
				output_str->append(L"(");
			if (structPREdef * structPd = getPreDef((emPreDef)*raw_str++)){
				output_str->append(boost::regex_replace(std::wstring((wchar_t*)structPd->value), slash, slash_r));
				len --;
			}
			if (genCapture)
				output_str->append(L")");
			break;
		case opREFERENCE:
			index = (short)*raw_str++; len--;

			wchar_t buffer [33];
			swprintf(buffer,L"%d",index);
			output_str->append(L"$");
			output_str->append(buffer);
			break;
		case opANY:
			if (genCapture)
				output_str->append(L"(");
			output_str->append(L"[\\x{20}-\\x{7d}\\x{ff}-\\x{ffff}]");
			if (genCapture)
				output_str->append(L")");
			break;
		case opAND:
			if (*raw_str++ != opPREDEFINED) { return false; };
			len--;

			structPREdef * preDef = getPreDef((emPreDef)*raw_str);
			ouput_vks->push_back((BYTE)preDef->value[0]);

			raw_str++, len--;

			while(len--){
				if (*raw_str++ != opPREDEFINED) { return false; };

				preDef = getPreDef((emPreDef)*raw_str++);
				ouput_vks->push_back((BYTE)preDef->value[0]);
				len--;
			}

			return true;
		}
	}
	return true;
}

bool LoadKeymapFile(int index)
{
	TCHAR szKBPath[MAX_PATH];

	if (!GetKeyBoard(index, szKBPath))
		return false;

	if (!klf.fromFile(szKBPath))
		return false;

	if (!MakeLeftRules())
		return false;

	return true;
}

void GetShortCuts(){

	TCHAR szINI[MAX_PATH]={0};
	TCHAR szKBNames[MAX_PATH]={0};
	TCHAR szKBP[] = TEXT("KeyBoardPaths");
	TCHAR szSC[] = TEXT("ShortCuts");

	lstrcpy(szINI, szDir);
	PathAppend(szINI, TEXT("\\KeyMagic.ini"));

	GetPrivateProfileString(szKBP, NULL, NULL, szKBNames, 500, szINI);

	for (int i=0,Length = lstrlen(&szKBNames[i]);
		Length > 0;
		i+=Length+1, Length = lstrlen(&szKBNames[i])) {
			WORD Hotkey = GetPrivateProfileInt(szSC, &szKBNames[i], 0, szINI);

			KM_ShortCut * SC = new KM_ShortCut;
			SC->ukey = (char) Hotkey;
			SC->modkey = Hotkey >> 8;

			vtSC.push_back(*SC);
	}

}


void Logger(TCHAR* fmt, ...)
{
#ifdef DEBUG
	char Memory[1024];

	va_list list;

	va_start(list, fmt);
	//Format
	wvsprintf(Memory, fmt,list);

	OutputDebugStringA(Memory);
	
	//Cleanup
	va_end(list);
#else
	return;
#endif
}

LPCTSTR GetKeyBoard(UINT Index, TCHAR * szKBPath){
	TCHAR szINI[MAX_PATH];
	TCHAR szKBNames[500];
	TCHAR szKBFile[MAX_PATH];
	TCHAR szAllUser[MAX_PATH];
	TCHAR szKBP[] = TEXT("KeyBoardPaths");

	lstrcpy(szINI,szDir);
	PathAppend(szINI, TEXT("KeyMagic.ini"));

	GetPrivateProfileString(szKBP, NULL, NULL, (LPTSTR)szKBNames, 500, szINI);

	for (int i=0,Length = lstrlen(&szKBNames[i]),j=0; 
		j <= Index;
		i+=Length+1, j++, Length = lstrlen(&szKBNames[i])){
			GetPrivateProfileString(szKBP, (LPCTSTR)&szKBNames[i], NULL, (LPTSTR)szKBFile, MAX_PATH, szINI);
	}

	if (szKBFile[1] == ':'){
		lstrcpy(szKBPath, szKBFile);
		return szKBPath;
	}
	
	lstrcpy(szKBPath, szDir);
	PathAppend(szKBPath, szKBFile);

	return szKBPath;
}

UINT TranslateToUnicode (WORD *uVKey, LPBYTE GlobalKeyStates){

	BYTE KeyStates[256];
	UINT USvk = 0;

	GetKeyboardState(KeyStates);
	KeyStates[VK_CONTROL]=KeyStates[VK_MENU]=KeyStates[VK_LMENU]=KeyStates[VK_RMENU]=0;
	if (!klf.layout.trackCaps)
		KeyStates[VK_CAPITAL] = 0;

 	WCHAR TransedChar = NULL;
	UINT ScanCode = MapVirtualKey(*uVKey, MAPVK_VK_TO_VSC);

	if (!ScanCode)
		return false;

	if (klf.layout.posBased==true && GetKeyboardLayout(0) != (HKL)0x04090409){
		USvk = ScancodeToVirtualkey(ScanCode);
		if (USvk != *uVKey && USvk <= 255){
			GlobalKeyStates[USvk] = KeyStates[USvk] = KeyStates[*uVKey];
			GlobalKeyStates[*uVKey] = KeyStates[*uVKey] = 0x00;
			*uVKey = USvk;
		}
	}else { USvk = *uVKey ; }

	int Return = ToUnicodeEx(*uVKey, ScanCode, KeyStates, &TransedChar, 1, 0, hkl);

	if (!Return) return false;

	if (TransedChar > 33 || TransedChar < 126)
		*uVKey = TransedChar;
	else {return false;}

	return USvk;
}