FLAGS=-std=c++11 -Wall -Wextra -O3 -w

all: clean makedir ipmt clean

ipmt: SuffixArray.o SuffixTree.o lzw.o lz77.o compress.o search.o
	g++ $(FLAGS) SuffixArray.o SuffixTree.o lzw.o lz77.o compress.o search.o main.cpp -o ../bin/ipmt

main.o: main.cpp
	g++ $(FLAGS) main.cpp -o main.o

lz77.o: lz77.cpp
	g++ $(FLAGS) -c lz77.cpp -o lz77.o

lzw.o: lzw.cpp
	g++ $(FLAGS) -c lzw.cpp -o lzw.o

compress.o: compress.cpp
	g++ $(FLAGS) -c compress.cpp -o compress.o

search.o: search.cpp
	g++ $(FLAGS) -c search.cpp -o search.o

SuffixArray.o: SuffixArray.cpp
	g++ $(FLAGS) -c SuffixArray.cpp -o SuffixArray.o

SuffixTree.o: SuffixTree.cpp
	g++ $(FLAGS) -c SuffixTree.cpp -o SuffixTree.o

clean:
	rm -rf *o pmt

makedir:
	mkdir -p ../bin/
