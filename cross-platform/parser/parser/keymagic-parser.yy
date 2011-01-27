%skeleton "lalr1.cc"                          /*  -*- C++ -*- */
%require "2.4.3"
%defines
%define parser_class_name "keymagic_parser"
%code requires {
# include <string>
# include <list>
class keymagic_driver;
}
// The parsing context.
%parse-param { keymagic_driver& driver }
%lex-param   { keymagic_driver& driver }
%locations
%initial-action
{
  // Initialize the initial location.
  @$.begin.filename = @$.end.filename = &driver.file;
};
%debug
%error-verbose
// Symbols.
%union
{
	std::list<int> * lval;
	std::string * sval;
	std::wstring * wsval;
	int		ival;
};
%code {
# include "keymagic-driver.hh"
typedef std::list<int> containerType;	
}
// Tokens for Layout Options
%token OPT_NAME
%token OPT_FONTFAMILY
%token OPT_DESCRIPTION
%token OPT_CAPS
%token OPT_EAT
%token OPT_US_LAYOUT
%token OPT_SMART_BACKSPACE

%token IDENTIFIER "identifier"
%token EQUAL "="
%token RULE_OUT "=>"
%token PLUS "+"
%token REFERENCE "match-reference"
%token NEWLINE "new-line"
%token PREDEFINED "predefined-value"
%token UNIHEX "unicode"
%token CHAR "character"
%token END 0 "end-of-file"
%token ANY "any-key"
%token VK_STATES_BEGIN "virtual-key-states-begin (<)"
%token VK_STATES_END "virtual-key-states-end (>)"
%token AND "&"

%token MOD_MATCH_INDEX "match-index"
%token MOD_ANY "any (*)"
%token MOD_NOT "not (^)"
%token SWITCH_START "(" SWITCH_END ")"

%type <sval> PREDEFINED
%type <lval> lhs_variable rhs_variable
%type <lval> var_decl var_assign var_assign_exp
%type <lval> left_rule_exp left_rule_exps right_rule_exp right_rule_exps
%type <lval> vk_states vk_keys
%type <sval> char_array
%type <wsval> unicode IDENTIFIER switch
%type <lval> lhs_modifier rhs_modifier
%type <ival> MOD_MATCH_INDEX REFERENCE
%type <ival> UNIHEX CHAR

%destructor { delete $$; } <sval>

%%
%start input;
input	:	| statement_list
			;
		
options : options option
				| option

option : option_caps
				| option_eat
				| option_pos
				| option_name
				| option_font
				| option_desc
				| option_bksp

option_name : OPT_NAME
							| OPT_NAME char_array
							;
							
option_font : OPT_FONTFAMILY
							| OPT_FONTFAMILY char_array
							;
							
option_desc : OPT_DESCRIPTION
							|OPT_DESCRIPTION char_array
							;
							
option_caps : OPT_CAPS 
							| OPT_CAPS char_array {driver.layoutOptions.trackCaps = driver.StringToBool(*$2); }
							;
							
option_eat : OPT_EAT 
							| OPT_EAT char_array {driver.layoutOptions.eat = driver.StringToBool(*$2); }
							;
							
option_pos : OPT_US_LAYOUT
							| OPT_US_LAYOUT char_array {driver.layoutOptions.posBased = driver.StringToBool(*$2); }
							;
							
option_bksp : OPT_SMART_BACKSPACE
							| OPT_SMART_BACKSPACE char_array {driver.layoutOptions.smartBksp = driver.StringToBool(*$2); }
							;

statement_list : statement_list statement
				| statement
				| END
				;

statement	:	rule
				| var_decl
				| options
				| NEWLINE
				;

var_decl	:	IDENTIFIER EQUAL var_assign_exp
				{ if (!driver.StoreVariable(*$1, *$3, yylloc)) YYABORT; }
				;
				
var_assign_exp	:	var_assign_exp PLUS var_assign {$$ = $1; $$->insert($$->end(), $3->begin(), $3->end()); }
					| var_assign

var_assign	:	unicode {$$ = new containerType(); driver.AppendString(*$$, *$1, false); }
				| char_array {$$ = new containerType(); driver.AppendString(*$$, *driver.U8toU16(*$1), false); }
				| IDENTIFIER {$$ = new containerType(); $$->push_back(opVARIABLE); $$->push_back(driver.IndexOfVariable(*$1, yylloc)); }
				;
				
lhs_modifier :	{$$ = new containerType();}
				|	MOD_ANY {$$ = new containerType(); $$->push_back(opMODIFIER); $$->push_back(opANYOF); } 
				| MOD_NOT {$$ = new containerType(); $$->push_back(opMODIFIER); $$->push_back(opNANYOF); } 
				
rhs_modifier :	{$$ = new containerType();}
				| MOD_MATCH_INDEX {$$ = new containerType(); $$->push_back(opMODIFIER); $$->push_back($1); }
				
lhs_variable :	IDENTIFIER lhs_modifier
				{
					int indx = driver.IndexOfVariable(*$1, yylloc);
					if (indx == -1) YYABORT;
 					$$ = new containerType();
					$$->push_back(opVARIABLE);
					$$->push_back(indx);
					driver.AppendList(*$$, *$2);
				}

rhs_variable :	IDENTIFIER rhs_modifier
				{
					int indx = driver.IndexOfVariable(*$1, yylloc);
					if (indx == -1) YYABORT;
 					$$ = new containerType();
					$$->push_back(opVARIABLE);
					$$->push_back(indx);
					driver.AppendList(*$$, *$2);
				}
				
rule : left_rule_exps RULE_OUT right_rule_exps
		{ driver.StoreRule(*$1, *$3, yylloc); }
		;
		
left_rule_exps : left_rule_exps PLUS left_rule_exp {$$ = $1; $$->insert($$->end(), $3->begin(), $3->end()); }
				| left_rule_exp

left_rule_exp : unicode {$$ = new containerType(); driver.AppendString(*$$, *$1); }
				| switch {$$ = new containerType(); $$->push_back(opSWITCH); $$->push_back(driver.IndexOfSwitch(*$1, yylloc)); }
				| char_array {$$ = new containerType(); driver.AppendString(*$$, *driver.U8toU16(*$1)); }
				| lhs_variable {$$ = new containerType(); driver.AppendList(*$$, *$1); }
				| ANY {$$ = new containerType(); $$->push_back(opANY); }
				| PREDEFINED {$$ = new containerType(); $$->push_back(opPREDEFINED); $$->push_back(driver.IndexOfPreDefined(*$1)); }
				| vk_states {$$ = new containerType(); $$->push_back(opAND); $$->insert($$->end(), $1->begin(), $1->end()); }
				;
				
right_rule_exps : right_rule_exps PLUS right_rule_exp {$$ = $1; $$->insert($$->end(), $3->begin(), $3->end()); }
				 | right_rule_exp
				
right_rule_exp : unicode {$$ = new containerType(); driver.AppendString(*$$, *$1); }
				| char_array {$$ = new containerType(); driver.AppendString(*$$, *driver.U8toU16(*$1)); }
				| rhs_variable {$$ = new containerType(); driver.AppendList(*$$, *$1); }
				| REFERENCE {$$ = new containerType(); $$->push_back(opREFERENCE); $$->push_back($1); }
				| switch {$$ = new containerType(); $$->push_back(opSWITCH); $$->push_back(driver.IndexOfSwitch(*$1, yylloc)); }
				| PREDEFINED {$$ = new containerType(); $$->push_back(opPREDEFINED); $$->push_back(driver.IndexOfPreDefined(*$1)); }
				;
				
vk_states : VK_STATES_BEGIN vk_keys VK_STATES_END {$$ = $2; }

vk_keys : vk_keys AND PREDEFINED  { $$->push_back(opPREDEFINED); $$->push_back(driver.IndexOfPreDefined(*$3)); }
				| PREDEFINED {$$ = new containerType(); $$->push_back(opPREDEFINED); $$->push_back(driver.IndexOfPreDefined(*$1)); }
				;
				
char_array	:	char_array CHAR { $$->push_back($2); }
				| CHAR { $$ = new std::string(); $$->push_back($1); }	
				
unicode 	: UNIHEX { $$ = new std::wstring(); $$->push_back($1); }
					;
					
switch	: SWITCH_START char_array SWITCH_END { std::wstring * wcs = driver.U8toU16(*$2); $$ = wcs; driver.StoreSwitch(*wcs); }
%%
void
yy::keymagic_parser::error (const yy::keymagic_parser::location_type& l,
                          const std::string& m)
{
  driver.error (l, m);
}
