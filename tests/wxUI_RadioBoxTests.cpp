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
#include "wxUI_TestControlCommon.h"
#include <catch2/catch_test_macros.hpp>
#include <wxUI/RadioBox.h>

#include <wx/wx.h>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity, misc-use-anonymous-namespace, cppcoreguidelines-avoid-do-while)
using TypeUnderTest = wxUI::RadioBox;

struct RadioBoxTestPolicy {
    using TypeUnderTest = TypeUnderTest;
    static auto createUUT() { return TypeUnderTest { TypeUnderTest::withChoices {}, std::vector<wxString> { "Hello", "Goodbye" } }; }
    static auto testStyle() { return wxRA_SPECIFY_COLS; }
    static auto testPosition() { return wxPoint { 1, 2 }; }
    static auto testSize() { return wxSize { 10, 12 }; }
    static auto expectedStyle() { return testStyle(); }
    static auto expectedPosition() { return testPosition(); }
    static auto expectedSize() { return testSize(); }
};
static auto createUUT() { return RadioBoxTestPolicy::createUUT(); }

TEST_CASE("RadioBox")
{
    SECTION("choices")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = createUUT();
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
        using namespace std::literals;
        auto uut = TypeUnderTest { "Greetings", TypeUnderTest::withChoices {}, { "Hello"s, "Goodbye"s } };
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
        using namespace std::literals;
        auto uut = TypeUnderTest { 10000, TypeUnderTest::withChoices {}, { "Hello"s, "Goodbye"s } };
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
        auto uut = TypeUnderTest { 10000, "Greetings", TypeUnderTest::withChoices {}, { "Hello", "Goodbye" } };
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
        using namespace std::literals;
        auto uut = TypeUnderTest { wxSizerFlags(1), TypeUnderTest::withChoices {}, { "Hello"s, "Goodbye"s } };
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
        using namespace std::literals;
        auto uut = TypeUnderTest { wxSizerFlags(1), "Greetings", TypeUnderTest::withChoices {}, { "Hello"s, "Goodbye"s } };
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
        using namespace std::literals;
        auto uut = TypeUnderTest { wxSizerFlags(1), 10000, TypeUnderTest::withChoices {}, { "Hello"s, "Goodbye"s } };
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
        using namespace std::literals;
        auto uut = TypeUnderTest { wxSizerFlags(1), 10000, "Greetings", TypeUnderTest::withChoices {}, { "Hello"s, "Goodbye"s } };
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(10000 == window->GetId());
        CHECK("Greetings" == window->GetLabel());
        CHECK(2 == window->GetCount());
        CHECK(0 == window->GetSelection());
        CHECK("Hello" == window->GetString(0));
        CHECK("Goodbye" == window->GetString(1));
    }

    SECTION("choices.ranges")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { TypeUnderTest::withChoices {}, std::vector<std::string> { "Hello", "Goodbye" } };
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(window->GetLabel().empty());
        CHECK(2 == window->GetCount());
        CHECK(0 == window->GetSelection());
        CHECK("Hello" == window->GetString(0));
        CHECK("Goodbye" == window->GetString(1));
    }

    SECTION("name.choice.ranges")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        using namespace std::literals;
        auto uut = TypeUnderTest { "Greetings", TypeUnderTest::withChoices {}, std::vector<std::string> { "Hello"s, "Goodbye"s } };
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK("Greetings" == window->GetLabel());
        CHECK(2 == window->GetCount());
        CHECK(0 == window->GetSelection());
        CHECK("Hello" == window->GetString(0));
        CHECK("Goodbye" == window->GetString(1));
    }

    SECTION("id.choices.ranges")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        using namespace std::literals;
        auto uut = TypeUnderTest { 10000, TypeUnderTest::withChoices {}, std::vector<std::string> { "Hello"s, "Goodbye"s } };
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(10000 == window->GetId());
        CHECK(window->GetLabel().empty());
        CHECK(2 == window->GetCount());
        CHECK(0 == window->GetSelection());
        CHECK("Hello" == window->GetString(0));
        CHECK("Goodbye" == window->GetString(1));
    }

    SECTION("id.name.choice.ranges")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { 10000, "Greetings", TypeUnderTest::withChoices {}, std::vector<std::string> { "Hello", "Goodbye" } };
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(10000 == window->GetId());
        CHECK("Greetings" == window->GetLabel());
        CHECK(2 == window->GetCount());
        CHECK(0 == window->GetSelection());
        CHECK("Hello" == window->GetString(0));
        CHECK("Goodbye" == window->GetString(1));
    }

    SECTION("size.choice.ranges")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        using namespace std::literals;
        auto uut = TypeUnderTest { wxSizerFlags(1), TypeUnderTest::withChoices {}, std::vector<std::string> { "Hello"s, "Goodbye"s } };
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(window->GetLabel().empty());
        CHECK(2 == window->GetCount());
        CHECK(0 == window->GetSelection());
        CHECK("Hello" == window->GetString(0));
        CHECK("Goodbye" == window->GetString(1));
    }

    SECTION("size.name.choice.ranges")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        using namespace std::literals;
        auto uut = TypeUnderTest { wxSizerFlags(1), "Greetings", TypeUnderTest::withChoices {}, std::vector<std::string> { "Hello"s, "Goodbye"s } };
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK("Greetings" == window->GetLabel());
        CHECK(2 == window->GetCount());
        CHECK(0 == window->GetSelection());
        CHECK("Hello" == window->GetString(0));
        CHECK("Goodbye" == window->GetString(1));
    }

    SECTION("size.id.choice.ranges")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        using namespace std::literals;
        auto uut = TypeUnderTest { wxSizerFlags(1), 10000, TypeUnderTest::withChoices {}, std::vector<std::string> { "Hello"s, "Goodbye"s } };
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(10000 == window->GetId());
        CHECK(window->GetLabel().empty());
        CHECK(2 == window->GetCount());
        CHECK(0 == window->GetSelection());
        CHECK("Hello" == window->GetString(0));
        CHECK("Goodbye" == window->GetString(1));
    }

    SECTION("size.id.name.choice.ranges")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        using namespace std::literals;
        auto uut = TypeUnderTest { wxSizerFlags(1), 10000, "Greetings", TypeUnderTest::withChoices {}, std::vector<std::string> { "Hello"s, "Goodbye"s } };
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
        auto uut = createUUT().withStyle(wxRA_SPECIFY_COLS);
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(window->GetWindowStyle() == (wxRA_SPECIFY_COLS | wxTAB_TRAVERSAL));
    }

    SECTION("pos")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = createUUT().withPosition({ 1, 2 });
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(window->GetPosition() == wxPoint { 1, 2 });
    }

    SECTION("size")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = createUUT().withSize({ 1, 2 });
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(window->GetSize() == wxSize { 1, 2 });
    }

    COMMON_TESTS(RadioBoxTestPolicy)
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity, misc-use-anonymous-namespace, cppcoreguidelines-avoid-do-while)
