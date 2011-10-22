struct layout_options
{
	bool trackCaps;
	bool autoBksp;
	bool eat;
	bool posBased;
};

typedef struct
{
	char cMagicCode[4];
	byte bMajorVersion;
	byte bMinorVersion;
	short shNumOfString;
	short shNumOfRules;
	layout_options layout;
} FileHeader13 ;

typedef struct
{
	char cMagicCode[4];
	byte bMajorVersion;
	byte bMinorVersion;
	short shNumOfString;
	short shNumOfInfos;
	short shNumOfRules;
	layout_options layout;
} FileHeader ;


struct String
{
	short sLength;
	wchar_t wcString[1];
};