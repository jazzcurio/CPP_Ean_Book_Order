//	GS1Prefix.h
//	Soonil Hong
//	s.hong.5181@gmail.com


#include <iostream>
#include <cstring>
#include <fstream>

const int MAX = 700;

class Prefix {
private:
	int no;					//	number of entries
	int area[MAX];			//	area elements
	char pubLow[MAX][8];	//	low end of publisher range
	char pubHgh[MAX][8];	//	high end of publisher range
	int pubLen[MAX];		//	no of chars in publisher string
public:
	Prefix();
	~Prefix();
	Prefix(const char * f);
	bool isRegistered(int a) const;					//	returns true if the area is an area element listed in the prefix table;
													//	false otherwise

	int minNoDigits(int a) const;					//	returns the minimum number of digits if the area element is in the prefix table;
													//	zero otherwise

	bool isRegistered(int a, const char * p) const;	//	returns true if the publisher identifier is within a range listed under the area;
													//	false otherwise
};
