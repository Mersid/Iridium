#pragma once

#include "Base.h"

class Derived : public Base
{
public:
	Derived()
	{
		std::cout << "Derived ctor!\n";
	}

	~Derived() override
	{
		std::cout << "Derived dtor!\n";
	}
};