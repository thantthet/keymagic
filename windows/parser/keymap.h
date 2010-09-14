struct layout_options
{
	bool trackCaps;
	bool autoBksp;
	bool eat;
	bool posBased;
};

struct FileHeader
{
	char cMagicCode[4];
	byte bMajorVersion;
	byte bMinorVersion;
	short shNumOfString;
	short shNumOfRules;
	layout_options layout;
};

struct String
{
	short sLength;
	wchar_t wcString[1];
};