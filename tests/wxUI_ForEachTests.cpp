/*
MIT License

Copyright (c) 2022-2026 Richard Powell

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
#include <memory>
#include <ranges>
#include <wxUI/Button.hpp>
#include <wxUI/ForEach.hpp>

#include <wx/wx.h>

using namespace wxUITests;

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity)

TEST_CASE("ForEach")
{
    auto kCaseVForEach = std::vector<std::string> {
        "Create:Sizer[orientation=wxVERTICAL]",
        "Create:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"A\"]",
        "Create:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"B\"]",
        "Create:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"C\"]",
        "topsizer:Sizer[orientation=wxVERTICAL]",
        "controller:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"A\"]",
        "SetEnabled:true",
        "controller:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"B\"]",
        "SetEnabled:true",
        "controller:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"C\"]",
        "SetEnabled:true",
        "sizer:Sizer[orientation=wxVERTICAL]",
        "Add:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"A\"]:flags:(0,0x0,0)",
        "Add:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"B\"]:flags:(0,0x0,0)",
        "Add:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"C\"]:flags:(0,0x0,0)",
        "SetSizeHints:[id=0, pos=(0,0), size=(0,0), style=0]",
    };
    auto kCaseHForEach = std::vector<std::string> {
        "Create:Sizer[orientation=wxHORIZONTAL]",
        "Create:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"A\"]",
        "Create:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"B\"]",
        "Create:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"C\"]",
        "topsizer:Sizer[orientation=wxHORIZONTAL]",
        "controller:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"A\"]",
        "SetEnabled:true",
        "controller:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"B\"]",
        "SetEnabled:true",
        "controller:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"C\"]",
        "SetEnabled:true",
        "sizer:Sizer[orientation=wxHORIZONTAL]",
        "Add:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"A\"]:flags:(0,0x0,0)",
        "Add:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"B\"]:flags:(0,0x0,0)",
        "Add:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"C\"]:flags:(0,0x0,0)",
        "SetSizeHints:[id=0, pos=(0,0), size=(0,0), style=0]",
    };
    auto kCaseVForEachWithID = std::vector<std::string> {
        "Create:Sizer[orientation=wxVERTICAL]",
        "Create:wxButton[id=5101, pos=(-1,-1), size=(-1,-1), style=0, text=\"A\"]",
        "Create:wxButton[id=5100, pos=(-1,-1), size=(-1,-1), style=0, text=\"B\"]",
        "Create:wxButton[id=5102, pos=(-1,-1), size=(-1,-1), style=0, text=\"C\"]",
        "topsizer:Sizer[orientation=wxVERTICAL]",
        "controller:wxButton[id=5101, pos=(-1,-1), size=(-1,-1), style=0, text=\"A\"]",
        "SetEnabled:true",
        "controller:wxButton[id=5100, pos=(-1,-1), size=(-1,-1), style=0, text=\"B\"]",
        "SetEnabled:true",
        "controller:wxButton[id=5102, pos=(-1,-1), size=(-1,-1), style=0, text=\"C\"]",
        "SetEnabled:true",
        "sizer:Sizer[orientation=wxVERTICAL]",
        "Add:wxButton[id=5101, pos=(-1,-1), size=(-1,-1), style=0, text=\"A\"]:flags:(0,0x0,0)",
        "Add:wxButton[id=5100, pos=(-1,-1), size=(-1,-1), style=0, text=\"B\"]:flags:(0,0x0,0)",
        "Add:wxButton[id=5102, pos=(-1,-1), size=(-1,-1), style=0, text=\"C\"]:flags:(0,0x0,0)",
        "SetSizeHints:[id=0, pos=(0,0), size=(0,0), style=0]",
    };
    auto kCaseHForEachWithID = std::vector<std::string> {
        "Create:Sizer[orientation=wxHORIZONTAL]",
        "Create:wxButton[id=5101, pos=(-1,-1), size=(-1,-1), style=0, text=\"A\"]",
        "Create:wxButton[id=5100, pos=(-1,-1), size=(-1,-1), style=0, text=\"B\"]",
        "Create:wxButton[id=5102, pos=(-1,-1), size=(-1,-1), style=0, text=\"C\"]",
        "topsizer:Sizer[orientation=wxHORIZONTAL]",
        "controller:wxButton[id=5101, pos=(-1,-1), size=(-1,-1), style=0, text=\"A\"]",
        "SetEnabled:true",
        "controller:wxButton[id=5100, pos=(-1,-1), size=(-1,-1), style=0, text=\"B\"]",
        "SetEnabled:true",
        "controller:wxButton[id=5102, pos=(-1,-1), size=(-1,-1), style=0, text=\"C\"]",
        "SetEnabled:true",
        "sizer:Sizer[orientation=wxHORIZONTAL]",
        "Add:wxButton[id=5101, pos=(-1,-1), size=(-1,-1), style=0, text=\"A\"]:flags:(0,0x0,0)",
        "Add:wxButton[id=5100, pos=(-1,-1), size=(-1,-1), style=0, text=\"B\"]:flags:(0,0x0,0)",
        "Add:wxButton[id=5102, pos=(-1,-1), size=(-1,-1), style=0, text=\"C\"]:flags:(0,0x0,0)",
        "SetSizeHints:[id=0, pos=(0,0), size=(0,0), style=0]",
    };
    auto kCaseVForEachFiltered = std::vector<std::string> {
        "Create:Sizer[orientation=wxVERTICAL]",
        "Create:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"A\"]",
        "topsizer:Sizer[orientation=wxVERTICAL]",
        "controller:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"A\"]",
        "SetEnabled:true",
        "sizer:Sizer[orientation=wxVERTICAL]",
        "Add:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"A\"]:flags:(0,0x0,0)",
        "SetSizeHints:[id=0, pos=(0,0), size=(0,0), style=0]",
    };
    auto kCaseHForEachFiltered = std::vector<std::string> {
        "Create:Sizer[orientation=wxHORIZONTAL]",
        "Create:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"A\"]",
        "topsizer:Sizer[orientation=wxHORIZONTAL]",
        "controller:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"A\"]",
        "SetEnabled:true",
        "sizer:Sizer[orientation=wxHORIZONTAL]",
        "Add:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"A\"]:flags:(0,0x0,0)",
        "SetSizeHints:[id=0, pos=(0,0), size=(0,0), style=0]",
    };
    auto kCaseVForEachFilteredWithID = std::vector<std::string> {
        "Create:Sizer[orientation=wxVERTICAL]",
        "Create:wxButton[id=5101, pos=(-1,-1), size=(-1,-1), style=0, text=\"A\"]",
        "topsizer:Sizer[orientation=wxVERTICAL]",
        "controller:wxButton[id=5101, pos=(-1,-1), size=(-1,-1), style=0, text=\"A\"]",
        "SetEnabled:true",
        "sizer:Sizer[orientation=wxVERTICAL]",
        "Add:wxButton[id=5101, pos=(-1,-1), size=(-1,-1), style=0, text=\"A\"]:flags:(0,0x0,0)",
        "SetSizeHints:[id=0, pos=(0,0), size=(0,0), style=0]",
    };
    auto kCaseHForEachFilteredWithID = std::vector<std::string> {
        "Create:Sizer[orientation=wxHORIZONTAL]",
        "Create:wxButton[id=5101, pos=(-1,-1), size=(-1,-1), style=0, text=\"A\"]",
        "topsizer:Sizer[orientation=wxHORIZONTAL]",
        "controller:wxButton[id=5101, pos=(-1,-1), size=(-1,-1), style=0, text=\"A\"]",
        "SetEnabled:true",
        "sizer:Sizer[orientation=wxHORIZONTAL]",
        "Add:wxButton[id=5101, pos=(-1,-1), size=(-1,-1), style=0, text=\"A\"]:flags:(0,0x0,0)",
        "SetSizeHints:[id=0, pos=(0,0), size=(0,0), style=0]",
    };
    SECTION("ForEach.Vector.T.xV")
    {
        TestParent frame;
        wxUI::VForEach(
            std::vector { "A", "B", "C" },
            [](auto name) {
                return wxUI::Button { name };
            })
            .fitTo(&frame);
        CHECK(frame.dump() == kCaseVForEach);
    }
    SECTION("ForEach.Vector.T.V")
    {
        TestParent frame;
        wxUI::VForEach(
            std::vector { "A", "B", "C" },
            [](auto name) {
                return wxUI::Button { name };
            })
            .fitTo(&frame);
        CHECK(frame.dump() == kCaseVForEach);
    }
    SECTION("ForEach.Vector.T.H")
    {
        TestParent frame;
        wxUI::HForEach(
            std::vector { "A", "B", "C" },
            [](auto name) {
                return wxUI::Button { name };
            })
            .fitTo(&frame);
        CHECK(frame.dump() == kCaseHForEach);
    }
    SECTION("ForEach.lvalue.Vector.T")
    {
        auto data = std::vector { "A", "B", "C" };
        TestParent frame;
        wxUI::VForEach(data, [](auto name) {
            return wxUI::Button { name };
        }).fitTo(&frame);
        CHECK(frame.dump() == kCaseVForEach);
    }
    SECTION("ForEach.Vector.Tuple")
    {
        TestParent frame;
        wxUI::VForEach(
            std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" }, { wxID_APPLY, "C" } },
            [](auto name) {
                return wxUI::Button { std::get<0>(name), std::get<1>(name) };
            })
            .fitTo(&frame);
        CHECK(frame.dump() == kCaseVForEachWithID);
    }
    SECTION("ForEach.Vector.Tuple.Multiarg")
    {
        TestParent frame;
        wxUI::VForEach(
            std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" }, { wxID_APPLY, "C" } },
            [](auto identity, auto name) {
                return wxUI::Button { identity, name };
            })
            .fitTo(&frame);
        CHECK(frame.dump() == kCaseVForEachWithID);
    }
    SECTION("ForEach.Range.T")
    {
        TestParent frame;
        wxUI::VForEach(
            std::vector { "A", "B", "C" } | std::views::filter([](auto str) { return str[0] == 'A'; }),
            [](auto name) {
                return wxUI::Button { name };
            })
            .fitTo(&frame);
        CHECK(frame.dump() == kCaseVForEachFiltered);
    }
    SECTION("ForEach.lvalue.Range.T")
    {
        auto data = std::vector { "A", "B", "C" } | std::views::filter([](auto str) { return str[0] == 'A'; });
        TestParent frame;
        wxUI::VForEach(data, [](auto name) {
            return wxUI::Button { name };
        }).fitTo(&frame);
        CHECK(frame.dump() == kCaseVForEachFiltered);
    }
    SECTION("ForEach.Range.Tuple")
    {
        TestParent frame;
        wxUI::VForEach(
            std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" }, { wxID_APPLY, "C" } } | std::views::filter([](auto item) { return std::get<1>(item)[0] == 'A'; }),
            [](auto name) {
                return wxUI::Button { std::get<0>(name), std::get<1>(name) };
            })
            .fitTo(&frame);
        CHECK(frame.dump() == kCaseVForEachFilteredWithID);
    }
    SECTION("ForEach.Range.Tuple.Multiarg")
    {
        TestParent frame;
        wxUI::VForEach(
            std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" }, { wxID_APPLY, "C" } } | std::views::filter([](auto item) { return std::get<1>(item)[0] == 'A'; }),
            [](auto identity, auto name) {
                return wxUI::Button { identity, name };
            })
            .fitTo(&frame);
        CHECK(frame.dump() == kCaseVForEachFilteredWithID);
    }
    SECTION("ForEach.InitializerList.T")
    {
        TestParent frame;
        wxUI::VForEach(
            { "A", "B", "C" },
            [](auto name) {
                return wxUI::Button { name };
            })
            .fitTo(&frame);
        CHECK(frame.dump() == kCaseVForEach);
    }
    SECTION("ForEach.lvalue.InitializerList.T")
    {
        auto data = { "A", "B", "C" };
        TestParent frame;
        wxUI::VForEach(data, [](auto name) {
            return wxUI::Button { name };
        }).fitTo(&frame);
        CHECK(frame.dump() == kCaseVForEach);
    }
    SECTION("ForEach.InitializerList.Tuple")
    {
        TestParent frame;
        wxUI::VForEach(
            { std::tuple<wxWindowID, std::string> { wxID_CANCEL, "A" }, std::tuple<wxWindowID, std::string> { wxID_OK, "B" }, std::tuple<wxWindowID, std::string> { wxID_APPLY, "C" } },
            [](auto name) {
                return wxUI::Button { std::get<0>(name), std::get<1>(name) };
            })
            .fitTo(&frame);
        CHECK(frame.dump() == kCaseVForEachWithID);
    }
    SECTION("ForEach.InitializerList.Tuple.Multiarg")
    {
        TestParent frame;
        wxUI::VForEach(
            { std::tuple<wxWindowID, std::string> { wxID_CANCEL, "A" }, std::tuple<wxWindowID, std::string> { wxID_OK, "B" }, std::tuple<wxWindowID, std::string> { wxID_APPLY, "C" } },
            [](auto identity, auto name) {
                return wxUI::Button { identity, name };
            })
            .fitTo(&frame);
        CHECK(frame.dump() == kCaseVForEachWithID);
    }
    SECTION("ForEach.Flags.Vector.T")
    {
        TestParent frame;
        wxUI::VForEach(
            wxSizerFlags {},
            std::vector { "A", "B", "C" },
            [](auto name) {
                return wxUI::Button { name };
            })
            .fitTo(&frame);
        CHECK(frame.dump() == kCaseVForEach);
    }
    SECTION("ForEach.Flags.lvalue.Vector.T")
    {
        auto data = std::vector { "A", "B", "C" };
        TestParent frame;
        wxUI::VForEach(
            wxSizerFlags {},
            data,
            [](auto name) {
                return wxUI::Button { name };
            })
            .fitTo(&frame);
        CHECK(frame.dump() == kCaseVForEach);
    }
    SECTION("ForEach.Flags.Vector.Tuple")
    {
        TestParent frame;
        wxUI::VForEach(
            wxSizerFlags {},
            std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" }, { wxID_APPLY, "C" } },
            [](auto name) {
                return wxUI::Button { std::get<0>(name), std::get<1>(name) };
            })
            .fitTo(&frame);
        CHECK(frame.dump() == kCaseVForEachWithID);
    }
    SECTION("ForEach.Flags.Vector.Tuple.Multiarg")
    {
        TestParent frame;
        wxUI::VForEach(
            wxSizerFlags {},
            std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" }, { wxID_APPLY, "C" } },
            [](auto identity, auto name) {
                return wxUI::Button { identity, name };
            })
            .fitTo(&frame);
        CHECK(frame.dump() == kCaseVForEachWithID);
    }
    SECTION("ForEach.Flags.Range.T")
    {
        TestParent frame;
        wxUI::VForEach(
            wxSizerFlags {},
            std::vector { "A", "B", "C" } | std::views::filter([](auto str) { return str[0] == 'A'; }),
            [](auto name) {
                return wxUI::Button { name };
            })
            .fitTo(&frame);
        CHECK(frame.dump() == kCaseVForEachFiltered);
    }
    SECTION("ForEach.Flags.lvalue.Range.T")
    {
        auto data = std::vector { "A", "B", "C" } | std::views::filter([](auto str) { return str[0] == 'A'; });
        TestParent frame;
        wxUI::VForEach(
            wxSizerFlags {},
            data,
            [](auto name) {
                return wxUI::Button { name };
            })
            .fitTo(&frame);
        CHECK(frame.dump() == kCaseVForEachFiltered);
    }
    SECTION("ForEach.Flags.Range.Tuple")
    {
        TestParent frame;
        wxUI::VForEach(
            wxSizerFlags {},
            std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" }, { wxID_APPLY, "C" } } | std::views::filter([](auto item) { return std::get<1>(item)[0] == 'A'; }),
            [](auto name) {
                return wxUI::Button { std::get<0>(name), std::get<1>(name) };
            })
            .fitTo(&frame);
        CHECK(frame.dump() == kCaseVForEachFilteredWithID);
    }
    SECTION("ForEach.Flags.Range.Tuple.Multiarg")
    {
        TestParent frame;
        wxUI::VForEach(
            wxSizerFlags {},
            std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" }, { wxID_APPLY, "C" } } | std::views::filter([](auto item) { return std::get<1>(item)[0] == 'A'; }),
            [](auto identity, auto name) {
                return wxUI::Button { identity, name };
            })
            .fitTo(&frame);
        CHECK(frame.dump() == kCaseVForEachFilteredWithID);
    }
    SECTION("ForEach.Flags.InitializerList.T")
    {
        TestParent frame;
        wxUI::VForEach(
            wxSizerFlags {},
            { "A", "B", "C" },
            [](auto name) {
                return wxUI::Button { name };
            })
            .fitTo(&frame);
        CHECK(frame.dump() == kCaseVForEach);
    }
    SECTION("ForEach.Flags.lvalue.InitializerList.T")
    {
        auto data = { "A", "B", "C" };
        TestParent frame;
        wxUI::VForEach(
            wxSizerFlags {},
            data,
            [](auto name) {
                return wxUI::Button { name };
            })
            .fitTo(&frame);
        CHECK(frame.dump() == kCaseVForEach);
    }
    SECTION("ForEach.Flags.InitializerList.Tuple")
    {
        TestParent frame;
        wxUI::VForEach(
            wxSizerFlags {},
            { std::tuple<wxWindowID, std::string> { wxID_CANCEL, "A" }, std::tuple<wxWindowID, std::string> { wxID_OK, "B" }, std::tuple<wxWindowID, std::string> { wxID_APPLY, "C" } },
            [](auto name) {
                return wxUI::Button { std::get<0>(name), std::get<1>(name) };
            })
            .fitTo(&frame);
        CHECK(frame.dump() == kCaseVForEachWithID);
    }
    SECTION("ForEach.Flags.InitializerList.Tuple.Multiarg")
    {
        TestParent frame;
        wxUI::VForEach(
            wxSizerFlags {},
            { std::tuple<wxWindowID, std::string> { wxID_CANCEL, "A" }, std::tuple<wxWindowID, std::string> { wxID_OK, "B" }, std::tuple<wxWindowID, std::string> { wxID_APPLY, "C" } },
            [](auto identity, auto name) {
                return wxUI::Button { identity, name };
            })
            .fitTo(&frame);
        CHECK(frame.dump() == kCaseVForEachWithID);
    }
}

TEST_CASE("ForEach - Move-only lambda support")
{
    TestParent provider;

    SECTION("ForEach with move-only lambda (unique_ptr capture)")
    {
        auto resource = std::make_unique<int>(42);

        wxUI::VForEach({ "A", "B", "C" }, [resource = std::move(resource)](auto const& name) {
            CHECK(resource != nullptr);
            CHECK(*resource == 42);
            return wxUI::Button { name };
        }).fitTo(&provider);

        auto dump = provider.dump();
        CHECK(dump.size() > 0);
    }

    SECTION("HForEach with move-only lambda")
    {
        auto data = std::make_unique<std::string>("test");

        wxUI::HForEach({ "X", "Y" }, [data = std::move(data)](auto const& name) {
            CHECK(data != nullptr);
            CHECK(*data == "test");
            return wxUI::Button { name };
        }).fitTo(&provider);

        auto dump = provider.dump();
        CHECK(dump.size() > 0);
    }
}

TEST_CASE("ForEach - Mutable lambda support")
{
    TestParent provider;

    SECTION("VForEach with mutable lambda")
    {
        int counter = 0;

        wxUI::VForEach({ "A", "B", "C" }, [counter](auto const& name) mutable {
            counter++;
            CHECK(counter <= 3);
            return wxUI::Button { name };
        }).fitTo(&provider);

        auto dump = provider.dump();
        CHECK(dump.size() > 0);
    }

    SECTION("HForEach with mutable lambda modifying captured state")
    {
        int value = 0;

        wxUI::HForEach({ "X", "Y" }, [value](auto const& name) mutable {
            value += 10;
            CHECK(value <= 20);
            return wxUI::Button { name };
        }).fitTo(&provider);

        auto dump = provider.dump();
        CHECK(dump.size() > 0);
    }
}

TEST_CASE("ForEach - Move-only mutable lambda")
{
    TestParent provider;

    SECTION("ForEach with move-only mutable lambda")
    {
        auto counter = std::make_unique<int>(0);

        wxUI::VForEach({ "A", "B" }, [counter = std::move(counter)](auto const& name) mutable {
            CHECK(counter != nullptr);
            (*counter)++;
            CHECK(*counter <= 2);
            return wxUI::Button { name };
        }).fitTo(&provider);

        auto dump = provider.dump();
        CHECK(dump.size() > 0);
    }
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity)
