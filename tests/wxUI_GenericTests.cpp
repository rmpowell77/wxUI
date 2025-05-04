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
#include <catch2/catch_test_macros.hpp>
#include <wx/button.h>
#include <wxUI/Generic.h>

#include <wx/wx.h>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity)

TEST_CASE("Generic")
{
    SECTION("flags.window.generic")
    {
        // using type = wxWindow;
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto* button = new wxButton(&frame, wxID_ANY, "Generic");
        auto uut = wxUI::Generic { wxSizerFlags(1), button };
        [[maybe_unused]] wxWindow* window = uut.create(&frame);
    }

    SECTION("window.generic")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto* button = new wxButton(&frame, wxID_ANY, "Generic");
        auto uut = wxUI::Generic { button };
        [[maybe_unused]] wxWindow* window = uut.create(&frame);
    }

    SECTION("flags.func.generic")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = wxUI::Generic { wxSizerFlags(1), [](wxWindow* parent) {
                                      return new wxButton(parent, wxID_ANY, "Generic");
                                  } };
        [[maybe_unused]] wxWindow* window = uut.create(&frame);
    }

    SECTION("func.generic")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = wxUI::Generic { [](wxWindow* parent) {
            return new wxButton(parent, wxID_ANY, "Generic");
        } };
        [[maybe_unused]] wxWindow* window = uut.create(&frame);
    }

    SECTION("flags.window")
    {
        using Type = wxButton;
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto* button = new wxButton(&frame, wxID_ANY, "Generic");
        auto uut = wxUI::Generic<Type> { wxSizerFlags(1), button };
        [[maybe_unused]] Type* window = uut.create(&frame);
    }

    SECTION("window")
    {
        using Type = wxButton;
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto* button = new wxButton(&frame, wxID_ANY, "Generic");
        auto uut = wxUI::Generic<Type> { button };
        [[maybe_unused]] Type* window = uut.create(&frame);
    }

    SECTION("flags.func")
    {
        using Type = wxButton;
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = wxUI::Generic<Type> { wxSizerFlags(1), [](wxWindow* parent) {
                                            return new wxButton(parent, wxID_ANY, "Generic");
                                        } };
        [[maybe_unused]] Type* window = uut.create(&frame);
    }

    SECTION("func")
    {
        using Type = wxButton;
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = wxUI::Generic<Type> { [](wxWindow* parent) {
            return new wxButton(parent, wxID_ANY, "Generic");
        } };
        [[maybe_unused]] Type* window = uut.create(&frame);
    }
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity)
