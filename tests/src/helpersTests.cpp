#include <iostream>
#include <exception>
#include "../pch.h"

#include "Helpers.h"

TEST(HelpersTests, isCinValidReturnFalse)
{
	std::cin.setstate(std::ios_base::eofbit);
	EXPECT_FALSE(helpers::isCinValid());
}

TEST(HelpersTests, isCinValidThrowException)
{
	std::cin.setstate(std::ios_base::failbit);
    EXPECT_THROW({
        try
        {
            helpers::isCinValid();
        }
        catch (const std::exception& e)
        {
            // this tests that it has the correct message
            EXPECT_STREQ("Reading input stream error.", e.what());
            throw;
        }
    }, std::exception);

    std::cin.setstate(std::ios_base::badbit);
    EXPECT_THROW({
        try
        {
            helpers::isCinValid();
        }
        catch (const std::exception& e)
        {
            EXPECT_STREQ("Reading input stream error.", e.what());
            throw;
        }
        }, std::exception);
}

TEST(HelpersTests, isCinValidReturnTrue)
{
    std::cin.setstate(std::ios_base::goodbit);
    EXPECT_TRUE(helpers::isCinValid());
}
