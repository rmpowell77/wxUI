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
#include <catch2/catch_test_macros.hpp>
#include <wxUI/Layout.h>
#include <wxUI/Menu.h>

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

auto CheckHSizerHasOne(wxSizer* sizer, auto passAnswer)
{
    CheckSizer(sizer, wxHORIZONTAL);

    auto children = sizer->GetChildren();
    CHECK(children.GetCount() == 1);
    passAnswer((*children.begin())->GetSizer());
}

auto CheckHSizerHasOne(wxSizer* sizer, std::string_view name, auto passAnswer)
{
    CheckSizer(sizer, wxHORIZONTAL, name);

    auto children = sizer->GetChildren();
    CHECK(children.GetCount() == 1);
    passAnswer((*children.begin())->GetSizer());
}

TEST_CASE("Size")
{
    SECTION("vSizer.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::VSizer {}.attachTo(&frame);
        CheckVSizerEmpty(frame.GetSizer());
    }
    SECTION("vSizer.named.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::VSizer { "Test1" }.attachTo(&frame);
        CheckVSizerNamedEmpty(frame.GetSizer(), "Test1");
    }
    SECTION("hSizer.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::HSizer {}.attachTo(&frame);
        CheckHSizerEmpty(frame.GetSizer());
    }
    SECTION("hSizer.named.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::HSizer { "Test2" }.attachTo(&frame);
        CheckHSizerNamedEmpty(frame.GetSizer(), "Test2");
    }

    SECTION("vSizer.collapse.vSizer.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::VSizer { wxUI::VSizer {} }.attachTo(&frame);
        CheckVSizerEmpty(frame.GetSizer());
    }
    SECTION("vSizer.collapse.vSizer.named.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::VSizer { wxUI::VSizer { "Test1" } }.attachTo(&frame);
        CheckVSizerNamedEmpty(frame.GetSizer(), "Test1");
    }
    SECTION("vSizer.hSizer.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::VSizer { wxUI::HSizer {} }.attachTo(&frame);
        CheckVSizerHasOne(frame.GetSizer(), [](wxSizer* sizer) {
            CheckHSizerEmpty(sizer);
        });
    }
    SECTION("vSizer.hSizer.named.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::VSizer { wxUI::HSizer { "Test2" } }.attachTo(&frame);
        CheckVSizerHasOne(frame.GetSizer(), [](wxSizer* sizer) {
            CheckHSizerNamedEmpty(sizer, "Test2");
        });
    }

    SECTION("vSizer.named.vSizer.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::VSizer { "Test3", wxUI::VSizer {} }.attachTo(&frame);
        CheckVSizerHasOne(frame.GetSizer(), "Test3", [](wxSizer* sizer) {
            CheckVSizerEmpty(sizer);
        });
    }
    SECTION("vSizer.named.vSizer.named.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::VSizer { "Test3", wxUI::VSizer { "Test1" } }.attachTo(&frame);
        CheckVSizerHasOne(frame.GetSizer(), "Test3", [](wxSizer* sizer) {
            CheckVSizerNamedEmpty(sizer, "Test1");
        });
    }
    SECTION("vSizer.named.hSizer.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::VSizer { "Test3", wxUI::HSizer {} }.attachTo(&frame);
        CheckVSizerHasOne(frame.GetSizer(), "Test3", [](wxSizer* sizer) {
            CheckHSizerEmpty(sizer);
        });
    }
    SECTION("vSizer.named.hSizer.named.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::VSizer { "Test3", wxUI::HSizer { "Test2" } }.attachTo(&frame);
        CheckVSizerHasOne(frame.GetSizer(), "Test3", [](wxSizer* sizer) {
            CheckHSizerNamedEmpty(sizer, "Test2");
        });
    }

    SECTION("hSizer.vSizer.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::HSizer { wxUI::VSizer {} }.attachTo(&frame);
        CheckHSizerHasOne(frame.GetSizer(), [](wxSizer* sizer) {
            CheckVSizerEmpty(sizer);
        });
    }
    SECTION("hSizer.vSizer.named.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::HSizer { wxUI::VSizer { "Test1" } }.attachTo(&frame);
        CheckHSizerHasOne(frame.GetSizer(), [](wxSizer* sizer) {
            CheckVSizerNamedEmpty(sizer, "Test1");
        });
    }
    SECTION("hSizer.collapse.hSizer.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::HSizer { wxUI::HSizer {} }.attachTo(&frame);
        CheckHSizerEmpty(frame.GetSizer());
    }
    SECTION("hSizer.collapse.hSizer.named.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::HSizer { wxUI::HSizer { "Test2" } }.attachTo(&frame);
        CheckHSizerNamedEmpty(frame.GetSizer(), "Test2");
    }
    SECTION("hSizer.named.vSizer.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::HSizer { "Test3", wxUI::VSizer {} }.attachTo(&frame);
        CheckHSizerHasOne(frame.GetSizer(), "Test3", [](wxSizer* sizer) {
            CheckVSizerEmpty(sizer);
        });
    }
    SECTION("hSizer.named.vSizer.named.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::HSizer { "Test3", wxUI::VSizer { "Test1" } }.attachTo(&frame);
        CheckHSizerHasOne(frame.GetSizer(), "Test3", [](wxSizer* sizer) {
            CheckVSizerNamedEmpty(sizer, "Test1");
        });
    }
    SECTION("hSizer.named.hSizer.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::HSizer { "Test3", wxUI::HSizer {} }.attachTo(&frame);
        CheckHSizerHasOne(frame.GetSizer(), "Test3", [](wxSizer* sizer) {
            CheckHSizerEmpty(sizer);
        });
    }
    SECTION("hSizer.named.hSizer.named.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::HSizer { "Test3", wxUI::HSizer { "Test2" } }.attachTo(&frame);
        CheckHSizerHasOne(frame.GetSizer(), "Test3", [](wxSizer* sizer) {
            CheckHSizerNamedEmpty(sizer, "Test2");
        });
    }
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity)
