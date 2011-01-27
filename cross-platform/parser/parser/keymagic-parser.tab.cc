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




#include "keymagic-parser.tab.hh"

/* User implementation prologue.  */


/* Unqualified %code blocks.  */


# include "keymagic-driver.hh"
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
        case 16: /* "\"predefined-value\"" */

	{ delete (yyvaluep->sval); };

	break;
      case 55: /* "char_array" */

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
	  case 20:

    {driver.layoutOptions.trackCaps = driver.StringToBool(*(yysemantic_stack_[(2) - (2)].sval)); }
    break;

  case 22:

    {driver.layoutOptions.eat = driver.StringToBool(*(yysemantic_stack_[(2) - (2)].sval)); }
    break;

  case 24:

    {driver.layoutOptions.posBased = driver.StringToBool(*(yysemantic_stack_[(2) - (2)].sval)); }
    break;

  case 26:

    {driver.layoutOptions.smartBksp = driver.StringToBool(*(yysemantic_stack_[(2) - (2)].sval)); }
    break;

  case 34:

    { if (!driver.StoreVariable(*(yysemantic_stack_[(3) - (1)].wsval), *(yysemantic_stack_[(3) - (3)].lval), yylloc)) YYABORT; }
    break;

  case 35:

    {(yyval.lval) = (yysemantic_stack_[(3) - (1)].lval); (yyval.lval)->insert((yyval.lval)->end(), (yysemantic_stack_[(3) - (3)].lval)->begin(), (yysemantic_stack_[(3) - (3)].lval)->end()); }
    break;

  case 37:

    {(yyval.lval) = new containerType(); driver.AppendString(*(yyval.lval), *(yysemantic_stack_[(1) - (1)].wsval), false); }
    break;

  case 38:

    {(yyval.lval) = new containerType(); driver.AppendString(*(yyval.lval), *driver.U8toU16(*(yysemantic_stack_[(1) - (1)].sval)), false); }
    break;

  case 39:

    {(yyval.lval) = new containerType(); (yyval.lval)->push_back(opVARIABLE); (yyval.lval)->push_back(driver.IndexOfVariable(*(yysemantic_stack_[(1) - (1)].wsval), yylloc)); }
    break;

  case 40:

    {(yyval.lval) = new containerType();}
    break;

  case 41:

    {(yyval.lval) = new containerType(); (yyval.lval)->push_back(opMODIFIER); (yyval.lval)->push_back(opANYOF); }
    break;

  case 42:

    {(yyval.lval) = new containerType(); (yyval.lval)->push_back(opMODIFIER); (yyval.lval)->push_back(opNANYOF); }
    break;

  case 43:

    {(yyval.lval) = new containerType();}
    break;

  case 44:

    {(yyval.lval) = new containerType(); (yyval.lval)->push_back(opMODIFIER); (yyval.lval)->push_back((yysemantic_stack_[(1) - (1)].ival)); }
    break;

  case 45:

    {
					int indx = driver.IndexOfVariable(*(yysemantic_stack_[(2) - (1)].wsval), yylloc);
					if (indx == -1) YYABORT;
 					(yyval.lval) = new containerType();
					(yyval.lval)->push_back(opVARIABLE);
					(yyval.lval)->push_back(indx);
					driver.AppendList(*(yyval.lval), *(yysemantic_stack_[(2) - (2)].lval));
				}
    break;

  case 46:

    {
					int indx = driver.IndexOfVariable(*(yysemantic_stack_[(2) - (1)].wsval), yylloc);
					if (indx == -1) YYABORT;
 					(yyval.lval) = new containerType();
					(yyval.lval)->push_back(opVARIABLE);
					(yyval.lval)->push_back(indx);
					driver.AppendList(*(yyval.lval), *(yysemantic_stack_[(2) - (2)].lval));
				}
    break;

  case 47:

    { driver.StoreRule(*(yysemantic_stack_[(3) - (1)].lval), *(yysemantic_stack_[(3) - (3)].lval), yylloc); }
    break;

  case 48:

    {(yyval.lval) = (yysemantic_stack_[(3) - (1)].lval); (yyval.lval)->insert((yyval.lval)->end(), (yysemantic_stack_[(3) - (3)].lval)->begin(), (yysemantic_stack_[(3) - (3)].lval)->end()); }
    break;

  case 50:

    {(yyval.lval) = new containerType(); driver.AppendString(*(yyval.lval), *(yysemantic_stack_[(1) - (1)].wsval)); }
    break;

  case 51:

    {(yyval.lval) = new containerType(); (yyval.lval)->push_back(opSWITCH); (yyval.lval)->push_back(driver.IndexOfSwitch(*(yysemantic_stack_[(1) - (1)].wsval), yylloc)); }
    break;

  case 52:

    {(yyval.lval) = new containerType(); driver.AppendString(*(yyval.lval), *driver.U8toU16(*(yysemantic_stack_[(1) - (1)].sval))); }
    break;

  case 53:

    {(yyval.lval) = new containerType(); driver.AppendList(*(yyval.lval), *(yysemantic_stack_[(1) - (1)].lval)); }
    break;

  case 54:

    {(yyval.lval) = new containerType(); (yyval.lval)->push_back(opANY); }
    break;

  case 55:

    {(yyval.lval) = new containerType(); (yyval.lval)->push_back(opPREDEFINED); (yyval.lval)->push_back(driver.IndexOfPreDefined(*(yysemantic_stack_[(1) - (1)].sval))); }
    break;

  case 56:

    {(yyval.lval) = new containerType(); (yyval.lval)->push_back(opAND); (yyval.lval)->insert((yyval.lval)->end(), (yysemantic_stack_[(1) - (1)].lval)->begin(), (yysemantic_stack_[(1) - (1)].lval)->end()); }
    break;

  case 57:

    {(yyval.lval) = (yysemantic_stack_[(3) - (1)].lval); (yyval.lval)->insert((yyval.lval)->end(), (yysemantic_stack_[(3) - (3)].lval)->begin(), (yysemantic_stack_[(3) - (3)].lval)->end()); }
    break;

  case 59:

    {(yyval.lval) = new containerType(); driver.AppendString(*(yyval.lval), *(yysemantic_stack_[(1) - (1)].wsval)); }
    break;

  case 60:

    {(yyval.lval) = new containerType(); driver.AppendString(*(yyval.lval), *driver.U8toU16(*(yysemantic_stack_[(1) - (1)].sval))); }
    break;

  case 61:

    {(yyval.lval) = new containerType(); driver.AppendList(*(yyval.lval), *(yysemantic_stack_[(1) - (1)].lval)); }
    break;

  case 62:

    {(yyval.lval) = new containerType(); (yyval.lval)->push_back(opREFERENCE); (yyval.lval)->push_back((yysemantic_stack_[(1) - (1)].ival)); }
    break;

  case 63:

    {(yyval.lval) = new containerType(); (yyval.lval)->push_back(opSWITCH); (yyval.lval)->push_back(driver.IndexOfSwitch(*(yysemantic_stack_[(1) - (1)].wsval), yylloc)); }
    break;

  case 64:

    {(yyval.lval) = new containerType(); (yyval.lval)->push_back(opPREDEFINED); (yyval.lval)->push_back(driver.IndexOfPreDefined(*(yysemantic_stack_[(1) - (1)].sval))); }
    break;

  case 65:

    {(yyval.lval) = (yysemantic_stack_[(3) - (2)].lval); }
    break;

  case 66:

    { (yyval.lval)->push_back(opPREDEFINED); (yyval.lval)->push_back(driver.IndexOfPreDefined(*(yysemantic_stack_[(3) - (3)].sval))); }
    break;

  case 67:

    {(yyval.lval) = new containerType(); (yyval.lval)->push_back(opPREDEFINED); (yyval.lval)->push_back(driver.IndexOfPreDefined(*(yysemantic_stack_[(1) - (1)].sval))); }
    break;

  case 68:

    { (yyval.sval)->push_back((yysemantic_stack_[(2) - (2)].ival)); }
    break;

  case 69:

    { (yyval.sval) = new std::string(); (yyval.sval)->push_back((yysemantic_stack_[(1) - (1)].ival)); }
    break;

  case 70:

    { (yyval.wsval) = new std::wstring(); (yyval.wsval)->push_back((yysemantic_stack_[(1) - (1)].ival)); }
    break;

  case 71:

    { std::wstring * wcs = driver.U8toU16(*(yysemantic_stack_[(3) - (2)].sval)); (yyval.wsval) = wcs; driver.StoreSwitch(*wcs); }
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
  const signed char keymagic_parser::yypact_ninf_ = -45;
  const signed char
  keymagic_parser::yypact_[] =
  {
        16,   -45,   -11,   -11,   -11,   -11,   -11,   -11,   -11,     4,
     -45,   -45,   -45,   -45,   -45,     1,   -11,    27,    91,   -45,
     -45,   -45,   -45,   -45,   -45,   -45,   -45,    42,   -45,   -45,
     -45,   -45,    -3,   -45,   -45,    20,   -45,   -45,    20,    20,
      20,    20,    20,    20,    20,    72,   -45,   -45,   -45,   -45,
      -9,    12,   -45,   -45,   -45,    67,    54,   -45,   -45,    28,
     -45,    20,   -45,   -45,    40,   -45,    43,   -45,   -45,   -45,
      50,   -45,    20,   -45,   -45,    30,   -45,    72,   -45,   -45,
     -45,    67,   -45,   -45
  };

  /* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
     doesn't specify something else to do.  Zero means the default is an
     error.  */
  const unsigned char
  keymagic_parser::yydefact_[] =
  {
         0,    29,    13,    15,    17,    19,    21,    23,    25,    40,
      33,    55,    70,    69,    54,     0,     0,     0,    32,     5,
       9,    10,    11,     6,     7,     8,    12,     3,    28,    31,
      53,    30,     0,    49,    56,    52,    50,    51,    14,    16,
      18,    20,    22,    24,    26,     0,    41,    42,    45,    67,
       0,     0,     1,     4,    27,     0,     0,    68,    39,    34,
      36,    38,    37,    65,     0,    71,    43,    62,    64,    61,
      47,    58,    60,    59,    63,    40,    48,     0,    66,    44,
      46,     0,    35,    57
  };

  /* YYPGOTO[NTERM-NUM].  */
  const signed char
  keymagic_parser::yypgoto_[] =
  {
       -45,   -45,   -45,    47,   -45,   -45,   -45,   -45,   -45,   -45,
     -45,   -45,    49,   -45,   -45,   -10,   -45,   -45,   -45,   -45,
     -45,   -45,    13,   -45,     5,   -45,   -45,    -2,   -37,   -44
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const signed char
  keymagic_parser::yydefgoto_[] =
  {
        -1,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    59,    60,    48,    80,    30,    69,
      31,    32,    33,    70,    71,    34,    50,    35,    36,    37
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If zero, do what YYDEFACT says.  */
  const signed char keymagic_parser::yytable_ninf_ = -1;
  const unsigned char
  keymagic_parser::yytable_[] =
  {
        38,    39,    40,    41,    42,    43,    44,    13,    62,    55,
      56,    74,    63,    64,    51,    45,     1,    49,    73,     2,
       3,     4,     5,     6,     7,     8,     9,    52,    46,    47,
      57,    10,    11,    12,    13,    14,    15,    74,    57,    65,
      62,    77,    16,    61,    73,     2,     3,     4,     5,     6,
       7,     8,     9,    72,    46,    47,    78,    10,    11,    12,
      13,    14,    15,    81,    75,    53,    79,    82,    16,    76,
      11,    12,    13,    14,    15,    61,    54,    66,     0,    72,
      16,    67,    58,    68,    12,    13,    83,     0,     0,    12,
      13,     0,     0,    16,     2,     3,     4,     5,     6,     7,
       8
  };

  /* YYCHECK.  */
  const signed char
  keymagic_parser::yycheck_[] =
  {
         2,     3,     4,     5,     6,     7,     8,    18,    45,    12,
      13,    55,    21,    22,    16,    11,     0,    16,    55,     3,
       4,     5,     6,     7,     8,     9,    10,     0,    24,    25,
      18,    15,    16,    17,    18,    19,    20,    81,    18,    27,
      77,    13,    26,    45,    81,     3,     4,     5,     6,     7,
       8,     9,    10,    55,    24,    25,    16,    15,    16,    17,
      18,    19,    20,    13,    10,    18,    23,    77,    26,    56,
      16,    17,    18,    19,    20,    77,    27,    10,    -1,    81,
      26,    14,    10,    16,    17,    18,    81,    -1,    -1,    17,
      18,    -1,    -1,    26,     3,     4,     5,     6,     7,     8,
       9
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  keymagic_parser::yystos_[] =
  {
         0,     0,     3,     4,     5,     6,     7,     8,     9,    10,
      15,    16,    17,    18,    19,    20,    26,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      46,    48,    49,    50,    53,    55,    56,    57,    55,    55,
      55,    55,    55,    55,    55,    11,    24,    25,    44,    16,
      54,    55,     0,    31,    40,    12,    13,    18,    10,    42,
      43,    55,    56,    21,    22,    27,    10,    14,    16,    47,
      51,    52,    55,    56,    57,    10,    50,    13,    16,    23,
      45,    13,    43,    52
  };

#if YYDEBUG
  /* TOKEN_NUMBER_[YYLEX-NUM] -- Internal symbol number corresponding
     to YYLEX-NUM.  */
  const unsigned short int
  keymagic_parser::yytoken_number_[] =
  {
         0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  keymagic_parser::yyr1_[] =
  {
         0,    28,    29,    29,    30,    30,    31,    31,    31,    31,
      31,    31,    31,    32,    32,    33,    33,    34,    34,    35,
      35,    36,    36,    37,    37,    38,    38,    39,    39,    39,
      40,    40,    40,    40,    41,    42,    42,    43,    43,    43,
      44,    44,    44,    45,    45,    46,    47,    48,    49,    49,
      50,    50,    50,    50,    50,    50,    50,    51,    51,    52,
      52,    52,    52,    52,    52,    53,    54,    54,    55,    55,
      56,    57
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  keymagic_parser::yyr2_[] =
  {
         0,     2,     0,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     1,     2,     1,     2,     1,
       2,     1,     2,     1,     2,     1,     2,     2,     1,     1,
       1,     1,     1,     1,     3,     3,     1,     1,     1,     1,
       0,     1,     1,     0,     1,     2,     2,     3,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     3,     3,     1,     2,     1,
       1,     3
  };

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
  const char*
  const keymagic_parser::yytname_[] =
  {
    "\"end-of-file\"", "error", "$undefined", "OPT_NAME", "OPT_FONTFAMILY",
  "OPT_DESCRIPTION", "OPT_CAPS", "OPT_EAT", "OPT_US_LAYOUT",
  "OPT_SMART_BACKSPACE", "\"identifier\"", "\"=\"", "\"=>\"", "\"+\"",
  "\"match-reference\"", "\"new-line\"", "\"predefined-value\"",
  "\"unicode\"", "\"character\"", "\"any-key\"",
  "\"virtual-key-states-begin (<)\"", "\"virtual-key-states-end (>)\"",
  "\"&\"", "\"match-index\"", "\"any (*)\"", "\"not (^)\"", "\"(\"",
  "\")\"", "$accept", "input", "options", "option", "option_name",
  "option_font", "option_desc", "option_caps", "option_eat", "option_pos",
  "option_bksp", "statement_list", "statement", "var_decl",
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
        29,     0,    -1,    -1,    39,    -1,    30,    31,    -1,    31,
      -1,    35,    -1,    36,    -1,    37,    -1,    32,    -1,    33,
      -1,    34,    -1,    38,    -1,     3,    -1,     3,    55,    -1,
       4,    -1,     4,    55,    -1,     5,    -1,     5,    55,    -1,
       6,    -1,     6,    55,    -1,     7,    -1,     7,    55,    -1,
       8,    -1,     8,    55,    -1,     9,    -1,     9,    55,    -1,
      39,    40,    -1,    40,    -1,     0,    -1,    48,    -1,    41,
      -1,    30,    -1,    15,    -1,    10,    11,    42,    -1,    42,
      13,    43,    -1,    43,    -1,    56,    -1,    55,    -1,    10,
      -1,    -1,    24,    -1,    25,    -1,    -1,    23,    -1,    10,
      44,    -1,    10,    45,    -1,    49,    12,    51,    -1,    49,
      13,    50,    -1,    50,    -1,    56,    -1,    57,    -1,    55,
      -1,    46,    -1,    19,    -1,    16,    -1,    53,    -1,    51,
      13,    52,    -1,    52,    -1,    56,    -1,    55,    -1,    47,
      -1,    14,    -1,    57,    -1,    16,    -1,    20,    54,    21,
      -1,    54,    22,    16,    -1,    16,    -1,    55,    18,    -1,
      18,    -1,    17,    -1,    26,    55,    27,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned char
  keymagic_parser::yyprhs_[] =
  {
         0,     0,     3,     4,     6,     9,    11,    13,    15,    17,
      19,    21,    23,    25,    27,    30,    32,    35,    37,    40,
      42,    45,    47,    50,    52,    55,    57,    60,    63,    65,
      67,    69,    71,    73,    75,    79,    83,    85,    87,    89,
      91,    92,    94,    96,    97,    99,   102,   105,   109,   113,
     115,   117,   119,   121,   123,   125,   127,   129,   133,   135,
     137,   139,   141,   143,   145,   147,   151,   155,   157,   160,
     162,   164
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned char
  keymagic_parser::yyrline_[] =
  {
         0,    77,    77,    77,    80,    81,    83,    84,    85,    86,
      87,    88,    89,    91,    92,    95,    96,    99,   100,   103,
     104,   107,   108,   111,   112,   115,   116,   119,   120,   121,
     124,   125,   126,   127,   130,   134,   135,   137,   138,   139,
     142,   143,   144,   146,   147,   149,   159,   169,   173,   174,
     176,   177,   178,   179,   180,   181,   182,   185,   186,   188,
     189,   190,   191,   192,   193,   196,   198,   199,   202,   203,
     205,   208
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
      25,    26,    27
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int keymagic_parser::yyeof_ = 0;
  const int keymagic_parser::yylast_ = 100;
  const int keymagic_parser::yynnts_ = 30;
  const int keymagic_parser::yyempty_ = -2;
  const int keymagic_parser::yyfinal_ = 52;
  const int keymagic_parser::yyterror_ = 1;
  const int keymagic_parser::yyerrcode_ = 256;
  const int keymagic_parser::yyntokens_ = 28;

  const unsigned int keymagic_parser::yyuser_token_number_max_ = 282;
  const keymagic_parser::token_number_type keymagic_parser::yyundef_token_ = 2;


} // yy




void
yy::keymagic_parser::error (const yy::keymagic_parser::location_type& l,
                          const std::string& m)
{
  driver.error (l, m);
}

