//	GS1Prefix.cpp
//	Soonil Hong
//	s.hong.5181@gmail.com

#include "GS1Prefix.h"

Prefix::Prefix() {
	//	A no-argument constructor - safe empty state
	//	-1 for ints
	//	"" for strings

	no = -1;
	for (int i = 0; i < MAX; i++) {
		area[i] = -1;
		pubLow[i][0] = '\0';
		pubHgh[i][0] = '\0';
		pubLen[i] = -1;
	}
}


Prefix::~Prefix()
{ }


//	A one-argument constructor
//	load copies the porefix table from f
Prefix::Prefix(const char * f) {
	//	bool rc = false;

	if (f != nullptr) {
		std::ifstream file(f);

		if (file.is_open()) {
			int n = 0;

			do {
				file >> area[n] >> pubLow[n] >> pubHgh[n];

				if (file.good()) {
					pubLen[n] = strlen(pubLow[n]);
					n++;
				}
			} while (file.good());

			if (n > 0) {
				//	rc = true;
				no = n;
			}
		}
	}
}


bool Prefix::isRegistered(int a) const
{
	int n = 0;
	bool ret_val = false;
	bool keepgoing = true;

	while (n < no && keepgoing == true) {
		if (a == area[n]) {
			ret_val = true;
			keepgoing = false;
		}
		else {
			n++;
		}
	};

	return ret_val;
}


int Prefix::minNoDigits(int a) const {
	//	getting minimum among pubLen[n]
	int ret_val = 0;
	int n = 0;

	while (n < no) {
		if (a == area[n]) {
			if (ret_val == 0 || ret_val > pubLen[n]) {
				ret_val = pubLen[n];
			}
		}

		n++;
	};

	return ret_val;
}


bool Prefix::isRegistered(int a, const char * p) const {
	int pl, ph;
	int pubCode = atoi(p);
	bool ret_val = false;
	bool keepgoing = true;

	for (int i = 0; i < no && keepgoing == true; i++) {
		if (area[i] == a) {
			if (strlen(p) == pubLen[i]) {
				pl = atoi(pubLow[i]);
				ph = atoi(pubHgh[i]);

				if (pl <= pubCode && pubCode <= ph) {
					ret_val = true;
					keepgoing = false;
				}
			}
		}
	}

	return ret_val;
}
