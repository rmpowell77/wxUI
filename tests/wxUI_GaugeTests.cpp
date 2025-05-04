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
#include "wxUI_TestControlCommon.h"
#include <catch2/catch_test_macros.hpp>
#include <wxUI/Gauge.h>

#include <wx/wx.h>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity, misc-use-anonymous-namespace, cppcoreguidelines-avoid-do-while)
using TypeUnderTest = wxUI::Gauge;

struct GaugeTestPolicy {
    using TypeUnderTest = wxUI::Gauge;
    static auto createUUT() { return TypeUnderTest {}; }
    static auto testStyle() { return wxGA_VERTICAL; }
    static auto testPosition() { return wxPoint { 1, 2 }; }
    static auto testSize() { return wxSize { 10, 12 }; }
    static auto expectedStyle() { return testStyle(); }
    static auto expectedPosition() { return testPosition(); }
    static auto expectedSize() { return testSize(); }
};
static auto createUUT() { return GaugeTestPolicy::createUUT(); }

TEST_CASE("Gauge")
{
    SECTION("noargs")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = createUUT();
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(window->GetRange() == 100);
    }

    SECTION("name")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { wxUI::Gauge::withRange {}, 200 };
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(window->GetRange() == 200);
    }

    SECTION("id")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { wxUI::Gauge::withIdentity {}, 10000 };
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(10000 == window->GetId());
        CHECK(window->GetRange() == 100);
    }

    SECTION("id.name")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { wxUI::Gauge::withIdentity {}, 10000, 200 };
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(10000 == window->GetId());
        CHECK(window->GetRange() == 200);
    }

    SECTION("size")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { wxSizerFlags(1) };
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(window->GetRange() == 100);
    }

    SECTION("size.name")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { wxSizerFlags(1), wxUI::Gauge::withRange {}, 200 };
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(window->GetRange() == 200);
    }

    SECTION("size.id")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { wxSizerFlags(1), wxUI::Gauge::withIdentity {}, 10000 };
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(10000 == window->GetId());
        CHECK(window->GetRange() == 100);
    }

    SECTION("size.id.name")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { wxSizerFlags(1), wxUI::Gauge::withIdentity {}, 10000, 200 };
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(10000 == window->GetId());
        CHECK(window->GetRange() == 200);
    }

    SECTION("style")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = createUUT().setStyle(wxGA_HORIZONTAL);
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(window->GetWindowStyle() == wxGA_HORIZONTAL);
    }

    SECTION("pos")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = createUUT().withPosition({ 1, 2 });
        auto* window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(window->GetRange() == 100);
    }

    SECTION("proxy.bind")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto proxy = TypeUnderTest::Proxy {};
        auto uut = proxy.bind(TypeUnderTest {}.withSize({ 1, 2 }));
        uut.create(&frame);
        CHECK(static_cast<int>(*proxy) == 0);
        *proxy = 30;
        CHECK(static_cast<int>(*proxy) == 30);
        CHECK(static_cast<int>(proxy.range()) == 100);
        proxy.range() = 200;
        CHECK(static_cast<int>(proxy.range()) == 200);
    }

    COMMON_TESTS(GaugeTestPolicy)
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity, misc-use-anonymous-namespace, cppcoreguidelines-avoid-do-while)
