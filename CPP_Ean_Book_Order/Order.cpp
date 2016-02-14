//	Order.cpp
//	Soonil Hong
//	s.hong.5181@gmail.com

#include <string>
#include "Order.h"

Order::Order() {
	eanOrd = EAN();
	ordered = 0;
	delivered = 0;
}

Order::Order(const EAN & ean) {
	eanOrd = ean;
	ordered = 0;
	delivered = 0;
}

EAN & Order::getEAN() {
	return eanOrd;
}

int Order::outstanding() const {
	return ordered;
}

bool Order::add(std::istream & is) {
	int ordered_;
	bool ret = false;

	std::cout << "Quantity (0 to quit) : ";
	is >> ordered_;
	is.ignore();

	if (ordered_ > 0) {
		ordered += ordered_;
		ret = true;
	}

	return ret;
}

bool Order::add(int n) {
	bool ret = false;

	if (n > 0 && !eanOrd.empty()) {
		ordered += n;
		ret = true;
	}

	return ret;
}

bool Order::receive(std::istream & is) {
	int delivered_;
	bool ret = false;
	bool keepgoing = true;

	int onOrder = ordered - delivered;

	while (keepgoing) {
		std::cout << "Quantity (0 to quit) : ";
		is >> delivered_;
		is.ignore();

		if (delivered_ == 0) {
			std::cout << "**No delivery recorded!" << std::endl;
			keepgoing = false;
		}
		else if (delivered_ > onOrder) {
			std::cout << delivered_ << " not on order. Only " << onOrder;
			std::cout << " are on order. Try again." << std::endl;
		}
		else if (delivered_ < 0) {
			std::cout << "Enter a positive number. Try again." << std::endl;
		}
		else {
			delivered += delivered_;
			ret = true;
			keepgoing = false;
		}
	}

	return ret;
}

void Order::display(std::ostream & os) const {
	eanOrd.display(os);
	os.width(9);
	os << ordered;
	os.width(11);
	os << delivered;
}

std::ostream & operator<<(std::ostream & os, const iOrder & order) {
	order.display(os);

	return os;
}


SpecialOrder::SpecialOrder() {
	//	no-argument constructor - safe empty state
	specInst = nullptr;
}

SpecialOrder::SpecialOrder(const EAN & ean, const char * instr) : Order(ean) {
	//	2-argument constructor - "ean" for the base class ctor and "instr" for the derived class ctor
	if (instr != nullptr) {
		specInst = new char[std::strlen(instr) + 1];
		//std::strcpy(specInst, instr);
		strcpy_s(specInst, sizeof specInst, instr);
	}
	else {
		specInst = nullptr;
	}
}

bool SpecialOrder::add(std::istream & is) {
	//	a modifier that adds the quantity ordered and special instructions to the current object

	bool ret = false;
	std::string str;

	if (Order::add(is)) {
		std::cout << "Instructions : ";

		if (std::getline(is, str)) {
			specInst = new char[str.length() + 1];
			//std::strcpy(specInst, str.c_str());
			strcpy_s(specInst, sizeof specInst, str.c_str());
			ret = true;
		}
	}

	return ret;
}

void SpecialOrder::display(std::ostream & os) const {
	//	a query that isplays a SpecialOrder
	//	- the order's EAN right-justified in a field of 17
	//	- the number of copies ordered in a field of 9
	//	- the number of copies delivered in a field of 11
	//	- the special instruction
	Order::display(os);
	os << ' ' << specInst;
}


SpecialOrder::SpecialOrder(const SpecialOrder & source) : Order(source) {
	//	a copy constructor
	if (source.specInst != nullptr) {
		specInst = new char[std::strlen(source.specInst) + 1];
		//std::strcpy(specInst, source.specInst);
		strcpy_s(specInst, sizeof specInst, source.specInst);
	}
	else {
		specInst = nullptr;
	}
}

SpecialOrder & SpecialOrder::operator=(const SpecialOrder & source) {
	//	an assignment operator
	if (this != &source) {
		Order & order = *this;
		order = source;
		if (source.specInst != nullptr) {
			specInst = new char[std::strlen(source.specInst) + 1];
			//std::strcpy(specInst, source.specInst);
			strcpy_s(specInst, sizeof specInst, source.specInst);
		}
		else {
			specInst = nullptr;
		}
	}
	return *this;
}

SpecialOrder::~SpecialOrder() {
	//	a destructor that deallocates memory
	delete[] specInst;
}
