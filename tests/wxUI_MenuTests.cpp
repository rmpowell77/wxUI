#include <catch2/catch_test_macros.hpp>
#include <wxUI/Menu.h>

#include <wx/wx.h>

constexpr auto CheckMenuBar0 = [](wxFrame const& frame, auto menusCount, auto menuName, auto menuCount) {
    CHECK(menusCount == frame.GetMenuBar()->GetMenuCount());
    if (menusCount < 1) {
        return;
    }
    auto menu_uut = frame.GetMenuBar()->GetMenu(0);
    CHECK(menuName == menu_uut->GetTitle());
    CHECK(menuCount == menu_uut->GetMenuItemCount());
};

constexpr auto CheckMenu0 = [](wxFrame const& frame, auto menuID, auto menuName, auto menuHelp, auto menuType) {
    auto menu_uut = frame.GetMenuBar()->GetMenu(0);
    auto menu_item_uut = menu_uut->FindItemByPosition(0);
    CHECK(menuID == menu_item_uut->GetId());
    CHECK(menuName == menu_item_uut->GetItemLabel());
    CHECK(menuHelp == menu_item_uut->GetHelp());
    CHECK(menuType == menu_item_uut->GetKind());
};

template <typename MenuType>
struct MenuTypeLookup;

template <>
struct MenuTypeLookup<wxUI::Item> {
    static constexpr auto itemKind = wxITEM_NORMAL;
};

template <>
struct MenuTypeLookup<wxUI::CheckItem> {
    static constexpr auto itemKind = wxITEM_CHECK;
};

template <>
struct MenuTypeLookup<wxUI::RadioItem> {
    static constexpr auto itemKind = wxITEM_RADIO;
};

template <typename MenuType>
auto RunMenuTest_id()
{
    wxFrame frame { nullptr, wxID_ANY, "" };
    wxUI::MenuBar menu {
        wxUI::Menu {
            "Menu1", MenuType { wxID_EXIT } }
    };
    menu.attachTo(&frame);
    CheckMenuBar0(frame, 1, "Menu1", 1);
    CheckMenu0(frame, wxID_EXIT, "&Quit", "Quit this program", MenuTypeLookup<MenuType> {}.itemKind);
}

template <typename MenuType, typename Function>
auto RunMenuTest_id_func(Function f)
{
    wxFrame frame { nullptr, wxID_ANY, "" };
    wxUI::MenuBar menu {
        wxUI::Menu {
            "Menu1", MenuType { wxID_EXIT, f } }
    };
    menu.attachTo(&frame);
    CheckMenuBar0(frame, 1, "Menu1", 1);
    CheckMenu0(frame, wxID_EXIT, "&Quit", "Quit this program", MenuTypeLookup<MenuType> {}.itemKind);
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
    wxFrame frame { nullptr, wxID_ANY, "" };
    wxUI::MenuBar menu {
        wxUI::Menu {
            "Menu1", MenuType { wxID_EXIT, "Item1" } }
    };
    menu.attachTo(&frame);
    CheckMenuBar0(frame, 1, "Menu1", 1);
    CheckMenu0(frame, wxID_EXIT, "Item1", "Quit this program", MenuTypeLookup<MenuType> {}.itemKind);
}

template <typename MenuType, typename Function>
auto RunMenuTest_id_name_func(Function f)
{
    wxFrame frame { nullptr, wxID_ANY, "" };
    wxUI::MenuBar menu {
        wxUI::Menu {
            "Menu1", MenuType { wxID_EXIT, "Item1", f } }
    };
    menu.attachTo(&frame);
    CheckMenuBar0(frame, 1, "Menu1", 1);
    CheckMenu0(frame, wxID_EXIT, "Item1", "Quit this program", MenuTypeLookup<MenuType> {}.itemKind);
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
    wxFrame frame { nullptr, wxID_ANY, "" };
    wxUI::MenuBar menu {
        wxUI::Menu {
            "Menu1", MenuType { wxID_EXIT, "Item1", "Help1" } }
    };
    menu.attachTo(&frame);
    CheckMenuBar0(frame, 1, "Menu1", 1);
    CheckMenu0(frame, wxID_EXIT, "Item1", "Help1", MenuTypeLookup<MenuType> {}.itemKind);
}

template <typename MenuType, typename Function>
auto RunMenuTest_id_name_help_func(Function f)
{
    wxFrame frame { nullptr, wxID_ANY, "" };
    wxUI::MenuBar menu {
        wxUI::Menu {
            "Menu1", MenuType { wxID_EXIT, "Item1", "Help1", f } }
    };
    menu.attachTo(&frame);
    CheckMenuBar0(frame, 1, "Menu1", 1);
    CheckMenu0(frame, wxID_EXIT, "Item1", "Help1", MenuTypeLookup<MenuType> {}.itemKind);
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
auto RunMenuTest_name_func(Function f)
{
    wxFrame frame { nullptr, wxID_ANY, "" };
    wxUI::MenuBar menu {
        wxUI::Menu {
            "Menu1", MenuType { "Item1", f } }
    };
    menu.attachTo(&frame);
    CheckMenuBar0(frame, 1, "Menu1", 1);
    CheckMenu0(frame, wxID_AUTO_LOWEST, "Item1", "", MenuTypeLookup<MenuType> {}.itemKind);
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
auto RunMenuTest_name_help_func(Function f)
{
    wxFrame frame { nullptr, wxID_ANY, "" };
    wxUI::MenuBar menu {
        wxUI::Menu {
            "Menu1", MenuType { "Item1", "Help1", f } }
    };
    menu.attachTo(&frame);
    CheckMenuBar0(frame, 1, "Menu1", 1);
    CheckMenu0(frame, wxID_AUTO_LOWEST, "Item1", "Help1", MenuTypeLookup<MenuType> {}.itemKind);
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
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::MenuBar menu {};
        menu.attachTo(&frame);
        CheckMenuBar0(frame, 0, "", 0);
    }

    SECTION("emptyMenuBar2")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::MenuBar menu { {} };
        menu.attachTo(&frame);
        CheckMenuBar0(frame, 0, "", 0);
    }
    SECTION("menu.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::MenuBar menu { wxUI::Menu { "Menu1" } };
        menu.attachTo(&frame);
        CheckMenuBar0(frame, 1, "Menu1", 0);
    }

    SECTION("menu.empty2")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::MenuBar menu { wxUI::Menu {
            "Menu1",
        } };
        menu.attachTo(&frame);
        CheckMenuBar0(frame, 1, "Menu1", 0);
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
        // RunMenuTest_id_name<wxUI::Item>();
    }

    SECTION("menu.checkitem.id.name")
    {
        // RunMenuTest_id_name<wxUI::CheckItem>();
    }

    SECTION("menu.radioitem.id.name")
    {
        // RunMenuTest_id_name<wxUI::RadioItem>();
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
