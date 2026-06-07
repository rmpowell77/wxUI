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
#include <wxUI/Hyperlink.hpp>
#include <wxUI/Layout.hpp>
#include <wxUI/wxUITypes.hpp>

#include <wx/wx.h>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity, misc-use-anonymous-namespace, cppcoreguidelines-avoid-do-while)
using TypeUnderTest = wxUI::Hyperlink;
using namespace wxUITests;
using namespace wxUI;

struct HyperlinkTestPolicy {
    using TypeUnderTest = wxUI::Hyperlink;
    static auto createUUT() { return TypeUnderTest { "Hello", "www.github.com" }; }
    static auto testStyle() { return wxHL_DEFAULT_STYLE; }
    static auto testPosition() { return wxPoint { 1, 2 }; }
    static auto testSize() { return wxSize { 10, 12 }; }
    static auto expectedStyle() { return testStyle(); }
    static auto expectedPosition() { return testPosition(); }
    static auto expectedSize() { return testSize(); }
};
static auto createUUT() { return HyperlinkTestPolicy::createUUT(); }

namespace {
using Dump = std::vector<std::string>;

std::string makeController(
    int identity,
    wxPoint pos,
    wxSize size,
    int style,
    const std::string& text,
    const std::string& url)
{
    return std::format("wxHyperlinkCtrl[id={}, pos=({},{}), size=({},{}), style={}, text=\"{}\", text2=\"{}\"]", identity, pos.x, pos.y, size.x, size.y, style, text, url);
}

Dump testDump(
    int identity,
    wxPoint pos,
    wxSize size,
    int style,
    const std::string& text,
    const std::string& url)
{
    auto controller = makeController(identity, pos, size, style, text, url);
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

TEST_CASE("Hyperlink")
{
    SECTION("name.url")
    {
        TestParent frame;
        wxUI::VSizer {
            createUUT()
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 2097161, "Hello", "www.github.com"));
    }

    SECTION("name.url_unicode")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { "🐨", "https://example.com" }
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 2097161, "🐨", "https://example.com"));
    }

    SECTION("name.url_tag")
    {
        TestParent frame;
        wxString wx_text { "Hello" };
        wxString wx_url { "www.github.com" };
        wxUI::VSizer {
            TypeUnderTest { wxUI_String {}, wx_text, wx_url }
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 2097161, "Hello", "www.github.com"));
    }

    SECTION("id.name.url")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { 10000, "Hello", "www.github.com" }
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(10000, { -1, -1 }, { -1, -1 }, 2097161, "Hello", "www.github.com"));
    }

    SECTION("style")
    {
        TestParent frame;
        wxUI::VSizer {
            createUUT()
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 2097161, "Hello", "www.github.com"));
    }

    SECTION("withStyle")
    {
        TestParent frame;
        wxUI::VSizer {
            createUUT().withoutStyle(wxHL_ALIGN_CENTRE).withStyle(wxHL_ALIGN_LEFT)
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 2097155, "Hello", "www.github.com"));
    }

    SECTION("pos")
    {
        TestParent frame;
        wxUI::VSizer {
            createUUT().withPosition({ 1, 2 })
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { 1, 2 }, { -1, -1 }, 2097161, "Hello", "www.github.com"));
    }

    SECTION("size")
    {
        TestParent frame;
        wxUI::VSizer {
            createUUT().withSize({ 1, 2 })
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { 1, 2 }, 2097161, "Hello", "www.github.com"));
    }

    COMMON_TESTS(HyperlinkTestPolicy)
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity, misc-use-anonymous-namespace, cppcoreguidelines-avoid-do-while)
