//	EAN.h
//	Soonil Hong
//	s.hong.5181@gmail.com


#include <iostream>
#include <cstring>

class Prefix;

class EAN {
private:
	char eanCode[14];
	char pre[4];
	char areaCode[6];
	char publisher[8];
	char title[7];
	char stl;
	bool reg_ox;
	bool isRegistered(const Prefix & list);
	void part(int start, int n, int &num);            //	to extract some part (as an integer) out of the whole string ean[14]
	void part(int start, int n, char * subStr);       //	to extract some part (as a string) out of the whole string ean[14]
public:
	EAN();
	EAN(const char * str, const Prefix & list);
	bool empty() const;
	bool isRegistered() const;
	void style(char);
	void toStr(char * str) const;
	void toStrWithStyle(char * str) const;
	void display(std::ostream & os) const;
	bool read(std::istream & is, const Prefix & list);
};

bool isValid(const char * str);

bool operator==(const EAN & left, const EAN & right);
std::ostream & operator<<(std::ostream & os, const EAN & ean);
