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
#include "wxUI_TestControlCommon.hpp"
#include <catch2/catch_test_macros.hpp>
#include <wxUI/BitmapToggleButton.hpp>

#include <wx/wx.h>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity, misc-use-anonymous-namespace, cppcoreguidelines-avoid-do-while)
using TypeUnderTest = wxUI::BitmapToggleButton;

struct BitmapToggleButtonTestPolicy {
    using TypeUnderTest = wxUI::BitmapToggleButton;
    static auto createUUT() { return TypeUnderTest { wxBitmap {} }; }
    static auto testStyle() { return (wxBU_LEFT | wxBU_EXACTFIT | wxBU_NOTEXT); }
    static auto testPosition() { return wxPoint { 1, 2 }; }
    static auto testSize() { return wxSize { 10, 12 }; }
    static auto expectedStyle() { return testStyle(); }
    static auto expectedPosition() { return testPosition(); }
    static auto expectedSize() { return testSize(); }
};
static auto createUUT() { return BitmapToggleButtonTestPolicy::createUUT(); }

TEST_CASE("BitmapToggleButton")
{
    SECTION("bitmap")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = createUUT();
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(window->GetLabel().empty());
    }

    SECTION("bitmap.toggle")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { wxBitmap {}, wxBitmap {} };
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(window->GetLabel().empty());
    }

    SECTION("id.bitmap")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { 10000, wxBitmap {} };
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(10000 == window->GetId());
        CHECK(window->GetLabel().empty());
    }

    SECTION("id.bitmap.toggle")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { 10000, wxBitmap {}, wxBitmap {} };
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(10000 == window->GetId());
        CHECK(window->GetLabel().empty());
    }

    SECTION("pos")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = createUUT().withPosition({ 1, 2 });
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(window->GetPosition() == wxPoint { 1, 2 });
    }

    SECTION("size")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = createUUT().withSize({ 1, 2 });
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(window->GetSize() == wxSize { 1, 2 });
    }

    COMMON_TESTS(BitmapToggleButtonTestPolicy)
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity, misc-use-anonymous-namespace, cppcoreguidelines-avoid-do-while)
