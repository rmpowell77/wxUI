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
#include "wxUI_TestControlCommon.h"
#include <catch2/catch_test_macros.hpp>
#include <wxUI/Splitter.h>
#include <wxUI/TextCtrl.h>

#include <wx/wx.h>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity, misc-use-anonymous-namespace, cppcoreguidelines-avoid-do-while)
using TypeUnderTest = wxUI::HSplitter<wxUI::TextCtrl, wxUI::TextCtrl>;

struct HSplitterTestPolicy {
    using TypeUnderTest = wxUI::HSplitter<wxUI::TextCtrl, wxUI::TextCtrl>;
    static auto createUUT() { return TypeUnderTest { wxUI::TextCtrl {}, wxUI::TextCtrl {} }; }
    static auto testStyle() { return wxSP_LIVE_UPDATE; }
    static auto testPosition() { return wxPoint { 14, 2 }; }
    static auto testSize() { return wxSize { 100, 40 }; }
    static auto expectedStyle() { return testStyle(); }
    static auto expectedPosition() { return wxPoint { 14, 2 }; }
    static auto expectedSize() { return wxSize { 100, 40 }; }
};

TEST_CASE("HSplitter")
{
    SECTION("noargs")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { wxUI::TextCtrl {}, wxUI::TextCtrl {} };
        [[maybe_unused]] auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
    }

    SECTION("id")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { 10000, wxUI::TextCtrl {}, wxUI::TextCtrl {} };
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(10000 == window->GetId());
    }

    SECTION("size")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { wxSizerFlags(1), wxUI::TextCtrl {}, wxUI::TextCtrl {} };
        [[maybe_unused]] auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
    }

    SECTION("size.id")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { wxSizerFlags(1), 10000, wxUI::TextCtrl {}, wxUI::TextCtrl {} };
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(10000 == window->GetId());
    }

    SECTION("proxy")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::SplitterProxy proxy;
        auto uut = proxy = TypeUnderTest { wxUI::TextCtrl {}, wxUI::TextCtrl {} };
        [[maybe_unused]] auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
    }

    SECTION("withGravity")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { wxUI::TextCtrl {}, wxUI::TextCtrl {} }.withStashGravity(1.0);
        [[maybe_unused]] auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
    }

    COMMON_TESTS(HSplitterTestPolicy)
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity, misc-use-anonymous-namespace, cppcoreguidelines-avoid-do-while)
