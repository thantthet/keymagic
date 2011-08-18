/* A Bison parser, made by GNU Bison 2.4.3.  */

/* Skeleton implementation for Bison LALR(1) parsers in C++
   
      Copyright (C) 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010 Free
   Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* First part of user declarations.  */




#include "keymagic-parser.tab.hpp"

/* User implementation prologue.  */


/* Unqualified %code blocks.  */


# include "keymagic-driver.hpp"
typedef std::list<int> containerType;	




#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* FIXME: INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#define YYUSE(e) ((void) (e))

/* Enable debugging if requested.  */
#if YYDEBUG

/* A pseudo ostream that takes yydebug_ into account.  */
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)	\
do {							\
  if (yydebug_)						\
    {							\
      *yycdebug_ << Title << ' ';			\
      yy_symbol_print_ ((Type), (Value), (Location));	\
      *yycdebug_ << std::endl;				\
    }							\
} while (false)

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug_)				\
    yy_reduce_print_ (Rule);		\
} while (false)

# define YY_STACK_PRINT()		\
do {					\
  if (yydebug_)				\
    yystack_print_ ();			\
} while (false)

#else /* !YYDEBUG */

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_REDUCE_PRINT(Rule)
# define YY_STACK_PRINT()

#endif /* !YYDEBUG */

#define yyerrok		(yyerrstatus_ = 0)
#define yyclearin	(yychar = yyempty_)

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)


namespace yy {

#if YYERROR_VERBOSE

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  keymagic_parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              /* Fall through.  */
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }

#endif

  /// Build a parser object.
  keymagic_parser::keymagic_parser (keymagic_driver& driver_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      driver (driver_yyarg)
  {
  }

  keymagic_parser::~keymagic_parser ()
  {
  }

#if YYDEBUG
  /*--------------------------------.
  | Print this symbol on YYOUTPUT.  |
  `--------------------------------*/

  inline void
  keymagic_parser::yy_symbol_value_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yyvaluep);
    switch (yytype)
      {
         default:
	  break;
      }
  }


  void
  keymagic_parser::yy_symbol_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    *yycdebug_ << (yytype < yyntokens_ ? "token" : "nterm")
	       << ' ' << yytname_[yytype] << " ("
	       << *yylocationp << ": ";
    yy_symbol_value_print_ (yytype, yyvaluep, yylocationp);
    *yycdebug_ << ')';
  }
#endif

  void
  keymagic_parser::yydestruct_ (const char* yymsg,
			   int yytype, semantic_type* yyvaluep, location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yymsg);
    YYUSE (yyvaluep);

    YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

    switch (yytype)
      {
        case 19: /* "\"predefined-value\"" */

	{ delete (yyvaluep->sval); };

	break;
      case 61: /* "char_array" */

	{ delete (yyvaluep->sval); };

	break;

	default:
	  break;
      }
  }

  void
  keymagic_parser::yypop_ (unsigned int n)
  {
    yystate_stack_.pop (n);
    yysemantic_stack_.pop (n);
    yylocation_stack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  keymagic_parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  keymagic_parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  keymagic_parser::debug_level_type
  keymagic_parser::debug_level () const
  {
    return yydebug_;
  }

  void
  keymagic_parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif

  int
  keymagic_parser::parse ()
  {
    /// Lookahead and lookahead in internal form.
    int yychar = yyempty_;
    int yytoken = 0;

    /* State.  */
    int yyn;
    int yylen = 0;
    int yystate = 0;

    /* Error handling.  */
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// Semantic value of the lookahead.
    semantic_type yylval;
    /// Location of the lookahead.
    location_type yylloc;
    /// The locations where the error started and ended.
    location_type yyerror_range[3];

    /// $$.
    semantic_type yyval;
    /// @$.
    location_type yyloc;

    int yyresult;

    YYCDEBUG << "Starting parse" << std::endl;


    /* User initialization code.  */
    
{
  // Initialize the initial location.
  yylloc.begin.filename = yylloc.end.filename = &driver.file;
}


    /* Initialize the stacks.  The initial state will be pushed in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystate_stack_ = state_stack_type (0);
    yysemantic_stack_ = semantic_stack_type (0);
    yylocation_stack_ = location_stack_type (0);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* New state.  */
  yynewstate:
    yystate_stack_.push (yystate);
    YYCDEBUG << "Entering state " << yystate << std::endl;

    /* Accept?  */
    if (yystate == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    /* Backup.  */
  yybackup:

    /* Try to take a decision without lookahead.  */
    yyn = yypact_[yystate];
    if (yyn == yypact_ninf_)
      goto yydefault;

    /* Read a lookahead token.  */
    if (yychar == yyempty_)
      {
	YYCDEBUG << "Reading a token: ";
	yychar = yylex (&yylval, &yylloc, driver);
      }


    /* Convert token to internal form.  */
    if (yychar <= yyeof_)
      {
	yychar = yytoken = yyeof_;
	YYCDEBUG << "Now at end of input." << std::endl;
      }
    else
      {
	yytoken = yytranslate_ (yychar);
	YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
      }

    /* If the proper action on seeing token YYTOKEN is to reduce or to
       detect an error, take that action.  */
    yyn += yytoken;
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yytoken)
      goto yydefault;

    /* Reduce or error.  */
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
	if (yyn == 0 || yyn == yytable_ninf_)
	goto yyerrlab;
	yyn = -yyn;
	goto yyreduce;
      }

    /* Shift the lookahead token.  */
    YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

    /* Discard the token being shifted.  */
    yychar = yyempty_;

    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* Count tokens shifted since error; after three, turn off error
       status.  */
    if (yyerrstatus_)
      --yyerrstatus_;

    yystate = yyn;
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystate];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    /* If YYLEN is nonzero, implement the default value of the action:
       `$$ = $1'.  Otherwise, use the top of the stack.

       Otherwise, the following line sets YYVAL to garbage.
       This behavior is undocumented and Bison
       users should not rely upon it.  */
    if (yylen)
      yyval = yysemantic_stack_[yylen - 1];
    else
      yyval = yysemantic_stack_[0];

    {
      slice<location_type, location_stack_type> slice (yylocation_stack_, yylen);
      YYLLOC_DEFAULT (yyloc, slice, yylen);
    }
    YY_REDUCE_PRINT (yyn);
    switch (yyn)
      {
	  case 16:

    { driver.AddInfo('name', (yysemantic_stack_[(2) - (2)].sval)->c_str(), (yysemantic_stack_[(2) - (2)].sval)->length()); delete (yysemantic_stack_[(2) - (2)].sval); }
    break;

  case 18:

    { driver.AddInfo('font', (yysemantic_stack_[(2) - (2)].sval)->c_str(), (yysemantic_stack_[(2) - (2)].sval)->length()); delete (yysemantic_stack_[(2) - (2)].sval); }
    break;

  case 20:

    { driver.AddInfo('desc', (yysemantic_stack_[(2) - (2)].sval)->c_str(), (yysemantic_stack_[(2) - (2)].sval)->length()); delete (yysemantic_stack_[(2) - (2)].sval); }
    break;

  case 22:

    { driver.SetIcon(*(yysemantic_stack_[(2) - (2)].sval)); delete (yysemantic_stack_[(2) - (2)].sval); }
    break;

  case 24:

    { driver.AddInfo('htky', (yysemantic_stack_[(2) - (2)].sval)->c_str(), (yysemantic_stack_[(2) - (2)].sval)->length()); delete (yysemantic_stack_[(2) - (2)].sval); }
    break;

  case 26:

    {driver.layoutOptions.trackCaps = driver.StringToBool(*(yysemantic_stack_[(2) - (2)].sval)); delete (yysemantic_stack_[(2) - (2)].sval); }
    break;

  case 28:

    {driver.layoutOptions.eat = driver.StringToBool(*(yysemantic_stack_[(2) - (2)].sval)); delete (yysemantic_stack_[(2) - (2)].sval); }
    break;

  case 30:

    {driver.layoutOptions.posBased = driver.StringToBool(*(yysemantic_stack_[(2) - (2)].sval)); delete (yysemantic_stack_[(2) - (2)].sval); }
    break;

  case 32:

    {driver.layoutOptions.smartBksp = driver.StringToBool(*(yysemantic_stack_[(2) - (2)].sval)); delete (yysemantic_stack_[(2) - (2)].sval); }
    break;

  case 34:

    {driver.layoutOptions.rightAlt = driver.StringToBool(*(yysemantic_stack_[(2) - (2)].sval)); delete (yysemantic_stack_[(2) - (2)].sval); }
    break;

  case 42:

    { if (!driver.StoreVariable(*(yysemantic_stack_[(3) - (1)].wsval), *(yysemantic_stack_[(3) - (3)].lval), yylloc)) YYABORT; }
    break;

  case 43:

    {(yyval.lval) = (yysemantic_stack_[(3) - (1)].lval); (yyval.lval)->insert((yyval.lval)->end(), (yysemantic_stack_[(3) - (3)].lval)->begin(), (yysemantic_stack_[(3) - (3)].lval)->end()); }
    break;

  case 45:

    {(yyval.lval) = new containerType(); driver.AppendString(*(yyval.lval), *(yysemantic_stack_[(1) - (1)].wsval), false); }
    break;

  case 46:

    {(yyval.lval) = new containerType(); driver.AppendString(*(yyval.lval), *driver.U8toU16(*(yysemantic_stack_[(1) - (1)].sval)), false); delete (yysemantic_stack_[(1) - (1)].sval); }
    break;

  case 47:

    {(yyval.lval) = new containerType(); (yyval.lval)->push_back(opVARIABLE); (yyval.lval)->push_back(driver.IndexOfVariable(*(yysemantic_stack_[(1) - (1)].wsval), yylloc)); }
    break;

  case 48:

    {(yyval.lval) = new containerType();}
    break;

  case 49:

    {(yyval.lval) = new containerType(); (yyval.lval)->push_back(opMODIFIER); (yyval.lval)->push_back(opANYOF); }
    break;

  case 50:

    {(yyval.lval) = new containerType(); (yyval.lval)->push_back(opMODIFIER); (yyval.lval)->push_back(opNANYOF); }
    break;

  case 51:

    {(yyval.lval) = new containerType();}
    break;

  case 52:

    {(yyval.lval) = new containerType(); (yyval.lval)->push_back(opMODIFIER); (yyval.lval)->push_back((yysemantic_stack_[(1) - (1)].ival)); }
    break;

  case 53:

    {
					int indx = driver.IndexOfVariable(*(yysemantic_stack_[(2) - (1)].wsval), yylloc);
					if (indx == -1) YYABORT;
 					(yyval.lval) = new containerType();
					(yyval.lval)->push_back(opVARIABLE);
					(yyval.lval)->push_back(indx);
					driver.AppendList(*(yyval.lval), *(yysemantic_stack_[(2) - (2)].lval));
				}
    break;

  case 54:

    {
					int indx = driver.IndexOfVariable(*(yysemantic_stack_[(2) - (1)].wsval), yylloc);
					if (indx == -1) YYABORT;
 					(yyval.lval) = new containerType();
					(yyval.lval)->push_back(opVARIABLE);
					(yyval.lval)->push_back(indx);
					driver.AppendList(*(yyval.lval), *(yysemantic_stack_[(2) - (2)].lval));
				}
    break;

  case 55:

    { driver.StoreRule(*(yysemantic_stack_[(3) - (1)].lval), *(yysemantic_stack_[(3) - (3)].lval), yylloc); }
    break;

  case 56:

    {(yyval.lval) = (yysemantic_stack_[(3) - (1)].lval); (yyval.lval)->insert((yyval.lval)->end(), (yysemantic_stack_[(3) - (3)].lval)->begin(), (yysemantic_stack_[(3) - (3)].lval)->end()); }
    break;

  case 58:

    {(yyval.lval) = new containerType(); driver.AppendString(*(yyval.lval), *(yysemantic_stack_[(1) - (1)].wsval)); }
    break;

  case 59:

    {(yyval.lval) = new containerType(); (yyval.lval)->push_back(opSWITCH); (yyval.lval)->push_back(driver.IndexOfSwitch(*(yysemantic_stack_[(1) - (1)].wsval), yylloc)); }
    break;

  case 60:

    {(yyval.lval) = new containerType(); driver.AppendString(*(yyval.lval), *driver.U8toU16(*(yysemantic_stack_[(1) - (1)].sval))); delete (yysemantic_stack_[(1) - (1)].sval); }
    break;

  case 61:

    {(yyval.lval) = new containerType(); driver.AppendList(*(yyval.lval), *(yysemantic_stack_[(1) - (1)].lval)); }
    break;

  case 62:

    {(yyval.lval) = new containerType(); (yyval.lval)->push_back(opANY); }
    break;

  case 63:

    {(yyval.lval) = new containerType(); (yyval.lval)->push_back(opPREDEFINED); (yyval.lval)->push_back(driver.IndexOfPreDefined(*(yysemantic_stack_[(1) - (1)].sval))); delete (yysemantic_stack_[(1) - (1)].sval); }
    break;

  case 64:

    {(yyval.lval) = new containerType(); (yyval.lval)->push_back(opAND); (yyval.lval)->insert((yyval.lval)->end(), (yysemantic_stack_[(1) - (1)].lval)->begin(), (yysemantic_stack_[(1) - (1)].lval)->end()); }
    break;

  case 65:

    {(yyval.lval) = (yysemantic_stack_[(3) - (1)].lval); (yyval.lval)->insert((yyval.lval)->end(), (yysemantic_stack_[(3) - (3)].lval)->begin(), (yysemantic_stack_[(3) - (3)].lval)->end()); }
    break;

  case 67:

    {(yyval.lval) = new containerType(); driver.AppendString(*(yyval.lval), *(yysemantic_stack_[(1) - (1)].wsval)); }
    break;

  case 68:

    {(yyval.lval) = new containerType(); driver.AppendString(*(yyval.lval), *driver.U8toU16(*(yysemantic_stack_[(1) - (1)].sval))); delete (yysemantic_stack_[(1) - (1)].sval); }
    break;

  case 69:

    {(yyval.lval) = new containerType(); driver.AppendList(*(yyval.lval), *(yysemantic_stack_[(1) - (1)].lval)); }
    break;

  case 70:

    {(yyval.lval) = new containerType(); (yyval.lval)->push_back(opREFERENCE); (yyval.lval)->push_back((yysemantic_stack_[(1) - (1)].ival)); }
    break;

  case 71:

    {(yyval.lval) = new containerType(); (yyval.lval)->push_back(opSWITCH); (yyval.lval)->push_back(driver.IndexOfSwitch(*(yysemantic_stack_[(1) - (1)].wsval), yylloc)); }
    break;

  case 72:

    {(yyval.lval) = new containerType(); (yyval.lval)->push_back(opPREDEFINED); (yyval.lval)->push_back(driver.IndexOfPreDefined(*(yysemantic_stack_[(1) - (1)].sval))); delete (yysemantic_stack_[(1) - (1)].sval); }
    break;

  case 73:

    { (yyval.lval) = (yysemantic_stack_[(3) - (2)].lval); }
    break;

  case 74:

    { (yyval.lval)->push_back(opPREDEFINED); (yyval.lval)->push_back(driver.IndexOfPreDefined(*(yysemantic_stack_[(3) - (3)].sval))); delete (yysemantic_stack_[(3) - (3)].sval); }
    break;

  case 75:

    {(yyval.lval) = new containerType(); (yyval.lval)->push_back(opPREDEFINED); (yyval.lval)->push_back(driver.IndexOfPreDefined(*(yysemantic_stack_[(1) - (1)].sval))); delete (yysemantic_stack_[(1) - (1)].sval); }
    break;

  case 76:

    { (yysemantic_stack_[(2) - (1)].sval)->push_back((yysemantic_stack_[(2) - (2)].ival)); (yyval.sval) = (yysemantic_stack_[(2) - (1)].sval); }
    break;

  case 77:

    { (yysemantic_stack_[(2) - (1)].sval)->append(driver.U16toU8((yysemantic_stack_[(2) - (2)].ival))->c_str()); (yyval.sval) = (yysemantic_stack_[(2) - (1)].sval); }
    break;

  case 78:

    { (yyval.sval) = new std::string(); (yyval.sval)->push_back((yysemantic_stack_[(1) - (1)].ival)); }
    break;

  case 79:

    { (yyval.sval) = driver.U16toU8((yysemantic_stack_[(1) - (1)].ival)); }
    break;

  case 80:

    { (yyval.wsval) = new std::wstring(); (yyval.wsval)->push_back((yysemantic_stack_[(1) - (1)].ival)); }
    break;

  case 81:

    { std::wstring * wcs = driver.U8toU16(*(yysemantic_stack_[(3) - (2)].sval)); (yyval.wsval) = wcs; driver.StoreSwitch(*wcs); delete (yysemantic_stack_[(3) - (2)].sval); }
    break;



	default:
          break;
      }
    YY_SYMBOL_PRINT ("-> $$ =", yyr1_[yyn], &yyval, &yyloc);

    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();

    yysemantic_stack_.push (yyval);
    yylocation_stack_.push (yyloc);

    /* Shift the result of the reduction.  */
    yyn = yyr1_[yyn];
    yystate = yypgoto_[yyn - yyntokens_] + yystate_stack_[0];
    if (0 <= yystate && yystate <= yylast_
	&& yycheck_[yystate] == yystate_stack_[0])
      yystate = yytable_[yystate];
    else
      yystate = yydefgoto_[yyn - yyntokens_];
    goto yynewstate;

  /*------------------------------------.
  | yyerrlab -- here on detecting error |
  `------------------------------------*/
  yyerrlab:
    /* If not already recovering from an error, report this error.  */
    if (!yyerrstatus_)
      {
	++yynerrs_;
	error (yylloc, yysyntax_error_ (yystate, yytoken));
      }

    yyerror_range[1] = yylloc;
    if (yyerrstatus_ == 3)
      {
	/* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

	if (yychar <= yyeof_)
	  {
	  /* Return failure if at end of input.  */
	  if (yychar == yyeof_)
	    YYABORT;
	  }
	else
	  {
	    yydestruct_ ("Error: discarding", yytoken, &yylval, &yylloc);
	    yychar = yyempty_;
	  }
      }

    /* Else will try to reuse lookahead token after shifting the error
       token.  */
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;

    yyerror_range[1] = yylocation_stack_[yylen - 1];
    /* Do not reclaim the symbols of the rule which action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    yystate = yystate_stack_[0];
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;	/* Each real token shifted decrements this.  */

    for (;;)
      {
	yyn = yypact_[yystate];
	if (yyn != yypact_ninf_)
	{
	  yyn += yyterror_;
	  if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
	    {
	      yyn = yytable_[yyn];
	      if (0 < yyn)
		break;
	    }
	}

	/* Pop the current state because it cannot handle the error token.  */
	if (yystate_stack_.height () == 1)
	YYABORT;

	yyerror_range[1] = yylocation_stack_[0];
	yydestruct_ ("Error: popping",
		     yystos_[yystate],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);
	yypop_ ();
	yystate = yystate_stack_[0];
	YY_STACK_PRINT ();
      }

    yyerror_range[2] = yylloc;
    // Using YYLLOC is tempting, but would change the location of
    // the lookahead.  YYLOC is available though.
    YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yyloc);

    /* Shift the error token.  */
    YY_SYMBOL_PRINT ("Shifting", yystos_[yyn],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);

    yystate = yyn;
    goto yynewstate;

    /* Accept.  */
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    /* Abort.  */
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (yychar != yyempty_)
      yydestruct_ ("Cleanup: discarding lookahead", yytoken, &yylval, &yylloc);

    /* Do not reclaim the symbols of the rule which action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (yystate_stack_.height () != 1)
      {
	yydestruct_ ("Cleanup: popping",
		   yystos_[yystate_stack_[0]],
		   &yysemantic_stack_[0],
		   &yylocation_stack_[0]);
	yypop_ ();
      }

    return yyresult;
  }

  // Generate an error message.
  std::string
  keymagic_parser::yysyntax_error_ (int yystate, int tok)
  {
    std::string res;
    YYUSE (yystate);
#if YYERROR_VERBOSE
    int yyn = yypact_[yystate];
    if (yypact_ninf_ < yyn && yyn <= yylast_)
      {
	/* Start YYX at -YYN if negative to avoid negative indexes in
	   YYCHECK.  */
	int yyxbegin = yyn < 0 ? -yyn : 0;

	/* Stay within bounds of both yycheck and yytname.  */
	int yychecklim = yylast_ - yyn + 1;
	int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
	int count = 0;
	for (int x = yyxbegin; x < yyxend; ++x)
	  if (yycheck_[x + yyn] == x && x != yyterror_)
	    ++count;

	// FIXME: This method of building the message is not compatible
	// with internationalization.  It should work like yacc.c does it.
	// That is, first build a string that looks like this:
	// "syntax error, unexpected %s or %s or %s"
	// Then, invoke YY_ on this string.
	// Finally, use the string as a format to output
	// yytname_[tok], etc.
	// Until this gets fixed, this message appears in English only.
	res = "syntax error, unexpected ";
	res += yytnamerr_ (yytname_[tok]);
	if (count < 5)
	  {
	    count = 0;
	    for (int x = yyxbegin; x < yyxend; ++x)
	      if (yycheck_[x + yyn] == x && x != yyterror_)
		{
		  res += (!count++) ? ", expecting " : " or ";
		  res += yytnamerr_ (yytname_[x]);
		}
	  }
      }
    else
#endif
      res = YY_("syntax error");
    return res;
  }


  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
  const signed char keymagic_parser::yypact_ninf_ = -48;
  const signed char
  keymagic_parser::yypact_[] =
  {
        60,   -48,   -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,
     -10,   -10,     5,   -48,   -48,   -48,   -48,   -48,   -48,    -6,
     -10,    16,   113,   -48,   -48,   -48,   -48,   -48,   -48,   -48,
     -48,   -48,   -48,    18,   -48,   -48,   -48,   -48,    -1,   -48,
     -48,    29,   -48,   -48,    29,    29,    29,    29,    29,    29,
      29,    29,    29,    29,    36,   -48,   -48,   -48,   -48,    28,
      24,   -48,   -48,   -48,    85,    73,   -48,   -48,   -48,    19,
     -48,    29,   -48,   -48,    13,   -48,    32,   -48,   -48,   -48,
      45,   -48,    29,   -48,   -48,    46,   -48,    36,   -48,   -48,
     -48,    85,   -48,   -48
  };

  /* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
     doesn't specify something else to do.  Zero means the default is an
     error.  */
  const unsigned char
  keymagic_parser::yydefact_[] =
  {
         0,    37,    15,    17,    19,    21,    23,    25,    27,    29,
      31,    33,    48,    41,    63,    80,    78,    79,    62,     0,
       0,     0,    40,     5,     9,    11,    12,    10,     6,     7,
       8,    13,    14,     3,    36,    39,    61,    38,     0,    57,
      64,    60,    58,    59,    16,    18,    20,    22,    24,    26,
      28,    30,    32,    34,     0,    49,    50,    53,    75,     0,
       0,     1,     4,    35,     0,     0,    76,    77,    47,    42,
      44,    46,    45,    73,     0,    81,    51,    70,    72,    69,
      55,    66,    68,    67,    71,    48,    56,     0,    74,    52,
      54,     0,    43,    65
  };

  /* YYPGOTO[NTERM-NUM].  */
  const signed char
  keymagic_parser::yypgoto_[] =
  {
       -48,   -48,   -48,    54,   -48,   -48,   -48,   -48,   -48,   -48,
     -48,   -48,   -48,   -48,    44,   -48,   -48,     0,   -48,   -48,
     -48,   -48,   -48,   -48,    23,   -48,     8,   -48,   -48,    -2,
     -44,   -47
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const signed char
  keymagic_parser::yydefgoto_[] =
  {
        -1,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    69,    70,    57,    90,
      36,    79,    37,    38,    39,    80,    81,    40,    59,    41,
      42,    43
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If zero, do what YYDEFACT says.  */
  const signed char keymagic_parser::yytable_ninf_ = -1;
  const unsigned char
  keymagic_parser::yytable_[] =
  {
        44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      72,    16,    17,    58,    64,    65,    61,    84,    60,    54,
      83,     2,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    88,    55,    56,    87,    13,    14,    15,    16,
      17,    18,    19,    72,    84,    66,    67,    83,    20,    68,
      66,    67,    71,    73,    74,    75,    15,    16,    17,    89,
       1,    91,    82,     2,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    55,    56,    62,    63,    13,    14,
      15,    16,    17,    18,    19,    71,    85,    92,    86,    82,
      20,     0,    14,    15,    16,    17,    18,    19,    76,    93,
       0,     0,    77,    20,    78,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,    20,     2,     3,     4,     5,
       6,     7,     8,     9,    10,    11
  };

  /* YYCHECK.  */
  const signed char
  keymagic_parser::yycheck_[] =
  {
         2,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      54,    21,    22,    19,    15,    16,     0,    64,    20,    14,
      64,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    19,    28,    29,    16,    18,    19,    20,    21,
      22,    23,    24,    87,    91,    21,    22,    91,    30,    13,
      21,    22,    54,    25,    26,    31,    20,    21,    22,    27,
       0,    16,    64,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    28,    29,    22,    33,    18,    19,
      20,    21,    22,    23,    24,    87,    13,    87,    65,    91,
      30,    -1,    19,    20,    21,    22,    23,    24,    13,    91,
      -1,    -1,    17,    30,    19,    20,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  keymagic_parser::yystos_[] =
  {
         0,     0,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    18,    19,    20,    21,    22,    23,    24,
      30,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    52,    54,    55,    56,
      59,    61,    62,    63,    61,    61,    61,    61,    61,    61,
      61,    61,    61,    61,    14,    28,    29,    50,    19,    60,
      61,     0,    35,    46,    15,    16,    21,    22,    13,    48,
      49,    61,    62,    25,    26,    31,    13,    17,    19,    53,
      57,    58,    61,    62,    63,    13,    56,    16,    19,    27,
      51,    16,    49,    58
  };

#if YYDEBUG
  /* TOKEN_NUMBER_[YYLEX-NUM] -- Internal symbol number corresponding
     to YYLEX-NUM.  */
  const unsigned short int
  keymagic_parser::yytoken_number_[] =
  {
         0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  keymagic_parser::yyr1_[] =
  {
         0,    32,    33,    33,    34,    34,    35,    35,    35,    35,
      35,    35,    35,    35,    35,    36,    36,    37,    37,    38,
      38,    39,    39,    39,    39,    40,    40,    41,    41,    42,
      42,    43,    43,    44,    44,    45,    45,    45,    46,    46,
      46,    46,    47,    48,    48,    49,    49,    49,    50,    50,
      50,    51,    51,    52,    53,    54,    55,    55,    56,    56,
      56,    56,    56,    56,    56,    57,    57,    58,    58,    58,
      58,    58,    58,    59,    60,    60,    61,    61,    61,    61,
      62,    63
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  keymagic_parser::yyr2_[] =
  {
         0,     2,     0,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     1,     2,     1,
       2,     1,     2,     1,     2,     1,     2,     1,     2,     1,
       2,     1,     2,     1,     2,     2,     1,     1,     1,     1,
       1,     1,     3,     3,     1,     1,     1,     1,     0,     1,
       1,     0,     1,     2,     2,     3,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     1,     2,     2,     1,     1,
       1,     3
  };

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
  const char*
  const keymagic_parser::yytname_[] =
  {
    "\"end-of-file\"", "error", "$undefined", "OPT_NAME", "OPT_FONTFAMILY",
  "OPT_DESCRIPTION", "OPT_ICON", "OPT_HOTKEY", "OPT_CAPS", "OPT_EAT",
  "OPT_US_LAYOUT", "OPT_SMART_BACKSPACE", "OPT_RALT", "\"identifier\"",
  "\"=\"", "\"=>\"", "\"+\"", "\"match-reference\"", "\"new-line\"",
  "\"predefined-value\"", "\"unicode\"", "\"character\"",
  "\"uni-character\"", "\"any-key\"", "\"virtual-key-states-begin (<)\"",
  "\"virtual-key-states-end (>)\"", "\"&\"", "\"match-index\"",
  "\"any (*)\"", "\"not (^)\"", "\"(\"", "\")\"", "$accept", "input",
  "options", "option", "option_name", "option_font", "option_desc",
  "option_icon", "option_caps", "option_eat", "option_pos", "option_bksp",
  "option_ralt", "statement_list", "statement", "var_decl",
  "var_assign_exp", "var_assign", "lhs_modifier", "rhs_modifier",
  "lhs_variable", "rhs_variable", "rule", "left_rule_exps",
  "left_rule_exp", "right_rule_exps", "right_rule_exp", "vk_states",
  "vk_keys", "char_array", "unicode", "switch", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const keymagic_parser::rhs_number_type
  keymagic_parser::yyrhs_[] =
  {
        33,     0,    -1,    -1,    45,    -1,    34,    35,    -1,    35,
      -1,    40,    -1,    41,    -1,    42,    -1,    36,    -1,    39,
      -1,    37,    -1,    38,    -1,    43,    -1,    44,    -1,     3,
      -1,     3,    61,    -1,     4,    -1,     4,    61,    -1,     5,
      -1,     5,    61,    -1,     6,    -1,     6,    61,    -1,     7,
      -1,     7,    61,    -1,     8,    -1,     8,    61,    -1,     9,
      -1,     9,    61,    -1,    10,    -1,    10,    61,    -1,    11,
      -1,    11,    61,    -1,    12,    -1,    12,    61,    -1,    45,
      46,    -1,    46,    -1,     0,    -1,    54,    -1,    47,    -1,
      34,    -1,    18,    -1,    13,    14,    48,    -1,    48,    16,
      49,    -1,    49,    -1,    62,    -1,    61,    -1,    13,    -1,
      -1,    28,    -1,    29,    -1,    -1,    27,    -1,    13,    50,
      -1,    13,    51,    -1,    55,    15,    57,    -1,    55,    16,
      56,    -1,    56,    -1,    62,    -1,    63,    -1,    61,    -1,
      52,    -1,    23,    -1,    19,    -1,    59,    -1,    57,    16,
      58,    -1,    58,    -1,    62,    -1,    61,    -1,    53,    -1,
      17,    -1,    63,    -1,    19,    -1,    24,    60,    25,    -1,
      60,    26,    19,    -1,    19,    -1,    61,    21,    -1,    61,
      22,    -1,    21,    -1,    22,    -1,    20,    -1,    30,    61,
      31,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned char
  keymagic_parser::yyprhs_[] =
  {
         0,     0,     3,     4,     6,     9,    11,    13,    15,    17,
      19,    21,    23,    25,    27,    29,    31,    34,    36,    39,
      41,    44,    46,    49,    51,    54,    56,    59,    61,    64,
      66,    69,    71,    74,    76,    79,    82,    84,    86,    88,
      90,    92,    94,    98,   102,   104,   106,   108,   110,   111,
     113,   115,   116,   118,   121,   124,   128,   132,   134,   136,
     138,   140,   142,   144,   146,   148,   152,   154,   156,   158,
     160,   162,   164,   166,   170,   174,   176,   179,   182,   184,
     186,   188
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned char
  keymagic_parser::yyrline_[] =
  {
         0,    81,    81,    81,    84,    85,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    97,    98,   101,   102,   105,
     106,   109,   110,   113,   114,   117,   118,   121,   122,   125,
     126,   129,   130,   133,   134,   137,   138,   139,   142,   143,
     144,   145,   148,   152,   153,   155,   156,   157,   160,   161,
     162,   164,   165,   167,   177,   187,   191,   192,   194,   195,
     196,   197,   198,   199,   200,   203,   204,   206,   207,   208,
     209,   210,   211,   214,   216,   217,   220,   221,   222,   223,
     225,   228
  };

  // Print the state stack on the debug stream.
  void
  keymagic_parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (state_stack_type::const_iterator i = yystate_stack_.begin ();
	 i != yystate_stack_.end (); ++i)
      *yycdebug_ << ' ' << *i;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  keymagic_parser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    /* Print the symbols being reduced, and their result.  */
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
	       << " (line " << yylno << "):" << std::endl;
    /* The symbols being reduced.  */
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
		       yyrhs_[yyprhs_[yyrule] + yyi],
		       &(yysemantic_stack_[(yynrhs) - (yyi + 1)]),
		       &(yylocation_stack_[(yynrhs) - (yyi + 1)]));
  }
#endif // YYDEBUG

  /* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
  keymagic_parser::token_number_type
  keymagic_parser::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
           0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int keymagic_parser::yyeof_ = 0;
  const int keymagic_parser::yylast_ = 125;
  const int keymagic_parser::yynnts_ = 32;
  const int keymagic_parser::yyempty_ = -2;
  const int keymagic_parser::yyfinal_ = 61;
  const int keymagic_parser::yyterror_ = 1;
  const int keymagic_parser::yyerrcode_ = 256;
  const int keymagic_parser::yyntokens_ = 32;

  const unsigned int keymagic_parser::yyuser_token_number_max_ = 286;
  const keymagic_parser::token_number_type keymagic_parser::yyundef_token_ = 2;


} // yy




void
yy::keymagic_parser::error (const yy::keymagic_parser::location_type& l,
                          const std::string& m)
{
  driver.error (l, m);
}

