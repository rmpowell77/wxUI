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
#include <wxUI/Slider.hpp>

#include <wx/wx.h>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity, misc-use-anonymous-namespace, cppcoreguidelines-avoid-do-while)
using TypeUnderTest = wxUI::Slider;
using namespace wxUITests;

struct SliderTestPolicy {
    using TypeUnderTest = wxUI::Slider;
    static auto createUUT() { return TypeUnderTest {}; }
    static auto testStyle() { return wxSL_LABELS; }
    static auto testPosition() { return wxPoint { 14, 2 }; }
    static auto testSize() { return wxSize { 100, 40 }; }
    static auto expectedStyle() { return testStyle(); }
    static auto expectedPosition() { return testPosition(); }
    static auto expectedSize() { return testSize(); }
};
static auto createUUT() { return SliderTestPolicy::createUUT(); }

TEST_CASE("Slider")
{
    SECTION("noargs")
    {
        TestParent provider;
        auto uut = createUUT();
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxSlider[id=-1, pos=(-1,-1), size=(-1,-1), style=0, value=0, range=[0,100]]",
                  "controller:wxSlider[id=-1, pos=(-1,-1), size=(-1,-1), style=0, value=0, range=[0,100]]",
                  "SetEnabled:true",
              });
    }

    SECTION("range")
    {
        TestParent provider;
        auto uut = TypeUnderTest { std::pair { 1, 5 } };
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxSlider[id=-1, pos=(-1,-1), size=(-1,-1), style=0, value=1, range=[1,5]]",
                  "controller:wxSlider[id=-1, pos=(-1,-1), size=(-1,-1), style=0, value=1, range=[1,5]]",
                  "SetEnabled:true",
              });
    }

    SECTION("range.init")
    {
        TestParent provider;
        auto uut = TypeUnderTest { std::pair { 1, 5 }, 3 };
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxSlider[id=-1, pos=(-1,-1), size=(-1,-1), style=0, value=3, range=[1,5]]",
                  "controller:wxSlider[id=-1, pos=(-1,-1), size=(-1,-1), style=0, value=3, range=[1,5]]",
                  "SetEnabled:true",
              });
    }

    SECTION("id")
    {
        TestParent provider;
        auto uut = TypeUnderTest { 10000 };
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxSlider[id=10000, pos=(-1,-1), size=(-1,-1), style=0, value=0, range=[0,100]]",
                  "controller:wxSlider[id=10000, pos=(-1,-1), size=(-1,-1), style=0, value=0, range=[0,100]]",
                  "SetEnabled:true",
              });
    }

    SECTION("id.range")
    {
        TestParent provider;
        auto uut = TypeUnderTest { 10000, std::pair { 1, 5 } };
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxSlider[id=10000, pos=(-1,-1), size=(-1,-1), style=0, value=1, range=[1,5]]",
                  "controller:wxSlider[id=10000, pos=(-1,-1), size=(-1,-1), style=0, value=1, range=[1,5]]",
                  "SetEnabled:true",
              });
    }

    SECTION("id.range.init")
    {
        TestParent provider;
        auto uut = TypeUnderTest { 10000, std::pair { 1, 5 }, 3 };
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxSlider[id=10000, pos=(-1,-1), size=(-1,-1), style=0, value=3, range=[1,5]]",
                  "controller:wxSlider[id=10000, pos=(-1,-1), size=(-1,-1), style=0, value=3, range=[1,5]]",
                  "SetEnabled:true",
              });
    }

    SECTION("AI")
    {
        TestParent provider;
        auto uut = wxUI::Slider { { 10, 20 }, 12 }.bind([] {});
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxSlider[id=-1, pos=(-1,-1), size=(-1,-1), style=0, value=12, range=[10,20]]",
                  "controller:wxSlider[id=-1, pos=(-1,-1), size=(-1,-1), style=0, value=12, range=[10,20]]",
                  "SetEnabled:true",
                  "BindEvents:1",
              });
    }

    COMMON_TESTS(SliderTestPolicy)
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity, misc-use-anonymous-namespace, cppcoreguidelines-avoid-do-while)
