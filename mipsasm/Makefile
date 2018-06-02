headers = lexer.h parser.h compile.h instruction.h
objects = lexer.o parser.o compile.o main.o

flags = -std=c++11 -Wall -Wextra

all: main

main: $(headers) $(objects)
	g++ $(flags) -o main $(objects)

main.o: main.cpp $(headers)
	g++ $(flags) -c -o $@ main.cpp

lexer.o: lexer.cpp $(headers)
	g++ $(flags) -c -o $@ lexer.cpp

parser.o: parser.cpp $(headers)
	g++ $(flags) -c -o $@ parser.cpp

compile.o: compile.cpp $(headers)
	g++ $(flags) -c -o $@ compile.cpp

clean:
	rm -f $(objects)

distclean:
	rm -f $(objects) main
