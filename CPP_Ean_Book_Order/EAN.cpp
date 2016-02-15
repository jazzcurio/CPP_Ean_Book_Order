//	EAN.cpp
//	Soonil Hong
//	s.hong.5181@gmail.com

#include "EAN.h"
#include "GS1Prefix.h"

EAN::EAN() {
	//	safe empty state
	eanCode[0] = '\0';
	pre[0] = '\0';
	areaCode[0] = '\0';
	publisher[0] = '\0';
	title[0] = '\0';
	stl = '-';
	reg_ox = false;
}


EAN::EAN(const char * str, const Prefix & list) {
	//	This creates an EAN object from the string
	//	and checks for registration using the Prefix object
	//	validation of EAN ==> check for registration
	eanCode[0] = '\0';
	pre[0] = '\0';
	areaCode[0] = '\0';
	publisher[0] = '\0';
	title[0] = '\0';
	stl = '-';
	reg_ox = false;

	if (isValid(str)) {
		//strcpy(eanCode, str);
		strcpy_s(eanCode, sizeof eanCode, str);
		eanCode[13] = '\0';
		if (isRegistered(list)) {
			stl = '-';
			reg_ox = true;
			//strncpy(pre, eanCode, 3);
			strncpy_s(pre, sizeof pre, eanCode, 3);
		}
	}
}


bool EAN::empty() const {
	//	returns true if the object is in a SES, false otherwise
	if (eanCode[0] == '\0') {
		return true;
	}
	else {
		return false;
	}
}


bool EAN::isRegistered() const {
	//	returns true if the object is registered, false otherwise
	return reg_ox;
}


void EAN::style(char st_ch) {
	//	sets the style to the character received
	//	if its value is one of the three legitimate values listed above
	//	does nothing if the style character received is not legitimate
	if (st_ch == '-' || st_ch == ' ' || st_ch == '\0') {
		stl = st_ch;
	}
}


void EAN::toStr(char * str) const {
	//	fills the address with the object's EAN
	//	assumes that the caller has allocated enough space to hold a 13-character string
	//strcpy(str, eanCode);
	strcpy_s(str, 14, eanCode);
	str[13] = '\0';
}

void EAN::toStrWithStyle(char * str) const {
	//	fille the address with a formatted version of the object's EAN appling the current style
	//	assumes that the caller has allocated enough space to hold up to a 17-character string
	int count = 0;

	if (!isRegistered() || stl == '\0') {
		toStr(str);
	}
	else {

		for (size_t i = 0; i < 3; i++) {
			str[count++] = pre[i];
		}

		str[count++] = stl;

		for (size_t j = 0; j < strlen(areaCode); j++) {
			str[count++] = areaCode[j];
		}

		str[count++] = stl;

		for (size_t k = 0; k < strlen(publisher); k++) {
			str[count++] = publisher[k];
		}

		str[count++] = stl;

		for (size_t l = 0; l < strlen(title); l++) {
			str[count++] = title[l];
		}

		str[count++] = stl;
		str[count++] = eanCode[12];
		str[count] = '\0';
	}
}

void EAN::display(std::ostream & os) const {
	//	insert the EAN string in the current style into output stream os
	//	If the EAN is not registered, this function displays it in concatenated style.
	//	In either case, your function displays the EAN right-justified in a field of 17 characters
	char s[18];
	toStrWithStyle(s);
	os.width(17);
	os << s;
}

bool EAN::read(std::istream & is, const Prefix & list) {
	//	prompts for and accepts a string of up to 13 characters
	//	if a valid EAN, accepts it and stores it in the current object
	//	if the input is unacceptable, this requests anouther string or '0' to quit
	//	if '0' is entered, this doesn't change the current object and return false; true otherwise
	bool keepgoing = true;
	bool ret_val = false;
	char ean_[14];

	while (keepgoing) {
		std::cout << "EAN (0 to quit) : ";
		is.getline(ean_, 14);

		if (!is) {
			is.clear();
			is.ignore(2000, '\n');
			std::cerr << "Error. Try Again " << std::endl;
		}
		else if (strcmp(ean_, "0") == 0) {
			keepgoing = false;
		}
		else if (strlen(ean_) < 13) {
			std::cerr << "Too few characters. Try again." << std::endl;
		}
		else if (!isValid(ean_)) {
			std::cerr << "Invalid check digit. Try again." << std::endl;
		}
		else {
			EAN goodEan(ean_, list);
			*this = goodEan;
			ret_val = true;
			keepgoing = false;
		}
	}

	return ret_val;
}

bool isValid(const char * str) {
	//	validation for check digit

	int candi[13];
	int total = 0;
	bool vali = false;

	if (str != NULL && strlen(str) == 13) {

		for (int i = 0; i < 13; i++) {
			candi[i] = str[i] - '0';
		}

		for (int i = 1; i < 13; i = i + 2) {
			total += (candi[i] * 3);
		}

		for (int i = 0; i < 13; i = i + 2) {
			total += candi[i];
		}

		if ((total % 10) == 0) {
			vali = true;
		}

	}

	return vali;
}



bool EAN::isRegistered(const Prefix & list) {
	//(const Prefix * p, const char * eanCode, char areaCode[], char publisher[], char title[])

	bool Reg_area = false, Reg_pub = false, keepgoing = true;
	int area_int, area_len, pub_len;
	char pub[8];
	char tit[7];
	int minND = 0;

	for (int i = 1; i <= 5 && keepgoing == true; i++) {
		part(4, i, area_int);

		if (list.isRegistered(area_int)) {
			Reg_area = true;
			area_len = i;

			for (int j = 0; j < area_len; j++) {
				areaCode[j] = eanCode[3 + j];
			}

			areaCode[area_len] = '\0';

			keepgoing = false;
		}
	}

	keepgoing = true;

	if (Reg_area == false) {
		return false;
	}

	minND = list.minNoDigits(area_int);

	for (int i = minND; i <= 7 && keepgoing == true; i++) {
		part(4 + area_len, i, pub);

		if (list.isRegistered(area_int, pub)) {
			Reg_pub = true;
			pub_len = i;

			//strcpy(publisher, pub);
			strcpy_s(publisher, sizeof publisher, pub);

			keepgoing = false;
		}
	}

	if (Reg_pub == false) {
		return false;
	}

	part(4 + area_len + pub_len, 9 - area_len - pub_len, tit);

	//strcpy(title, tit);
	strcpy_s(title, sizeof title, tit);

	return true;
}


//	to extract some part (as an integer) out of the whole string ean[14]
void EAN::part(int start, int n, int &num) {
	int t = 1;

	num = 0;

	for (int i = 0; i < n; i++) {
		num += (eanCode[start + n - 2 - i] - '0') * t;
		t *= 10;
	}
}


//	to extract some part (as a string) out of the whole string ean[14]
void EAN::part(int start, int n, char * subStr) {
	for (int i = 0; i < n; i++) {
		subStr[i] = eanCode[start - 1 + i];
	}
	subStr[n] = '\0';
}


//	Helper Function that determines if two EAN objects are the same or not
bool operator==(const EAN & left, const EAN & right) {
	bool ret = false;

	char l[14];
	char r[14];

	left.toStr(l);
	right.toStr(r);

	if (strcmp(l, r) == 0) {
		ret = true;
	}

	return ret;
}


//	Helper Operator to display the ean in a specific style
std::ostream & operator<<(std::ostream & os, const EAN & ean) {
	ean.display(os);

	return os;
}
