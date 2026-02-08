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

TEST_CASE("Hyperlink")
{
    SECTION("name.url")
    {
        TestProvider provider;
        auto uut = createUUT();
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxHyperlinkCtrl[id=-1, pos=(-1,-1), size=(-1,-1), style=2097161, text=\"Hello\", text2=\"www.github.com\"]",
                  "controller:wxHyperlinkCtrl[id=-1, pos=(-1,-1), size=(-1,-1), style=2097161, text=\"Hello\", text2=\"www.github.com\"]",
                  "SetEnabled:true",
              });
    }

    SECTION("name.url_unicode")
    {
        TestProvider provider;
        auto uut = TypeUnderTest { "🐨", "https://example.com" };
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxHyperlinkCtrl[id=-1, pos=(-1,-1), size=(-1,-1), style=2097161, text=\"🐨\", text2=\"https://example.com\"]",
                  "controller:wxHyperlinkCtrl[id=-1, pos=(-1,-1), size=(-1,-1), style=2097161, text=\"🐨\", text2=\"https://example.com\"]",
                  "SetEnabled:true",
              });
    }

    SECTION("name.url_tag")
    {
        TestProvider provider;
        wxString wx_text { "Hello" };
        wxString wx_url { "www.github.com" };
        auto uut = TypeUnderTest { wxUI_String {}, wx_text, wx_url };
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxHyperlinkCtrl[id=-1, pos=(-1,-1), size=(-1,-1), style=2097161, text=\"Hello\", text2=\"www.github.com\"]",
                  "controller:wxHyperlinkCtrl[id=-1, pos=(-1,-1), size=(-1,-1), style=2097161, text=\"Hello\", text2=\"www.github.com\"]",
                  "SetEnabled:true",
              });
    }

    SECTION("id.name.url")
    {
        TestProvider provider;
        auto uut = TypeUnderTest { 10000, "Hello", "www.github.com" };
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxHyperlinkCtrl[id=10000, pos=(-1,-1), size=(-1,-1), style=2097161, text=\"Hello\", text2=\"www.github.com\"]",
                  "controller:wxHyperlinkCtrl[id=10000, pos=(-1,-1), size=(-1,-1), style=2097161, text=\"Hello\", text2=\"www.github.com\"]",
                  "SetEnabled:true",
              });
    }

    SECTION("style")
    {
        TestProvider provider;
        auto uut = createUUT();
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxHyperlinkCtrl[id=-1, pos=(-1,-1), size=(-1,-1), style=2097161, text=\"Hello\", text2=\"www.github.com\"]",
                  "controller:wxHyperlinkCtrl[id=-1, pos=(-1,-1), size=(-1,-1), style=2097161, text=\"Hello\", text2=\"www.github.com\"]",
                  "SetEnabled:true",
              });
    }

    SECTION("withStyle")
    {
        TestProvider provider;
        auto uut = createUUT().withoutStyle(wxHL_ALIGN_CENTRE).withStyle(wxHL_ALIGN_LEFT);
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxHyperlinkCtrl[id=-1, pos=(-1,-1), size=(-1,-1), style=2097155, text=\"Hello\", text2=\"www.github.com\"]",
                  "controller:wxHyperlinkCtrl[id=-1, pos=(-1,-1), size=(-1,-1), style=2097155, text=\"Hello\", text2=\"www.github.com\"]",
                  "SetEnabled:true",
              });
    }

    SECTION("pos")
    {
        TestProvider provider;
        auto uut = createUUT().withPosition({ 1, 2 });
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxHyperlinkCtrl[id=-1, pos=(1,2), size=(-1,-1), style=2097161, text=\"Hello\", text2=\"www.github.com\"]",
                  "controller:wxHyperlinkCtrl[id=-1, pos=(1,2), size=(-1,-1), style=2097161, text=\"Hello\", text2=\"www.github.com\"]",
                  "SetEnabled:true",
              });
    }

    SECTION("size")
    {
        TestProvider provider;
        auto uut = createUUT().withSize({ 1, 2 });
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxHyperlinkCtrl[id=-1, pos=(-1,-1), size=(1,2), style=2097161, text=\"Hello\", text2=\"www.github.com\"]",
                  "controller:wxHyperlinkCtrl[id=-1, pos=(-1,-1), size=(1,2), style=2097161, text=\"Hello\", text2=\"www.github.com\"]",
                  "SetEnabled:true",
              });
    }

    COMMON_TESTS(HyperlinkTestPolicy)
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity, misc-use-anonymous-namespace, cppcoreguidelines-avoid-do-while)
