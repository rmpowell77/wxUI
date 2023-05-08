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
#include <wxUI/Slider.h>

#include <wx/wx.h>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity, misc-use-anonymous-namespace, cppcoreguidelines-avoid-do-while)
using TypeUnderTest = wxUI::Slider;

struct SliderTestPolicy {
    using TypeUnderTest = TypeUnderTest;
    static auto createUUT() { return TypeUnderTest {}; }
    static auto testStyle() { return wxSL_LABELS; }
    static auto testPosition() { return wxPoint { 14, 2 }; }
    static auto testSize() { return wxSize { 100, 40 }; }
    static auto expectedStyle() { return testStyle(); }
    static auto expectedPosition() { return wxPoint { 18, 2 }; }
    static auto expectedSize() { return wxSize { 85, 40 }; }
};
static auto createUUT() { return SliderTestPolicy::createUUT(); }

TEST_CASE("Slider")
{
    SECTION("noargs")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = createUUT();
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(window->GetValue() == 0);
    }

    SECTION("range")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { std::pair { 1, 5 } };
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(window->GetValue() == 1);
    }

    SECTION("range.init")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { std::pair { 1, 5 }, 3 };
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(window->GetValue() == 3);
    }

    SECTION("id")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { 10000 };
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(10000 == window->GetId());
        CHECK(window->GetValue() == 0);
    }

    SECTION("id.range")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { 10000, std::pair { 1, 5 } };
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(10000 == window->GetId());
        CHECK(window->GetValue() == 1);
    }

    SECTION("id.range.init")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { 10000, std::pair { 1, 5 }, 3 };
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(10000 == window->GetId());
        CHECK(window->GetValue() == 3);
    }

    SECTION("size")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { wxSizerFlags(1) };
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(window->GetLabel().empty());
    }

    SECTION("size.range")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { wxSizerFlags(1), std::pair { 1, 5 } };
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(window->GetValue() == 1);
    }

    SECTION("size.range.init")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { wxSizerFlags(1), std::pair { 1, 5 }, 3 };
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(window->GetValue() == 3);
    }

    SECTION("size.id")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { wxSizerFlags(1), 10000 };
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(10000 == window->GetId());
        CHECK(window->GetValue() == 0);
    }

    SECTION("size.id.range")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { wxSizerFlags(1), 10000, std::pair { 1, 5 } };
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(10000 == window->GetId());
        CHECK(window->GetValue() == 1);
    }

    SECTION("size.id.range.init")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { wxSizerFlags(1), 10000, std::pair { 1, 5 }, 3 };
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(10000 == window->GetId());
        CHECK(window->GetValue() == 3);
    }

    CHAINING_TEST(SliderTestPolicy)
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity, misc-use-anonymous-namespace, cppcoreguidelines-avoid-do-while)
