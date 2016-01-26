#ifndef __MCDRIVER_HPP__
#define __MCDRIVER_HPP__ 1

#include <string>
#include "CmdLexer.h"
#include "CmdParser.tab.hh"
#include "UCUpperHeaders.h"
#include "UnionClient.h"

namespace Cmd{

class CmdDriver{
public:
	CmdDriver(UnionClient&uc);
	virtual ~CmdDriver();

	void parse(std::istream& inStream);

	std::ostream& print(std::ostream &stream);
private:
	Cmd::CmdLexer scanner;
	Cmd::CmdParser parser;
	UnionClient& onion;
};

}
#endif
