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
        wxUI::VSizer {
            wxUI::ForEach {
                std::vector { "A", "B", "C" },
                [](auto name) {
                    return wxUI::Button { name };
                } },
        }
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
        auto builder = [](auto name) {
            return wxUI::Button { name };
        };
        {
            TestParent frame;
            wxUI::VSizer {
                wxUI::ForEach {
                    data,
                    [](auto name) {
                        return wxUI::Button { name };
                    } },
            }
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEach);
        }
        {
            TestParent frame;
            wxUI::VForEach(
                data,
                [](auto name) {
                    return wxUI::Button { name };
                })
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEach);
        }
        {
            TestParent frame;
            wxUI::HForEach(
                data,
                [](auto name) {
                    return wxUI::Button { name };
                })
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseHForEach);
        }
        {
            TestParent frame;
            wxUI::VSizer {
                wxUI::ForEach {
                    std::vector { "A", "B", "C" },
                    builder }
            }
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEach);
        }
        {
            TestParent frame;
            wxUI::VForEach(
                std::vector { "A", "B", "C" },
                builder)
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEach);
        }
        {
            TestParent frame;
            wxUI::HForEach(
                std::vector { "A", "B", "C" },
                builder)
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseHForEach);
        }
        {
            TestParent frame;
            wxUI::VSizer { wxUI::ForEach { data, builder } }.fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEach);
        }
        {
            TestParent frame;
            wxUI::VForEach(data, builder).fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEach);
        }
        {
            TestParent frame;
            wxUI::HForEach(data, builder).fitTo(&frame);
            CHECK(frame.dump() == kCaseHForEach);
        }
    }
    SECTION("ForEach.Vector.Tuple")
    {
        {
            TestParent frame;
            wxUI::VSizer { wxUI::ForEach {
                               std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" }, { wxID_APPLY, "C" } },
                               [](auto name) {
                                   return wxUI::Button { std::get<0>(name), std::get<1>(name) };
                               } } }
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEachWithID);
        }
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
        {
            TestParent frame;
            wxUI::HForEach(
                std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" }, { wxID_APPLY, "C" } },
                [](auto name) {
                    return wxUI::Button { std::get<0>(name), std::get<1>(name) };
                })
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseHForEachWithID);
        }
    }
    SECTION("ForEach.Vector.Tuple.Multiarg")
    {
        {
            TestParent frame;
            wxUI::VSizer { wxUI::ForEach {
                               std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" }, { wxID_APPLY, "C" } },
                               [](auto identity, auto name) {
                                   return wxUI::Button { identity, name };
                               } } }
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEachWithID);
        }
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
        {
            TestParent frame;
            wxUI::HForEach(
                std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" }, { wxID_APPLY, "C" } },
                [](auto identity, auto name) {
                    return wxUI::Button { identity, name };
                })
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseHForEachWithID);
        }
    }
    SECTION("ForEach.Range.T")
    {
        {
            TestParent frame;
            wxUI::VSizer { wxUI::ForEach {
                               std::vector { "A", "B", "C" } | std::views::filter([](auto str) { return str[0] == 'A'; }),
                               [](auto name) {
                                   return wxUI::Button { name };
                               } } }
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEachFiltered);
        }
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
        {
            TestParent frame;
            wxUI::HForEach(
                std::vector { "A", "B", "C" } | std::views::filter([](auto str) { return str[0] == 'A'; }),
                [](auto name) {
                    return wxUI::Button { name };
                })
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseHForEachFiltered);
        }
    }
    SECTION("ForEach.lvalue.Range.T")
    {
        auto data = std::vector { "A", "B", "C" } | std::views::filter([](auto str) { return str[0] == 'A'; });
        auto builder = [](auto name) {
            return wxUI::Button { name };
        };
        {
            TestParent frame;
            wxUI::VSizer { wxUI::ForEach {
                               data,
                               [](auto name) {
                                   return wxUI::Button { name };
                               } } }
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEachFiltered);
        }
        {
            TestParent frame;
            wxUI::VForEach(
                data,
                [](auto name) {
                    return wxUI::Button { name };
                })
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEachFiltered);
        }
        {
            TestParent frame;
            wxUI::HForEach(
                data,
                [](auto name) {
                    return wxUI::Button { name };
                })
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseHForEachFiltered);
        }
        {
            TestParent frame;
            wxUI::VSizer { wxUI::ForEach {
                               std::vector { "A", "B", "C" },
                               builder } }
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEach);
        }
        {
            TestParent frame;
            wxUI::VForEach(
                std::vector { "A", "B", "C" },
                builder)
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEach);
        }
        {
            TestParent frame;
            wxUI::HForEach(
                std::vector { "A", "B", "C" },
                builder)
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseHForEach);
        }
        {
            TestParent frame;
            wxUI::VSizer { wxUI::ForEach { data, builder } }.fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEachFiltered);
        }
        {
            TestParent frame;
            wxUI::VForEach(data, builder).fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEachFiltered);
        }
        {
            TestParent frame;
            wxUI::HForEach(data, builder).fitTo(&frame);
            CHECK(frame.dump() == kCaseHForEachFiltered);
        }
    }
    SECTION("ForEach.Range.Tuple")
    {
        {
            TestParent frame;
            wxUI::VSizer { wxUI::ForEach {
                               std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" }, { wxID_APPLY, "C" } } | std::views::filter([](auto item) { return std::get<1>(item)[0] == 'A'; }),
                               [](auto name) {
                                   return wxUI::Button { std::get<0>(name), std::get<1>(name) };
                               } } }
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEachFilteredWithID);
        }
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
        {
            TestParent frame;
            wxUI::HForEach(
                std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" }, { wxID_APPLY, "C" } } | std::views::filter([](auto item) { return std::get<1>(item)[0] == 'A'; }),
                [](auto name) {
                    return wxUI::Button { std::get<0>(name), std::get<1>(name) };
                })
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseHForEachFilteredWithID);
        }
    }
    SECTION("ForEach.Range.Tuple.Multiarg")
    {
        {
            TestParent frame;
            wxUI::VSizer { wxUI::ForEach {
                               std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" }, { wxID_APPLY, "C" } } | std::views::filter([](auto item) { return std::get<1>(item)[0] == 'A'; }),
                               [](auto identity, auto name) {
                                   return wxUI::Button { identity, name };
                               } } }
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEachFilteredWithID);
        }
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
        {
            TestParent frame;
            wxUI::HForEach(
                std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" }, { wxID_APPLY, "C" } } | std::views::filter([](auto item) { return std::get<1>(item)[0] == 'A'; }),
                [](auto identity, auto name) {
                    return wxUI::Button { identity, name };
                })
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseHForEachFilteredWithID);
        }
    }
    SECTION("ForEach.InitializerList.T")
    {
        {
            TestParent frame;
            wxUI::VSizer { wxUI::ForEach {
                               { "A", "B", "C" },
                               [](auto name) {
                                   return wxUI::Button { name };
                               } } }
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEach);
        }
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
        {
            TestParent frame;
            wxUI::HForEach(
                { "A", "B", "C" },
                [](auto name) {
                    return wxUI::Button { name };
                })
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseHForEach);
        }
    }
    SECTION("ForEach.lvalue.InitializerList.T")
    {
        auto data = { "A", "B", "C" };
        auto builder = [](auto name) {
            return wxUI::Button { name };
        };
        {
            TestParent frame;
            wxUI::VSizer { wxUI::ForEach {
                               data,
                               [](auto name) {
                                   return wxUI::Button { name };
                               } } }
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEach);
        }
        {
            TestParent frame;
            wxUI::VForEach(
                data,
                [](auto name) {
                    return wxUI::Button { name };
                })
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEach);
        }
        {
            TestParent frame;
            wxUI::HForEach(
                data,
                [](auto name) {
                    return wxUI::Button { name };
                })
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseHForEach);
        }
        {
            TestParent frame;
            wxUI::VSizer { wxUI::ForEach {
                               std::vector { "A", "B", "C" },
                               builder } }
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEach);
        }
        {
            TestParent frame;
            wxUI::VForEach(
                std::vector { "A", "B", "C" },
                builder)
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEach);
        }
        {
            TestParent frame;
            wxUI::HForEach(
                std::vector { "A", "B", "C" },
                builder)
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseHForEach);
        }
        {
            TestParent frame;
            wxUI::VSizer { wxUI::ForEach { data, builder } }.fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEach);
        }
        {
            TestParent frame;
            wxUI::VForEach(data, builder).fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEach);
        }
        {
            TestParent frame;
            wxUI::HForEach(data, builder).fitTo(&frame);
            CHECK(frame.dump() == kCaseHForEach);
        }
    }
    SECTION("ForEach.InitializerList.Tuple")
    {
        {
            TestParent frame;
            wxUI::VSizer { wxUI::ForEach {
                               { std::tuple<wxWindowID, std::string> { wxID_CANCEL, "A" }, std::tuple<wxWindowID, std::string> { wxID_OK, "B" }, std::tuple<wxWindowID, std::string> { wxID_APPLY, "C" } },
                               [](auto name) {
                                   return wxUI::Button { std::get<0>(name), std::get<1>(name) };
                               } } }
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEachWithID);
        }
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
        {
            TestParent frame;
            wxUI::HForEach(
                { std::tuple<wxWindowID, std::string> { wxID_CANCEL, "A" }, std::tuple<wxWindowID, std::string> { wxID_OK, "B" }, std::tuple<wxWindowID, std::string> { wxID_APPLY, "C" } },
                [](auto name) {
                    return wxUI::Button { std::get<0>(name), std::get<1>(name) };
                })
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseHForEachWithID);
        }
    }
    SECTION("ForEach.InitializerList.Tuple.Multiarg")
    {
        {
            TestParent frame;
            wxUI::VSizer { wxUI::ForEach {
                               { std::tuple<wxWindowID, std::string> { wxID_CANCEL, "A" }, std::tuple<wxWindowID, std::string> { wxID_OK, "B" }, std::tuple<wxWindowID, std::string> { wxID_APPLY, "C" } },
                               [](auto identity, auto name) {
                                   return wxUI::Button { identity, name };
                               } } }
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEachWithID);
        }
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
        {
            TestParent frame;
            wxUI::HForEach(
                { std::tuple<wxWindowID, std::string> { wxID_CANCEL, "A" }, std::tuple<wxWindowID, std::string> { wxID_OK, "B" }, std::tuple<wxWindowID, std::string> { wxID_APPLY, "C" } },
                [](auto identity, auto name) {
                    return wxUI::Button { identity, name };
                })
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseHForEachWithID);
        }
    }
    SECTION("ForEach.Flags.Vector.T")
    {
        {
            TestParent frame;
            wxUI::VSizer { wxUI::ForEach {
                               wxSizerFlags {},
                               std::vector { "A", "B", "C" },
                               [](auto name) {
                                   return wxUI::Button { name };
                               } } }
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEach);
        }
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
        {
            TestParent frame;
            wxUI::HForEach(
                wxSizerFlags {},
                std::vector { "A", "B", "C" },
                [](auto name) {
                    return wxUI::Button { name };
                })
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseHForEach);
        }
    }
    SECTION("ForEach.Flags.lvalue.Vector.T")
    {
        auto data = std::vector { "A", "B", "C" };
        auto builder = [](auto name) {
            return wxUI::Button { name };
        };
        {
            TestParent frame;
            wxUI::VSizer { wxUI::ForEach {
                               wxSizerFlags {},
                               data,
                               [](auto name) {
                                   return wxUI::Button { name };
                               } } }
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEach);
        }
        {
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
        {
            TestParent frame;
            wxUI::HForEach(
                wxSizerFlags {},
                data,
                [](auto name) {
                    return wxUI::Button { name };
                })
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseHForEach);
        }
        {
            TestParent frame;
            wxUI::VSizer { wxUI::ForEach {
                               wxSizerFlags {},
                               std::vector { "A", "B", "C" },
                               builder } }
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEach);
        }
        {
            TestParent frame;
            wxUI::VForEach(
                wxSizerFlags {},
                std::vector { "A", "B", "C" },
                builder)
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEach);
        }
        {
            TestParent frame;
            wxUI::HForEach(
                wxSizerFlags {},
                std::vector { "A", "B", "C" },
                builder)
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseHForEach);
        }
        {
            TestParent frame;
            wxUI::VSizer { wxUI::ForEach { wxSizerFlags {}, data, builder } }.fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEach);
        }
        {
            TestParent frame;
            wxUI::VForEach(wxSizerFlags {}, data, builder).fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEach);
        }
        {
            TestParent frame;
            wxUI::HForEach(wxSizerFlags {}, data, builder).fitTo(&frame);
            CHECK(frame.dump() == kCaseHForEach);
        }
    }
    SECTION("ForEach.Flags.Vector.Tuple")
    {
        {
            TestParent frame;
            wxUI::VSizer { wxUI::ForEach {
                               wxSizerFlags {},
                               std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" }, { wxID_APPLY, "C" } },
                               [](auto name) {
                                   return wxUI::Button { std::get<0>(name), std::get<1>(name) };
                               } } }
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEachWithID);
        }
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
        {
            TestParent frame;
            wxUI::HForEach(
                wxSizerFlags {},
                std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" }, { wxID_APPLY, "C" } },
                [](auto name) {
                    return wxUI::Button { std::get<0>(name), std::get<1>(name) };
                })
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseHForEachWithID);
        }
    }
    SECTION("ForEach.Flags.Vector.Tuple.Multiarg")
    {
        {
            TestParent frame;
            wxUI::VSizer { wxUI::ForEach {
                               wxSizerFlags {},
                               std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" }, { wxID_APPLY, "C" } },
                               [](auto identity, auto name) {
                                   return wxUI::Button { identity, name };
                               } } }
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEachWithID);
        }
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
        {
            TestParent frame;
            wxUI::HForEach(
                wxSizerFlags {},
                std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" }, { wxID_APPLY, "C" } },
                [](auto identity, auto name) {
                    return wxUI::Button { identity, name };
                })
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseHForEachWithID);
        }
    }
    SECTION("ForEach.Flags.Range.T")
    {
        {
            TestParent frame;
            wxUI::VSizer { wxUI::ForEach {
                               wxSizerFlags {},
                               std::vector { "A", "B", "C" } | std::views::filter([](auto str) { return str[0] == 'A'; }),
                               [](auto name) {
                                   return wxUI::Button { name };
                               } } }
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEachFiltered);
        }
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
        {
            TestParent frame;
            wxUI::HForEach(
                wxSizerFlags {},
                std::vector { "A", "B", "C" } | std::views::filter([](auto str) { return str[0] == 'A'; }),
                [](auto name) {
                    return wxUI::Button { name };
                })
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseHForEachFiltered);
        }
    }
    SECTION("ForEach.Flags.lvalue.Range.T")
    {
        auto data = std::vector { "A", "B", "C" } | std::views::filter([](auto str) { return str[0] == 'A'; });
        auto builder = [](auto name) {
            return wxUI::Button { name };
        };
        {
            TestParent frame;
            wxUI::VSizer { wxUI::ForEach {
                               wxSizerFlags {},
                               data,
                               [](auto name) {
                                   return wxUI::Button { name };
                               } } }
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEachFiltered);
        }
        {
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
        {
            TestParent frame;
            wxUI::HForEach(
                wxSizerFlags {},
                data,
                [](auto name) {
                    return wxUI::Button { name };
                })
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseHForEachFiltered);
        }
        {
            TestParent frame;
            wxUI::VSizer { wxUI::ForEach {
                               wxSizerFlags {},
                               std::vector { "A", "B", "C" },
                               builder } }
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEach);
        }
        {
            TestParent frame;
            wxUI::VForEach(
                wxSizerFlags {},
                std::vector { "A", "B", "C" },
                builder)
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEach);
        }
        {
            TestParent frame;
            wxUI::HForEach(
                wxSizerFlags {},
                std::vector { "A", "B", "C" },
                builder)
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseHForEach);
        }
        {
            TestParent frame;
            wxUI::VSizer { wxUI::ForEach { wxSizerFlags {}, data, builder } }.fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEachFiltered);
        }
        {
            TestParent frame;
            wxUI::VForEach(wxSizerFlags {}, data, builder).fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEachFiltered);
        }
        {
            TestParent frame;
            wxUI::HForEach(wxSizerFlags {}, data, builder).fitTo(&frame);
            CHECK(frame.dump() == kCaseHForEachFiltered);
        }
    }
    SECTION("ForEach.Flags.Range.Tuple")
    {
        {
            TestParent frame;
            wxUI::VSizer { wxUI::ForEach {
                               wxSizerFlags {},
                               std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" }, { wxID_APPLY, "C" } } | std::views::filter([](auto item) { return std::get<1>(item)[0] == 'A'; }),
                               [](auto name) {
                                   return wxUI::Button { std::get<0>(name), std::get<1>(name) };
                               } } }
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEachFilteredWithID);
        }
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
        {
            TestParent frame;
            wxUI::HForEach(
                wxSizerFlags {},
                std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" }, { wxID_APPLY, "C" } } | std::views::filter([](auto item) { return std::get<1>(item)[0] == 'A'; }),
                [](auto name) {
                    return wxUI::Button { std::get<0>(name), std::get<1>(name) };
                })
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseHForEachFilteredWithID);
        }
    }
    SECTION("ForEach.Flags.Range.Tuple.Multiarg")
    {
        {
            TestParent frame;
            wxUI::VSizer { wxUI::ForEach {
                               wxSizerFlags {},
                               std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" }, { wxID_APPLY, "C" } } | std::views::filter([](auto item) { return std::get<1>(item)[0] == 'A'; }),
                               [](auto identity, auto name) {
                                   return wxUI::Button { identity, name };
                               } } }
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEachFilteredWithID);
        }
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
        {
            TestParent frame;
            wxUI::HForEach(
                wxSizerFlags {},
                std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" }, { wxID_APPLY, "C" } } | std::views::filter([](auto item) { return std::get<1>(item)[0] == 'A'; }),
                [](auto identity, auto name) {
                    return wxUI::Button { identity, name };
                })
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseHForEachFilteredWithID);
        }
    }
    SECTION("ForEach.Flags.InitializerList.T")
    {
        {
            TestParent frame;
            wxUI::VSizer { wxUI::ForEach {
                               wxSizerFlags {},
                               { "A", "B", "C" },
                               [](auto name) {
                                   return wxUI::Button { name };
                               } } }
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEach);
        }
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
        {
            TestParent frame;
            wxUI::HForEach(
                wxSizerFlags {},
                { "A", "B", "C" },
                [](auto name) {
                    return wxUI::Button { name };
                })
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseHForEach);
        }
    }
    SECTION("ForEach.Flags.lvalue.InitializerList.T")
    {
        auto data = { "A", "B", "C" };
        auto builder = [](auto name) {
            return wxUI::Button { name };
        };
        {
            TestParent frame;
            wxUI::VSizer { wxUI::ForEach {
                               wxSizerFlags {},
                               data,
                               [](auto name) {
                                   return wxUI::Button { name };
                               } } }
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEach);
        }
        {
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
        {
            TestParent frame;
            wxUI::HForEach(
                wxSizerFlags {},
                data,
                [](auto name) {
                    return wxUI::Button { name };
                })
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseHForEach);
        }
        {
            TestParent frame;
            wxUI::VSizer { wxUI::ForEach {
                               wxSizerFlags {},
                               std::vector { "A", "B", "C" },
                               builder } }
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEach);
        }
        {
            TestParent frame;
            wxUI::VForEach(
                wxSizerFlags {},
                std::vector { "A", "B", "C" },
                builder)
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEach);
        }
        {
            TestParent frame;
            wxUI::HForEach(
                wxSizerFlags {},
                std::vector { "A", "B", "C" },
                builder)
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseHForEach);
        }
        {
            TestParent frame;
            wxUI::VSizer { wxUI::ForEach { wxSizerFlags {}, data, builder } }.fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEach);
        }
        {
            TestParent frame;
            wxUI::VForEach(wxSizerFlags {}, data, builder).fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEach);
        }
        {
            TestParent frame;
            wxUI::HForEach(wxSizerFlags {}, data, builder).fitTo(&frame);
            CHECK(frame.dump() == kCaseHForEach);
        }
    }
    SECTION("ForEach.Flags.InitializerList.Tuple")
    {
        {
            TestParent frame;
            wxUI::VSizer { wxUI::ForEach {
                               wxSizerFlags {},
                               { std::tuple<wxWindowID, std::string> { wxID_CANCEL, "A" }, std::tuple<wxWindowID, std::string> { wxID_OK, "B" }, std::tuple<wxWindowID, std::string> { wxID_APPLY, "C" } },
                               [](auto name) {
                                   return wxUI::Button { std::get<0>(name), std::get<1>(name) };
                               } } }
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEachWithID);
        }
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
        {
            TestParent frame;
            wxUI::HForEach(
                wxSizerFlags {},
                { std::tuple<wxWindowID, std::string> { wxID_CANCEL, "A" }, std::tuple<wxWindowID, std::string> { wxID_OK, "B" }, std::tuple<wxWindowID, std::string> { wxID_APPLY, "C" } },
                [](auto name) {
                    return wxUI::Button { std::get<0>(name), std::get<1>(name) };
                })
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseHForEachWithID);
        }
    }
    SECTION("ForEach.Flags.InitializerList.Tuple.Multiarg")
    {
        {
            TestParent frame;
            wxUI::VSizer { wxUI::ForEach {
                               wxSizerFlags {},
                               { std::tuple<wxWindowID, std::string> { wxID_CANCEL, "A" }, std::tuple<wxWindowID, std::string> { wxID_OK, "B" }, std::tuple<wxWindowID, std::string> { wxID_APPLY, "C" } },
                               [](auto identity, auto name) {
                                   return wxUI::Button { identity, name };
                               } } }
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseVForEachWithID);
        }
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
        {
            TestParent frame;
            wxUI::HForEach(
                wxSizerFlags {},
                { std::tuple<wxWindowID, std::string> { wxID_CANCEL, "A" }, std::tuple<wxWindowID, std::string> { wxID_OK, "B" }, std::tuple<wxWindowID, std::string> { wxID_APPLY, "C" } },
                [](auto identity, auto name) {
                    return wxUI::Button { identity, name };
                })
                .fitTo(&frame);
            CHECK(frame.dump() == kCaseHForEachWithID);
        }
    }
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity)
