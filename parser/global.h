enum emType {
	T_ANDOP,
	T_ASSIGN,
	T_UNICODE,
	T_STRING,
	T_REFERENCE,
	T_NEWLINE,
	T_IDENTIFIER,
	T_MODIFIER,
	T_PRINT
};

typedef struct
{
	emType Type;
	wchar_t *Value;
	int iStartIndex;
	int iLength;
}
tyObject;

wchar_t * Type2Str(emType Type)
{
	switch (Type)
	{
	case T_ANDOP:
		return _T("AND_OPERATOR");
		break;
	case T_ASSIGN:
		return _T("ASSIGN");
		break;
	case T_UNICODE:
		return _T("UNICODE");
		break;
	case T_STRING:
		return _T("STRING");
		break;
	case T_REFERENCE:
		return _T("REFERENCE");
		break;
	case T_NEWLINE:
		return _T("NEWLINE");
		break;
	case T_IDENTIFIER:
		return _T("IDENTIFIER");
		break;
	case T_MODIFIER:
		return _T("MODIFIER");
		break;
	case T_PRINT:
		return _T("PRINT");
		break;
	}
	return _T("UNKNOWN");
}

void Debug(wchar_t * string)
{
	OutputDebugString(string);
}

void DumpObject(wchar_t * d,tyObject kObject)
{
	wchar_t str[100];
	swprintf(str,
		_T("%s => Length = %d, StartIndex = %d, Type = %s, Value = '%s'\n"), d, 
		kObject.iLength, kObject.iStartIndex, Type2Str(kObject.Type), kObject.Value
		);
	Debug(str);
}

void Exit(int _Code)
{
	Debug(_T("Application is ending\n"));
	system("pause");
	exit(_Code);
}