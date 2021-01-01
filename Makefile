all: main.cpp board.cpp board.hpp
	g++ -o app main.cpp board.cpp

debug: main.cpp board.cpp board.hpp
	g++ -g -o debug main.cpp board.cpp

