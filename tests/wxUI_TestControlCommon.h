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
#include <wx/wx.h>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity)

template <typename W>
constexpr auto addWithFlags(wxSizerFlags const& value, W&& inUUT) -> W&&
{
    return std::forward<W>(inUUT).withFlags(value);
}

template <typename W>
constexpr auto addWithPosition(wxPoint const& value, W&& inUUT) -> W&&
{
    return std::forward<W>(inUUT).withPosition(value);
}

template <typename W>
constexpr auto addWithSize(wxSize const& value, W&& inUUT) -> W&&
{
    return std::forward<W>(inUUT).withSize(value);
}

template <typename W>
constexpr auto addWithWidth(int value, W&& inUUT) -> W&&
{
    return std::forward<W>(inUUT).withWidthSize(value);
}

template <typename W>
constexpr auto addWithHeight(int value, W&& inUUT) -> W&&
{
    return std::forward<W>(inUUT).withHeightSize(value);
}

template <typename W>
constexpr auto addSetStyle(int64_t value, W&& inUUT) -> W&&
{
    return std::forward<W>(inUUT).setStyle(value);
}

template <typename W>
constexpr auto addWithStyle(int64_t value, W&& inUUT) -> W&&
{
    return std::forward<W>(inUUT).withStyle(value);
}

template <typename W>
constexpr auto addWithoutStyle(int64_t value, W&& inUUT) -> W&&
{
    return std::forward<W>(inUUT).withoutStyle(value);
}

template <typename W>
constexpr auto addWithFont(wxFontInfo const& value, W&& inUUT) -> W&&
{
    return std::forward<W>(inUUT).withFont(value);
}

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

template <typename W>
auto checkIdentity(wxWindowID value, W const& window) -> W const&
{
    CHECK(window.getIdentity() == value);
    return window;
}

template <typename W>
auto checkPos(wxPoint value, W const& window) -> W const&
{
    CHECK(window.getPos() == value);
    return window;
}

template <typename W>
auto checkSize(wxSize value, W const& window) -> W const&
{
    CHECK(window.getSize() == value);
    return window;
}

template <typename W>
auto checkStyle(int64_t value, W const& window) -> W const&
{
    CHECK((window.getStyle() & value) == value);
    return window;
}

template <typename W>
auto checkAllBeforeCreate(W const& window, int64_t style, wxPoint pos, wxSize size) -> W const&
{
    return checkIdentity(wxID_ANY, checkStyle(style, checkPos(pos, checkSize(size, window))));
}

constexpr auto checkAll = [](auto* window, auto style, auto pos, auto size) {
    return checkWithStyle(style, checkWithPosition(pos, checkWithSize(size, window)));
};

// How do we test chaining?
// We make sure that we get the same outcome no matter the way we add things
template <typename WHICH>
auto DoChainingIterations()
{
    auto style = WHICH::testStyle();
    auto pos = WHICH::testPosition();
    auto size = WHICH::testSize();
    auto expectedStyle = WHICH::expectedStyle();
    auto expectedPos = WHICH::expectedPosition();
    auto expectedSize = WHICH::expectedSize();
    auto flags = wxSizerFlags(1).Proportion(1).Expand().Border(wxALL, 5);
    wxFrame frame { nullptr, wxID_ANY, "" };
    // decorate with one flow
    {
        auto uut = addWithFlags(flags, addWithStyle(style, addWithPosition(pos, addWithSize(size, WHICH::createUUT()))));
        checkAllBeforeCreate(uut, style, pos, size);
        checkAll(dynamic_cast<typename WHICH::TypeUnderTest::underlying_t*>(uut.create(&frame)), expectedStyle, expectedPos, expectedSize);
    }
    // inverted the other way around.
    {
        auto uut = addWithPosition(pos, addWithStyle(style, addWithSize(size, WHICH::createUUT())));
        checkAllBeforeCreate(uut, style, pos, size);
        checkAll(dynamic_cast<typename WHICH::TypeUnderTest::underlying_t*>(uut.create(&frame)), expectedStyle, expectedPos, expectedSize);
    }
    // Do height and width
    {
        auto uut = addWithFlags(flags, addWithStyle(style, addWithPosition(pos, addWithHeight(size.GetHeight(), addWithWidth(size.GetWidth(), WHICH::createUUT())))));
        checkAllBeforeCreate(uut, style, pos, size);
        checkAll(dynamic_cast<typename WHICH::TypeUnderTest::underlying_t*>(uut.create(&frame)), expectedStyle, expectedPos, expectedSize);
    }
    // Expand out style
    {
        auto uut = addWithFlags(flags, addWithStyle(style, addWithStyle(0, addWithPosition(pos, addWithHeight(size.GetHeight(), addWithWidth(size.GetWidth(), WHICH::createUUT()))))));
        checkAllBeforeCreate(uut, style, pos, size);
        checkAll(dynamic_cast<typename WHICH::TypeUnderTest::underlying_t*>(uut.create(&frame)), expectedStyle, expectedPos, expectedSize);
    }
    {
        auto uut = addWithFlags(flags, addSetStyle(style, addWithStyle(0xFFFF, addWithPosition(pos, addWithHeight(size.GetHeight(), addWithWidth(size.GetWidth(), WHICH::createUUT()))))));
        checkAllBeforeCreate(uut, style, pos, size);
        checkAll(dynamic_cast<typename WHICH::TypeUnderTest::underlying_t*>(uut.create(&frame)), expectedStyle, expectedPos, expectedSize);
    }
    // specifically test with lvalue.
    {
        auto unit = WHICH::createUUT();
        auto uut = addWithFlags(flags, addWithStyle(style, addWithPosition(pos, addWithSize(size, WHICH::createUUT()))));
        checkAllBeforeCreate(uut, style, pos, size);
        checkAll(dynamic_cast<typename WHICH::TypeUnderTest::underlying_t*>(uut.create(&frame)), expectedStyle, expectedPos, expectedSize);
    }
}

template <typename WHICH>
auto DoProxyTests()
{
    wxFrame frame { nullptr, wxID_ANY, "" };
    auto proxy = typename WHICH::TypeUnderTest::Proxy {};
    auto uut = proxy = WHICH::createUUT();
    uut.create(&frame);

    CHECK(proxy.control() != nullptr);
}

#define COMMON_TESTS(WHICH)            \
    SECTION("Chaining Iterations")     \
    {                                  \
        DoChainingIterations<WHICH>(); \
    }                                  \
    SECTION("Check Proxy")             \
    {                                  \
        DoProxyTests<WHICH>();         \
    }

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity)
