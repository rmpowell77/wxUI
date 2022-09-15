#include <catch2/catch_test_macros.hpp>
#include <wxUI/Choice.h>

#include <wx/wx.h>

using TypeUnderTest = wxUI::Choice;

TEST_CASE("Choice")
{
    SECTION("noargs.1")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        TypeUnderTest uut {};
        auto window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(0 == window->GetCount());
    }
    SECTION("noargs")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest {};
        auto window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(0 == window->GetCount());
    }

    SECTION("choice")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { { "Hello", "Goodbye" } };
        auto window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(2 == window->GetCount());
        CHECK(0 == window->GetSelection());
        CHECK("Hello" == window->GetString(0));
        CHECK("Goodbye" == window->GetString(1));
    }

    SECTION("id")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { 10000 };
        auto window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(10000 == window->GetId());
        CHECK(0 == window->GetCount());
    }

    SECTION("id.choice")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { 10000, { "Hello", "Goodbye" } };
        auto window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(10000 == window->GetId());
        CHECK(2 == window->GetCount());
        CHECK(0 == window->GetSelection());
        CHECK("Hello" == window->GetString(0));
        CHECK("Goodbye" == window->GetString(1));
    }

    SECTION("size")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { wxSizerFlags(1) };
        auto window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(0 == window->GetCount());
    }

    SECTION("size.choice")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { wxSizerFlags(1), { "Hello", "Goodbye" } };
        auto window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(2 == window->GetCount());
        CHECK(0 == window->GetSelection());
        CHECK("Hello" == window->GetString(0));
        CHECK("Goodbye" == window->GetString(1));
    }

    SECTION("size.id")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { wxSizerFlags(1), 10000 };
        auto window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(10000 == window->GetId());
        CHECK(0 == window->GetCount());
    }

    SECTION("size.id.choice")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { wxSizerFlags(1), 10000, { "Hello", "Goodbye" } };
        auto window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(10000 == window->GetId());
        CHECK(2 == window->GetCount());
        CHECK(0 == window->GetSelection());
        CHECK("Hello" == window->GetString(0));
        CHECK("Goodbye" == window->GetString(1));
    }
}
