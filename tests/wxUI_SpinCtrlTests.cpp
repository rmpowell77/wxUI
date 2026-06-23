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
#include <wxUI/Layout.hpp>
#include <wxUI/SpinCtrl.hpp>

#include <wx/wx.h>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity, misc-use-anonymous-namespace, cppcoreguidelines-avoid-do-while)
using TypeUnderTest = wxUI::SpinCtrl;
using namespace wxUITests;

struct SpinCtrlTestPolicy {
    using TypeUnderTest = wxUI::SpinCtrl;
    static auto createUUT() { return TypeUnderTest {}; }
    static auto testStyle() { return wxTE_PROCESS_ENTER; }
    static auto testPosition() { return wxPoint { 5, 2 }; }
    static auto testSize() { return wxSize { 10, 12 }; }
    static auto expectedStyle() { return testStyle(); }
    static auto expectedPosition() { return testPosition(); }
    static auto expectedSize() { return testSize(); }
};
static auto createUUT() { return SpinCtrlTestPolicy::createUUT(); }

namespace {
using Dump = std::vector<std::string>;

std::string makeController(
    int identity,
    wxPoint pos,
    wxSize size,
    int style,
    int value,
    int minRange,
    int maxRange)
{
    return std::format("wxSpinCtrl[id={}, pos=({},{}), size=({},{}), style={}, value={}, range=[{},{}]]", identity, pos.x, pos.y, size.x, size.y, style, value, minRange, maxRange);
}

Dump testDump(
    int identity,
    wxPoint pos,
    wxSize size,
    int style,
    int value,
    int minRange,
    int maxRange)
{
    auto controller = makeController(identity, pos, size, style, value, minRange, maxRange);
    return {
        "Create:Sizer[orientation=wxVERTICAL]",
        "Create:" + controller,
        "topsizer:Sizer[orientation=wxVERTICAL]",
        "controller:" + controller,
        "SetEnabled:true",
        "sizer:Sizer[orientation=wxVERTICAL]",
        "Add:" + controller + ":flags:(0,0x0,0)",
        "SetSizeHints:[id=0, pos=(0,0), size=(0,0), style=0]"
    };
}
}

TEST_CASE("SpinCtrl")
{
    SECTION("noargs")
    {
        TestParent frame;
        wxUI::VSizer {
            createUUT()
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 0, 0, 0, 100));
    }

    SECTION("range")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { std::pair { 1, 5 } }
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 0, 1, 1, 5));
    }

    SECTION("range.init")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { std::pair { 1, 5 }, 3 }
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 0, 3, 1, 5));
    }

    SECTION("id")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { 10000 }
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(10000, { -1, -1 }, { -1, -1 }, 0, 0, 0, 100));
    }

    SECTION("id.range")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { 10000, std::pair { 1, 5 } }
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(10000, { -1, -1 }, { -1, -1 }, 0, 1, 1, 5));
    }

    SECTION("id.range.init")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { 10000, std::pair { 1, 5 }, 3 }
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(10000, { -1, -1 }, { -1, -1 }, 0, 3, 1, 5));
    }

    SECTION("style")
    {
        TestParent frame;
        wxUI::VSizer {
            createUUT().withStyle(wxTE_PROCESS_ENTER)
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 1024, 0, 0, 100));
    }

    SECTION("pos")
    {
        TestParent frame;
        wxUI::VSizer {
            createUUT().withPosition({ 1, 2 })
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { 1, 2 }, { -1, -1 }, 0, 0, 0, 100));
    }

    SECTION("size")
    {
        TestParent frame;
        wxUI::VSizer {
            createUUT().withSize({ 1, 2 })
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { 1, 2 }, 0, 0, 0, 100));
    }

    SECTION("AI")
    {
        TestParent provider;
        auto uut = wxUI::SpinCtrl { { 0, 10 }, 3 }.bind([] { });
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxSpinCtrl[id=-1, pos=(-1,-1), size=(-1,-1), style=0, value=3, range=[0,10]]",
                  "controller:wxSpinCtrl[id=-1, pos=(-1,-1), size=(-1,-1), style=0, value=3, range=[0,10]]",
                  "SetEnabled:true",
                  "BindEvents:1",
              });
    }
    COMMON_TESTS(SpinCtrlTestPolicy)
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity, misc-use-anonymous-namespace, cppcoreguidelines-avoid-do-while)
