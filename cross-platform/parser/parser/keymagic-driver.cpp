#include <sstream>

#include "keymagic-driver.hpp"
#include "keymagic-parser.tab.hpp"
#include "ConvertUTF.h"

#ifdef _WIN32
#pragma warning (disable: 4996)
#endif

#ifdef _WIN32
#include <windows.h>
#define FullPath(a,b) _fullpath(b, a, 256)
#define GetCurrentDir(a) GetCurrentDirectoryA(256, a)
#define SetCurrentDir(a) SetCurrentDirectoryA(a)
#else
#define FullPath(a,b) realpath(a,b)
#define GetCurrentDir(a) getcwd(a, 256)
#define SetCurrentDir(a) chdir(a)
#endif
keymagic_driver::keymagic_driver ()
  : trace_scanning (false), trace_parsing (false)
{
	//std::list<int> dummyList;
	//variables.push_back(dummyList);
	{
	predefineds["NULL"] = pdNULL;
	predefineds["null"] = pdNULL;

	predefineds["VK_BACK"] = pdVK_BACK;
	predefineds["VK_TAB"] = pdVK_TAB;
	predefineds["VK_RETURN"] = pdVK_RETURN;
	predefineds["VK_ENTER"] = pdVK_RETURN;
	predefineds["VK_SHIFT"] = pdVK_SHIFT;
	predefineds["VK_CONTROL"] = pdVK_CONTROL;
	predefineds["VK_CTRL"] = pdVK_CONTROL;
	predefineds["VK_MENU"] = pdVK_MENU;
	predefineds["VK_ALT"] = pdVK_MENU;
	predefineds["VK_PAUSE"] = pdVK_PAUSE;
	predefineds["VK_CAPITAL"] = pdVK_CAPITAL;
	predefineds["VK_CAPSLOCK"] = pdVK_CAPITAL;
	predefineds["VK_KANJI"] = pdVK_KANJI;
	predefineds["VK_ESCAPE"] = pdVK_ESCAPE;
	predefineds["VK_ESC"] = pdVK_ESCAPE;
	predefineds["VK_SPACE"] = pdVK_SPACE;
	predefineds["VK_PRIOR"] = pdVK_PRIOR;
	predefineds["VK_NEXT"] = pdVK_NEXT;
	predefineds["VK_DELETE"] = pdVK_DELETE;

	predefineds["VK_KEY_0"] = pdVK_KEY_0;
	predefineds["VK_KEY_1"] = pdVK_KEY_1;
	predefineds["VK_KEY_2"] = pdVK_KEY_2;
	predefineds["VK_KEY_3"] = pdVK_KEY_3;
	predefineds["VK_KEY_4"] = pdVK_KEY_4;
	predefineds["VK_KEY_5"] = pdVK_KEY_5;
	predefineds["VK_KEY_6"] = pdVK_KEY_6;
	predefineds["VK_KEY_7"] = pdVK_KEY_7;
	predefineds["VK_KEY_8"] = pdVK_KEY_8;
	predefineds["VK_KEY_9"] = pdVK_KEY_9;

	predefineds["VK_KEY_A"] = pdVK_KEY_A;
	predefineds["VK_KEY_B"] = pdVK_KEY_B;
	predefineds["VK_KEY_C"] = pdVK_KEY_C;
	predefineds["VK_KEY_D"] = pdVK_KEY_D;
	predefineds["VK_KEY_E"] = pdVK_KEY_E;
	predefineds["VK_KEY_F"] = pdVK_KEY_F;
	predefineds["VK_KEY_G"] = pdVK_KEY_G;
	predefineds["VK_KEY_H"] = pdVK_KEY_H;
	predefineds["VK_KEY_I"] = pdVK_KEY_I;
	predefineds["VK_KEY_J"] = pdVK_KEY_J;
	predefineds["VK_KEY_K"] = pdVK_KEY_K;
	predefineds["VK_KEY_L"] = pdVK_KEY_L;
	predefineds["VK_KEY_M"] = pdVK_KEY_M;
	predefineds["VK_KEY_N"] = pdVK_KEY_N;
	predefineds["VK_KEY_O"] = pdVK_KEY_O;
	predefineds["VK_KEY_P"] = pdVK_KEY_P;
	predefineds["VK_KEY_Q"] = pdVK_KEY_Q;
	predefineds["VK_KEY_R"] = pdVK_KEY_R;
	predefineds["VK_KEY_S"] = pdVK_KEY_S;
	predefineds["VK_KEY_T"] = pdVK_KEY_T;
	predefineds["VK_KEY_U"] = pdVK_KEY_U;
	predefineds["VK_KEY_V"] = pdVK_KEY_V;
	predefineds["VK_KEY_W"] = pdVK_KEY_W;
	predefineds["VK_KEY_X"] = pdVK_KEY_X;
	predefineds["VK_KEY_Y"] = pdVK_KEY_Y;
	predefineds["VK_KEY_Z"] = pdVK_KEY_Z;

	predefineds["VK_NUMPAD0"] = pdVK_NUMPAD0;
	predefineds["VK_NUMPAD1"] = pdVK_NUMPAD1;
	predefineds["VK_NUMPAD2"] = pdVK_NUMPAD2;
	predefineds["VK_NUMPAD3"] = pdVK_NUMPAD3;
	predefineds["VK_NUMPAD4"] = pdVK_NUMPAD4;
	predefineds["VK_NUMPAD5"] = pdVK_NUMPAD5;
	predefineds["VK_NUMPAD6"] = pdVK_NUMPAD6;
	predefineds["VK_NUMPAD7"] = pdVK_NUMPAD7;
	predefineds["VK_NUMPAD8"] = pdVK_NUMPAD8;
	predefineds["VK_NUMPAD9"] = pdVK_NUMPAD9;

	predefineds["VK_MULTIPLY"] = pdVK_MULTIPLY;
	predefineds["VK_ADD"] = pdVK_ADD;
	predefineds["VK_SEPARATOR"] = pdVK_SEPARATOR;
	predefineds["VK_SUBTRACT"] = pdVK_SUBTRACT;
	predefineds["VK_DECIMAL"] = pdVK_DECIMAL;
	predefineds["VK_DIVIDE"] = pdVK_DIVIDE;

	predefineds["VK_F1"] = pdVK_F1;
	predefineds["VK_F2"] = pdVK_F2;
	predefineds["VK_F3"] = pdVK_F3;
	predefineds["VK_F4"] = pdVK_F4;
	predefineds["VK_F5"] = pdVK_F5;
	predefineds["VK_F6"] = pdVK_F6;
	predefineds["VK_F7"] = pdVK_F7;
	predefineds["VK_F8"] = pdVK_F8;
	predefineds["VK_F9"] = pdVK_F9;
	predefineds["VK_F10"] = pdVK_F10;
	predefineds["VK_F11"] = pdVK_F11;
	predefineds["VK_F12"] = pdVK_F12;

	predefineds["VK_LSHIFT"] = pdVK_LSHIFT;
	predefineds["VK_RSHIFT"] = pdVK_RSHIFT;

	predefineds["VK_LCONTROL"] = pdVK_LCONTROL;
	predefineds["VK_LCTRL"] = pdVK_LCONTROL;
	predefineds["VK_RCONTROL"] = pdVK_RCONTROL;
	predefineds["VK_RCTRL"] = pdVK_RCONTROL;

	predefineds["VK_LMENU"] = pdVK_LMENU;
	predefineds["VK_LALT"] = pdVK_LMENU;
	predefineds["VK_RMENU"] = pdVK_RMENU;
	predefineds["VK_RALT"] = pdVK_RMENU;
	predefineds["VK_ALT_GR"] = pdVK_RMENU;

	predefineds["VK_OEM_1"] = pdVK_OEM_1;
	predefineds["VK_COLON"] = pdVK_OEM_1;

	predefineds["VK_OEM_PLUS"] = pdVK_OEM_PLUS;
	predefineds["VK_OEM_COMMA"] = pdVK_OEM_COMMA;
	predefineds["VK_OEM_MINUS"] = pdVK_OEM_MINUS;
	predefineds["VK_OEM_PERIOD"] = pdVK_OEM_PERIOD;

	predefineds["VK_OEM_2"] = pdVK_OEM_2;
	predefineds["VK_QUESTION"] = pdVK_OEM_2;

	predefineds["VK_OEM_3"] = pdVK_OEM_3;
	predefineds["VK_CFLEX"] = pdVK_OEM_3;

	predefineds["VK_OEM_4"] = pdVK_OEM_4;
	predefineds["VK_LBRACKET"] = pdVK_OEM_4;

	predefineds["VK_OEM_5"] = pdVK_OEM_5;
	predefineds["VK_BACKSLASH"] = pdVK_OEM_5;

	predefineds["VK_OEM_6"] = pdVK_OEM_6;
	predefineds["VK_RBRACKET"] = pdVK_OEM_6;

	predefineds["VK_OEM_7"] = pdVK_OEM_7;
	predefineds["VK_QUOTE"] = pdVK_OEM_7;

	predefineds["VK_OEM_8"] = pdVK_OEM_8;
	predefineds["VK_EXCM"] = pdVK_OEM_8;

	predefineds["VK_OEM_AX"] = pdVK_OEM_AX;

	predefineds["VK_OEM_102"] = pdVK_OEM_102;
	predefineds["VK_LESSTHEN"] = pdVK_OEM_102;

	predefineds["VK_ICO_HELP"] = pdVK_ICO_HELP;
	predefineds["VK_ICO_00"] = pdVK_ICO_00;
	}

	layoutOptions.smartBksp = false;
	layoutOptions.eat = false;
	layoutOptions.trackCaps = true;
	layoutOptions.posBased = false;
	layoutOptions.rightAlt = true;
}

keymagic_driver::~keymagic_driver ()
{
}

int
keymagic_driver::parse (const std::string &f)
{
  file = f;
  scan_begin ();
  yy::keymagic_parser parser (*this);
  parser.set_debug_level (trace_parsing);
  int res = parser.parse ();
  scan_end ();
  return res;
}

bool
keymagic_driver::CreateLayoutFile(const std::string &f)
{
	FILEHEADER fileHeader;
	// DEFAULT FILE SIGNATURE
	fileHeader.cMagicCode[0] = 'K';
	fileHeader.cMagicCode[1] = 'M';
	fileHeader.cMagicCode[2] = 'K';
	fileHeader.cMagicCode[3] = 'L';
	
	// FILE VERSION
	fileHeader.bMajorVersion = KMKL_VERSION;
	fileHeader.bMinorVersion = KMKL_SUBVERSION;
	
	// VARIABLE COUNT
	fileHeader.shNumOfString = variables.size();
	// INFO COUNT
	fileHeader.shNumOfInfos = infos.size();
	// RULE COUNT
	fileHeader.shNumOfRules = rules.size();
	
	// COPY LAYOUT OPTIONS
	fileHeader.layout = layoutOptions;
	
	if (fileHeader.shNumOfRules == 0) {
		std::cerr << "Warning: No rule is defined!";
	}

	FILE * fileOut = fopen(f.c_str(), "wb");
	if (fileOut == 0) {
		error("Cannot open file to write!");
		return false;
	}
	fwrite(&fileHeader, sizeof(fileHeader), 1, fileOut);
	
	for (VARIABLES::const_iterator it = variables.begin(); it != variables.end(); it++) {
		std::list<int> value = *it;
		short len = value.size();
		fwrite(&len, sizeof(len), 1, fileOut);
		for (std::list<int>::const_iterator lit = value.begin(); lit != value.end(); lit++) {
			short s = *lit;
			fwrite(&s, sizeof(s), 1, fileOut);
		}
	}
	
	for (INFOS::const_iterator it = infos.begin(); it != infos.end(); it++) {
		INFO info = *it;
		int id = info.id;
		short len = info.data.size();
		fwrite(&id, sizeof(id), 1, fileOut);
		fwrite(&len, sizeof(len), 1, fileOut);
		for (std::vector<char>::const_iterator vit = info.data.begin(); vit != info.data.end(); vit++) {
			fwrite(&*vit, sizeof(char), 1, fileOut);
		}
	}
	
	for (RULES::const_iterator it = rules.begin(); it != rules.end(); it++) {
		RULE rule = *it;
		
		fwrite(&rule.lhsRuleLength, sizeof(short), 1, fileOut);
		fwrite(&rule.lhsRule[0], rule.lhsRuleLength * sizeof(short), 1, fileOut);
		
		fwrite(&rule.rhsRuleLength, sizeof(short), 1, fileOut);
		fwrite(&rule.rhsRule[0], rule.rhsRuleLength * sizeof(short), 1, fileOut);
	}
	
	fclose(fileOut);
	return true;
}

bool
keymagic_driver::StoreRule(const std::list<int>& lhs, const std::list<int>& rhs, yy::location l)
{
	short * lhs_bytes = new short[lhs.size()];
	short * rhs_bytes = new short[rhs.size()];
	
	int i;

	ValidateRule(lhs, rhs, l);
	
	i = 0;
	for (std::list<int>::const_iterator it = lhs.begin(); it != lhs.end(); it++) {
		lhs_bytes[i++] = *it;
	}
	
	i = 0;
	for (std::list<int>::const_iterator it = rhs.begin(); it != rhs.end(); it++) {
		rhs_bytes[i++] = *it;
	}
	
	RULE rule(lhs_bytes, rhs_bytes, lhs.size(), rhs.size());
	rules.push_back(rule);
	return true;
}

bool
keymagic_driver::CheckPreDefined(const std::string& id)
{
	/*if (predefineds.find(id) == predefineds.end()) {
		return false;
	}*/
	for (PREDEFINEDVALUES::const_iterator it = predefineds.begin();
			it != predefineds.end(); it++)
			{
				if ((*it).first == id) {
					return true;
				}
			}
	return false;
}

/*const std::string
keymagic_driver::LoadPreDefined(const std::string& id)
{
	if (predefineds.find(id) == predefineds.end()) {
		return "";
	}
	return predefineds[id];
}*/

int
keymagic_driver::IndexOfPreDefined(const std::string& id)
{
	/*PREDEFINEDVALUES::const_iterator it = predefineds.find(id);
	if (it == predefineds.end()) {
		return 0;
	}
	return (*it).second;*/
	for (PREDEFINEDVALUES::const_iterator it = predefineds.begin();
			it != predefineds.end(); it++)
			{
				if ((*it).first == id) {
					return (*it).second;
				}
			}
	return 0;
}

/// switches are also store in variables list
bool
keymagic_driver::StoreSwitch(const std::wstring& name)
{
	if (varindexs.find(name) == varindexs.end()) {
		std::list<int> * dummyList = new std::list<int>();
		dummyList->push_back('K'); // just a char to have it's length
		variables.push_back(*dummyList);
		varindexs[name] = variables.size() - 1;
	} else {
		return false;
	}
	return true;
}

int
keymagic_driver::IndexOfSwitch(const std::wstring& name, yy::location l)
{	
	if (varindexs.find(name) == varindexs.end()) {
		result = 1;
		std::cerr << l << ":No such switch named : ";
		std::wcerr << name;
		std::cerr << std::endl;
		return 0;
	}
	return varindexs[name] + 1;
}

bool
keymagic_driver::StoreVariable(const std::wstring& name, const std::list<int>& value, yy::location l)
{
	if (varindexs.find(name) == varindexs.end()) {
		variables.push_back(value);
		varindexs[name] = variables.size() - 1;
	} else {
		result = 1;
		std::cerr << l << ":Variable named : '";
		std::wcerr << name;
		std::cerr << "' already defined!" << std::endl;
		exit(1);
	}
	return true;
}

std::list<int>&
keymagic_driver::LoadVariable(const std::wstring& name, yy::location l)
{
	VARINDEXS::iterator i;
	i = varindexs.find(name);
	
	if (i == varindexs.end()) {
		std::cerr << l << ":Variable named : '";
		std::wcerr << name;
		std::cerr << "' does not exist!" << std::endl;
		exit(1);
	}
	return variables[varindexs[name] ];
}

int
keymagic_driver::IndexOfVariable(const std::wstring& name, yy::location l)
{
	VARINDEXS::iterator it;
	it = varindexs.find(name);
	if (it == varindexs.end()) {
		std::cerr << l << ":Variable named : '";
		std::wcerr << name;
		std::cerr << "' does not exist!" << std::endl;
		exit(1);
	}
	return varindexs[name] + 1;
}

bool
keymagic_driver::ValidateRule(const std::list<int>& left, const std::list<int>& right, yy::location l)
{
	int segCount = 0;
	std::list<int>::const_iterator it;
	bool vkstate = false;
	char vksmsg[] = ":Error: virtual-key states should be on the left most of the rule!";
	std::map<int, bool> haveMod;

	for (it = left.begin(); it != left.end(); it++)
	{
		switch (*it) {
			case opVARIABLE:
				if (vkstate) { std::cerr << *l.begin.filename << ":" << l.begin.line << vksmsg << std::endl; exit(1); };
				segCount++; it++;
				break;
			case opANYOF:
			case opNANYOF:
				haveMod[segCount] = true;
				break;
			case opANY:
				segCount++;
				break;
			case opSTRING:
				if (vkstate) { std::cerr << *l.begin.filename << ":" << l.begin.line << vksmsg << std::endl; exit(1); };
				segCount++; it++;
				break;
			case opSWITCH:
				if (vkstate) { std::cerr << *l.begin.filename << ":" << l.begin.line << vksmsg << std::endl; exit(1); };
				it++;
				break;
			case opAND:
				vkstate = true;
				break;
			case opPREDEFINED:
				if (vkstate == false)
					segCount++; it++;
				break;
		}
	}

	for (it = right.begin(); it != right.end(); it++)
	{
		switch (*it) {
			case opVARIABLE:
			case opSWITCH:
				it++;
				break;
			case opSTRING:
				for (int i = *++it; i > 0; i--) it++;
				break;
			case opREFERENCE:
				if (segCount < *++it) {
					std::cerr << *l.begin.filename << ":" << l.begin.line << ":Warning: reference should be maximum " << segCount << std::endl;
				} else if (*it == 0) {
					std::cerr << *l.begin.filename << ":" << l.begin.line << ":Error: reference should start from 1" << std::endl;
					exit(1);
				}
				break;
			case opMODIFIER:
				if (haveMod.find(*++it) == haveMod.end()) {
					std::cerr << *l.begin.filename << ":" << l.begin.line << ":Warning: reference modifier number can only be ";
					for (std::map<int, bool>::const_iterator it = haveMod.begin(); it != haveMod.end(); it++) {
						std::cerr << it->first << " ";
					}
					std::cerr << std::endl;
				}
				break;
		}
	}
	return true;
}

void
itoa(int i, wchar_t * a)
{
	swprintf(a, 5, L"%d", i);
}

const wchar_t*
keymagic_driver::HumanReadable(std::list<int>& opCodes)
{
	std::wstring* s = new std::wstring();
	int length = 0;
	wchar_t t[5];
	
	for (std::list<int>::iterator it = opCodes.begin(); it != opCodes.end(); it++) {
		switch (*it) {
			case opVARIABLE:
				s->append(L"[var ");
				itoa(*++it, t);
				s->append(t);
				s->append(L"]");
				break;
				
			case opSTRING:
				s->append(L"[string ");
				length = *++it;
				for (int i = 0; i < length; i++) {
					wchar_t c = *++it;
					s->append(1, c);
				}
				s->append(L"]");
				break;
				
			case opSWITCH:
				s->append(L"[switch ");
				itoa(*++it, t);
				s->append(t);
				s->append(L"]");
				break;
			
			case opMODIFIER:
				s->append(L"[mod ");
				switch (*++it) {
					case opANYOF:
						s->append(L"any");
						break;
					case opNANYOF:
						s->append(L"not-any");
						break;
					default:
						s->append(L"ref ");
						itoa(*it, t);
						s->append(t);
						break;
				}
				s->append(L"]");
				break;
				
			case opREFERENCE:
				s->append(L"[ref ");
				itoa(*++it, t);
				s->append(t);
				s->append(L"]");
				break;
				
			case opAND:
				s->append(L"[vk-keys ");
				while (*++it == opPREDEFINED) {
					s->append(1, *++it);
				}
				--it;
				s->append(L"]");
				break;

			default:
				s->append(1, *it);
				break;
		}
	}
	
	return s->c_str();
}

void
keymagic_driver::AppendString(std::list<int>& container, const std::string& str, bool Op_Length)
{
	if (Op_Length) {
		container.push_back(opSTRING);
		container.push_back(str.length());
	}
	for (std::string::const_iterator si = str.begin(); si != str.end(); si++) {
		container.push_back(*si);
	}
}

void
keymagic_driver::AppendString(std::list<int>& container, const std::wstring& str, bool Op_Length)
{
	if (Op_Length) {
		container.push_back(opSTRING);
		container.push_back(str.length());
	}
	for (std::wstring::const_iterator si = str.begin(); si != str.end(); si++) {
		container.push_back(*si);
	}
}

void
keymagic_driver::AppendList(std::list<int>& container, const std::list<int>& l)
{
	for (std::list<int>::const_iterator li = l.begin(); li != l.end(); li++) {
		container.push_back(*li);
	}
}

bool
keymagic_driver::AddInfo(int id, const char * data, short size)
{
	INFO info;
	info.id = id;
	for (int i = 0; i < size; i++) {
		info.data.push_back(data[i]);
	}
	
	infos.push_back(info);
	return true;
}

bool
keymagic_driver::SetIcon(const std::string& path)
{
	char cwd[256],
	base_path[256];
	bool retVal = false;
	
	GetCurrentDir(cwd);
	FullPath(file.c_str(), base_path);
	for (int i = strlen(base_path); i > 0 &&
		 !(base_path[i] == '/' || base_path[i] == '\\'); --i) {
		base_path[i] = '\0';
	}
	SetCurrentDir(base_path);
	
	FILE * f = fopen(path.c_str(), "rb");
	
	SetCurrentDir(cwd);

	if (f  == 0) {
		std::cerr << "Error: Cannot open icon file '" << path.c_str() << "'" << std::endl;
		exit(1);
	}
	
	fseek(f, 0, SEEK_END);
	long s = ftell(f);
	fseek(f, 0, SEEK_SET);
	
	char * data = new char[s+1];
	
	int read = fread(data, 1, s, f);
	if (read == s) {
		AddInfo('icon', data, s);
		retVal = true;
	}

	
	fclose(f);
	delete[] data;	
	
	return retVal;
}

std::wstring *
keymagic_driver::U8toU16(const std::string& u8)
{
	unsigned int length = u8.length();
	length++;

	const UTF8 * source = (UTF8*)u8.c_str();
	const UTF8 * sourceStart = source;
	const UTF8 * sourceEnd = source + length;
	
	UTF16 * target = new UTF16[length];
	UTF16 * targetStart = target;
	UTF16 * targetEnd = target + length;
	
	ConvertUTF8toUTF16(&sourceStart, sourceEnd, &targetStart, targetEnd, lenientConversion);
	
	wchar_t * wcs = new wchar_t[length];
	memset(wcs, 0, length * sizeof (wchar_t));
	for (int i = 0; i < targetStart - target; i++) {
		wcs[i] = target[i];
	}

	std::wstring * s = new std::wstring(wcs);

	delete[] wcs;
	delete[] target;

	return s;
}

std::string *
keymagic_driver::U16toU8(const std::wstring& u16)
{
	unsigned int length = u16.length() * 2;
	length++;

	const UTF16 * source = (UTF16*)u16.c_str();
	const UTF16 * sourceStart = source;
	const UTF16 * sourceEnd = source + length;
	
	UTF8 * target = new UTF8[length];
	UTF8 * targetStart = target;
	UTF8 * targetEnd = target + length;
	
	ConvertUTF16toUTF8(&sourceStart, sourceEnd, &targetStart, targetEnd, lenientConversion);
	
	char * cs = new char[length];
	memset(cs, 0, length * sizeof (wchar_t));
	for (int i = 0; i < targetStart - target; i++) {
		cs[i] = target[i];
	}

	std::string * s = new std::string(cs);

	delete[] cs;
	delete[] target;

	return s;
}

std::string *
keymagic_driver::U16toU8(int u16)
{
	std::wstring * ws = new std::wstring();
	ws->push_back(u16);
	std::string * s = U16toU8(*ws);
	delete ws;
	return s;
}

bool
keymagic_driver::openIncludeFile(const std::string& from, const std::string& incFile, std::string * realPath)
{
	bool fRet;
	char cwd[256],
		base_path[256];

	GetCurrentDir(cwd);
	FullPath(from.c_str(), base_path);

	for (int i = strlen(base_path); i > 0 &&
		!(base_path[i] == '/' || base_path[i] == '\\'); --i) {
		base_path[i] = '\0';
	}
	*realPath = base_path + incFile;

	SetCurrentDir(base_path);
	fRet = openFile(incFile);
	SetCurrentDir(cwd);

	return fRet;
}

void
keymagic_driver::error (const yy::location& l, const std::string& m)
{
  std::cerr << l << ": " << m << std::endl;
	exit(1);
}

void
keymagic_driver::error (const std::string& m)
{
  std::cerr << m << std::endl;
	exit(1);
}

void 
keymagic_driver::warn (const yy::location& l, const std::string& m)
{
	std::cerr << l << ": " << "Warning: " << m << std::endl;
}

bool
keymagic_driver::StringToBool(std::string& booleanStr) 
{
	if (booleanStr == "true" || booleanStr == "TRUE")
		return true;
	return false;
}
