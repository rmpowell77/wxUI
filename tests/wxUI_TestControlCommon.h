/*
MIT License

Copyright (c) 2022 Richard Powell

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
#include <wx/wx.h>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity)

constexpr auto addWithStyle = [](auto style, auto&& inUUT) {
    return inUUT.withStyle(style);
};

constexpr auto addWithPosition = [](auto pos, auto&& inUUT) {
    return inUUT.withPosition(pos);
};

constexpr auto addWithSize = [](auto size, auto&& inUUT) {
    return inUUT.withSize(size);
};

constexpr auto checkWithStyle = [](auto style, auto* window) {
    CHECK((window->GetWindowStyle() & style) == style);
    return window;
};

constexpr auto checkWithPosition = [](auto pos, auto* window) {
    CHECK(window->GetPosition() == pos);
    return window;
};

constexpr auto checkWithSize = [](auto size, auto* window) {
    CHECK(window->GetSize() == size);
    return window;
};

constexpr auto checkAll = [](auto* window, auto style, auto pos, auto size) {
    return checkWithStyle(style, checkWithPosition(pos, checkWithSize(size, window)));
};

// How do we test chaining?
template <typename WHICH>
auto DoChainingIterations()
{
    auto style = WHICH::testStyle();
    auto pos = WHICH::testPosition();
    auto size = WHICH::testSize();
    auto expectedStyle = WHICH::expectedStyle();
    auto expectedPos = WHICH::expectedPosition();
    auto expectedSize = WHICH::expectedSize();
    wxFrame frame { nullptr, wxID_ANY, "" };
    {
        auto uut = addWithStyle(style, addWithPosition(pos, addWithSize(size, WHICH::createUUT())));
        checkAll(dynamic_cast<typename WHICH::TypeUnderTest::underlying_t*>(uut.create(&frame)), expectedStyle, expectedPos, expectedSize);
    }
    {
        auto uut = addWithPosition(pos, addWithStyle(style, addWithSize(size, WHICH::createUUT())));
        checkAll(dynamic_cast<typename WHICH::TypeUnderTest::underlying_t*>(uut.create(&frame)), expectedStyle, expectedPos, expectedSize);
    }
    {
        auto uut = addWithSize(size, addWithStyle(style, addWithPosition(pos, WHICH::createUUT())));
        checkAll(dynamic_cast<typename WHICH::TypeUnderTest::underlying_t*>(uut.create(&frame)), expectedStyle, expectedPos, expectedSize);
    }
    {
        auto uut = addWithStyle(style, addWithSize(size, addWithPosition(pos, WHICH::createUUT())));
        checkAll(dynamic_cast<typename WHICH::TypeUnderTest::underlying_t*>(uut.create(&frame)), expectedStyle, expectedPos, expectedSize);
    }
    {
        auto uut = addWithPosition(pos, addWithSize(size, addWithStyle(style, WHICH::createUUT())));
        checkAll(dynamic_cast<typename WHICH::TypeUnderTest::underlying_t*>(uut.create(&frame)), expectedStyle, expectedPos, expectedSize);
    }
    {
        auto uut = addWithSize(size, addWithPosition(pos, addWithStyle(style, WHICH::createUUT())));
        checkAll(dynamic_cast<typename WHICH::TypeUnderTest::underlying_t*>(uut.create(&frame)), expectedStyle, expectedPos, expectedSize);
    }
}

#define CHAINING_TEST(WHICH)           \
    SECTION("check all iterations")    \
    {                                  \
        DoChainingIterations<WHICH>(); \
    }

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity)
