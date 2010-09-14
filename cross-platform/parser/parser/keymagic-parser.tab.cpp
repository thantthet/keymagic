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


/* Line 311 of lalr1.cc  */
#line 40 "keymagic-parser.tab.cpp"


#include "keymagic-parser.tab.h"

/* User implementation prologue.  */


/* Line 317 of lalr1.cc  */
#line 49 "keymagic-parser.tab.cpp"
/* Unqualified %code blocks.  */

/* Line 318 of lalr1.cc  */
#line 29 "keymagic-parser.yy"

# include "keymagic-driver.h"
typedef std::list<int> containerType;	



/* Line 318 of lalr1.cc  */
#line 61 "keymagic-parser.tab.cpp"

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

/* Line 380 of lalr1.cc  */
#line 127 "keymagic-parser.tab.cpp"
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
        case 9: /* "\"identifier\"" */

/* Line 480 of lalr1.cc  */
#line 72 "keymagic-parser.yy"
	{ delete (yyvaluep->sval); };

/* Line 480 of lalr1.cc  */
#line 234 "keymagic-parser.tab.cpp"
	break;
      case 15: /* "\"predefined-value\"" */

/* Line 480 of lalr1.cc  */
#line 72 "keymagic-parser.yy"
	{ delete (yyvaluep->sval); };

/* Line 480 of lalr1.cc  */
#line 243 "keymagic-parser.tab.cpp"
	break;
      case 53: /* "char_array" */

/* Line 480 of lalr1.cc  */
#line 72 "keymagic-parser.yy"
	{ delete (yyvaluep->sval); };

/* Line 480 of lalr1.cc  */
#line 252 "keymagic-parser.tab.cpp"
	break;
      case 55: /* "switch" */

/* Line 480 of lalr1.cc  */
#line 72 "keymagic-parser.yy"
	{ delete (yyvaluep->sval); };

/* Line 480 of lalr1.cc  */
#line 261 "keymagic-parser.tab.cpp"
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
    
/* Line 559 of lalr1.cc  */
#line 15 "keymagic-parser.yy"
{
  // Initialize the initial location.
  yylloc.begin.filename = yylloc.end.filename = &driver.file;
}

/* Line 559 of lalr1.cc  */
#line 347 "keymagic-parser.tab.cpp"

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
	  case 15:

/* Line 678 of lalr1.cc  */
#line 92 "keymagic-parser.yy"
    {driver.layoutOptions.trackCaps = driver.StringToBool(*(yysemantic_stack_[(2) - (2)].sval)); }
    break;

  case 16:

/* Line 678 of lalr1.cc  */
#line 93 "keymagic-parser.yy"
    {driver.layoutOptions.eat = driver.StringToBool(*(yysemantic_stack_[(2) - (2)].sval)); }
    break;

  case 17:

/* Line 678 of lalr1.cc  */
#line 94 "keymagic-parser.yy"
    {driver.layoutOptions.posBased = driver.StringToBool(*(yysemantic_stack_[(2) - (2)].sval)); }
    break;

  case 25:

/* Line 678 of lalr1.cc  */
#line 108 "keymagic-parser.yy"
    { if (!driver.StoreVariable(*(yysemantic_stack_[(3) - (1)].sval), *(yysemantic_stack_[(3) - (3)].lval))) YYABORT; }
    break;

  case 26:

/* Line 678 of lalr1.cc  */
#line 111 "keymagic-parser.yy"
    {(yyval.lval) = (yysemantic_stack_[(3) - (1)].lval); (yyval.lval)->insert((yyval.lval)->end(), (yysemantic_stack_[(3) - (3)].lval)->begin(), (yysemantic_stack_[(3) - (3)].lval)->end()); }
    break;

  case 28:

/* Line 678 of lalr1.cc  */
#line 114 "keymagic-parser.yy"
    {(yyval.lval) = new containerType(); driver.AppendString(*(yyval.lval), *(yysemantic_stack_[(1) - (1)].wsval), false); }
    break;

  case 29:

/* Line 678 of lalr1.cc  */
#line 115 "keymagic-parser.yy"
    {(yyval.lval) = new containerType(); driver.AppendString(*(yyval.lval), *(yysemantic_stack_[(1) - (1)].sval), false); }
    break;

  case 30:

/* Line 678 of lalr1.cc  */
#line 116 "keymagic-parser.yy"
    {(yyval.lval) = new containerType(); (yyval.lval)->push_back(opVARIABLE); (yyval.lval)->push_back(driver.IndexOfVariable(*(yysemantic_stack_[(1) - (1)].sval))); }
    break;

  case 31:

/* Line 678 of lalr1.cc  */
#line 119 "keymagic-parser.yy"
    {(yyval.lval) = new containerType();}
    break;

  case 32:

/* Line 678 of lalr1.cc  */
#line 120 "keymagic-parser.yy"
    {(yyval.lval) = new containerType(); (yyval.lval)->push_back(opMODIFIER); (yyval.lval)->push_back(opANYOF); }
    break;

  case 33:

/* Line 678 of lalr1.cc  */
#line 121 "keymagic-parser.yy"
    {(yyval.lval) = new containerType(); (yyval.lval)->push_back(opMODIFIER); (yyval.lval)->push_back(opNANYOF); }
    break;

  case 34:

/* Line 678 of lalr1.cc  */
#line 123 "keymagic-parser.yy"
    {(yyval.lval) = new containerType();}
    break;

  case 35:

/* Line 678 of lalr1.cc  */
#line 124 "keymagic-parser.yy"
    {(yyval.lval) = new containerType(); (yyval.lval)->push_back(opMODIFIER); (yyval.lval)->push_back((yysemantic_stack_[(1) - (1)].ival)); }
    break;

  case 36:

/* Line 678 of lalr1.cc  */
#line 127 "keymagic-parser.yy"
    {
					int indx = driver.IndexOfVariable(*(yysemantic_stack_[(2) - (1)].sval));
					if (indx == -1) YYABORT;
 					(yyval.lval) = new containerType();
					(yyval.lval)->push_back(opVARIABLE);
					(yyval.lval)->push_back(indx);
					driver.AppendList(*(yyval.lval), *(yysemantic_stack_[(2) - (2)].lval));
				}
    break;

  case 37:

/* Line 678 of lalr1.cc  */
#line 137 "keymagic-parser.yy"
    {
					int indx = driver.IndexOfVariable(*(yysemantic_stack_[(2) - (1)].sval));
					if (indx == -1) YYABORT;
 					(yyval.lval) = new containerType();
					(yyval.lval)->push_back(opVARIABLE);
					(yyval.lval)->push_back(indx);
					driver.AppendList(*(yyval.lval), *(yysemantic_stack_[(2) - (2)].lval));
				}
    break;

  case 38:

/* Line 678 of lalr1.cc  */
#line 147 "keymagic-parser.yy"
    { driver.StoreRule(*(yysemantic_stack_[(3) - (1)].lval), *(yysemantic_stack_[(3) - (3)].lval), yylloc); }
    break;

  case 39:

/* Line 678 of lalr1.cc  */
#line 150 "keymagic-parser.yy"
    {(yyval.lval) = (yysemantic_stack_[(3) - (1)].lval); (yyval.lval)->insert((yyval.lval)->end(), (yysemantic_stack_[(3) - (3)].lval)->begin(), (yysemantic_stack_[(3) - (3)].lval)->end()); }
    break;

  case 41:

/* Line 678 of lalr1.cc  */
#line 153 "keymagic-parser.yy"
    {(yyval.lval) = new containerType(); driver.AppendString(*(yyval.lval), *(yysemantic_stack_[(1) - (1)].wsval)); }
    break;

  case 42:

/* Line 678 of lalr1.cc  */
#line 154 "keymagic-parser.yy"
    {(yyval.lval) = new containerType(); (yyval.lval)->push_back(opSWITCH); (yyval.lval)->push_back(driver.IndexOfSwitch(*(yysemantic_stack_[(1) - (1)].sval))); }
    break;

  case 43:

/* Line 678 of lalr1.cc  */
#line 155 "keymagic-parser.yy"
    {(yyval.lval) = new containerType(); driver.AppendString(*(yyval.lval), *(yysemantic_stack_[(1) - (1)].sval)); }
    break;

  case 44:

/* Line 678 of lalr1.cc  */
#line 156 "keymagic-parser.yy"
    {(yyval.lval) = new containerType(); driver.AppendList(*(yyval.lval), *(yysemantic_stack_[(1) - (1)].lval)); }
    break;

  case 45:

/* Line 678 of lalr1.cc  */
#line 157 "keymagic-parser.yy"
    {(yyval.lval) = new containerType(); (yyval.lval)->push_back(opANY); }
    break;

  case 46:

/* Line 678 of lalr1.cc  */
#line 158 "keymagic-parser.yy"
    {(yyval.lval) = new containerType(); (yyval.lval)->push_back(opPREDEFINED); (yyval.lval)->push_back(driver.IndexOfPreDefined(*(yysemantic_stack_[(1) - (1)].sval))); }
    break;

  case 47:

/* Line 678 of lalr1.cc  */
#line 159 "keymagic-parser.yy"
    {(yyval.lval) = new containerType(); (yyval.lval)->push_back(opAND); (yyval.lval)->insert((yyval.lval)->end(), (yysemantic_stack_[(1) - (1)].lval)->begin(), (yysemantic_stack_[(1) - (1)].lval)->end()); }
    break;

  case 48:

/* Line 678 of lalr1.cc  */
#line 162 "keymagic-parser.yy"
    {(yyval.lval) = (yysemantic_stack_[(3) - (1)].lval); (yyval.lval)->insert((yyval.lval)->end(), (yysemantic_stack_[(3) - (3)].lval)->begin(), (yysemantic_stack_[(3) - (3)].lval)->end()); }
    break;

  case 50:

/* Line 678 of lalr1.cc  */
#line 165 "keymagic-parser.yy"
    {(yyval.lval) = new containerType(); driver.AppendString(*(yyval.lval), *(yysemantic_stack_[(1) - (1)].wsval)); }
    break;

  case 51:

/* Line 678 of lalr1.cc  */
#line 166 "keymagic-parser.yy"
    {(yyval.lval) = new containerType(); driver.AppendString(*(yyval.lval), *(yysemantic_stack_[(1) - (1)].sval)); }
    break;

  case 52:

/* Line 678 of lalr1.cc  */
#line 167 "keymagic-parser.yy"
    {(yyval.lval) = new containerType(); driver.AppendList(*(yyval.lval), *(yysemantic_stack_[(1) - (1)].lval)); }
    break;

  case 53:

/* Line 678 of lalr1.cc  */
#line 168 "keymagic-parser.yy"
    {(yyval.lval) = new containerType(); (yyval.lval)->push_back(opREFERENCE); (yyval.lval)->push_back((yysemantic_stack_[(1) - (1)].ival)); }
    break;

  case 54:

/* Line 678 of lalr1.cc  */
#line 169 "keymagic-parser.yy"
    {(yyval.lval) = new containerType(); (yyval.lval)->push_back(opSWITCH); (yyval.lval)->push_back(driver.IndexOfSwitch(*(yysemantic_stack_[(1) - (1)].sval))); }
    break;

  case 55:

/* Line 678 of lalr1.cc  */
#line 170 "keymagic-parser.yy"
    {(yyval.lval) = new containerType(); (yyval.lval)->push_back(opPREDEFINED); (yyval.lval)->push_back(driver.IndexOfPreDefined(*(yysemantic_stack_[(1) - (1)].sval))); }
    break;

  case 56:

/* Line 678 of lalr1.cc  */
#line 173 "keymagic-parser.yy"
    {(yyval.lval) = (yysemantic_stack_[(3) - (2)].lval); }
    break;

  case 57:

/* Line 678 of lalr1.cc  */
#line 175 "keymagic-parser.yy"
    { (yyval.lval)->push_back(opPREDEFINED); (yyval.lval)->push_back(driver.IndexOfPreDefined(*(yysemantic_stack_[(3) - (3)].sval))); }
    break;

  case 58:

/* Line 678 of lalr1.cc  */
#line 176 "keymagic-parser.yy"
    {(yyval.lval) = new containerType(); (yyval.lval)->push_back(opPREDEFINED); (yyval.lval)->push_back(driver.IndexOfPreDefined(*(yysemantic_stack_[(1) - (1)].sval))); }
    break;

  case 59:

/* Line 678 of lalr1.cc  */
#line 179 "keymagic-parser.yy"
    { (yyval.sval)->push_back((yysemantic_stack_[(2) - (2)].ival)); }
    break;

  case 60:

/* Line 678 of lalr1.cc  */
#line 180 "keymagic-parser.yy"
    { (yyval.sval) = new std::string(); (yyval.sval)->push_back((yysemantic_stack_[(1) - (1)].ival)); }
    break;

  case 61:

/* Line 678 of lalr1.cc  */
#line 182 "keymagic-parser.yy"
    { (yyval.wsval) = new std::wstring(); (yyval.wsval)->push_back((yysemantic_stack_[(1) - (1)].ival)); }
    break;

  case 62:

/* Line 678 of lalr1.cc  */
#line 185 "keymagic-parser.yy"
    { (yyval.sval) = (yysemantic_stack_[(3) - (2)].sval); driver.StoreSwitch(*(yysemantic_stack_[(3) - (2)].sval)); }
    break;



/* Line 678 of lalr1.cc  */
#line 746 "keymagic-parser.tab.cpp"
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
  const signed char keymagic_parser::yypact_ninf_ = -46;
  const signed char
  keymagic_parser::yypact_[] =
  {
        48,   -46,    -9,    -9,    -9,    -9,    -9,    -9,    -1,   -46,
     -46,   -46,   -46,   -46,    -5,    -9,    12,    97,   -46,   -46,
     -46,   -46,   -46,   -46,   -46,    74,   -46,   -46,   -46,   -46,
       3,   -46,   -46,     9,   -46,   -46,     9,     9,     9,     9,
       9,     9,    52,   -46,   -46,   -46,   -46,     4,    13,   -46,
     -46,   -46,    19,     2,   -46,   -46,    17,   -46,     9,   -46,
     -46,    16,   -46,    15,   -46,   -46,   -46,    29,   -46,     9,
     -46,   -46,    22,   -46,    52,   -46,   -46,   -46,    19,   -46,
     -46
  };

  /* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
     doesn't specify something else to do.  Zero means the default is an
     error.  */
  const unsigned char
  keymagic_parser::yydefact_[] =
  {
         0,    20,     0,     0,     0,     0,     0,     0,    31,    24,
      46,    61,    60,    45,     0,     0,     0,    23,     5,     9,
      10,    11,     6,     7,     8,     3,    19,    22,    44,    21,
       0,    40,    47,    43,    41,    42,    12,    13,    14,    15,
      16,    17,     0,    32,    33,    36,    58,     0,     0,     1,
       4,    18,     0,     0,    59,    30,    25,    27,    29,    28,
      56,     0,    62,    34,    53,    55,    52,    38,    49,    51,
      50,    54,    31,    39,     0,    57,    35,    37,     0,    26,
      48
  };

  /* YYPGOTO[NTERM-NUM].  */
  const signed char
  keymagic_parser::yypgoto_[] =
  {
       -46,   -46,   -46,    26,   -46,   -46,   -46,   -46,   -46,   -46,
     -46,    24,   -46,   -46,   -27,   -46,   -46,   -46,   -46,   -46,
     -46,     5,   -46,   -19,   -46,   -46,    -2,   -36,   -45
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const signed char
  keymagic_parser::yydefgoto_[] =
  {
        -1,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    56,    57,    45,    77,    28,    66,    29,
      30,    31,    67,    68,    32,    47,    33,    34,    35
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If zero, do what YYDEFACT says.  */
  const signed char keymagic_parser::yytable_ninf_ = -1;
  const unsigned char
  keymagic_parser::yytable_[] =
  {
        36,    37,    38,    39,    40,    41,    59,    71,    12,    42,
      46,    72,    49,    48,    52,    53,    70,    10,    11,    12,
      13,    14,    43,    44,    60,    61,    54,    15,    63,    74,
      54,    75,    64,    71,    65,    11,    12,    76,    59,    62,
      58,    78,    70,    50,    15,    43,    44,    79,     1,    51,
      69,     2,     3,     4,     5,     6,     7,     8,    73,    80,
       0,    55,     9,    10,    11,    12,    13,    14,    11,    12,
       0,     0,    58,    15,     0,     0,    69,     2,     3,     4,
       5,     6,     7,     8,     0,     0,     0,     0,     9,    10,
      11,    12,    13,    14,     0,     0,     0,     0,     0,    15,
       2,     3,     4,     5,     6,     7
  };

  /* YYCHECK.  */
  const signed char
  keymagic_parser::yycheck_[] =
  {
         2,     3,     4,     5,     6,     7,    42,    52,    17,    10,
      15,     9,     0,    15,    11,    12,    52,    15,    16,    17,
      18,    19,    23,    24,    20,    21,    17,    25,     9,    12,
      17,    15,    13,    78,    15,    16,    17,    22,    74,    26,
      42,    12,    78,    17,    25,    23,    24,    74,     0,    25,
      52,     3,     4,     5,     6,     7,     8,     9,    53,    78,
      -1,     9,    14,    15,    16,    17,    18,    19,    16,    17,
      -1,    -1,    74,    25,    -1,    -1,    78,     3,     4,     5,
       6,     7,     8,     9,    -1,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
       3,     4,     5,     6,     7,     8
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  keymagic_parser::yystos_[] =
  {
         0,     0,     3,     4,     5,     6,     7,     8,     9,    14,
      15,    16,    17,    18,    19,    25,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    44,    46,
      47,    48,    51,    53,    54,    55,    53,    53,    53,    53,
      53,    53,    10,    23,    24,    42,    15,    52,    53,     0,
      30,    38,    11,    12,    17,     9,    40,    41,    53,    54,
      20,    21,    26,     9,    13,    15,    45,    49,    50,    53,
      54,    55,     9,    48,    12,    15,    22,    43,    12,    41,
      50
  };

#if YYDEBUG
  /* TOKEN_NUMBER_[YYLEX-NUM] -- Internal symbol number corresponding
     to YYLEX-NUM.  */
  const unsigned short int
  keymagic_parser::yytoken_number_[] =
  {
         0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  keymagic_parser::yyr1_[] =
  {
         0,    27,    28,    28,    29,    29,    30,    30,    30,    30,
      30,    30,    31,    32,    33,    34,    35,    36,    37,    37,
      37,    38,    38,    38,    38,    39,    40,    40,    41,    41,
      41,    42,    42,    42,    43,    43,    44,    45,    46,    47,
      47,    48,    48,    48,    48,    48,    48,    48,    49,    49,
      50,    50,    50,    50,    50,    50,    51,    52,    52,    53,
      53,    54,    55
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  keymagic_parser::yyr2_[] =
  {
         0,     2,     0,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     2,     2,     2,     2,     2,     1,
       1,     1,     1,     1,     1,     3,     3,     1,     1,     1,
       1,     0,     1,     1,     0,     1,     2,     2,     3,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     1,     2,
       1,     1,     3
  };

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
  const char*
  const keymagic_parser::yytname_[] =
  {
    "\"end-of-file\"", "error", "$undefined", "OPT_NAME", "OPT_FONTFAMILY",
  "OPT_DESCRIPTION", "OPT_CAPS", "OPT_EAT", "OPT_US_LAYOUT",
  "\"identifier\"", "\"=\"", "\"=>\"", "\"+\"", "\"match-reference\"",
  "\"new-line\"", "\"predefined-value\"", "\"unicode\"", "\"character\"",
  "\"any-key\"", "\"virtual-key-states-begin (<)\"",
  "\"virtual-key-states-end (>)\"", "\"&\"", "\"match-index\"",
  "\"any (*)\"", "\"not (^)\"", "\"(\"", "\")\"", "$accept", "input",
  "options", "option", "option_name", "option_font", "option_desc",
  "option_caps", "option_eat", "option_pos", "statement_list", "statement",
  "var_decl", "var_assign_exp", "var_assign", "lhs_modifier",
  "rhs_modifier", "lhs_variable", "rhs_variable", "rule", "left_rule_exps",
  "left_rule_exp", "right_rule_exps", "right_rule_exp", "vk_states",
  "vk_keys", "char_array", "unicode", "switch", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const keymagic_parser::rhs_number_type
  keymagic_parser::yyrhs_[] =
  {
        28,     0,    -1,    -1,    37,    -1,    29,    30,    -1,    30,
      -1,    34,    -1,    35,    -1,    36,    -1,    31,    -1,    32,
      -1,    33,    -1,     3,    53,    -1,     4,    53,    -1,     5,
      53,    -1,     6,    53,    -1,     7,    53,    -1,     8,    53,
      -1,    37,    38,    -1,    38,    -1,     0,    -1,    46,    -1,
      39,    -1,    29,    -1,    14,    -1,     9,    10,    40,    -1,
      40,    12,    41,    -1,    41,    -1,    54,    -1,    53,    -1,
       9,    -1,    -1,    23,    -1,    24,    -1,    -1,    22,    -1,
       9,    42,    -1,     9,    43,    -1,    47,    11,    49,    -1,
      47,    12,    48,    -1,    48,    -1,    54,    -1,    55,    -1,
      53,    -1,    44,    -1,    18,    -1,    15,    -1,    51,    -1,
      49,    12,    50,    -1,    50,    -1,    54,    -1,    53,    -1,
      45,    -1,    13,    -1,    55,    -1,    15,    -1,    19,    52,
      20,    -1,    52,    21,    15,    -1,    15,    -1,    53,    17,
      -1,    17,    -1,    16,    -1,    25,    53,    26,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned char
  keymagic_parser::yyprhs_[] =
  {
         0,     0,     3,     4,     6,     9,    11,    13,    15,    17,
      19,    21,    23,    26,    29,    32,    35,    38,    41,    44,
      46,    48,    50,    52,    54,    56,    60,    64,    66,    68,
      70,    72,    73,    75,    77,    78,    80,    83,    86,    90,
      94,    96,    98,   100,   102,   104,   106,   108,   110,   114,
     116,   118,   120,   122,   124,   126,   128,   132,   136,   138,
     141,   143,   145
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned char
  keymagic_parser::yyrline_[] =
  {
         0,    76,    76,    76,    79,    80,    82,    83,    84,    85,
      86,    87,    89,    90,    91,    92,    93,    94,    96,    97,
      98,   101,   102,   103,   104,   107,   111,   112,   114,   115,
     116,   119,   120,   121,   123,   124,   126,   136,   146,   150,
     151,   153,   154,   155,   156,   157,   158,   159,   162,   163,
     165,   166,   167,   168,   169,   170,   173,   175,   176,   179,
     180,   182,   185
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
      25,    26
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int keymagic_parser::yyeof_ = 0;
  const int keymagic_parser::yylast_ = 105;
  const int keymagic_parser::yynnts_ = 29;
  const int keymagic_parser::yyempty_ = -2;
  const int keymagic_parser::yyfinal_ = 49;
  const int keymagic_parser::yyterror_ = 1;
  const int keymagic_parser::yyerrcode_ = 256;
  const int keymagic_parser::yyntokens_ = 27;

  const unsigned int keymagic_parser::yyuser_token_number_max_ = 281;
  const keymagic_parser::token_number_type keymagic_parser::yyundef_token_ = 2;


} // yy

/* Line 1054 of lalr1.cc  */
#line 1252 "keymagic-parser.tab.cpp"


/* Line 1056 of lalr1.cc  */
#line 186 "keymagic-parser.yy"

void
yy::keymagic_parser::error (const yy::keymagic_parser::location_type& l,
                          const std::string& m)
{
  driver.error (l, m);
}

