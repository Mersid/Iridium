#pragma once

#include <iostream>

class Base
{
public:
	Base()
	{
		std::cout << "Base ctor!\n";
	}

	virtual ~Base()
	{
		std::cout << "Base dtor!\n";
	}
};