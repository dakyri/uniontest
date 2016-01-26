%skeleton "lalr1.cc"
%require  "3.0"
%debug 
%defines 
%define api.namespace {Cmd}
%define parser_class_name {CmdParser}

%code requires{
	namespace Cmd {
		class CmdDriver;
 		class CmdLexer;
	}
}

%lex-param { CmdLexer &scanner }
%parse-param { CmdLexer &scanner }

%lex-param { CmdDriver &driver }
%parse-param { CmdDriver &driver }

%code{
   #include <iostream>
   #include <cstdlib>
   #include <fstream>
   
   /* include for all driver functions */
   #include "CmdDriver.h"
  
   /* this is silly, but I can't figure out a way around */
	static int yylex(Cmd::CmdParser::semantic_type *yylval,
					Cmd::CmdLexer &scanner,
					Cmd::CmdDriver &driver);
}

/* token types */
%union {
	std::string *sval;
	int ival;
}
 
%token			END	0	 "end of file"
%token	<sval>	WORD
%token	<sval>	SYMBOL
%token	<ival>	INT
%token			ASSGN
%token			LSQB
%token			RSQB
%token			LBRA
%token			RBRA
%token			ROOM
%token			CLIENT
%token			JOIN
%token			LEAVE
%token			NEWLINE


/* destructor rule for <sval> objects */
%destructor { if ($$)  { delete ($$); ($$) = nullptr; } } <sval>


%%

list_option : END | command_list END;

command_list : command  | command_list command;

command : create_room_command { }
		| join_room_command { }
		| create_client_command { }
		| WORD	{ }
		| NEWLINE { }
	;
	
create_room_command : ROOM expression {};

join_room_command : JOIN expression {}; 

create_client_command : CLIENT expression {};

expression	: WORD
		;

%%
 

void 
Cmd::CmdParser::error( const std::string &err_message )
{
   std::cerr << "Error: " << err_message << "\n"; 
}


/* include for access to scanner.yylex */
#include "CmdLexer.h"
static int 
yylex( Cmd::CmdParser::semantic_type *yylval,
	   Cmd::CmdLexer  &scanner,
	   Cmd::CmdDriver   &driver )
{
   return( scanner.yylex(yylval) );
}

