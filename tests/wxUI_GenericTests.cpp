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
#include "TestCustomizations.hpp"
#include <catch2/catch_test_macros.hpp>
#include <wx/button.h>
#include <wxUI/Generic.hpp>
#include <wxUI/Layout.hpp>

#include <wx/wx.h>

using namespace wxUITests;

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity)

TEST_CASE("Generic")
{
    TestProvider provider;
    auto* button = wxUI::customizations::ParentCreate<wxButton>(&provider, wxID_ANY, std::string { "Generic" }, wxDefaultPosition, wxDefaultSize, int64_t {});
    using Type = std::remove_pointer_t<decltype(button)>;
    SECTION("flags.window.generic")
    {
        wxUI::VSizer { wxUI::Generic { wxSizerFlags(1), button } }.fitTo(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "topsizer:Sizer[orientation=wxVERTICAL]",
                  "controller:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"Generic\"]",
                  "sizer:Sizer[orientation=wxVERTICAL]",
                  "Add:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"Generic\"]:flags:(1,0x0,0)",
                  "SetSizeHints:[id=0, pos=(0,0), size=(0,0), style=0]",
              });
    }

    SECTION("window.generic")
    {
        wxUI::VSizer { wxUI::Generic { button } }.fitTo(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "topsizer:Sizer[orientation=wxVERTICAL]",
                  "controller:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"Generic\"]",
                  "sizer:Sizer[orientation=wxVERTICAL]",
                  "Add:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"Generic\"]:flags:(0,0x0,0)",
                  "SetSizeHints:[id=0, pos=(0,0), size=(0,0), style=0]",
              });
    }

    SECTION("flags.window")
    {
        wxUI::VSizer { wxUI::Generic<Type> { wxSizerFlags(1), button } }.fitTo(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "topsizer:Sizer[orientation=wxVERTICAL]",
                  "controller:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"Generic\"]",
                  "sizer:Sizer[orientation=wxVERTICAL]",
                  "Add:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"Generic\"]:flags:(1,0x0,0)",
                  "SetSizeHints:[id=0, pos=(0,0), size=(0,0), style=0]",
              });
    }

    SECTION("window")
    {
        wxUI::VSizer { wxUI::Generic<Type> { button } }.fitTo(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "topsizer:Sizer[orientation=wxVERTICAL]",
                  "controller:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"Generic\"]",
                  "sizer:Sizer[orientation=wxVERTICAL]",
                  "Add:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"Generic\"]:flags:(0,0x0,0)",
                  "SetSizeHints:[id=0, pos=(0,0), size=(0,0), style=0]",
              });
    }
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity)
