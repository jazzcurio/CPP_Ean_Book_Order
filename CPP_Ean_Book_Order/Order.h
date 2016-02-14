//	Order.h
//	Soonil Hong
//	s.hong.5181@gmail.com


#include <iostream>
#include <cstring>

#include "EAN.h"

class iOrder {
public:
	virtual EAN & getEAN() = 0;
	virtual bool add(int) = 0;
	virtual bool add(std::istream &) = 0;
	virtual bool receive(std::istream &) = 0;
	virtual int outstanding() const = 0;
	virtual void display(std::ostream &) const = 0;
	virtual ~iOrder() {};
};

class Order : public iOrder {
private:
	EAN eanOrd;
	int ordered;
	int delivered;
public:
	Order();
	Order(const EAN & ean);
	EAN & getEAN();
	int outstanding() const;
	bool add(std::istream & is);
	bool add(int n);
	bool receive(std::istream & is);
	void display(std::ostream & os) const;
};

std::ostream & operator<<(std::ostream & os, const iOrder & order);


class SpecialOrder : public Order {
private:
	char * specInst;
public:
	SpecialOrder();
	SpecialOrder(const EAN & ean, const char * instr);
	bool add(std::istream & is);
	void display(std::ostream & os) const;

	SpecialOrder(const SpecialOrder & source);
	SpecialOrder & operator=(const SpecialOrder & source);
	~SpecialOrder();
};
