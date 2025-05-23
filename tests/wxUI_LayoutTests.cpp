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
#include <ranges>
#include <wxUI/Button.hpp>
#include <wxUI/Layout.hpp>
#include <wxUI/Menu.hpp>

#include <wx/wx.h>

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
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::VSizer {}.fitTo(&frame);
        CheckVSizerEmpty(frame.GetSizer());
    }
    SECTION("vSizer.named.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::VSizer { "Test1" }.fitTo(&frame);
        CheckVSizerNamedEmpty(frame.GetSizer(), "Test1");
    }
    SECTION("hSizer.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::HSizer {}.fitTo(&frame);
        CheckHSizerEmpty(frame.GetSizer());
    }
    SECTION("hSizer.named.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::HSizer { "Test2" }.fitTo(&frame);
        CheckHSizerNamedEmpty(frame.GetSizer(), "Test2");
    }

    SECTION("vSizer.collapse.vSizer.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::VSizer { wxUI::VSizer {} }.fitTo(&frame);
        CheckVSizerEmpty(frame.GetSizer());
    }
    SECTION("vSizer.collapse.vSizer.named.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::VSizer { wxUI::VSizer { "Test1" } }.fitTo(&frame);
        CheckVSizerNamedEmpty(frame.GetSizer(), "Test1");
    }
    SECTION("vSizer.hSizer.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::VSizer { wxUI::HSizer {} }.fitTo(&frame);
        CheckVSizerHasOne(frame.GetSizer(), [](wxSizer* sizer) {
            CheckHSizerEmpty(sizer);
        });
    }
    SECTION("vSizer.hSizer.named.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::VSizer { wxUI::HSizer { "Test2" } }.fitTo(&frame);
        CheckVSizerHasOne(frame.GetSizer(), [](wxSizer* sizer) {
            CheckHSizerNamedEmpty(sizer, "Test2");
        });
    }

    SECTION("vSizer.named.vSizer.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::VSizer { "Test3", wxUI::VSizer {} }.fitTo(&frame);
        CheckVSizerHasOne(frame.GetSizer(), "Test3", [](wxSizer* sizer) {
            CheckVSizerEmpty(sizer);
        });
    }
    SECTION("vSizer.named.vSizer.named.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::VSizer { "Test3", wxUI::VSizer { "Test1" } }.fitTo(&frame);
        CheckVSizerHasOne(frame.GetSizer(), "Test3", [](wxSizer* sizer) {
            CheckVSizerNamedEmpty(sizer, "Test1");
        });
    }
    SECTION("vSizer.named.hSizer.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::VSizer { "Test3", wxUI::HSizer {} }.fitTo(&frame);
        CheckVSizerHasOne(frame.GetSizer(), "Test3", [](wxSizer* sizer) {
            CheckHSizerEmpty(sizer);
        });
    }
    SECTION("vSizer.named.hSizer.named.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::VSizer { "Test3", wxUI::HSizer { "Test2" } }.fitTo(&frame);
        CheckVSizerHasOne(frame.GetSizer(), "Test3", [](wxSizer* sizer) {
            CheckHSizerNamedEmpty(sizer, "Test2");
        });
    }

    SECTION("hSizer.vSizer.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::HSizer { wxUI::VSizer {} }.fitTo(&frame);
        CheckHSizerHasN(1, frame.GetSizer(), [](wxSizer* sizer) {
            CheckVSizerEmpty(sizer);
        });
    }
    SECTION("hSizer.vSizer.named.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::HSizer { wxUI::VSizer { "Test1" } }.fitTo(&frame);
        CheckHSizerHasN(1, frame.GetSizer(), [](wxSizer* sizer) {
            CheckVSizerNamedEmpty(sizer, "Test1");
        });
    }
    SECTION("hSizer.collapse.hSizer.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::HSizer { wxUI::HSizer {} }.fitTo(&frame);
        CheckHSizerEmpty(frame.GetSizer());
    }
    SECTION("hSizer.collapse.hSizer.named.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::HSizer { wxUI::HSizer { "Test2" } }.fitTo(&frame);
        CheckHSizerNamedEmpty(frame.GetSizer(), "Test2");
    }
    SECTION("hSizer.named.vSizer.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::HSizer { "Test3", wxUI::VSizer {} }.fitTo(&frame);
        CheckHSizerHasN(1, frame.GetSizer(), "Test3", [](wxSizer* sizer) {
            CheckVSizerEmpty(sizer);
        });
    }
    SECTION("hSizer.named.vSizer.named.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::HSizer { "Test3", wxUI::VSizer { "Test1" } }.fitTo(&frame);
        CheckHSizerHasN(1, frame.GetSizer(), "Test3", [](wxSizer* sizer) {
            CheckVSizerNamedEmpty(sizer, "Test1");
        });
    }
    SECTION("hSizer.named.hSizer.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::HSizer { "Test3", wxUI::HSizer {} }.fitTo(&frame);
        CheckHSizerHasN(1, frame.GetSizer(), "Test3", [](wxSizer* sizer) {
            CheckHSizerEmpty(sizer);
        });
    }
    SECTION("hSizer.named.hSizer.named.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::HSizer { "Test3", wxUI::HSizer { "Test2" } }.fitTo(&frame);
        CheckHSizerHasN(1, frame.GetSizer(), "Test3", [](wxSizer* sizer) {
            CheckHSizerNamedEmpty(sizer, "Test2");
        });
    }
    SECTION("layoutif.true.1sizer")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::HSizer { "Test3", wxUI::LayoutIf { true, wxUI::HSizer { "Test2" } } }.fitTo(&frame);
        CheckHSizerHasN(1, frame.GetSizer(), "Test3", [](wxSizer* sizer) {
            CheckHSizerNamedEmpty(sizer, "Test2");
        });
    }
    SECTION("layoutif.false.1sizer")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::HSizer { "Test3", wxUI::LayoutIf { false, wxUI::HSizer { "Test2" } } }.fitTo(&frame);
        CheckHSizerNamedEmpty(frame.GetSizer(), "Test3");
    }
    SECTION("layoutif.true.button")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::HSizer { "Test3", wxUI::LayoutIf { true, wxUI::Button { "Test2" } } }.fitTo(&frame);
        CheckHSizerHasNItems(1, frame.GetSizer(), "Test3");
    }
    SECTION("layoutif.false.button")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::HSizer { "Test3", wxUI::LayoutIf { false, wxUI::Button { "Test2" } } }.fitTo(&frame);
        CheckHSizerHasNItems(0, frame.GetSizer(), "Test3");
    }
    SECTION("layoutif.true.true.button")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::HSizer { "Test3", wxUI::LayoutIf { true, wxUI::Button { "Test2" }, wxUI::LayoutIf { true, wxUI::Button { "Test2" } } } }.fitTo(&frame);
        CheckHSizerHasNItems(2, frame.GetSizer(), "Test3");
    }
    SECTION("layoutif.true.false.button")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::HSizer { "Test3", wxUI::LayoutIf { true, wxUI::Button { "Test2" }, wxUI::LayoutIf { false, wxUI::Button { "Test2" } } } }.fitTo(&frame);
        CheckHSizerHasNItems(1, frame.GetSizer(), "Test3");
    }
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity)
