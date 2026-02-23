/*
MIT License

Copyright (c) 2022-2026 Richard Powell

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
#include <wxUI/Bitmap.hpp>

#include <wx/wx.h>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity, misc-use-anonymous-namespace, cppcoreguidelines-avoid-do-while)
using TypeUnderTest = wxUI::Bitmap;
using namespace wxUITests;

struct BitmapTestPolicy {
    using TypeUnderTest = wxUI::Bitmap;
    static auto createUUT() { return TypeUnderTest { wxBitmap {} }; }
    static auto testStyle() { return (0); }
    static auto testPosition() { return wxPoint { 1, 2 }; }
    static auto testSize() { return wxSize { 10, 12 }; }
    static auto expectedStyle() { return testStyle(); }
    static auto expectedPosition() { return testPosition(); }
    static auto expectedSize() { return testSize(); }
};
static auto createUUT() { return BitmapTestPolicy::createUUT(); }

TEST_CASE("Bitmap")
{
    SECTION("bitmap")
    {
        TestParent provider;
        auto uut = createUUT();
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxStaticBitmap[id=-1, pos=(-1,-1), size=(-1,-1), style=0, bitmap=(-1,-1)]",
                  "controller:wxStaticBitmap[id=-1, pos=(-1,-1), size=(-1,-1), style=0, bitmap=(-1,-1)]",
                  "SetEnabled:true",
              });
    }

    SECTION("id.bitmap")
    {
        TestParent provider;
        auto uut = TypeUnderTest { 10000, wxBitmap {} };
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxStaticBitmap[id=10000, pos=(-1,-1), size=(-1,-1), style=0, bitmap=(-1,-1)]",
                  "controller:wxStaticBitmap[id=10000, pos=(-1,-1), size=(-1,-1), style=0, bitmap=(-1,-1)]",
                  "SetEnabled:true",
              });
    }

    COMMON_TESTS(BitmapTestPolicy)
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity, misc-use-anonymous-namespace, cppcoreguidelines-avoid-do-while)
