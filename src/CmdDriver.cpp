#include <cctype>
#include <fstream>
#include <cassert>

#include "CmdDriver.h"

Cmd::CmdDriver::CmdDriver(UnionClient&uc)
	: scanner(&cin)
	, parser(scanner, *this)
	, onion(uc) {
}

Cmd::CmdDriver::~CmdDriver() {
}

void 
Cmd::CmdDriver::parse(std::istream& inStream)
{
	scanner.yyrestart(&inStream);
	const int accept( 0 );
	if( parser.parse() != accept ) {
		std::cerr << "Unrecognized command!!\n";
	}
}

/*
void 
Cmd::CmdDriver::add_upper()
{ 
	uppercase++;
	chars++;
	words++;
}

void 
Cmd::CmdDriver::add_lower()
{ 
	lowercase++;
	chars++;
	words++;
}

void 
Cmd::CmdDriver::add_word( const std::string &word )
{
	words++;
	chars += word.length();
	for(const char &c : word ){
		if( islower( c ) ) {
			lowercase++;
		} else if ( isupper( c ) )  {
			uppercase++;
		}
	}
}

*/

std::ostream& 
Cmd::CmdDriver::print( std::ostream &stream )
{
	stream << "Uppercase: " << uppercase << "\n";
	return(stream);
}
