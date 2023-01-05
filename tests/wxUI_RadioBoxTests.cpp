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
#include <wxUI/RadioBox.h>

#include <wx/wx.h>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity)
using TypeUnderTest = wxUI::RadioBox;

TEST_CASE("RadioBox")
{
    SECTION("choices")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { std::vector<wxString> { "Hello", "Goodbye" } };
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(window->GetLabel().empty());
        CHECK(2 == window->GetCount());
        CHECK(0 == window->GetSelection());
        CHECK("Hello" == window->GetString(0));
        CHECK("Goodbye" == window->GetString(1));
    }

    SECTION("name.choice")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { "Greetings", { "Hello", "Goodbye" } };
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK("Greetings" == window->GetLabel());
        CHECK(2 == window->GetCount());
        CHECK(0 == window->GetSelection());
        CHECK("Hello" == window->GetString(0));
        CHECK("Goodbye" == window->GetString(1));
    }

    SECTION("id.choices")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { 10000, std::vector<wxString> { "Hello", "Goodbye" } };
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(10000 == window->GetId());
        CHECK(window->GetLabel().empty());
        CHECK(2 == window->GetCount());
        CHECK(0 == window->GetSelection());
        CHECK("Hello" == window->GetString(0));
        CHECK("Goodbye" == window->GetString(1));
    }

    SECTION("id.name.choice")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { 10000, "Greetings", { "Hello", "Goodbye" } };
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(10000 == window->GetId());
        CHECK("Greetings" == window->GetLabel());
        CHECK(2 == window->GetCount());
        CHECK(0 == window->GetSelection());
        CHECK("Hello" == window->GetString(0));
        CHECK("Goodbye" == window->GetString(1));
    }

    SECTION("size.choice")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { wxSizerFlags(1), std::vector<wxString> { "Hello", "Goodbye" } };
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(window->GetLabel().empty());
        CHECK(2 == window->GetCount());
        CHECK(0 == window->GetSelection());
        CHECK("Hello" == window->GetString(0));
        CHECK("Goodbye" == window->GetString(1));
    }

    SECTION("size.name.choice")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { wxSizerFlags(1), "Greetings", { "Hello", "Goodbye" } };
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK("Greetings" == window->GetLabel());
        CHECK(2 == window->GetCount());
        CHECK(0 == window->GetSelection());
        CHECK("Hello" == window->GetString(0));
        CHECK("Goodbye" == window->GetString(1));
    }

    SECTION("size.id.choice")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { wxSizerFlags(1), 10000, std::vector<wxString> { "Hello", "Goodbye" } };
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(10000 == window->GetId());
        CHECK(window->GetLabel().empty());
        CHECK(2 == window->GetCount());
        CHECK(0 == window->GetSelection());
        CHECK("Hello" == window->GetString(0));
        CHECK("Goodbye" == window->GetString(1));
    }

    SECTION("size.id.name.choice")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { wxSizerFlags(1), 10000, "Greetings", { "Hello", "Goodbye" } };
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(10000 == window->GetId());
        CHECK("Greetings" == window->GetLabel());
        CHECK(2 == window->GetCount());
        CHECK(0 == window->GetSelection());
        CHECK("Hello" == window->GetString(0));
        CHECK("Goodbye" == window->GetString(1));
    }

    SECTION("style")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { std::vector<wxString> { "Hello", "Goodbye" } }.style(wxRA_SPECIFY_COLS);
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(window->GetWindowStyle() == (wxRA_SPECIFY_COLS | wxTAB_TRAVERSAL));
    }

    SECTION("pos")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { std::vector<wxString> { "Hello", "Goodbye" } }.withPosition({ 1, 2 });
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(window->GetPosition() == wxPoint { 1, 2 });
    }

    SECTION("size")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { std::vector<wxString> { "Hello", "Goodbye" } }.withSize({ 1, 2 });
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(window->GetSize() == wxSize { 1, 2 });
    }
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity)
