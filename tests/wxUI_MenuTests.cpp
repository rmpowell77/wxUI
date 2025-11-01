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
#include "TestCustomizations.hpp"
#include <catch2/catch_test_macros.hpp>
#include <wxUI/Menu.hpp>

#include <wx/wx.h>

using namespace wxUITests;

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity)
template <typename MenuType>
struct MenuTypeLookup;

template <>
struct MenuTypeLookup<wxUI::Item> {
    static constexpr auto itemKind = wxITEM_NORMAL;
    static constexpr auto name = "normal";
};

template <>
struct MenuTypeLookup<wxUI::CheckItem> {
    static constexpr auto itemKind = wxITEM_CHECK;
    static constexpr auto name = "check";
};

template <>
struct MenuTypeLookup<wxUI::RadioItem> {
    static constexpr auto itemKind = wxITEM_RADIO;
    static constexpr auto name = "radio";
};

template <typename MenuType>
auto RunMenuTest_id()
{
    TestProvider frame;
    wxUI::MenuBar menu {
        wxUI::Menu {
            "Menu1", MenuType { wxID_EXIT } }
    };
    menu.fitTo(&frame);
    CHECK(frame.dump() == std::vector<std::string> {
              std::format("menu:MenuBar:[[title:Menu1:[(menuItem:id=5006,kind={},label=\"Quit\",help=\"Quit this program\"),],]", MenuTypeLookup<MenuType>::name),
          });
}

template <typename MenuType, typename Function>
auto RunMenuTest_id_func(Function function)
{
    TestProvider frame;
    wxUI::MenuBar menu {
        wxUI::Menu {
            "Menu1", MenuType { wxID_EXIT, function } }
    };
    menu.fitTo(&frame);
    CHECK(frame.dump() == std::vector<std::string> {
              "BindMenu:5006:1",
              std::format("menu:MenuBar:[[title:Menu1:[(menuItem:id=5006,kind={},label=\"Quit\",help=\"Quit this program\"),],]", MenuTypeLookup<MenuType>::name),
          });
}

template <typename MenuType>
auto RunMenuTest_id_func1()
{
    RunMenuTest_id_func<MenuType>([]() {});
}

template <typename MenuType>
auto RunMenuTest_id_func2()
{
    RunMenuTest_id_func<MenuType>([](wxCommandEvent&) {});
}

template <typename MenuType>
auto RunMenuTest_id_name()
{
    TestProvider frame;
    wxUI::MenuBar menu {
        wxUI::Menu {
            "Menu1", MenuType { wxID_EXIT, "Item1" } }
    };
    menu.fitTo(&frame);
    CHECK(frame.dump() == std::vector<std::string> {
              std::format("menu:MenuBar:[[title:Menu1:[(menuItem:id=5006,kind={},label=\"Item1\",help=\"Quit this program\"),],]", MenuTypeLookup<MenuType>::name),
          });
}

template <typename MenuType, typename Function>
auto RunMenuTest_id_name_func(Function function)
{
    TestProvider frame;
    wxUI::MenuBar menu {
        wxUI::Menu {
            "Menu1", MenuType { wxID_EXIT, "Item1", function } }
    };
    menu.fitTo(&frame);
    CHECK(frame.dump() == std::vector<std::string> {
              "BindMenu:5006:1",
              std::format("menu:MenuBar:[[title:Menu1:[(menuItem:id=5006,kind={},label=\"Item1\",help=\"Quit this program\"),],]", MenuTypeLookup<MenuType>::name),
          });
}

template <typename MenuType>
auto RunMenuTest_id_name_func1()
{
    RunMenuTest_id_name_func<MenuType>([]() {});
}

template <typename MenuType>
auto RunMenuTest_id_name_func2()
{
    RunMenuTest_id_name_func<MenuType>([](wxCommandEvent&) {});
}

template <typename MenuType>
auto RunMenuTest_id_name_help()
{
    TestProvider frame;
    wxUI::MenuBar menu {
        wxUI::Menu {
            "Menu1", MenuType { wxID_EXIT, "Item1", "Help1" } }
    };
    menu.fitTo(&frame);
    CHECK(frame.dump() == std::vector<std::string> {
              "BindMenu:5006:1",
              std::format("menu:MenuBar:[[title:Menu1:[(menuItem:id=5006,kind={},label=\"Item1\",help=\"Quit this program\"),],]", MenuTypeLookup<MenuType>::name),
          });
}

template <typename MenuType, typename Function>
auto RunMenuTest_id_name_help_func(Function function)
{
    TestProvider frame;
    wxUI::MenuBar menu {
        wxUI::Menu {
            "Menu1", MenuType { wxID_EXIT, "Item1", "Help1", function } }
    };
    menu.fitTo(&frame);
    CHECK(frame.dump() == std::vector<std::string> {
              "BindMenu:5006:1",
              std::format("menu:MenuBar:[[title:Menu1:[(menuItem:id=5006,kind={},label=\"Item1\",help=\"Help1\"),],]", MenuTypeLookup<MenuType>::name),
          });
}

template <typename MenuType>
auto RunMenuTest_id_name_help_func1()
{
    RunMenuTest_id_name_help_func<MenuType>([]() {});
}

template <typename MenuType>
auto RunMenuTest_id_name_help_func2()
{
    RunMenuTest_id_name_help_func<MenuType>([](wxCommandEvent&) {});
}

template <typename MenuType, typename Function>
auto RunMenuTest_name_func(Function function)
{
    TestProvider frame;
    wxUI::MenuBar menu {
        wxUI::Menu {
            "Menu1", MenuType { "Item1", function } }
    };
    menu.fitTo(&frame);
    CHECK(frame.dump() == std::vector<std::string> {
              "BindMenu:-1000000:1",
              std::format("menu:MenuBar:[[title:Menu1:[(menuItem:id=-1000000,kind={},label=\"Item1\",help=\"\"),],]", MenuTypeLookup<MenuType>::name),
          });
}

template <typename MenuType>
auto RunMenuTest_name_func1()
{
    RunMenuTest_name_func<MenuType>([]() {});
}

template <typename MenuType>
auto RunMenuTest_name_func2()
{
    RunMenuTest_name_func<MenuType>([](wxCommandEvent&) {});
}

template <typename MenuType, typename Function>
auto RunMenuTest_name_help_func(Function function)
{
    TestProvider frame;
    wxUI::MenuBar menu {
        wxUI::Menu {
            "Menu1", MenuType { "Item1", "Help1", function } }
    };
    menu.fitTo(&frame);
    CHECK(frame.dump() == std::vector<std::string> {
              "BindMenu:-1000000:1",
              std::format("menu:MenuBar:[[title:Menu1:[(menuItem:id=-1000000,kind={},label=\"Item1\",help=\"Help1\"),],]", MenuTypeLookup<MenuType>::name),
          });
}

template <typename MenuType>
auto RunMenuTest_name_help_func1()
{
    RunMenuTest_name_help_func<MenuType>([]() {});
}

template <typename MenuType>
auto RunMenuTest_name_help_func2()
{
    RunMenuTest_name_help_func<MenuType>([](wxCommandEvent&) {});
}

TEST_CASE("Menu")
{
    SECTION("emptyMenuBar1")
    {
        TestProvider frame;
        wxUI::MenuBar menu {};
        menu.fitTo(&frame);
        CHECK(frame.dump() == std::vector<std::string> {
                  "menu:MenuBar:[]",
              });
    }
    SECTION("menu.empty")
    {
        TestProvider frame;
        wxUI::MenuBar menu { wxUI::Menu { "Menu1" } };
        menu.fitTo(&frame);
        CHECK(frame.dump() == std::vector<std::string> {
                  "menu:MenuBar:[[title:Menu1:[],]",
              });
    }

    SECTION("menu.item.id")
    {
        RunMenuTest_id<wxUI::Item>();
    }

    SECTION("menu.checkitem.id")
    {
        RunMenuTest_id<wxUI::CheckItem>();
    }

    SECTION("menu.radioitem.id")
    {
        RunMenuTest_id<wxUI::RadioItem>();
    }

    SECTION("menu.item.id.funct1")
    {
        RunMenuTest_id_func1<wxUI::Item>();
    }

    SECTION("menu.checkitem.id.funct1")
    {
        RunMenuTest_id_func1<wxUI::CheckItem>();
    }

    SECTION("menu.radioitem.id.funct1")
    {
        RunMenuTest_id_func1<wxUI::RadioItem>();
    }

    SECTION("menu.item.id.funct2")
    {
        RunMenuTest_id_func2<wxUI::Item>();
    }

    SECTION("menu.checkitem.id.funct2")
    {
        RunMenuTest_id_func2<wxUI::CheckItem>();
    }

    SECTION("menu.radioitem.id.funct2")
    {
        RunMenuTest_id_func2<wxUI::RadioItem>();
    }

    SECTION("menu.item.id.name")
    {
        RunMenuTest_id_name<wxUI::Item>();
    }

    SECTION("menu.checkitem.id.name")
    {
        RunMenuTest_id_name<wxUI::CheckItem>();
    }

    SECTION("menu.radioitem.id.name")
    {
        RunMenuTest_id_name<wxUI::RadioItem>();
    }

    SECTION("menu.item.id.name.funct1")
    {
        RunMenuTest_id_name_func1<wxUI::RadioItem>();
    }

    SECTION("menu.checkitem.id.name.funct1")
    {
        RunMenuTest_id_name_func1<wxUI::CheckItem>();
    }

    SECTION("menu.radioitem.id.name.funct1")
    {
        RunMenuTest_id_name_func1<wxUI::RadioItem>();
    }

    SECTION("menu.item.id.name.funct2")
    {
        RunMenuTest_id_name_func2<wxUI::RadioItem>();
    }

    SECTION("menu.checkitem.id.name.funct2")
    {
        RunMenuTest_id_name_func2<wxUI::CheckItem>();
    }

    SECTION("menu.radioitem.id.name.funct2")
    {
        RunMenuTest_id_name_func2<wxUI::RadioItem>();
    }

    SECTION("menu.item.id.name.help")
    {
        // RunMenuTest_id_name_help<wxUI::RadioItem>();
    }

    SECTION("menu.checkitem.id.name.help")
    {
        // RunMenuTest_id_name_help<wxUI::CheckItem>();
    }

    SECTION("menu.radioitem.id.name.help")
    {
        // RunMenuTest_id_name_help<wxUI::RadioItem>();
    }

    SECTION("menu.item.id.name.help.funct1")
    {
        RunMenuTest_id_name_help_func1<wxUI::RadioItem>();
    }

    SECTION("menu.checkitem.id.name.help.funct1")
    {
        RunMenuTest_id_name_help_func1<wxUI::CheckItem>();
    }

    SECTION("menu.radioitem.id.name.help.funct1")
    {
        RunMenuTest_id_name_help_func1<wxUI::RadioItem>();
    }

    SECTION("menu.item.id.name.help.funct2")
    {
        RunMenuTest_id_name_help_func2<wxUI::RadioItem>();
    }

    SECTION("menu.checkitem.name.funct1")
    {
        RunMenuTest_name_func1<wxUI::CheckItem>();
    }

    SECTION("menu.radioitem.name.funct1")
    {
        RunMenuTest_name_func1<wxUI::RadioItem>();
    }

    SECTION("menu.item.name.funct2")
    {
        RunMenuTest_name_func2<wxUI::RadioItem>();
    }

    SECTION("menu.checkitem.name.funct2")
    {
        RunMenuTest_name_func2<wxUI::CheckItem>();
    }

    SECTION("menu.radioitem.name.funct2")
    {
        RunMenuTest_name_func2<wxUI::RadioItem>();
    }

    SECTION("menu.item.name.help.funct1")
    {
        RunMenuTest_name_help_func1<wxUI::RadioItem>();
    }

    SECTION("menu.checkitem.name.help.funct1")
    {
        RunMenuTest_name_help_func1<wxUI::CheckItem>();
    }

    SECTION("menu.radioitem.name.help.funct1")
    {
        RunMenuTest_name_help_func1<wxUI::RadioItem>();
    }

    SECTION("menu.item.name.help.funct2")
    {
        RunMenuTest_name_help_func2<wxUI::RadioItem>();
    }

    SECTION("menu.checkitem.name.help.funct2")
    {
        RunMenuTest_name_help_func2<wxUI::CheckItem>();
    }

    SECTION("menu.radioitem.name.help.funct2")
    {
        RunMenuTest_name_help_func2<wxUI::RadioItem>();
    }
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity)
