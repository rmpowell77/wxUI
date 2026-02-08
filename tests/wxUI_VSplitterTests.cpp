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
#include <wxUI/Splitter.hpp>
#include <wxUI/TextCtrl.hpp>

#include <wx/wx.h>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity, misc-use-anonymous-namespace, cppcoreguidelines-avoid-do-while)
using TypeUnderTest = wxUI::VSplitter<wxUI::TextCtrl, wxUI::TextCtrl>;
using namespace wxUITests;

struct VSplitterTestPolicy {
    using TypeUnderTest = wxUI::VSplitter<wxUI::TextCtrl, wxUI::TextCtrl>;
    static auto createUUT() { return TypeUnderTest { wxUI::TextCtrl {}, wxUI::TextCtrl {} }; }
    static auto testStyle() { return wxSP_3D | wxSP_LIVE_UPDATE; }
    static auto testPosition() { return wxPoint { 14, 2 }; }
    static auto testSize() { return wxSize { 100, 40 }; }
    static auto expectedStyle() { return testStyle(); }
    static auto expectedPosition() { return wxPoint { 14, 2 }; }
    static auto expectedSize() { return wxSize { 100, 40 }; }
};

TEST_CASE("VSplitter")
{
    SECTION("noargs")
    {
        TestProvider provider;
        auto uut = TypeUnderTest { wxUI::TextCtrl {}, wxUI::TextCtrl {} };
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxSplitterWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=896]",
                  "controller:wxSplitterWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=896]",
                  "Create:wxTextCtrl[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"\"]",
                  "Create:wxTextCtrl[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"\"]",
                  "SplitVertical:wxTextCtrl[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"\"]:wxTextCtrl[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"\"]",
                  "SetEnabled:true",
              });
    }

    SECTION("id")
    {
        TestProvider provider;
        auto uut = TypeUnderTest { 10000, wxUI::TextCtrl {}, wxUI::TextCtrl {} };
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxSplitterWindow[id=10000, pos=(-1,-1), size=(-1,-1), style=896]",
                  "controller:wxSplitterWindow[id=10000, pos=(-1,-1), size=(-1,-1), style=896]",
                  "Create:wxTextCtrl[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"\"]",
                  "Create:wxTextCtrl[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"\"]",
                  "SplitVertical:wxTextCtrl[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"\"]:wxTextCtrl[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"\"]",
                  "SetEnabled:true",
              });
    }

    SECTION("withGravity")
    {
        TestProvider provider;
        auto uut = TypeUnderTest { wxUI::TextCtrl {}, wxUI::TextCtrl {} }.withStashGravity(1.0);
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxSplitterWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=896]",
                  "controller:wxSplitterWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=896]",
                  "Create:wxTextCtrl[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"\"]",
                  "Create:wxTextCtrl[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"\"]",
                  "SplitVertical:wxTextCtrl[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"\"]:wxTextCtrl[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"\"]",
                  "SetSashGravity:1",
                  "SetEnabled:true",
              });
    }

    COMMON_TESTS(VSplitterTestPolicy)
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity, misc-use-anonymous-namespace, cppcoreguidelines-avoid-do-while)
