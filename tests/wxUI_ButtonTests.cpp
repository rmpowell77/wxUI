#include <catch2/catch_test_macros.hpp>
#include <wxUI/Button.h>

#include <wx/wx.h>

using TypeUnderTest = wxUI::Button;
TEST_CASE("Button")
{
    SECTION("noargs.1")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        TypeUnderTest uut {};
        auto window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK("" == window->GetLabel());
    }
    SECTION("noargs")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest {};
        auto window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK("" == window->GetLabel());
    }

    SECTION("name")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { "Hello" };
        auto window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK("Hello" == window->GetLabel());
    }

    SECTION("id")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { 10000 };
        auto window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(10000 == window->GetId());
        CHECK("" == window->GetLabel());
    }

    SECTION("id.name")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { 10000, "Hello" };
        auto window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(10000 == window->GetId());
        CHECK("Hello" == window->GetLabel());
    }

    SECTION("size")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { wxSizerFlags(1) };
        auto window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK("" == window->GetLabel());
    }

    SECTION("size.name")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { wxSizerFlags(1), "Hello" };
        auto window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK("Hello" == window->GetLabel());
    }

    SECTION("size.id")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { wxSizerFlags(1), 10000 };
        auto window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(10000 == window->GetId());
        CHECK("" == window->GetLabel());
    }

    SECTION("size.id.name")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { wxSizerFlags(1), 10000, "Hello" };
        auto window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(10000 == window->GetId());
        CHECK("Hello" == window->GetLabel());
    }
}