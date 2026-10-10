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
#include <wxUI/TreeList.hpp>

#include <wx/wx.h>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity, misc-use-anonymous-namespace, cppcoreguidelines-avoid-do-while)
using TypeUnderTest = wxUI::TreeList<2>;
using namespace wxUITests;

struct TreeListTestPolicy {
    using TypeUnderTest = wxUI::TreeList<2>;
    static auto createUUT() { return TypeUnderTest { wxUI::Columns { "Name", "Value" } }; }
    static auto testStyle() { return (wxTL_MULTIPLE); }
    static auto testPosition() { return wxPoint { 1, 2 }; }
    static auto testSize() { return wxSize { 10, 12 }; }
    static auto expectedStyle() { return testStyle(); }
    static auto expectedPosition() { return testPosition(); }
    static auto expectedSize() { return testSize(); }
};
static auto createUUT() { return TreeListTestPolicy::createUUT(); }

namespace {
using Dump = std::vector<std::string>;

std::string makeController(int identity, wxPoint pos, wxSize size, int style)
{
    return std::format("wxTreeListCtrl[id={}, pos=({},{}), size=({},{}), style={}]", identity, pos.x, pos.y, size.x, size.y, style);
}

// `creation` is what the control logs while being created (columns, images, items).
Dump testDump(
    int identity,
    wxPoint pos,
    wxSize size,
    int style,
    Dump const& creation)
{
    auto controller = makeController(identity, pos, size, style);
    Dump result {
        "Create:Sizer[orientation=wxVERTICAL]",
        "Create:" + controller,
        "topsizer:Sizer[orientation=wxVERTICAL]",
        "controller:" + controller,
    };
    result.insert(result.end(), creation.begin(), creation.end());
    result.insert(result.end(), {
                                    "SetEnabled:true",
                                    "sizer:Sizer[orientation=wxVERTICAL]",
                                    "Add:" + controller + ":flags:(0,0x0,0)",
                                    "SetSizeHints:[id=0, pos=(0,0), size=(0,0), style=0]",
                                });
    return result;
}

std::string columnLog(std::string const& title, int width, wxAlignment align, int flags)
{
    return std::format("AppendColumn:\"{}\",{},{},{}", title, width, static_cast<int>(align), flags);
}

Dump const columns {
    columnLog("Name", wxCOL_WIDTH_DEFAULT, wxALIGN_LEFT, wxCOL_RESIZABLE),
    columnLog("Value", wxCOL_WIDTH_DEFAULT, wxALIGN_LEFT, wxCOL_RESIZABLE),
    "SetImageList:0",
};
}

TEST_CASE("TreeList")
{
    SECTION("compile test")
    {
        TypeUnderTest { wxUI::Columns { "a", "b" } };
        TypeUnderTest { 10000, wxUI::Columns { "a", "b" } };
        TypeUnderTest { wxUI::Columns { "a", "b" }, std::vector<wxUI::Entry<2>> {} };
        TypeUnderTest { 10000, wxUI::Columns { "a", "b" }, std::vector<wxUI::Entry<2>> {} };
        wxUI::TreeList { wxUI::Columns { "a", "b", "c" } };
    }

    SECTION("noitems")
    {
        TestParent frame;
        wxUI::VSizer {
            createUUT()
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 0, columns));
    }

    SECTION("id")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { 10000, wxUI::Columns { "Name", "Value" } }
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(10000, { -1, -1 }, { -1, -1 }, 0, columns));
    }

    SECTION("column.options")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { wxUI::Columns { wxUI::Column { "Name" }.autoSize(), wxUI::Column { "Value" }.pixels(50).alignRight().fixed().sortable() } }
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 0, {
                                                                          columnLog("Name", wxCOL_WIDTH_AUTOSIZE, wxALIGN_LEFT, wxCOL_RESIZABLE),
                                                                          columnLog("Value", 50, wxALIGN_RIGHT, wxCOL_SORTABLE),
                                                                          "SetImageList:0",
                                                                      }));
    }

    SECTION("items")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { wxUI::Columns { "Name", "Value" }, { { { "a 🐨", "1" } }, { { "b", "2" } } } }
        }
            .fitTo(&frame);
        auto creation = columns;
        creation.insert(creation.end(), {
                                            "AppendItem:2:parent=1:\"a 🐨\",-1,-1",
                                            "SetItemText:2:1:\"1\"",
                                            "AppendItem:3:parent=1:\"b\",-1,-1",
                                            "SetItemText:3:1:\"2\"",
                                        });
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 0, creation));
    }

    SECTION("id.items.nested")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { 10000, wxUI::Columns { "Name", "Value" }, { { { "a", "1" }, { { { "a.1", "11" } }, { { "a.2", "12" }, { { { "a.2.1", "121" } } } } } }, { { "b", "2" } } } }
        }
            .fitTo(&frame);
        auto creation = columns;
        creation.insert(creation.end(), {
                                            "AppendItem:2:parent=1:\"a\",-1,-1",
                                            "SetItemText:2:1:\"1\"",
                                            "AppendItem:3:parent=2:\"a.1\",-1,-1",
                                            "SetItemText:3:1:\"11\"",
                                            "AppendItem:4:parent=2:\"a.2\",-1,-1",
                                            "SetItemText:4:1:\"12\"",
                                            "AppendItem:5:parent=4:\"a.2.1\",-1,-1",
                                            "SetItemText:5:1:\"121\"",
                                            "AppendItem:6:parent=1:\"b\",-1,-1",
                                            "SetItemText:6:1:\"2\"",
                                        });
        CHECK(frame.dump() == testDump(10000, { -1, -1 }, { -1, -1 }, 0, creation));
    }

    SECTION("style")
    {
        TestParent frame;
        wxUI::VSizer {
            createUUT().withStyle(wxTL_MULTIPLE)
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, wxTL_MULTIPLE, columns));
    }

    SECTION("pos")
    {
        TestParent frame;
        wxUI::VSizer {
            createUUT().withPosition({ 1, 2 })
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { 1, 2 }, { -1, -1 }, 0, columns));
    }

    SECTION("size")
    {
        TestParent frame;
        wxUI::VSizer {
            createUUT().withSize({ 1, 2 })
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { 1, 2 }, 0, columns));
    }

    SECTION("bind")
    {
        TestParent frame;
        wxUI::VSizer {
            createUUT().bind([] { })
        }
            .fitTo(&frame);
        auto result = testDump(-1, { -1, -1 }, { -1, -1 }, 0, columns);
        result.insert(result.begin() + 8, "BindEvents:1");
        CHECK(frame.dump() == result);
    }

    SECTION("bindActivated")
    {
        TestParent frame;
        wxUI::VSizer {
            createUUT().bindActivated([] { })
        }
            .fitTo(&frame);
        auto result = testDump(-1, { -1, -1 }, { -1, -1 }, 0, columns);
        result.insert(result.begin() + 8, "BindEvents:1");
        CHECK(frame.dump() == result);
    }

    COMMON_TESTS(TreeListTestPolicy)
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity, misc-use-anonymous-namespace, cppcoreguidelines-avoid-do-while)
