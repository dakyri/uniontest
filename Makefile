#CPPFLAGS=-I/home/dkarla/src/gtest-1.7.0/include -I/home/dkarla/repository/unionclientlibrary/include "-I/home/dkarla/repository/tinyxml2" "-I/home/dkarla/repository/libuv/include" -O0 -g3 -Wall -c -std=c++11 -fPIC -pthread
CPPFLAGS=-I/home/dkarla/src/gtest-1.7.0/include "-ID:\src\cpp\workspace\UnionClient\include" "-ID:\src\cpp\workspace\TinyXML2" "-ID:\repository\libuv\include" -O0 -g3 -Wall -c -std=c++11 -fPIC -pthread
OBJS=\
src/UnionTest.o
UTOBJS=\
src/ConnectionTest.o\
src/RoomTest.o

UnionTest: $(OBJS)
#	g++ "-L/home/dkarla/lib" "-L/home/dkarla/repository/unionclientlibrary" -std=c++11 -o UnionTest $(OBJS) -lUnionClient -luv
	g++ "-LD:\src\cpp\workspace\TinyXML2\Debug" "-LD:\repository\libuv\Debug" "-LD:\src\cpp\workspace\UnionClient\Debug" -std=c++11 -o UnionTest $(OBJS) -lUnionClient -luv
clean: 
	rm -rf $(OBJS) $(UTOBJS) ct rt UnionTest
rt: src/RoomTest.o
#	g++ "-L/home/dkarla/lib" "-L/home/dkarla/repository/unionclientlibrary" -std=c++11 -o rt src/RoomTest.o -lUnionClient -luv
	g++ "-LD:\src\cpp\workspace\TinyXML2\Debug" "-LD:\repository\libuv\Debug" "-LD:\src\cpp\workspace\UnionClient\Debug" -std=c++11 -o rt src/RoomTest.o -lUnionClient -luv
ct: src/ConnectionTest.o
#	g++ "-L/home/dkarla/lib" "-L/home/dkarla/repository/unionclientlibrary" -std=c++11 -o ct src/ConnectionTest.o -lUnionClient -luv
	g++ "-LD:\src\cpp\workspace\TinyXML2\Debug" "-LD:\repository\libuv\Debug" "-LD:\src\cpp\workspace\UnionClient\Debug" -std=c++11 -o ct src/ConnectionTest.o -lUnionClient -luv
