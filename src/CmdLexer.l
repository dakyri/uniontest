%{
/* C++ string header, for string ops below */
#include <string>
/* Implementation of yyFlexScanner */ 
#include "CmdLexer.h"

/* typedef to make the returns for the tokens shorter */
typedef Cmd::CmdParser::token token;

/* define yyterminate as this instead of NULL */
#define yyterminate() return( token::END )

/* msvc2010 requires that we exclude this header file. */
#define YY_NO_UNISTD_H

%}
 
%option debug 
%option nodefault 
%option yyclass="CmdLexer" 
%option noyywrap 
%option c++

NUM	[0-9]
ALF	[a-zA-Z]
ALNUM [a-zA-Z0-9]
ID {ALF}{ALNUM}*

%%
{NUM}+	{ 
	return(token::INT);     
}

"=" {
	return(token::ASSGN);
}

"[" {
	return(token::LSQB);
}

"]" {
	return(token::RSQB);
}

"{" {
	return(token::LBRA);
}

"}" {
	return(token::RBRA);
}

room {
	return(token::ROOM);
}

client {
	return(token::CLIENT);
}

join {
	return(token::JOIN);
}

leave {
	return(token::LEAVE);
}

"$"{ID} {
	yylval->sval = new std::string(yytext);  
	return(token::SYMBOL);      
}

{ID} { 
	yylval->sval = new std::string(yytext);  
	return(token::WORD);      
}

[ \t] {
	// skip white space
}

\n { 
	// skip white space, but perhaps also give an interactive prompt
}

. { 
	; // oops      
}
%%


