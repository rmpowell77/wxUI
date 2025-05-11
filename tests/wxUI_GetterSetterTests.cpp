/*
MIT License

Copyright (c) 2022-2025 Richard Powell

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include <wxUI/GetterSetter.hpp>

#include <wx/wx.h>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, cppcoreguidelines-avoid-do-while, readability-magic-numbers, readability-function-cognitive-complexity, hicpp-function-size, misc-use-anonymous-namespace)
#define BASIC_CHECK()                                   \
    do {                                                \
        CHECK(value == checkValue);                     \
        CHECK(getterCallCount == checkGetterCallCount); \
        CHECK(setterCallCount == checkSetterCallCount); \
    } while (0)

TEST_CASE("GetterSetter")
{
    SECTION("basics")
    {
        auto getterCallCount = int {};
        auto setterCallCount = int {};
        auto value = int {};

        auto uut = wxUI::details::GetterSetter {
            [&] {
                ++getterCallCount;
                return value;
            },
            [&](int v) {
                ++setterCallCount;
                value = v;
            }
        };
        // Basic check
        CHECK(value == 0);
        CHECK(getterCallCount == 0);
        CHECK(setterCallCount == 0);

        CHECK(0 == uut.get());

        CHECK(value == 0);
        CHECK(getterCallCount == 1);
        CHECK(setterCallCount == 0);

        uut.set(1);

        CHECK(value == 1);
        CHECK(getterCallCount == 1);
        CHECK(setterCallCount == 1);

        // Operator check
        CHECK(1 == static_cast<int>(uut));

        CHECK(value == 1);
        CHECK(getterCallCount == 2);
        CHECK(setterCallCount == 1);

        uut = 2;

        CHECK(value == 2);
        CHECK(getterCallCount == 2);
        CHECK(setterCallCount == 2);

        auto ostream = std::ostringstream {};
        ostream << uut;
    };

    SECTION("strings")
    {
        auto setterCallCount = int {};
        auto getterCallCount = int {};
        auto value = std::string { "hello" };

        auto checkGetterCallCount = int {};
        auto checkSetterCallCount = int {};
        auto checkValue = std::string { "hello" };

        auto uut = wxUI::details::GetterSetter {
            [&] {
                ++getterCallCount;
                return value;
            },
            [&](std::string v) {
                ++setterCallCount;
                value = v;
            }
        };

        BASIC_CHECK();

        CHECK("hello" == uut.get());
        checkGetterCallCount += 1;

        BASIC_CHECK();

        // << and >>
        auto ostream = std::ostringstream {};
        ostream << uut;
        checkGetterCallCount += 1;

        CHECK(ostream.str() == "hello");

        BASIC_CHECK();

        auto istream = std::istringstream { "test1\ntest2" };
        istream >> uut;
        checkSetterCallCount += 1;
        checkValue = "test1";

        BASIC_CHECK();
    };

    SECTION("ints")
    {
        auto getterCallCount = int {};
        auto setterCallCount = int {};
        auto value = int {};

        auto checkGetterCallCount = int {};
        auto checkSetterCallCount = int {};
        auto checkValue = int {};

        auto uut = wxUI::details::GetterSetter {
            [&] {
                ++getterCallCount;
                return value;
            },
            [&](int v) {
                ++setterCallCount;
                value = v;
            }
        };
        BASIC_CHECK();

        CHECK(0 == uut.get());
        checkGetterCallCount += 1;

        BASIC_CHECK();

        uut.set(1);
        checkSetterCallCount += 1;
        checkValue = 1;

        BASIC_CHECK();

        // Operator check
        CHECK(1 == static_cast<int>(uut));
        checkGetterCallCount += 1;

        BASIC_CHECK();

        uut = 2;
        checkSetterCallCount += 1;
        checkValue = 2;

        BASIC_CHECK();

        // comparisions
        CHECK(2 == uut);
        checkGetterCallCount += 1;

        BASIC_CHECK();

        CHECK(1 != uut);
        checkGetterCallCount += 1;

        BASIC_CHECK();

        CHECK(3 > uut);
        checkGetterCallCount += 1;

        BASIC_CHECK();

        CHECK(2 >= uut);
        checkGetterCallCount += 1;

        BASIC_CHECK();

        CHECK(1 < uut);
        checkGetterCallCount += 1;

        BASIC_CHECK();

        CHECK(2 <= uut);
        checkGetterCallCount += 1;

        BASIC_CHECK();

        CHECK(uut == 2);
        checkGetterCallCount += 1;

        BASIC_CHECK();

        CHECK(uut != 1);
        checkGetterCallCount += 1;

        BASIC_CHECK();

        CHECK(uut < 3);
        checkGetterCallCount += 1;

        BASIC_CHECK();

        CHECK(uut <= 2);
        checkGetterCallCount += 1;

        BASIC_CHECK();

        CHECK(uut > 1);
        checkGetterCallCount += 1;

        BASIC_CHECK();

        CHECK(uut >= 2);
        checkGetterCallCount += 1;

        BASIC_CHECK();

        // compound assignments
        uut += 1;
        checkGetterCallCount += 1;
        checkSetterCallCount += 1;
        checkValue += 1;

        BASIC_CHECK();

        uut -= 1;
        checkGetterCallCount += 1;
        checkSetterCallCount += 1;
        checkValue -= 1;

        BASIC_CHECK();

        uut *= 2;
        checkGetterCallCount += 1;
        checkSetterCallCount += 1;
        checkValue *= 2;

        BASIC_CHECK();

        uut /= 2;
        checkGetterCallCount += 1;
        checkSetterCallCount += 1;
        checkValue /= 2;

        BASIC_CHECK();

        value = 10;
        uut %= 7;
        checkGetterCallCount += 1;
        checkSetterCallCount += 1;
        checkValue = 3;

        BASIC_CHECK();

        value = 2;
        uut <<= 2;
        checkGetterCallCount += 1;
        checkSetterCallCount += 1;
        checkValue = 8;

        BASIC_CHECK();

        uut >>= 2;
        checkGetterCallCount += 1;
        checkSetterCallCount += 1;
        checkValue = 2;

        BASIC_CHECK();

        uut |= 4;
        checkGetterCallCount += 1;
        checkSetterCallCount += 1;
        checkValue = 6;

        BASIC_CHECK();

        uut &= 4;
        checkGetterCallCount += 1;
        checkSetterCallCount += 1;
        checkValue = 4;

        BASIC_CHECK();

        uut ^= 6;
        checkGetterCallCount += 1;
        checkSetterCallCount += 1;
        checkValue = 2;

        BASIC_CHECK();

        // chaining
        value = 3;
        auto t = 2 + (uut = 5);
        checkGetterCallCount += 1;
        checkSetterCallCount += 1;
        checkValue = 5;

        CHECK(t == 7);
        BASIC_CHECK();

        // increment decrement
        value = 3;
        t = uut++;
        checkGetterCallCount += 1;
        checkSetterCallCount += 1;
        checkValue = 4;

        CHECK(t == 3);
        BASIC_CHECK();

        t = ++uut;
        // this increments by 2 because there's the getter for ++ and then for the assignment to t
        checkGetterCallCount += 2;
        checkSetterCallCount += 1;
        checkValue = 5;

        CHECK(t == 5);
        BASIC_CHECK();

        t = uut--;
        checkGetterCallCount += 1;
        checkSetterCallCount += 1;
        checkValue = 4;

        CHECK(t == 5);
        BASIC_CHECK();
        t = --uut;
        // this increments by 2 because there's the getter for -- and then for the assignment to t
        checkGetterCallCount += 2;
        checkSetterCallCount += 1;
        checkValue = 3;

        CHECK(t == 3);
        BASIC_CHECK();

        // << and >>
        auto ostream = std::ostringstream {};
        ostream << uut;
        checkGetterCallCount += 1;

        CHECK(ostream.str() == "3");
        BASIC_CHECK();

        auto istream = std::istringstream { "2\n3.5" };
        istream >> uut;
        checkSetterCallCount += 1;
        checkValue = 2;

        BASIC_CHECK();

        istream >> uut;
        checkSetterCallCount += 1;
        checkValue = 3;

        BASIC_CHECK();
    };

    SECTION("bools")
    {
        auto getterCallCount = int {};
        auto setterCallCount = int {};
        auto value = bool {};

        auto checkGetterCallCount = int {};
        auto checkSetterCallCount = int {};
        auto checkValue = bool {};

        auto uut = wxUI::details::GetterSetter {
            [&] {
                ++getterCallCount;
                return value;
            },
            [&](bool v) {
                ++setterCallCount;
                value = v;
            }
        };
        BASIC_CHECK();

        CHECK(false == uut.get());
        checkGetterCallCount += 1;

        BASIC_CHECK();

        uut.set(true);
        checkSetterCallCount += 1;
        checkValue = 1;

        BASIC_CHECK();

        // Operator check
        CHECK(true == static_cast<bool>(uut));
        checkGetterCallCount += 1;

        BASIC_CHECK();

        uut = false;
        checkSetterCallCount += 1;
        checkValue = false;

        BASIC_CHECK();

        // comparisions
        CHECK(false == uut);
        checkGetterCallCount += 1;

        BASIC_CHECK();

        CHECK(true != uut);
        checkGetterCallCount += 1;

        BASIC_CHECK();

        CHECK(true > uut);
        checkGetterCallCount += 1;

        BASIC_CHECK();

        CHECK(false >= uut);
        checkGetterCallCount += 1;

        BASIC_CHECK();

        value = true;
        CHECK(false < uut);
        checkGetterCallCount += 1;
        checkValue = true;

        BASIC_CHECK();

        CHECK(true <= uut);
        checkGetterCallCount += 1;

        BASIC_CHECK();

        CHECK(uut == true);
        checkGetterCallCount += 1;

        BASIC_CHECK();

        CHECK(uut != false);
        checkGetterCallCount += 1;

        BASIC_CHECK();

        value = false;
        CHECK(uut < true);
        checkGetterCallCount += 1;
        checkValue = false;

        BASIC_CHECK();

        CHECK(uut <= false);
        checkGetterCallCount += 1;

        BASIC_CHECK();

        value = true;
        CHECK(uut > false);
        checkGetterCallCount += 1;
        checkValue = true;

        BASIC_CHECK();

        CHECK(uut >= true);
        checkGetterCallCount += 1;

        BASIC_CHECK();

        // compound assignments
        value = true;
        uut |= true;
        checkGetterCallCount += 1;
        checkSetterCallCount += 1;
        checkValue = true;

        BASIC_CHECK();

        uut &= false;
        checkGetterCallCount += 1;
        checkSetterCallCount += 1;
        checkValue = false;

        BASIC_CHECK();

        uut ^= true;
        checkGetterCallCount += 1;
        checkSetterCallCount += 1;
        checkValue = true;

        BASIC_CHECK();

        // chaining
        value = true;
        auto t = true && (uut = false);
        checkGetterCallCount += 1;
        checkSetterCallCount += 1;
        checkValue = false;

        CHECK(t == false);
        BASIC_CHECK();

        // << and >>
        auto ostream = std::ostringstream {};
        ostream << uut;
        checkGetterCallCount += 1;

        CHECK(ostream.str() == "0");
        BASIC_CHECK();

        auto istream = std::istringstream { "1\n0" };
        istream >> uut;
        checkSetterCallCount += 1;
        checkValue = true;

        BASIC_CHECK();

        istream >> uut;
        checkSetterCallCount += 1;
        checkValue = false;

        BASIC_CHECK();
    };

    SECTION("floats")
    {
        auto getterCallCount = int {};
        auto setterCallCount = int {};
        auto value = float {};

        auto checkGetterCallCount = int {};
        auto checkSetterCallCount = int {};
        auto checkValue = float {};

        auto uut = wxUI::details::GetterSetter {
            [&] {
                ++getterCallCount;
                return value;
            },
            [&](float v) {
                ++setterCallCount;
                value = v;
            }
        };
        // Basic check
        BASIC_CHECK();

        uut = 2.5;
        checkSetterCallCount += 1;
        checkValue = 2.5;

        BASIC_CHECK();

        // << and >>
        auto ostream = std::ostringstream {};
        ostream << uut;
        checkGetterCallCount += 1;

        CHECK(ostream.str() == "2.5");
        BASIC_CHECK();

        auto istream = std::istringstream { "2\n3.5" };
        istream >> uut;
        checkSetterCallCount += 1;
        checkValue = 2.0;

        BASIC_CHECK();

        istream >> uut;
        checkSetterCallCount += 1;
        checkValue = 3.5;

        BASIC_CHECK();
    }
    // put all the negative tests here.
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, cppcoreguidelines-avoid-do-while, readability-magic-numbers, readability-function-cognitive-complexity, hicpp-function-size, misc-use-anonymous-namespace)
