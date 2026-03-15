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
#include <wx/button.h>
#include <wxUI/Factory.hpp>
#include <wxUI/Layout.hpp>

#include <wx/wx.h>

using namespace wxUITests;

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity)
TEST_CASE("Factory")
{
    TestParent provider;
    SECTION("window.Factory")
    {
        wxUI::VSizer {
            wxUI::Factory {
                [](auto* parent) { return wxUI::customizations::ParentCreate<wxButton>(parent, wxID_ANY, std::string { "Factory" }, wxDefaultPosition, wxDefaultSize, int64_t {}); } }
        }
            .fitTo(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:Sizer[orientation=wxVERTICAL]",
                  "Create:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"Factory\"]",
                  "topsizer:Sizer[orientation=wxVERTICAL]",
                  "controller:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"Factory\"]",
                  "sizer:Sizer[orientation=wxVERTICAL]",
                  "Add:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"Factory\"]:flags:(0,0x0,0)",
                  "SetSizeHints:[id=0, pos=(0,0), size=(0,0), style=0]",
              });
    }

    SECTION("flags.window.Factory")
    {
        wxUI::VSizer {
            wxUI::Factory {
                wxSizerFlags(1), [](auto* parent) { return wxUI::customizations::ParentCreate<wxButton>(parent, wxID_ANY, std::string { "Factory" }, wxDefaultPosition, wxDefaultSize, int64_t {}); } }
        }
            .fitTo(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:Sizer[orientation=wxVERTICAL]",
                  "Create:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"Factory\"]",
                  "topsizer:Sizer[orientation=wxVERTICAL]",
                  "controller:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"Factory\"]",
                  "sizer:Sizer[orientation=wxVERTICAL]",
                  "Add:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"Factory\"]:flags:(1,0x0,0)",
                  "SetSizeHints:[id=0, pos=(0,0), size=(0,0), style=0]",
              });
    }
}

TEST_CASE("Factory - Move-only lambda support")
{
    TestParent provider;

    SECTION("Factory with move-only lambda (unique_ptr capture)")
    {
        auto resource = std::make_unique<std::string>("test");

        wxUI::VSizer {
            wxUI::Factory {
                [resource = std::move(resource)](auto* parent) {
                    CHECK(resource != nullptr);
                    CHECK(*resource == "test");
                    return wxUI::customizations::ParentCreate<wxButton>(parent, wxID_ANY, std::string { "Factory" }, wxDefaultPosition, wxDefaultSize, int64_t {});
                } }
        }
            .fitTo(&provider);

        auto dump = provider.dump();
        CHECK(dump.size() > 0);
    }

    SECTION("Factory with flags and move-only lambda")
    {
        auto data = std::make_unique<int>(42);

        wxUI::VSizer {
            wxUI::Factory {
                wxSizerFlags(1),
                [data = std::move(data)](auto* parent) {
                    CHECK(data != nullptr);
                    CHECK(*data == 42);
                    return wxUI::customizations::ParentCreate<wxButton>(parent, wxID_ANY, std::string { "Factory" }, wxDefaultPosition, wxDefaultSize, int64_t {});
                } }
        }
            .fitTo(&provider);

        auto dump = provider.dump();
        CHECK(dump.size() > 0);
    }
}

TEST_CASE("Factory - Mutable lambda support")
{
    TestParent provider;

    SECTION("Factory with mutable lambda")
    {
        int counter = 0;

        wxUI::VSizer {
            wxUI::Factory {
                [counter](auto* parent) mutable {
                    counter++;
                    CHECK(counter == 1);
                    return wxUI::customizations::ParentCreate<wxButton>(parent, wxID_ANY, std::string { "Factory" }, wxDefaultPosition, wxDefaultSize, int64_t {});
                } }
        }
            .fitTo(&provider);

        auto dump = provider.dump();
        CHECK(dump.size() > 0);
    }

    SECTION("Factory with move-only mutable lambda")
    {
        auto value = std::make_unique<int>(10);

        wxUI::VSizer {
            wxUI::Factory {
                [value = std::move(value)](auto* parent) mutable {
                    CHECK(value != nullptr);
                    *value += 5;
                    CHECK(*value == 15);
                    return wxUI::customizations::ParentCreate<wxButton>(parent, wxID_ANY, std::string { "Factory" }, wxDefaultPosition, wxDefaultSize, int64_t {});
                } }
        }
            .fitTo(&provider);

        auto dump = provider.dump();
        CHECK(dump.size() > 0);
    }
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity)
