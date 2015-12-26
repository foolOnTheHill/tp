FLAGS=-std=c++11 -Wall -Wextra -O2 -w

all: clean makedir pmt clean

pmt: aho-corasick.o boyer-moore.o wu-manber.o sellers.o
	g++ $(FLAGS) aho-corasick.o boyer-moore.o wu-manber.o sellers.o main.cpp -o ../bin/pmt

main.o: main.cpp
	g++ $(FLAGS) main.cpp -o main.o

aho-corasick.o: aho-corasick.cpp aho-corasick.h
	g++ -c aho-corasick.cpp -o aho-corasick.o

boyer-moore.o: boyer-moore.cpp boyer-moore.h
	g++ -c boyer-moore.cpp -o boyer-moore.o

wu-manber.o: wu-manber.cpp wu-manber.h
	g++ -c wu-manber.cpp -o wu-manber.o

sellers.o: sellers.cpp sellers.h
		g++ -c sellers.cpp -o sellers.o

clean:
	rm -rf *o pmt

makedir:
	mkdir -p ../bin/
