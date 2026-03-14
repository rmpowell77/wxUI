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
#include <wxUI/Custom.hpp>
#include <wxUI/Layout.hpp>

#include <wx/wx.h>

using namespace wxUITests;

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity)

TEST_CASE("Custom")
{
    TestParent provider;

    SECTION("Custom with simple lambda")
    {
        bool called = false;
        wxUI::VSizer {
            wxUI::Custom {
                [&called](auto* /*parent*/, auto* /*sizer*/, auto const& /*flags*/) {
                    called = true;
                } }
        }
            .fitTo(&provider);

        CHECK(called);
    }

    SECTION("Custom with flags")
    {
        bool called = false;
        wxUI::VSizer {
            wxUI::Custom {
                wxSizerFlags(1),
                [&called](auto* /*parent*/, auto* /*sizer*/, auto const& /*flags*/) {
                    called = true;
                } }
        }
            .fitTo(&provider);

        CHECK(called);
    }
}

TEST_CASE("Custom - Move-only lambda support")
{
    TestParent provider;

    SECTION("Custom with move-only lambda (unique_ptr capture)")
    {
        auto resource = std::make_unique<int>(42);
        bool called = false;

        wxUI::VSizer {
            wxUI::Custom {
                [resource = std::move(resource), &called](auto* /*parent*/, auto* /*sizer*/, auto const& /*flags*/) mutable {
                    // Verify we have the resource
                    CHECK(resource != nullptr);
                    CHECK(*resource == 42);
                    called = true;
                } }
        }
            .fitTo(&provider);

        CHECK(called);
    }

    SECTION("Custom with flags and move-only lambda")
    {
        auto resource = std::make_unique<std::string>("test");
        bool called = false;

        wxUI::VSizer {
            wxUI::Custom {
                wxSizerFlags(1),
                [resource = std::move(resource), &called](auto* /*parent*/, auto* /*sizer*/, auto const& /*flags*/) mutable {
                    CHECK(resource != nullptr);
                    CHECK(*resource == "test");
                    called = true;
                } }
        }
            .fitTo(&provider);

        CHECK(called);
    }
}

TEST_CASE("Custom - Mutable lambda support")
{
    TestParent provider;

    SECTION("Custom with mutable lambda")
    {
        int callCount = 0;

        wxUI::VSizer {
            wxUI::Custom {
                [callCount](auto* /*parent*/, auto* /*sizer*/, auto const& /*flags*/) mutable {
                    callCount++;
                    CHECK(callCount == 1);
                } }
        }
            .fitTo(&provider);
    }

    SECTION("Custom with flags and mutable lambda")
    {
        int value = 10;
        bool called = false;

        wxUI::VSizer {
            wxUI::Custom {
                wxSizerFlags(1),
                [value, &called](auto* /*parent*/, auto* /*sizer*/, auto const& /*flags*/) mutable {
                    value += 5;
                    CHECK(value == 15);
                    called = true;
                } }
        }
            .fitTo(&provider);

        CHECK(called);
    }
}

TEST_CASE("Custom - Lambda with mutable captured unique_ptr")
{
    TestParent provider;

    SECTION("Move-only mutable lambda")
    {
        auto data = std::make_unique<int>(100);
        bool called = false;

        wxUI::VSizer {
            wxUI::Custom {
                [data = std::move(data), &called](auto* /*parent*/, auto* /*sizer*/, auto const& /*flags*/) mutable {
                    CHECK(data != nullptr);
                    *data += 50;
                    CHECK(*data == 150);
                    called = true;
                } }
        }
            .fitTo(&provider);

        CHECK(called);
    }
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity)
