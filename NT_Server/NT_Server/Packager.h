#pragma once
#include <string>
class Packager
{
public:
	Packager();
	~Packager();
	int a;
	char b;
	std::string meta = "";
	std::string toString();
private:

};

Packager::Packager()
{
}

Packager::~Packager()
{
}
std::string Packager::toString() {
	return std::to_string(a)+" "+b;
}