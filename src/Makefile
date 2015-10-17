FLAGS=-std=c++11 -Wall -Wextra -O2 -w

all: clean makedir pmt clean

pmt: aho-corasick.o boyer-moore.o
	g++ $(FLAGS) aho-corasick.o boyer-moore.o main.cpp -o ../bin/pmt

main.o: main.cpp
	g++ $(FLAGS) main.cpp -o main.o

aho-corasick.o: aho-corasick.cpp aho-corasick.h
	g++ -c aho-corasick.cpp -o aho-corasick.o

boyer-moore.o: boyer-moore.cpp boyer-moore.h
	g++ -c boyer-moore.cpp -o boyer-moore.o

clean:
	rm -rf *o pmt

makedir:
	mkdir -p ../bin/
