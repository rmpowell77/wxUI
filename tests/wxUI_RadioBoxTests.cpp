#include <catch2/catch_test_macros.hpp>
#include <wxUI/RadioBox.h>

#include <wx/wx.h>

using TypeUnderTest = wxUI::RadioBox;

TEST_CASE("RadioBox")
{
    // These tests should be enabled after
    // https://github.com/wxWidgets/wxWidgets/issues/22751
    // is resolved
    SECTION("noargs.1")
    {
        // wxFrame frame { nullptr, wxID_ANY, "" };
        // TypeUnderTest uut {};
        // auto window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        // CHECK("" == window->GetLabel());
        // CHECK(0 == window->GetCount());
    }

    SECTION("noargs")
    {
        // wxFrame frame { nullptr, wxID_ANY, "" };
        // auto uut = TypeUnderTest {};
        // auto window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        // CHECK("" == window->GetLabel());
        // CHECK(0 == window->GetCount());
    }

    SECTION("choices")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { std::vector<wxString> { "Hello", "Goodbye" } };
        auto window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK("" == window->GetLabel());
        CHECK(2 == window->GetCount());
        CHECK(0 == window->GetSelection());
        CHECK("Hello" == window->GetString(0));
        CHECK("Goodbye" == window->GetString(1));
    }

    SECTION("name")
    {
        // wxFrame frame { nullptr, wxID_ANY, "" };
        // auto uut = TypeUnderTest { "Greetings" };
        // auto window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        // CHECK("Greetings" == window->GetLabel());
        // CHECK(0 == window->GetCount());
    }

    SECTION("name.choice")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { "Greetings", { "Hello", "Goodbye" } };
        auto window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK("Greetings" == window->GetLabel());
        CHECK(2 == window->GetCount());
        CHECK(0 == window->GetSelection());
        CHECK("Hello" == window->GetString(0));
        CHECK("Goodbye" == window->GetString(1));
    }

    SECTION("id")
    {
        // wxFrame frame { nullptr, wxID_ANY, "" };
        // auto uut = TypeUnderTest { 10000 };
        // auto window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        // CHECK(10000 == window->GetId());
        // CHECK("" == window->GetLabel());
        // CHECK(0 == window->GetCount());
    }

    SECTION("id.choices")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { 10000, std::vector<wxString> { "Hello", "Goodbye" } };
        auto window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(10000 == window->GetId());
        CHECK("" == window->GetLabel());
        CHECK(2 == window->GetCount());
        CHECK(0 == window->GetSelection());
        CHECK("Hello" == window->GetString(0));
        CHECK("Goodbye" == window->GetString(1));
    }

    SECTION("id.name")
    {
        // wxFrame frame { nullptr, wxID_ANY, "" };
        // auto uut = TypeUnderTest { 10000, "Greetings" };
        // auto window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        // CHECK(10000 == window->GetId());
        // CHECK("Greetings" == window->GetLabel());
        // CHECK(0 == window->GetCount());
    }

    SECTION("id.name.choice")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { 10000, "Greetings", { "Hello", "Goodbye" } };
        auto window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(10000 == window->GetId());
        CHECK("Greetings" == window->GetLabel());
        CHECK(2 == window->GetCount());
        CHECK(0 == window->GetSelection());
        CHECK("Hello" == window->GetString(0));
        CHECK("Goodbye" == window->GetString(1));
    }

    SECTION("size")
    {
        // wxFrame frame { nullptr, wxID_ANY, "" };
        // auto uut = TypeUnderTest { wxSizerFlags(1), wxID_ANY };
        // auto window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        // CHECK("" == window->GetLabel());
        // CHECK(0 == window->GetCount());
    }

    SECTION("size.choice")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { wxSizerFlags(1), std::vector<wxString> { "Hello", "Goodbye" } };
        auto window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK("" == window->GetLabel());
        CHECK(2 == window->GetCount());
        CHECK(0 == window->GetSelection());
        CHECK("Hello" == window->GetString(0));
        CHECK("Goodbye" == window->GetString(1));
    }

    SECTION("size.name")
    {
        // wxFrame frame { nullptr, wxID_ANY, "" };
        // auto uut = TypeUnderTest { wxSizerFlags(1), "Greetings" };
        // auto window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        // CHECK("Greetings" == window->GetLabel());
        // CHECK(0 == window->GetCount());
    }

    SECTION("size.name.choice")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { wxSizerFlags(1), "Greetings", { "Hello", "Goodbye" } };
        auto window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK("Greetings" == window->GetLabel());
        CHECK(2 == window->GetCount());
        CHECK(0 == window->GetSelection());
        CHECK("Hello" == window->GetString(0));
        CHECK("Goodbye" == window->GetString(1));
    }

    SECTION("size.id")
    {
        // wxFrame frame { nullptr, wxID_ANY, "" };
        // auto uut = TypeUnderTest { wxSizerFlags(1), 10000 };
        // auto window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        // CHECK(10000 == window->GetId());
        // CHECK("" == window->GetLabel());
        // CHECK(0 == window->GetCount());
    }

    SECTION("size.id.choice")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { wxSizerFlags(1), 10000, std::vector<wxString> { "Hello", "Goodbye" } };
        auto window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(10000 == window->GetId());
        CHECK("" == window->GetLabel());
        CHECK(2 == window->GetCount());
        CHECK(0 == window->GetSelection());
        CHECK("Hello" == window->GetString(0));
        CHECK("Goodbye" == window->GetString(1));
    }

    SECTION("size.id.name")
    {
        // wxFrame frame { nullptr, wxID_ANY, "" };
        // auto uut = TypeUnderTest { wxSizerFlags(1), 10000, "Greetings" };
        // auto window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        // CHECK(10000 == window->GetId());
        // CHECK("" == window->GetLabel());
        // CHECK(0 == window->GetCount());
    }

    SECTION("size.id.name.choice")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { wxSizerFlags(1), 10000, "Greetings", { "Hello", "Goodbye" } };
        auto window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(10000 == window->GetId());
        CHECK("Greetings" == window->GetLabel());
        CHECK(2 == window->GetCount());
        CHECK(0 == window->GetSelection());
        CHECK("Hello" == window->GetString(0));
        CHECK("Goodbye" == window->GetString(1));
    }
}