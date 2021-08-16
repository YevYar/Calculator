#include "../pch.h"
#include <iostream>

#include "Helpers.h"

TEST(HelpersTests, isCinValidReturnFalse)
{
	std::cin.setstate(std::ios_base::eofbit);
	EXPECT_FALSE(helpers::isCinValid());
}
