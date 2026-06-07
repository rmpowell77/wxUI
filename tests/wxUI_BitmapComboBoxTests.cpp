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
#include "TestCustomizations.hpp"
#include "wxUI_TestControlCommon.hpp"
#include <catch2/catch_test_macros.hpp>
#include <wxUI/BitmapComboBox.hpp>

#include <wx/wx.h>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity, misc-use-anonymous-namespace, cppcoreguidelines-avoid-do-while)
using TypeUnderTest = wxUI::BitmapComboBox;
using namespace wxUITests;

struct BitmapComboBoxTestPolicy {
    using TypeUnderTest = wxUI::BitmapComboBox;
    static auto createUUT() { return TypeUnderTest { { std::string {}, wxBitmap {} }, { std::string {}, wxBitmap {} } }; }
    static auto testStyle() { return (wxCB_SORT); }
    static auto testPosition() { return wxPoint { 1, 2 }; }
    static auto testSize() { return wxSize { 10, 12 }; }
    static auto expectedStyle() { return testStyle(); }
    static auto expectedPosition() { return testPosition(); }
    static auto expectedSize() { return testSize(); }
};
static auto createUUT() { return BitmapComboBoxTestPolicy::createUUT(); }

namespace {
using Dump = std::vector<std::string>;

std::string makeController(
    int identity,
    wxPoint pos,
    wxSize size,
    int style,
    std::string const& text,
    std::vector<std::string> const& choices)
{
    auto fmtChoices = [&]() {
        std::string result;
        for (size_t i = 0; i < choices.size(); ++i) {
            result += std::string { "\"" } + choices[i] + "\"";
            result += ",";
        }
        return result;
    }();
    return std::format("wxBitmapComboBox[id={}, pos=({},{}), size=({},{}), style={}, text=\"{}\", choices=({})]", identity, pos.x, pos.y, size.x, size.y, style, text, fmtChoices);
}

Dump testDump(
    int identity,
    wxPoint pos,
    wxSize size,
    int style,
    std::string const& text,
    std::vector<std::string> const& choices,
    int selection = 0)
{
    auto controller = makeController(identity, pos, size, style, text, choices);
    std::vector<std::string> results = {
        "Create:" + controller,
        "controller:" + controller,
    };
    for (auto i : std::views::iota(0UL, choices.size())) {
        results.push_back(std::format("SetItemBitmap:{}:(-1,-1)", i));
    }
    results.push_back(std::format("SetSelection:{}", selection));
    results.push_back("SetEnabled:true");
    return results;
}
}

TEST_CASE("BitmapComboBox")
{
    SECTION("bitmap")
    {
        TestParent provider;
        auto uut = createUUT();
        uut.create(&provider);
        CHECK(provider.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 0, "", { "", "" }));
    }

    SECTION("id.bitmap")
    {
        TestParent provider;
        auto uut = TypeUnderTest { 10000, { { std::string {}, wxBitmap {} }, { std::string {}, wxBitmap {} } } };
        uut.create(&provider);
        CHECK(provider.dump() == testDump(10000, { -1, -1 }, { -1, -1 }, 0, "", { "", "" }));
    }

    SECTION("size.bitmap")
    {
        TestParent provider;
        auto uut = TypeUnderTest { { { std::string {}, wxBitmap {} }, { std::string {}, wxBitmap {} } } };
        uut.create(&provider);
        CHECK(provider.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 0, "", { "", "" }));
    }

    SECTION("size.id.bitmap")
    {
        TestParent provider;
        auto uut = TypeUnderTest { 10000, { { std::string {}, wxBitmap {} }, { std::string {}, wxBitmap {} } } };
        uut.create(&provider);
        CHECK(provider.dump() == testDump(10000, { -1, -1 }, { -1, -1 }, 0, "", { "", "" }));
    }

    SECTION("bitmap.ranges")
    {
        TestParent provider;
        auto uut = TypeUnderTest { std::vector<std::tuple<std::string, wxBitmap>> { { std::string {}, wxBitmap {} }, { std::string {}, wxBitmap {} } } };
        uut.create(&provider);
        CHECK(provider.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 0, "", { "", "" }));
    }

    SECTION("id.bitmap.ranges")
    {
        TestParent provider;
        auto uut = TypeUnderTest { 10000, std::vector<std::tuple<std::string, wxBitmap>> { { std::string {}, wxBitmap {} }, { std::string {}, wxBitmap {} } } };
        uut.create(&provider);
        CHECK(provider.dump() == testDump(10000, { -1, -1 }, { -1, -1 }, 0, "", { "", "" }));
    }

    SECTION("pos")
    {
        TestParent provider;
        auto uut = createUUT().withPosition({ 1, 2 });
        uut.create(&provider);
        CHECK(provider.dump() == testDump(-1, { 1, 2 }, { -1, -1 }, 0, "", { "", "" }));
    }

    SECTION("size")
    {
        TestParent provider;
        auto uut = createUUT().withSize({ 1, 2 });
        uut.create(&provider);
        CHECK(provider.dump() == testDump(-1, { -1, -1 }, { 1, 2 }, 0, "", { "", "" }));
    }

    SECTION("setSelection")
    {
        TestParent provider;
        auto uut = TypeUnderTest { 10000, { { "Hello 🐨", wxBitmap {} }, { "Goodbye", wxBitmap {} } } }.withSize({ 1, 2 }).withSelection(1);
        uut.create(&provider);
        CHECK(provider.dump() == testDump(10000, { -1, -1 }, { 1, 2 }, 0, "Hello 🐨", { "Hello 🐨", "Goodbye" }, 1));
    }

    SECTION("Empty")
    {
        TestParent provider;
        auto uut = wxUI::BitmapComboBox { {} };
        uut.create(&provider);
        CHECK(provider.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 0, "", { "" }));
    }
    COMMON_TESTS(BitmapComboBoxTestPolicy)
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity, misc-use-anonymous-namespace, cppcoreguidelines-avoid-do-while)
