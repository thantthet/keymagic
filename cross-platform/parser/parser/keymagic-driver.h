#ifndef KEYMAGIC_DRIVER_HH
# define KEYMAGIC_DRIVER_HH
# include <string>
# include <map>
# include <vector>
# include <list>
# include "keymagic-parser.tab.h"
// Tell Flex the lexer's prototype ...
# define YY_DECL                                        \
  yy::keymagic_parser::token_type                         \
  yylex (yy::keymagic_parser::semantic_type* yylval,      \
         yy::keymagic_parser::location_type* yylloc,      \
         keymagic_driver& driver)
// ... and declare it for the parser's sake.
YY_DECL;

#define opSTRING		0x00F0
#define opVARIABLE		0x00F1
#define opREFERENCE		0x00F2
#define opPREDEFINED	0x00F3
#define opMODIFIER		0x00F4
#define opANYOF			0x00F5
#define opAND			0x00F6
#define opNANYOF		0x00F7
#define opANY			0x00F8
#define opSWITCH		0x00F9

enum predefinedID {
	pdNULL = 1,

	pdVK_BACK,
	pdVK_TAB,
	pdVK_RETURN,
	pdVK_SHIFT,
	pdVK_CONTROL,
	pdVK_MENU,
	pdVK_PAUSE,
	pdVK_CAPITAL,
	pdVK_KANJI,
	pdVK_ESCAPE,
	pdVK_SPACE,
	pdVK_PRIOR,
	pdVK_NEXT,
	pdVK_DELETE,

	pdVK_KEY_0,
	pdVK_KEY_1,
	pdVK_KEY_2,
	pdVK_KEY_3,
	pdVK_KEY_4,
	pdVK_KEY_5,
	pdVK_KEY_6,
	pdVK_KEY_7,
	pdVK_KEY_8,
	pdVK_KEY_9,

	pdVK_KEY_A,
	pdVK_KEY_B,
	pdVK_KEY_C,
	pdVK_KEY_D,
	pdVK_KEY_E,
	pdVK_KEY_F,
	pdVK_KEY_G,
	pdVK_KEY_H,
	pdVK_KEY_I,
	pdVK_KEY_J,
	pdVK_KEY_K,
	pdVK_KEY_L,
	pdVK_KEY_M,
	pdVK_KEY_N,
	pdVK_KEY_O,
	pdVK_KEY_P,
	pdVK_KEY_Q,
	pdVK_KEY_R,
	pdVK_KEY_S,
	pdVK_KEY_T,
	pdVK_KEY_U,
	pdVK_KEY_V,
	pdVK_KEY_W,
	pdVK_KEY_X,
	pdVK_KEY_Y,
	pdVK_KEY_Z,

	pdVK_NUMPAD0,
	pdVK_NUMPAD1,
	pdVK_NUMPAD2,
	pdVK_NUMPAD3,
	pdVK_NUMPAD4,
	pdVK_NUMPAD5,
	pdVK_NUMPAD6,
	pdVK_NUMPAD7,
	pdVK_NUMPAD8,
	pdVK_NUMPAD9,

	pdVK_MULTIPLY,
	pdVK_ADD,
	pdVK_SEPARATOR,
	pdVK_SUBTRACT,
	pdVK_DECIMAL,
	pdVK_DIVIDE,

	pdVK_F1,
	pdVK_F2,
	pdVK_F3,
	pdVK_F4,
	pdVK_F5,
	pdVK_F6,
	pdVK_F7,
	pdVK_F8,
	pdVK_F9,
	pdVK_F10,
	pdVK_F11,
	pdVK_F12,

	pdVK_LSHIFT,
	pdVK_RSHIFT,

	pdVK_LCONTROL,
	pdVK_RCONTROL,
	pdVK_LMENU,
	pdVK_RMENU,

	pdVK_OEM_1,
	pdVK_OEM_PLUS,
	pdVK_OEM_COMMA,
	pdVK_OEM_MINUS,
	pdVK_OEM_PERIOD,
	pdVK_OEM_2,
	pdVK_OEM_3,

	pdVK_OEM_4,
	pdVK_OEM_5,
	pdVK_OEM_6,
	pdVK_OEM_7,
	pdVK_OEM_8,

	pdVK_OEM_AX,
	pdVK_OEM_102,
	pdVK_ICO_HELP,
	pdVK_ICO_00,
};

#define KMKL_VERSION 1
#define KMKL_SUBVERSION 3

// Conducting the whole scanning and parsing of Calc++.
class keymagic_driver
{
public:
	keymagic_driver ();
	virtual ~keymagic_driver ();

	typedef std::map<std::string, int > VARINDEXS;
	VARINDEXS varindexs;
	typedef std::vector<std::list<int> > VARIABLES;
	VARIABLES variables;
	typedef std::map<std::string, int > PREDEFINEDVALUES;
	PREDEFINEDVALUES predefineds;

	int result;
	// Handling the scanner.
	void scan_begin ();
	void scan_end ();
	bool trace_scanning;
	//
	bool StoreSwitch(const std::string& name);
	int IndexOfSwitch(const std::string& name);
	bool StoreVariable(const std::string& name, const std::list<int>& value);
	std::list<int>& LoadVariable(const std::string& name);
	int IndexOfVariable(const std::string& name);
	bool CheckPreDefined(const std::string& id);
	//const std::string LoadPreDefined(const std::string& id);
	int IndexOfPreDefined(const std::string& id);
	const wchar_t* HumanReadable(std::list<int>& opCodes);
	//
	void AppendString(std::list<int>& container, const std::string& str, bool Op_Length = true);
	void AppendString(std::list<int>& container, const std::wstring& str, bool Op_Length = true);
	void AppendList(std::list<int>& container, const std::list<int>& str);
	//
	bool StoreRule(const std::list<int>& lhs, const std::list<int>& rhs, yy::location l);
	bool ValidateRule(const std::list<int>& left, const std::list<int>& right, yy::location l);
	bool CreateLayoutFile(const std::string &f);
	//
	bool StringToBool(std::string& booleanStr) ;
	// Run the parser.  Return 0 on success.
	int parse (const std::string& f);
	std::string file;
	bool trace_parsing;
	// Error handling.
	void error (const yy::location& l, const std::string& m);
	void error (const std::string& m);
	void warn (const yy::location& l, const std::string& m);
	
	typedef struct Rule_ {
		short* lhsRule;
		short* rhsRule;
		
		short lhsRuleLength;
		short rhsRuleLength;
		
		Rule_(short * rule1, short * rule2, short length1, short length2) {
			lhsRule = rule1;
			rhsRule = rule2;
			
			lhsRuleLength = length1;
			rhsRuleLength = length2;
		}
	} RULE;
	
	typedef struct LayoutOptions_
	{
		bool trackCaps;
		bool autoBksp;
		bool eat;
		bool posBased;
	} LAYOUTOPTIONS;
	
	typedef struct FileHeader_
	{
		char cMagicCode[4];
		char bMajorVersion;
		char bMinorVersion;
		short shNumOfString;
		short shNumOfRules;
		LAYOUTOPTIONS layout;
	} FILEHEADER;
	
	typedef struct String_
	{
		short sLength;
		short wcString[1];
	} STRING;
	
	typedef std::vector<RULE> RULES;
	
	RULES rules;
	LAYOUTOPTIONS layoutOptions;
};
#endif // ! KEYMAGIC_DRIVER_HH
