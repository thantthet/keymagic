%{                                            /* -*- C++ -*- */
# include <cstdlib>
# include <cerrno>
# include <climits>
# include <string>
# include "keymagic-driver.hh"
# include "keymagic-parser.tab.hh"

#define yyterminate() return token::END
%}

%option noyywrap caseless batch debug
%option never-interactive

alpha	[a-zA-Z_]
alnum	[a-zA-Z0-9_]
hex	[a-fA-F0-9]

%x S_SQSTRING
%x S_DQSTRING
%x S_SQUAREBRK
%x S_SWITCH
%x S_VKSTATE
%x S_LINECOMMENT
%x S_BLOCKCOMMENT
%x S_OPTIONS
%x S_NEXTLINE
%x S_INCLUDE
%x S_INCFILE
%x S_ERROR

%{
int lastState=0;
# define YY_USER_ACTION  yylloc->columns (yyleng);
#define MAX_INCLUDE_DEPTH 100
int include_stack_ptr = 0;
std::string includeFile;

typedef struct {
std::string fileName;
YY_BUFFER_STATE yyBuffer;
yy::location yylloc;
} FILEINFO;

FILEINFO fileinfo[MAX_INCLUDE_DEPTH];

%}

%%

%{
  yylloc->step ();
  typedef yy::keymagic_parser::token token;
%}
<INITIAL>{
[\t ]+			;
\[					{BEGIN(S_SQUAREBRK);}
\${alpha}{alnum}*	{
						yylval->wsval = driver.U8toU16(std::string(yytext)); return(token::IDENTIFIER);
						}
(u|U){hex}{4}	{sscanf(&(yytext[1]),"%x", &yylval->ival); return(token::UNIHEX);}
=					{return(token::EQUAL);}
"=>"				{return(token::RULE_OUT);}
\$[0-9]+			{yylval->ival = atoi(&yytext[1]); return(token::REFERENCE);}
\+					{return(token::PLUS);}
\"					{lastState=0; BEGIN(S_DQSTRING);}
\'					{lastState=0; BEGIN(S_SQSTRING);}
"ANY"				{return(token::ANY);}
\r*\n				{yylloc->lines (yyleng); return(token::NEWLINE);}
\(					{BEGIN(S_SWITCH); return(token::SWITCH_START);}
\<					{lastState=0; BEGIN(S_VKSTATE); return(token::VK_STATES_BEGIN); }
"//"				{lastState=0; BEGIN(S_LINECOMMENT);}
"/*"				{lastState=0; BEGIN(S_BLOCKCOMMENT);}
\\					{BEGIN(S_NEXTLINE);}
include			{includeFile="";BEGIN(S_INCLUDE);}
[^\t\r\n ]+		{
						if (driver.CheckPreDefined(yytext) == false) {
							yylloc->columns (-yyleng);
							REJECT;
						}
						yylval->sval = new std::string(yytext);
						return(token::PREDEFINED);
					}
<<EOF>>			{
						if (--include_stack_ptr < 0 ) {
							yyterminate();
						} else {
							yy_delete_buffer( YY_CURRENT_BUFFER );
							FILEINFO thisFileInfo = fileinfo[include_stack_ptr];
							yy_switch_to_buffer(thisFileInfo.yyBuffer );
							delete yylloc->begin.filename;
							*yylloc = thisFileInfo.yylloc;
						}
					}
.					{BEGIN(S_ERROR); unput(*yytext); }
}

<S_INCLUDE>{
	[ \t]+		;
	\(				{lastState=0;BEGIN(S_INCFILE);}
	.				{BEGIN(S_ERROR); unput(*yytext); }
}

<S_INCFILE>{
	[ \t]+		;
	\"[^"]+\"	{includeFile=std::string(yytext).substr(1, strlen(yytext)-2 );}
	\)				{ /* got the include file name */
						if ( include_stack_ptr >= MAX_INCLUDE_DEPTH ) {
							driver.error("Includes nested too deeply");
						}
						std::string thisFile, incfileName;

						thisFile = *yylloc->begin.filename;
						FILEINFO * thisFileInfo = &fileinfo[include_stack_ptr++];
						thisFileInfo->yylloc = *yylloc;
						thisFileInfo->yyBuffer = YY_CURRENT_BUFFER;
						if ( ! driver.openIncludeFile(thisFile, includeFile, &incfileName) )
							driver.error(*yylloc, std::string("cannot open include file:") + incfileName);

						yy_switch_to_buffer( yy_create_buffer( yyin, YY_BUF_SIZE ) );
						yylloc->begin.line = yylloc->begin.column = 1;
						yylloc->end.line = yylloc->end.column = 1;

						yylloc->begin.filename = yylloc->end.filename = new std::string(incfileName);
						BEGIN(INITIAL);
					}
	.				{BEGIN(S_ERROR); unput(*yytext); }
}
					
<S_ERROR>{
	[^\t ]+		{
					driver.error(*yylloc, "Unrecognized token!");
					}
}

<S_VKSTATE>{
	\>			{BEGIN(lastState); return(token::VK_STATES_END); }
	\&			{return(token::AND); }
	[ \t]
	[^\t ]+		{
					if (driver.CheckPreDefined(yytext) == false) {
						yylloc->columns (-yyleng);
						REJECT;
					}
					yylval->sval = new std::string(yytext);
					return(token::PREDEFINED);
				}
}

<S_OPTIONS>{
	[\t ]					;
	NAME					{return (token::OPT_NAME); }
	FONTFAMILY			{return (token::OPT_FONTFAMILY); }
	DESCRIPTION			{return (token::OPT_DESCRIPTION); }
	TRACK_CAPSLOCK		{return (token::OPT_CAPS); }
	EAT_ALL_UNUSED_KEYS {return (token::OPT_EAT); }
	EAT_KEYS				{return (token::OPT_EAT); }
	US_LAYOUT_BASED	{return (token::OPT_US_LAYOUT); }
	POSITIONAL_LAYOUT	{return (token::OPT_US_LAYOUT); }
	SMART_BACKSPACE	{return (token::OPT_SMART_BACKSPACE); }
	TREAT_CTRL_ALT_AS_RALT {return (token::OPT_RALT); }
	\"						{BEGIN(S_DQSTRING);}
	\'						{BEGIN(S_SQSTRING);}
	\=
	[^\t ]				{driver.warn(*yylloc, "Unrecognized Option!"); BEGIN(lastState);}
	\r*\n					{BEGIN(lastState); unput('\n');}
}

<S_LINECOMMENT>{
	\r*\n	{yylloc->lines (yyleng); BEGIN(0);}
	@		{lastState=S_LINECOMMENT; BEGIN(S_OPTIONS);}
	.		;
}

<S_BLOCKCOMMENT>{
	"*/"	{BEGIN(0);}
	@		{lastState=S_BLOCKCOMMENT; BEGIN(S_OPTIONS);}
	\r*\n	{yylloc->lines (yyleng);}
	.		;
}

<S_SWITCH>{
	\"		{lastState=S_SWITCH;BEGIN(S_DQSTRING);}
	\'		{lastState=S_SWITCH;BEGIN(S_SQSTRING);}
	\)		{BEGIN(INITIAL); return(token::SWITCH_END);}
}

<S_SQUAREBRK>{
	\]			{BEGIN(INITIAL);}
	\$[0-9]+	{yylval->ival = atoi(&yytext[1]); return(token::MOD_MATCH_INDEX);}
	\*			{return(token::MOD_ANY); }
	\^			{return(token::MOD_NOT); }
	[\t ]		;
	.			{BEGIN(S_ERROR); unput(*yytext); }
}

<S_SQSTRING>{
	\'						{BEGIN(lastState);}
	[^\\'\r\n]			{yylval->ival=yytext[0]; return(token::CHAR);}
}

<S_DQSTRING>{
	\"						{BEGIN(lastState);}
	[^\\"\r\n]			{yylval->ival=yytext[0]; return(token::CHAR);}
}

<S_SQSTRING,S_DQSTRING>{
	\r*\n					{driver.error(*yylloc, "Unterminated string");BEGIN(lastState);}
	<<EOF>>				{driver.error(*yylloc, "EOF in string");BEGIN(lastState);}
	\\(U|u){hex}{4}	{sscanf(&(yytext[2]),"%4x", &yylval->ival); return(token::CHAR);}
	\\(x|X){hex}{2,4}	{sscanf(&(yytext[2]),"%x", &yylval->ival); return(token::CHAR);}
	\\\r*\n
	\\[^\r\n]    		{
								switch(yytext[yyleng-1]) {
								case 'b' : yylval->ival = '\b'; return(token::CHAR);
								case 't' : yylval->ival = '\t'; return(token::CHAR);
								case 'n' : yylval->ival = '\n'; return(token::CHAR);
								case 'v' : yylval->ival = '\v'; return(token::CHAR);
								case 'f' : yylval->ival = '\f'; return(token::CHAR);
								case 'r' : yylval->ival = '\r'; return(token::CHAR);
								default: yylval->ival = yytext[yyleng-1]; return(token::CHAR);
								}
							}
}

<S_NEXTLINE>{
	[\t ]
	\r*\n		{yylloc->lines (yyleng); BEGIN(INITIAL); }
	[^\r\n]		{driver.error(*yylloc, "Expecting new line!"); yyterminate();}
}

%%

void
keymagic_driver::scan_begin ()
{
	yy_flex_debug = trace_scanning;
	if (file == "-")
		yyin = stdin;
	else if (!openFile(file)) {
      error (std::string ("cannot open ") + file);
      exit (1);
	}
}

bool
keymagic_driver::openFile (const std::string& strFile)
{
	if (!(yyin = fopen (strFile.c_str (), "r"))) {
      return false;
	}

	unsigned char UTF8[3] = { 0xEF, 0xBB, 0xBF };

	char buf[3];
	fread(buf, 1, 3, yyin);
	if (memcmp(buf, UTF8, 3) != 0) {
		fpos_t p = 0;
		fsetpos(yyin, &p);
	}
	return true;
}

void
keymagic_driver::scan_end ()
{
  fclose (yyin);
}
