source: CmdParser.tab.cc CmdLexer.cpp 

%cpp: %l
	flex --outfile=$@  $<

%tab.cc: %yy
	/usr/local/bin/bison -d -v $<
%cc: %yy
	/usr/local/bin/bison -d -v --output=$@ $<