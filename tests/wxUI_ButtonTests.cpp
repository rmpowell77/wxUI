/*
MIT License

Copyright (c) 2022 Richard Powell

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
#include <catch2/catch_test_macros.hpp>
#include <wxUI/Button.h>

#include <wx/wx.h>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity)
using TypeUnderTest = wxUI::Button;
TEST_CASE("Button")
{
    SECTION("noargs.1")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        TypeUnderTest uut {};
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(window->GetLabel().empty());
        uut.withPosition(wxPoint {});
    }
    SECTION("noargs")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest {};
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(window->GetLabel().empty());
    }

    SECTION("name")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { "Hello" };
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK("Hello" == window->GetLabel());
    }

    SECTION("id")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { 10000 };
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(10000 == window->GetId());
        CHECK(window->GetLabel().empty());
    }

    SECTION("id.name")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { 10000, "Hello" };
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(10000 == window->GetId());
        CHECK("Hello" == window->GetLabel());
    }

    SECTION("size")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { wxSizerFlags(1) };
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(window->GetLabel().empty());
    }

    SECTION("size.name")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { wxSizerFlags(1), "Hello" };
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK("Hello" == window->GetLabel());
    }

    SECTION("size.id")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { wxSizerFlags(1), 10000 };
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(10000 == window->GetId());
        CHECK(window->GetLabel().empty());
    }

    SECTION("size.id.name")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { wxSizerFlags(1), 10000, "Hello" };
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(10000 == window->GetId());
        CHECK("Hello" == window->GetLabel());
    }

    SECTION("style")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest {}.withStyle(wxBU_LEFT);
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(window->GetWindowStyle() == wxBU_LEFT);
    }

    SECTION("pos")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest {}.withPosition({ 1, 2 });
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(window->GetPosition() == wxPoint { 1, 2 });
    }

    SECTION("size")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest {}.withSize({ 1, 2 });
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(window->GetSize() == wxSize { 1, 2 });
    }
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity)
