
#ifndef BITARRAY_H_
#define BITARRAY_H_

#include <iostream>

using namespace std;

class BitArray {
private:
	static const int BLOCK_SIZE = sizeof(long) * 8;

	unsigned long *arr;
	int size;
	int bits;

public:
	BitArray() {
		this->bits = 1;
		size = (bits-1)/BLOCK_SIZE + 1;
		arr = new unsigned long[size];

		for (int i = 0; i < size; i++) {
			arr[i] = 0;
		}
	}

	BitArray(int bits) {
		this->bits = bits;
		size = (bits-1)/BLOCK_SIZE + 1;
		arr = new unsigned long[size];

		for (int i = 0; i < size; i++) {
			arr[i] = 0;
		}
	}

	BitArray(BitArray &o) {
		size = o.size;
		bits = o.bits;
		arr = new unsigned long[size];

		for (int i = 0; i < size; i++) {
			arr[i] = o.arr[i];
		}
	}

	~BitArray() {
		delete[] arr;
	}

	void shiftLeft() {
		int last = 0;
		for (int i = 0; i < size-1; i++) {
			last = ((arr[i+1] >> (BLOCK_SIZE-1)) & 1);
			arr[i] <<= 1;
			arr[i] |= last;
		}
		arr[size-1] <<= 1;
	}

	void set() {
		for (int i = 0; i < size; i++) {
			arr[i] = ~0;
		}
	}

	void set(int pos) {
		arr[size - pos/BLOCK_SIZE - 1] |= 1U << pos%BLOCK_SIZE;
	}

	void reset() {
		for (int i = 0; i < size; i++) {
			arr[i] = 0;
		}
	}

	void reset(int pos) {
		arr[size - pos/BLOCK_SIZE - 1] &= ~(1U << pos%BLOCK_SIZE);
	}


	BitArray operator | (const BitArray &b) {
		BitArray r(*this);
		if (size == b.size) {
			for (int i = 0; i < size; i++) {
				r.arr[i] = arr[i] | b.arr[i];
			}

		} else {
			cout << "| Operator only works for BitArrays of same size " << size << " " << b.size << endl;
		}

		return r;
	}

	BitArray operator & (const BitArray &b) {
		BitArray r(*this);
		if (size == b.size) {
			for (int i = 0; i < size; i++) {
				r.arr[i] = arr[i] & b.arr[i];
			}
		} else {
			cout << "& Operator only works for BitArrays of same size" << endl;
		}

		return r;
	}

	BitArray operator ~ () {
		BitArray r(*this);
		for (int i = 0; i < size; i++) {
			r.arr[i] = ~arr[i];
		}
		return r;
	}

	void operator = (const BitArray &b) {
		size = b.size;
		bits = b.bits;

		for (int i = 0; i < size; i++) {
			arr[i] = b.arr[i];
		}
	}

	BitArray operator << (const unsigned int n) {
		BitArray r(*this);
		if (n == 1) {
			int last = 0;
			for (int i = 0; i < size-1; i++) {
				last = ((arr[i+1] >> 31) & 1);
				r.arr[i] <<= 1;
				r.arr[i] |= last;
			}
			r.arr[size-1] <<= 1;
		}

		return r;
	}


	bool isZero() {
		bool r = true;
		for (int i = 0; i < size; i++) {
			if (arr[i] != 0) {
				r = false;
				break;
			}
		}
		return r;
	}
};


#endif
