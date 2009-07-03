struct FileHeader
{
	char cMagicCode[4];
	byte bMajorVersion;
	byte bMinorVersion;
	short sNumOfString;
	short sNumOfRules;
};

struct String
{
	short sLength;
	wchar_t wcString[1];
};