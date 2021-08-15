#include <exception>
#include <iostream>

#include "Helpers.h"

bool helpers::isCinValid()
{
	if (std::cin.eof()) {
		return false;
	}

	if (std::cin.fail() || std::cin.bad()) {
		throw std::exception("Reading input stream error.");
	}

	return true;
}
