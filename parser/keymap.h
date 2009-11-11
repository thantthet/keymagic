struct layout_options
{
	bool trackCaps;
	bool autoBksp;
	bool eatUndef;
};

struct FileHeader
{
	char cMagicCode[4];
	byte bMajorVersion;
	byte bMinorVersion;
	short shNumOfString;
	short shNumOfRules;
	layout_options lo;
	short shNumOfDKeys;
	BYTE deadKeys[1];
};

struct String
{
	short sLength;
	wchar_t wcString[1];
};