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
#include <wxUI/Gauge.hpp>

#include <wx/wx.h>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity, misc-use-anonymous-namespace, cppcoreguidelines-avoid-do-while)
using TypeUnderTest = wxUI::Gauge;
using namespace wxUITests;

struct GaugeTestPolicy {
    using TypeUnderTest = wxUI::Gauge;
    static auto createUUT() { return TypeUnderTest {}; }
    static auto testStyle() { return wxGA_HORIZONTAL; }
    static auto testPosition() { return wxPoint { 1, 2 }; }
    static auto testSize() { return wxSize { 10, 12 }; }
    static auto expectedStyle() { return testStyle(); }
    static auto expectedPosition() { return testPosition(); }
    static auto expectedSize() { return testSize(); }
};
static auto createUUT() { return GaugeTestPolicy::createUUT(); }

TEST_CASE("Gauge")
{
    SECTION("noargs")
    {
        TestProvider provider;
        auto uut = createUUT();
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxGauge[id=-1, pos=(-1,-1), size=(-1,-1), style=4, value=100]",
                  "controller:wxGauge[id=-1, pos=(-1,-1), size=(-1,-1), style=4, value=100]",
                  "SetEnabled:true",
              });
    }

    SECTION("name")
    {
        TestProvider provider;
        auto uut = TypeUnderTest { wxUI::Gauge::withRange {}, 200 };
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxGauge[id=-1, pos=(-1,-1), size=(-1,-1), style=4, value=200]",
                  "controller:wxGauge[id=-1, pos=(-1,-1), size=(-1,-1), style=4, value=200]",
                  "SetEnabled:true",
              });
    }

    SECTION("id")
    {
        TestProvider provider;
        auto uut = TypeUnderTest { wxUI::Gauge::withIdentity {}, 10000 };
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxGauge[id=10000, pos=(-1,-1), size=(-1,-1), style=4, value=100]",
                  "controller:wxGauge[id=10000, pos=(-1,-1), size=(-1,-1), style=4, value=100]",
                  "SetEnabled:true",
              });
    }

    SECTION("id.name")
    {
        TestProvider provider;
        auto uut = TypeUnderTest { wxUI::Gauge::withIdentity {}, 10000, 200 };
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxGauge[id=10000, pos=(-1,-1), size=(-1,-1), style=4, value=200]",
                  "controller:wxGauge[id=10000, pos=(-1,-1), size=(-1,-1), style=4, value=200]",
                  "SetEnabled:true",
              });
    }

    SECTION("style")
    {
        TestProvider provider;
        auto uut = createUUT().setStyle(wxGA_VERTICAL);
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxGauge[id=-1, pos=(-1,-1), size=(-1,-1), style=8, value=100]",
                  "controller:wxGauge[id=-1, pos=(-1,-1), size=(-1,-1), style=8, value=100]",
                  "SetEnabled:true",
              });
    }

    SECTION("pos")
    {
        TestProvider provider;
        auto uut = createUUT().withPosition({ 1, 2 });
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxGauge[id=-1, pos=(1,2), size=(-1,-1), style=4, value=100]",
                  "controller:wxGauge[id=-1, pos=(1,2), size=(-1,-1), style=4, value=100]",
                  "SetEnabled:true",
              });
    }

    SECTION("AI")
    {
        TestProvider provider;
        auto uut = wxUI::Gauge { wxUI::Gauge::withRange {}, 250 };
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxGauge[id=-1, pos=(-1,-1), size=(-1,-1), style=4, value=250]",
                  "controller:wxGauge[id=-1, pos=(-1,-1), size=(-1,-1), style=4, value=250]",
                  "SetEnabled:true",
              });
    }

    COMMON_TESTS(GaugeTestPolicy)
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity, misc-use-anonymous-namespace, cppcoreguidelines-avoid-do-while)
