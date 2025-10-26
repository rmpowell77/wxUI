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
#include <ranges>
#include <wxUI/Button.hpp>
#include <wxUI/Layout.hpp>
#include <wxUI/Menu.hpp>

#include <wx/wx.h>

using namespace wxUITests;

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity)
auto CheckSizerEmpty(wxSizer* sizer)
{
    CHECK(nullptr != sizer);
    CHECK(sizer->IsEmpty() == true);
}

auto CheckSizer(wxSizer* sizer, int orientation)
{
    auto* boxSizer = dynamic_cast<wxBoxSizer*>(sizer);
    CHECK(nullptr != boxSizer);
    CHECK(orientation == boxSizer->GetOrientation());
    auto* staticBoxSizer = dynamic_cast<wxStaticBoxSizer*>(sizer);
    CHECK(nullptr == staticBoxSizer);
}

auto CheckSizer(wxSizer* sizer, int orientation, std::string_view name)
{
    auto* boxSizer = dynamic_cast<wxBoxSizer*>(sizer);
    CHECK(nullptr != boxSizer);
    CHECK(orientation == boxSizer->GetOrientation());
    auto* staticBoxSizer = dynamic_cast<wxStaticBoxSizer*>(sizer);
    CHECK(nullptr != staticBoxSizer);
    auto* box = staticBoxSizer->GetStaticBox();
    CHECK(nullptr != box);
    CHECK(box->GetLabel() == std::string { name });
}

auto CheckVSizerEmpty(wxSizer* sizer)
{
    CheckSizerEmpty(sizer);
    CheckSizer(sizer, wxVERTICAL);
}

auto CheckHSizerEmpty(wxSizer* sizer)
{
    CheckSizerEmpty(sizer);
    CheckSizer(sizer, wxHORIZONTAL);
}

auto CheckVSizerNamedEmpty(wxSizer* sizer, std::string_view name)
{
    CheckSizerEmpty(sizer);
    CheckSizer(sizer, wxVERTICAL, name);
}

auto CheckHSizerNamedEmpty(wxSizer* sizer, std::string_view name)
{
    CheckSizerEmpty(sizer);
    CheckSizer(sizer, wxHORIZONTAL, name);
}

auto CheckVSizerHasOne(wxSizer* sizer, auto passAnswer)
{
    CheckSizer(sizer, wxVERTICAL);

    auto children = sizer->GetChildren();
    CHECK(children.GetCount() == 1);
    passAnswer((*children.begin())->GetSizer());
}

auto CheckVSizerHasOne(wxSizer* sizer, std::string_view name, auto passAnswer)
{
    CheckSizer(sizer, wxVERTICAL, name);

    auto children = sizer->GetChildren();
    CHECK(children.GetCount() == 1);
    passAnswer((*children.begin())->GetSizer());
}

auto CheckHSizerHasN(size_t n, wxSizer* sizer, auto passAnswer)
{
    CheckSizer(sizer, wxHORIZONTAL);

    auto children = sizer->GetChildren();
    CHECK(children.GetCount() == n);
    passAnswer((*children.begin())->GetSizer());
}

auto CheckHSizerHasN(size_t n, wxSizer* sizer, std::string_view name, auto passAnswer)
{
    CheckSizer(sizer, wxHORIZONTAL, name);

    auto children = sizer->GetChildren();
    CHECK(children.GetCount() == n);
    passAnswer((*children.begin())->GetSizer());
}

auto CheckHSizerHasNItems(size_t n, wxSizer* sizer, std::string_view name)
{
    CheckSizer(sizer, wxHORIZONTAL, name);

    auto numberItems = sizer->GetItemCount();
    CHECK(numberItems == n);
}

TEST_CASE("Size")
{
    SECTION("vSizer.empty")
    {
        TestProvider frame;
        wxUI::VSizer {}.fitTo(&frame);
        CHECK(frame.dump() == std::vector<std::string> {
                  "topsizer:Sizer[orientation=wxVERTICAL]",
                  "sizer:Sizer[orientation=wxVERTICAL]",
                  "SetSizerHints:[id=0, pos=(0,0), size=(0,0), style=0]",
              });
    }
    SECTION("vSizer.empty1")
    {
        TestProvider frame;
        wxUI::VSizer { wxUI::Button { "Hello" } }.fitTo(&frame);
        CHECK(frame.dump() == std::vector<std::string> {
                  "topsizer:Sizer[orientation=wxVERTICAL]",
                  "controller:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"Hello\"]",
                  "SetEnabled:true",
                  "sizer:Sizer[orientation=wxVERTICAL]",
                  "Add:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"Hello\"]:flags:(0,0x0,0)",
                  "SetSizerHints:[id=0, pos=(0,0), size=(0,0), style=0]",
              });
    }
    SECTION("vSizer.named.empty")
    {
        TestProvider frame;
        wxUI::VSizer { "Test1" }.fitTo(&frame);
        CHECK(frame.dump() == std::vector<std::string> {
                  "topsizer:Sizer[orientation=wxVERTICAL, caption=\"Test1\"]",
                  "sizer:Sizer[orientation=wxVERTICAL, caption=\"Test1\"]",
                  "SetSizerHints:[id=0, pos=(0,0), size=(0,0), style=0]",
              });
    }
    SECTION("hSizer.empty")
    {
        TestProvider frame;
        wxUI::HSizer {}.fitTo(&frame);
        CHECK(frame.dump() == std::vector<std::string> {
                  "topsizer:Sizer[orientation=wxHORIZONTAL]",
                  "sizer:Sizer[orientation=wxHORIZONTAL]",
                  "SetSizerHints:[id=0, pos=(0,0), size=(0,0), style=0]",
              });
    }
    SECTION("hSizer.named.empty")
    {
        TestProvider frame;
        wxUI::HSizer { "Test2" }.fitTo(&frame);
        CHECK(frame.dump() == std::vector<std::string> {
                  "topsizer:Sizer[orientation=wxHORIZONTAL, caption=\"Test2\"]",
                  "sizer:Sizer[orientation=wxHORIZONTAL, caption=\"Test2\"]",
                  "SetSizerHints:[id=0, pos=(0,0), size=(0,0), style=0]",
              });
    }

    SECTION("vSizer.collapse.vSizer.empty")
    {
        TestProvider frame;
        wxUI::VSizer { wxUI::VSizer {} }.fitTo(&frame);
        CHECK(frame.dump() == std::vector<std::string> {
                  "topsizer:Sizer[orientation=wxVERTICAL]",
                  "sizer:Sizer[orientation=wxVERTICAL]",
                  "SetSizerHints:[id=0, pos=(0,0), size=(0,0), style=0]",
              });
    }
    SECTION("vSizer.collapse.vSizer.named.empty")
    {
        TestProvider frame;
        wxUI::VSizer { wxUI::VSizer { "Test1" } }.fitTo(&frame);
        CHECK(frame.dump() == std::vector<std::string> {
                  "topsizer:Sizer[orientation=wxVERTICAL, caption=\"Test1\"]",
                  "sizer:Sizer[orientation=wxVERTICAL, caption=\"Test1\"]",
                  "SetSizerHints:[id=0, pos=(0,0), size=(0,0), style=0]",
              });
    }
    SECTION("vSizer.hSizer.empty")
    {
        TestProvider frame;
        wxUI::VSizer { wxUI::HSizer {} }.fitTo(&frame);
        CHECK(frame.dump() == std::vector<std::string> {
                  "topsizer:Sizer[orientation=wxVERTICAL]",
                  "sizer:Sizer[orientation=wxVERTICAL]",
                  "AddSizer:Sizer[orientation=wxHORIZONTAL]:flags:(0,0x0,0)",
                  "SetSizerHints:[id=0, pos=(0,0), size=(0,0), style=0]",
                  "sizer:Sizer[orientation=wxHORIZONTAL]",
              });
    }
    SECTION("vSizer.hSizer.named.empty")
    {
        TestProvider frame;
        wxUI::VSizer { wxUI::HSizer { "Test2" } }.fitTo(&frame);
        CHECK(frame.dump() == std::vector<std::string> {
                  "topsizer:Sizer[orientation=wxVERTICAL]",
                  "sizer:Sizer[orientation=wxVERTICAL]",
                  "AddSizer:Sizer[orientation=wxHORIZONTAL, caption=\"Test2\"]:flags:(0,0x0,0)",
                  "SetSizerHints:[id=0, pos=(0,0), size=(0,0), style=0]",
                  "sizer:Sizer[orientation=wxHORIZONTAL, caption=\"Test2\"]",
              });
    }

    SECTION("vSizer.named.vSizer.empty")
    {
        TestProvider frame;
        wxUI::VSizer { "Test3", wxUI::VSizer {} }.fitTo(&frame);
        CHECK(frame.dump() == std::vector<std::string> {
                  "topsizer:Sizer[orientation=wxVERTICAL, caption=\"Test3\"]",
                  "sizer:Sizer[orientation=wxVERTICAL, caption=\"Test3\"]",
                  "AddSizer:Sizer[orientation=wxVERTICAL]:flags:(0,0x0,0)",
                  "SetSizerHints:[id=0, pos=(0,0), size=(0,0), style=0]",
                  "sizer:Sizer[orientation=wxVERTICAL]",
              });
    }
    SECTION("vSizer.named.vSizer.named.empty")
    {
        TestProvider frame;
        wxUI::VSizer { "Test3", wxUI::VSizer { "Test1" } }.fitTo(&frame);
        CHECK(frame.dump() == std::vector<std::string> {
                  "topsizer:Sizer[orientation=wxVERTICAL, caption=\"Test3\"]",
                  "sizer:Sizer[orientation=wxVERTICAL, caption=\"Test3\"]",
                  "AddSizer:Sizer[orientation=wxVERTICAL, caption=\"Test1\"]:flags:(0,0x0,0)",
                  "SetSizerHints:[id=0, pos=(0,0), size=(0,0), style=0]",
                  "sizer:Sizer[orientation=wxVERTICAL, caption=\"Test1\"]",
              });
    }
    SECTION("vSizer.named.hSizer.empty")
    {
        TestProvider frame;
        wxUI::VSizer { "Test3", wxUI::HSizer {} }.fitTo(&frame);
        CHECK(frame.dump() == std::vector<std::string> {
                  "topsizer:Sizer[orientation=wxVERTICAL, caption=\"Test3\"]",
                  "sizer:Sizer[orientation=wxVERTICAL, caption=\"Test3\"]",
                  "AddSizer:Sizer[orientation=wxHORIZONTAL]:flags:(0,0x0,0)",
                  "SetSizerHints:[id=0, pos=(0,0), size=(0,0), style=0]",
                  "sizer:Sizer[orientation=wxHORIZONTAL]",
              });
    }
    SECTION("vSizer.named.hSizer.named.empty")
    {
        TestProvider frame;
        wxUI::VSizer { "Test3", wxUI::HSizer { "Test2" } }.fitTo(&frame);
        CHECK(frame.dump() == std::vector<std::string> {
                  "topsizer:Sizer[orientation=wxVERTICAL, caption=\"Test3\"]",
                  "sizer:Sizer[orientation=wxVERTICAL, caption=\"Test3\"]",
                  "AddSizer:Sizer[orientation=wxHORIZONTAL, caption=\"Test2\"]:flags:(0,0x0,0)",
                  "SetSizerHints:[id=0, pos=(0,0), size=(0,0), style=0]",
                  "sizer:Sizer[orientation=wxHORIZONTAL, caption=\"Test2\"]",
              });
    }

    SECTION("hSizer.vSizer.empty")
    {
        TestProvider frame;
        wxUI::HSizer { wxUI::VSizer {} }.fitTo(&frame);
        CHECK(frame.dump() == std::vector<std::string> {
                  "topsizer:Sizer[orientation=wxHORIZONTAL]",
                  "sizer:Sizer[orientation=wxHORIZONTAL]",
                  "AddSizer:Sizer[orientation=wxVERTICAL]:flags:(0,0x0,0)",
                  "SetSizerHints:[id=0, pos=(0,0), size=(0,0), style=0]",
                  "sizer:Sizer[orientation=wxVERTICAL]",
              });
    }
    SECTION("hSizer.vSizer.named.empty")
    {
        TestProvider frame;
        wxUI::HSizer { wxUI::VSizer { "Test1" } }.fitTo(&frame);
        CHECK(frame.dump() == std::vector<std::string> {
                  "topsizer:Sizer[orientation=wxHORIZONTAL]",
                  "sizer:Sizer[orientation=wxHORIZONTAL]",
                  "AddSizer:Sizer[orientation=wxVERTICAL, caption=\"Test1\"]:flags:(0,0x0,0)",
                  "SetSizerHints:[id=0, pos=(0,0), size=(0,0), style=0]",
                  "sizer:Sizer[orientation=wxVERTICAL, caption=\"Test1\"]",
              });
    }
    SECTION("hSizer.collapse.hSizer.empty")
    {
        TestProvider frame;
        wxUI::HSizer { wxUI::HSizer {} }.fitTo(&frame);
        CHECK(frame.dump() == std::vector<std::string> {
                  "topsizer:Sizer[orientation=wxHORIZONTAL]",
                  "sizer:Sizer[orientation=wxHORIZONTAL]",
                  "SetSizerHints:[id=0, pos=(0,0), size=(0,0), style=0]",
              });
    }
    SECTION("hSizer.collapse.hSizer.named.empty")
    {
        TestProvider frame;
        wxUI::HSizer { wxUI::HSizer { "Test2" } }.fitTo(&frame);
        CHECK(frame.dump() == std::vector<std::string> {
                  "topsizer:Sizer[orientation=wxHORIZONTAL, caption=\"Test2\"]",
                  "sizer:Sizer[orientation=wxHORIZONTAL, caption=\"Test2\"]",
                  "SetSizerHints:[id=0, pos=(0,0), size=(0,0), style=0]",
              });
    }
    SECTION("hSizer.named.vSizer.empty")
    {
        TestProvider frame;
        wxUI::HSizer { "Test3", wxUI::VSizer {} }.fitTo(&frame);
        CHECK(frame.dump() == std::vector<std::string> {
                  "topsizer:Sizer[orientation=wxHORIZONTAL, caption=\"Test3\"]",
                  "sizer:Sizer[orientation=wxHORIZONTAL, caption=\"Test3\"]",
                  "AddSizer:Sizer[orientation=wxVERTICAL]:flags:(0,0x0,0)",
                  "SetSizerHints:[id=0, pos=(0,0), size=(0,0), style=0]",
                  "sizer:Sizer[orientation=wxVERTICAL]",
              });
    }
    SECTION("hSizer.named.vSizer.named.empty")
    {
        TestProvider frame;
        wxUI::HSizer { "Test3", wxUI::VSizer { "Test1" } }.fitTo(&frame);
        CHECK(frame.dump() == std::vector<std::string> {
                  "topsizer:Sizer[orientation=wxHORIZONTAL, caption=\"Test3\"]",
                  "sizer:Sizer[orientation=wxHORIZONTAL, caption=\"Test3\"]",
                  "AddSizer:Sizer[orientation=wxVERTICAL, caption=\"Test1\"]:flags:(0,0x0,0)",
                  "SetSizerHints:[id=0, pos=(0,0), size=(0,0), style=0]",
                  "sizer:Sizer[orientation=wxVERTICAL, caption=\"Test1\"]",
              });
    }
    SECTION("hSizer.named.hSizer.empty")
    {
        TestProvider frame;
        wxUI::HSizer { "Test3", wxUI::HSizer {} }.fitTo(&frame);
        CHECK(frame.dump() == std::vector<std::string> {
                  "topsizer:Sizer[orientation=wxHORIZONTAL, caption=\"Test3\"]",
                  "sizer:Sizer[orientation=wxHORIZONTAL, caption=\"Test3\"]",
                  "AddSizer:Sizer[orientation=wxHORIZONTAL]:flags:(0,0x0,0)",
                  "SetSizerHints:[id=0, pos=(0,0), size=(0,0), style=0]",
                  "sizer:Sizer[orientation=wxHORIZONTAL]",
              });
    }
    SECTION("hSizer.named.hSizer.named.empty")
    {
        TestProvider frame;
        wxUI::HSizer { "Test3", wxUI::HSizer { "Test2" } }.fitTo(&frame);
        CHECK(frame.dump() == std::vector<std::string> {
                  "topsizer:Sizer[orientation=wxHORIZONTAL, caption=\"Test3\"]",
                  "sizer:Sizer[orientation=wxHORIZONTAL, caption=\"Test3\"]",
                  "AddSizer:Sizer[orientation=wxHORIZONTAL, caption=\"Test2\"]:flags:(0,0x0,0)",
                  "SetSizerHints:[id=0, pos=(0,0), size=(0,0), style=0]",
                  "sizer:Sizer[orientation=wxHORIZONTAL, caption=\"Test2\"]",
              });
    }
    SECTION("layoutif.true.1sizer")
    {
        TestProvider frame;
        wxUI::HSizer { "Test3", wxUI::LayoutIf { true, wxUI::HSizer { "Test2" } } }.fitTo(&frame);
        CHECK(frame.dump() == std::vector<std::string> {
                  "topsizer:Sizer[orientation=wxHORIZONTAL, caption=\"Test3\"]",
                  "sizer:Sizer[orientation=wxHORIZONTAL, caption=\"Test3\"]",
                  "AddSizer:Sizer[orientation=wxHORIZONTAL, caption=\"Test2\"]:flags:(0,0x0,0)",
                  "SetSizerHints:[id=0, pos=(0,0), size=(0,0), style=0]",
                  "sizer:Sizer[orientation=wxHORIZONTAL, caption=\"Test2\"]",
              });
    }
    SECTION("layoutif.false.1sizer")
    {
        TestProvider frame;
        wxUI::HSizer { "Test3", wxUI::LayoutIf { false, wxUI::HSizer { "Test2" } } }.fitTo(&frame);
        CHECK(frame.dump() == std::vector<std::string> {
                  "topsizer:Sizer[orientation=wxHORIZONTAL, caption=\"Test3\"]",
                  "sizer:Sizer[orientation=wxHORIZONTAL, caption=\"Test3\"]",
                  "SetSizerHints:[id=0, pos=(0,0), size=(0,0), style=0]",
              });
    }
    SECTION("layoutif.true.button")
    {
        TestProvider frame;
        wxUI::HSizer { "Test3", wxUI::LayoutIf { true, wxUI::Button { "Test2" } } }.fitTo(&frame);
        CHECK(frame.dump() == std::vector<std::string> {
                  "topsizer:Sizer[orientation=wxHORIZONTAL, caption=\"Test3\"]",
                  "controller:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"Test2\"]",
                  "SetEnabled:true",
                  "sizer:Sizer[orientation=wxHORIZONTAL, caption=\"Test3\"]",
                  "Add:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"Test2\"]:flags:(0,0x0,0)",
                  "SetSizerHints:[id=0, pos=(0,0), size=(0,0), style=0]",
              });
    }
    SECTION("layoutif.false.button")
    {
        TestProvider frame;
        wxUI::HSizer { "Test3", wxUI::LayoutIf { false, wxUI::Button { "Test2" } } }.fitTo(&frame);
        CHECK(frame.dump() == std::vector<std::string> {
                  "topsizer:Sizer[orientation=wxHORIZONTAL, caption=\"Test3\"]",
                  "sizer:Sizer[orientation=wxHORIZONTAL, caption=\"Test3\"]",
                  "SetSizerHints:[id=0, pos=(0,0), size=(0,0), style=0]",
              });
    }
    SECTION("layoutif.true.true.button")
    {
        TestProvider frame;
        wxUI::HSizer { "Test3", wxUI::LayoutIf { true, wxUI::Button { "Test2" }, wxUI::LayoutIf { true, wxUI::Button { "Test2" } } } }.fitTo(&frame);
        CHECK(frame.dump() == std::vector<std::string> {
                  "topsizer:Sizer[orientation=wxHORIZONTAL, caption=\"Test3\"]",
                  "controller:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"Test2\"]",
                  "SetEnabled:true",
                  "controller:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"Test2\"]",
                  "SetEnabled:true",
                  "sizer:Sizer[orientation=wxHORIZONTAL, caption=\"Test3\"]",
                  "Add:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"Test2\"]:flags:(0,0x0,0)",
                  "Add:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"Test2\"]:flags:(0,0x0,0)",
                  "SetSizerHints:[id=0, pos=(0,0), size=(0,0), style=0]",
              });
    }
    SECTION("layoutif.true.false.button")
    {
        TestProvider frame;
        wxUI::HSizer { "Test3", wxUI::LayoutIf { true, wxUI::Button { "Test2" }, wxUI::LayoutIf { false, wxUI::Button { "Test2" } } } }.fitTo(&frame);
        CHECK(frame.dump() == std::vector<std::string> {
                  "topsizer:Sizer[orientation=wxHORIZONTAL, caption=\"Test3\"]",
                  "controller:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"Test2\"]",
                  "SetEnabled:true",
                  "sizer:Sizer[orientation=wxHORIZONTAL, caption=\"Test3\"]",
                  "Add:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"Test2\"]:flags:(0,0x0,0)",
                  "SetSizerHints:[id=0, pos=(0,0), size=(0,0), style=0]",
              });
    }
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity)
