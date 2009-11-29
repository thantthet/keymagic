#include "parser.h"

using namespace std;

//#define DEBUG 1

std::wstring format_arg_list(LPCWSTR fmt, va_list args)
{
    if (!fmt) return L"";
    int   result = -1, length = 256;
    wchar_t *buffer = 0;
    while (result == -1)
    {
        if (buffer) delete [] buffer;
        buffer = new wchar_t [length + 1];
        memset(buffer, 0, length + 1);
        result = _vsnwprintf(buffer, length, fmt, args);
        length *= 2;
    }
    std::wstring s(buffer);
    delete [] buffer;
    return s;
}

std::wstring format(LPCWSTR fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    std::wstring s = format_arg_list(fmt, args);
    va_end(args);
    return s;
}

LRESULT parser::checkToken (int * objIndex, emType Type)
{
	if ( tokens.size() - 1 < *objIndex)
	{
		Debug(L"No more token => nObjIndex = %d\n", *objIndex);
		return false;
	}

	Debug(L"\nObjIndex = %d, TypeWanted = %s\n", *objIndex, Type2Str(Type));
	int pos = tokens.at(*objIndex).iStartIndex;
	Debug(L"Parsing at line %d, pos %d\n", Script.getLineNum(pos), Script.getPosLine(pos));

	if (tokens.at(*objIndex).Type == T_BSLASH){
		(*objIndex)++;
		if (tokens.at(*objIndex).Type != T_NEWLINE){
			pos = tokens.at(*objIndex).iStartIndex;
			LastError = format(L"ERROR : Syntax %s at Line=%d Pos=%d\n", tokens.at(*objIndex).Value, Script.getLineNum(pos), Script.getPosLine(pos));
			return false;
		}
		else { (*objIndex)++; }
	}

	if (tokens.at(*objIndex).Type != Type){
		DumpToken(L"NOT MATCHED: ", tokens.at(*objIndex));
		return false;
	}

	LRESULT retValue;

	DumpToken(L"MATCHED FOUND: ", tokens.at(*objIndex));
	retValue = (LRESULT)tokens.at(*objIndex).Value;

	(*objIndex)++;
	return retValue;
}

bool parser::validVarName(wchar_t * VarName)
{
	return true;
}

bool parser::complexExpression(int * objIndex, wstring * varValue)
{

	wchar_t * retVal=0;

	if (retVal = (wchar_t*)checkToken(objIndex, T_STRING)) { }

	else if (retVal = (wchar_t*)checkToken(objIndex, T_UNICODE)){}

	else if (retVal = (wchar_t*)checkToken(objIndex, T_PREDEFINED)){}

	else if (retVal = identifier(objIndex)){
		varValue->push_back(opVARIABLE);
	}

	else {
		int pos = tokens.at(*objIndex).iStartIndex;
		LastError = format(L"ERROR : Syntax %s at Line=%d Pos=%d\n", tokens.at(*objIndex).Value, Script.getLineNum(pos), Script.getPosLine(pos));
		return false;
	}

	if (retVal)
		varValue->append(retVal);

	int OriginalIndex = *objIndex;

	if (checkToken(objIndex, T_ADDOP)){
		if (!complexExpression(objIndex, varValue))
			*objIndex = OriginalIndex;
	}
	return true;
}

bool parser::vardeclaration(int * objIndex)
{
	int OriginalIndex = *objIndex;

	wchar_t * varName = (wchar_t*)checkToken(objIndex, T_IDENTIFIER);

	if (!varName)
	{
		*objIndex = OriginalIndex;
		return false;
	}

	if (!checkToken( objIndex, T_ASSIGN ))
	{
		Debug(L"Not a vardeclaration\n");
		*objIndex = OriginalIndex;
		return false;
	}


	wstring * varValue = new wstring;

	if (!complexExpression(objIndex, varValue))
	{
		varValue->~basic_string();
		*objIndex = OriginalIndex;
		return false;
	}

	setVar(varName, (wchar_t*)varValue->c_str());

	delete varValue;

	newline(objIndex);

	return true;
}

void parser::setVar(wchar_t * varName, const wchar_t * varValue)
{
	int iLength = wcslen(varValue);
	wchar_t * wsValue = new wchar_t [iLength+1];
	wcscpy(wsValue, varValue);

	kmklf.add_str(wsValue);
	mpVariables[varName] = wsValue;
}

bool parser::pattern(int * objIndex, wstring * outStr)
{
	if (checkToken(objIndex, T_ADDOP))
	{
		if (condition(objIndex, outStr))
			return true;
	}

	return false;
}

// Return :	int: VariableIndex
//			int: Modifeir

wchar_t * parser::identifier(int * objIndex)
{
	wchar_t * varName = (wchar_t*)checkToken(objIndex, T_IDENTIFIER);
	if (!varName)
		return false;

	wchar_t * varvalue = mpVariables[varName];

	if (!varvalue)
	{
		int pos = tokens.at((*objIndex)-1).iStartIndex;
		Exit(0, L"ERROR: Unknown identifier '%s' at Line=%d Pos=%d\n", varName, Script.getLineNum(pos), Script.getPosLine(pos));
		return false;
	}

	wstring * strValue = new wstring();

	strValue->append<int>(1,getVar(varvalue));

	if (wchar_t * moder = (wchar_t *)checkToken(objIndex, T_MODIFIER))
	{
		strValue->push_back(opMODIFIER);
		if (wcscmp(moder, L"*")==0)
			strValue->push_back(opANYOF);
		else if (wcscmp(moder, L"^")==0)
			strValue->push_back(opNANYOF);
		else if (moder[0]=='$')
		{
			int index=0;
			swscanf(&moder[1], L"%d", &index);
			if (index) strValue->push_back(index);
			else
			{
				int pos = tokens.at((*objIndex)-1).iStartIndex;
				Exit(0, L"ERROR: Syntax '%s' at Line=%d Pos=%d\n", moder, Script.getLineNum(pos), Script.getPosLine(pos));
			}
		}
		else
		{
			int pos = tokens.at((*objIndex)-1).iStartIndex;
			Exit(0, L"ERROR: Syntax '%s' at Line=%d Pos=%d\n", moder, Script.getLineNum(pos), Script.getPosLine(pos));
		}
	}

	return (wchar_t*)strValue->c_str();
}

int parser::getVar(const wchar_t * value)
{
#ifdef DEBUG
	wchar_t wc_integer[2];
	wsprintf(wc_integer, L"%d", kmklf.is_exist(value));
	return (int) wc_integer[0];
#endif
	return kmklf.is_exist(value)+1;
}

bool parser::combination(int * objIndex, wstring * outStr)
{
	if (wchar_t * s = (wchar_t*)checkToken(objIndex, T_PREDEFINED))
	{
		outStr->push_back(opPREDEFINED);
		structPREdef * preDef = getPreDef(s, wcslen(s), true);
		outStr->push_back(preDef->preDef);
	}
	else {return false;}

	int OriginalIndex = *objIndex;

	if (checkToken(objIndex, T_ANDOP))
	{
		if (!combination(objIndex, outStr))
			*objIndex = OriginalIndex;
	}
	return true;
}

bool parser::context(int * objIndex, wstring * outStr)
{
	int OriginalIndex = *objIndex;

	if (wchar_t * s = (wchar_t*)checkToken(objIndex, T_STRING))
	{
		outStr->push_back(opSTRING);
		outStr->push_back(wcslen(s));
		outStr->append(s);
		return true;
	}

	else if (wchar_t * s = identifier(objIndex))
	{
		outStr->push_back(opVARIABLE);
		outStr->append(s);
		return true;
	}

	else if (wchar_t * s = (wchar_t*)checkToken(objIndex, T_REFERENCE))
	{
		outStr->push_back(opREFERENCE);
		int index=0;
		swscanf(s, L"%d", &index);
		outStr->push_back(index);
		return true;
	}

	else if (wchar_t * s = (wchar_t*)checkToken(objIndex, T_PREDEFINED))
	{
		outStr->push_back(opPREDEFINED);
		structPREdef * preDef = getPreDef(s, wcslen(s), true);
		outStr->push_back(preDef->preDef);
		return true;
	}
	else if (wchar_t * s = (wchar_t*)checkToken(objIndex, T_ANY))
	{
		outStr->push_back(opANY);
		return true;
	}
	else if (checkToken(objIndex, T_COMBINE_START)){
		outStr->push_back(opAND);
		if (combination(objIndex, outStr))
			if (checkToken(objIndex, T_COMBINE_END)){
				return true;
			} else {
				int pos = tokens.at(*objIndex).iStartIndex;
				LastError = format(L"ERROR: Syntax '%s' at Line=%d Pos=%d\n", tokens.at(*objIndex).Value, Script.getLineNum(pos), Script.getPosLine(pos));
			}
		*objIndex = OriginalIndex;
	}
	else if (checkToken(objIndex, T_SWITCH_START)){
		if (wchar_t * s = (wchar_t*)checkToken(objIndex, T_STRING)) {
			static int last_var_id;
			int var_id;
			std::wstring * unique_name = new std::wstring(L"\xbeef");
			unique_name->append(s);
			if (!( var_id = getVar(unique_name->c_str()) )){
				wchar_t * buffer = new wchar_t[5];
				last_var_id++;
				swprintf(buffer, L"%d", last_var_id);
				setVar(buffer, unique_name->c_str() );
				var_id = last_var_id;
			}
			outStr->push_back(opSWITCH);
			outStr->push_back(getVar(unique_name->c_str()));
			unique_name->clear();
			if (!checkToken(objIndex, T_SWITCH_END)){
				int pos = tokens.at(*objIndex).iStartIndex;
				LastError = format(L"ERROR: Syntax '%s' at Line=%d Pos=%d\n", tokens.at(*objIndex).Value, Script.getLineNum(pos), Script.getPosLine(pos));
			}
			return true;
		}
		else {
			int pos = tokens.at(*objIndex).iStartIndex;
			LastError = format(L"ERROR: Syntax '%s' at Line=%d Pos=%d\n", tokens.at(*objIndex).Value, Script.getLineNum(pos), Script.getPosLine(pos));
		}
	}

	return false;
}

bool parser::condition(int * objIndex, wstring * outStr)
{
	if (!context(objIndex, outStr))
		return false;

	pattern(objIndex, outStr);

	return true;
}

bool parser::vk_status(int * objIndex, wstring * outStr)
{
	if (!checkToken(objIndex, T_ADDOP))
		return false;

	pattern(objIndex, outStr);

	return true;
}

bool parser::rule(int * objIndex)
{
	wstring outStr;
	wstring inStr;
	int OriginalIndex = *objIndex;

	if (!condition(objIndex, &inStr))
		return false;

	if (!checkToken(objIndex, T_PRINT))
	{
		//int pos = tokens.at(*objIndex).iStartIndex;
		//LastError = format(L"ERROR: Syntax '%s' at Line=%d Pos=%d\n", tokens.at(*objIndex).Value, Script.getLineNum(pos), Script.getPosLine(pos));
		*objIndex = OriginalIndex;
		return false;
	}

	if (!condition(objIndex, &outStr))
	{
		*objIndex = OriginalIndex;
		return false;
	}

	kmklf.add_rule(inStr.c_str(),outStr.c_str());

	newline(objIndex);

	return true;
}

bool parser::newline(int * objIndex)
{
	if (!checkToken(objIndex, T_NEWLINE))
		return false;

	if (expression(objIndex))
		return true;

	return false;
}

bool parser::expression(int * objIndex)
{
	if (newline(objIndex))
		return true;

	else if (vardeclaration(objIndex))
		return true;

	else if (rule(objIndex))
		return true;

	return false;
}

bool parser::begin_parse()
{
	static boost::wregex reName(L"//\\s*&name\\s*=\\s*([^\\n\\r]+).*", boost::regex::egrep);
	static boost::wregex reFont(L"//\\s*&font\\s*=\\s*(.+).*", boost::regex::egrep);
	static boost::wregex reDesc(L"//\\s*&description\\s*=\\s*(.+).*", boost::regex::egrep);
	static boost::wregex reCaps(L"//\\s*&track_capslocks\\s*=\\s*(true|false).*", boost::regex::egrep);
	static boost::wregex reBksp(L"//\\s*&backspace_as_undo\\s*=\\s*(true|false).*", boost::regex::egrep);
	static boost::wregex reUnused(L"//\\s*&eat_all_unused_keys\\s*=\\s*(true|false).*", boost::regex::egrep);

	boost::wcmatch matches;
	if (boost::regex_match(Script.lpwStrAt(0), matches, reCaps))
		if (wcsncmp((matches[1].first),L"false",5)==0)
			kmklf.layout.trackCaps = false;
	if (boost::regex_match(Script.lpwStrAt(0), matches, reUnused))
		if (wcsncmp((matches[1].first),L"false",5)==0)
			kmklf.layout.eat = false;
	
	int objIndex = 0;
	if (expression(&objIndex) && objIndex == tokens.size())
		return true;

	return false;
}
