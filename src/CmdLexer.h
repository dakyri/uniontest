#ifndef __MCSCANNER_HPP__
#define __MCSCANNER_HPP__ 1

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#undef  YY_DECL
#define YY_DECL int  Cmd::CmdLexer::yylex()

#include "CmdParser.tab.hh"

namespace Cmd {

	class CmdLexer : public yyFlexLexer{
	public:

		CmdLexer(std::istream *in) : yyFlexLexer(in),
								  yylval( nullptr ){};

		int yylex(Cmd::CmdParser::semantic_type *lval)
		{
			yylval = lval;
			return( yylex() );
		}


	private:
		int yylex();
		/* yyval ptr */
		Cmd::CmdParser::semantic_type *yylval;
	};

}

#endif /* END __MCSCANNER_HPP__ */
