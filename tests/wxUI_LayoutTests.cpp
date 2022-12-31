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

auto CheckStack(wxSizer* sizer, int orientation)
{
    auto* boxSizer = dynamic_cast<wxBoxSizer*>(sizer);
    CHECK(nullptr != boxSizer);
    CHECK(orientation == boxSizer->GetOrientation());
    auto* staticBoxSizer = dynamic_cast<wxStaticBoxSizer*>(sizer);
    CHECK(nullptr == staticBoxSizer);
}

auto CheckStack(wxSizer* sizer, int orientation, std::string_view name)
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

auto CheckVStackEmpty(wxSizer* sizer)
{
    CheckSizerEmpty(sizer);
    CheckStack(sizer, wxVERTICAL);
}

auto CheckHStackEmpty(wxSizer* sizer)
{
    CheckSizerEmpty(sizer);
    CheckStack(sizer, wxHORIZONTAL);
}

auto CheckVStackNamedEmpty(wxSizer* sizer, std::string_view name)
{
    CheckSizerEmpty(sizer);
    CheckStack(sizer, wxVERTICAL, name);
}

auto CheckHStackNamedEmpty(wxSizer* sizer, std::string_view name)
{
    CheckSizerEmpty(sizer);
    CheckStack(sizer, wxHORIZONTAL, name);
}

auto CheckVStackHasOne(wxSizer* sizer, auto passAnswer)
{
    CheckStack(sizer, wxVERTICAL);

    auto children = sizer->GetChildren();
    CHECK(children.GetCount() == 1);
    passAnswer((*children.begin())->GetSizer());
}

auto CheckVStackHasOne(wxSizer* sizer, std::string_view name, auto passAnswer)
{
    CheckStack(sizer, wxVERTICAL, name);

    auto children = sizer->GetChildren();
    CHECK(children.GetCount() == 1);
    passAnswer((*children.begin())->GetSizer());
}

auto CheckHStackHasOne(wxSizer* sizer, auto passAnswer)
{
    CheckStack(sizer, wxHORIZONTAL);

    auto children = sizer->GetChildren();
    CHECK(children.GetCount() == 1);
    passAnswer((*children.begin())->GetSizer());
}

auto CheckHStackHasOne(wxSizer* sizer, std::string_view name, auto passAnswer)
{
    CheckStack(sizer, wxHORIZONTAL, name);

    auto children = sizer->GetChildren();
    CHECK(children.GetCount() == 1);
    passAnswer((*children.begin())->GetSizer());
}

TEST_CASE("Size")
{
    SECTION("vstack.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::VStack {}.attachTo(&frame);
        CheckVStackEmpty(frame.GetSizer());
    }
    SECTION("vstack.named.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::VStack { "Test1" }.attachTo(&frame);
        CheckVStackNamedEmpty(frame.GetSizer(), "Test1");
    }
    SECTION("hstack.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::HStack {}.attachTo(&frame);
        CheckHStackEmpty(frame.GetSizer());
    }
    SECTION("hstack.named.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::HStack { "Test2" }.attachTo(&frame);
        CheckHStackNamedEmpty(frame.GetSizer(), "Test2");
    }

    SECTION("vstack.collapse.vstack.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::VStack { wxUI::VStack {} }.attachTo(&frame);
        CheckVStackEmpty(frame.GetSizer());
    }
    SECTION("vstack.collapse.vstack.named.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::VStack { wxUI::VStack { "Test1" } }.attachTo(&frame);
        CheckVStackNamedEmpty(frame.GetSizer(), "Test1");
    }
    SECTION("vstack.vstack.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::VStack { std::tuple { wxUI::VStack {} } }.attachTo(&frame);
        CheckVStackHasOne(frame.GetSizer(), [](wxSizer* sizer) {
            CheckVStackEmpty(sizer);
        });
    }
    SECTION("vstack.vstack.named.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::VStack { std::tuple { wxUI::VStack { "Test1" } } }.attachTo(&frame);
        CheckVStackHasOne(frame.GetSizer(), [](wxSizer* sizer) {
            CheckVStackNamedEmpty(sizer, "Test1");
        });
    }
    SECTION("vstack.hstack.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::VStack { wxUI::HStack {} }.attachTo(&frame);
        CheckVStackHasOne(frame.GetSizer(), [](wxSizer* sizer) {
            CheckHStackEmpty(sizer);
        });
    }
    SECTION("vstack.hstack.named.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::VStack { wxUI::HStack { "Test2" } }.attachTo(&frame);
        CheckVStackHasOne(frame.GetSizer(), [](wxSizer* sizer) {
            CheckHStackNamedEmpty(sizer, "Test2");
        });
    }

    SECTION("vstack.named.vstack.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::VStack { "Test3", wxUI::VStack {} }.attachTo(&frame);
        CheckVStackHasOne(frame.GetSizer(), "Test3", [](wxSizer* sizer) {
            CheckVStackEmpty(sizer);
        });
    }
    SECTION("vstack.named.vstack.named.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::VStack { "Test3", wxUI::VStack { "Test1" } }.attachTo(&frame);
        CheckVStackHasOne(frame.GetSizer(), "Test3", [](wxSizer* sizer) {
            CheckVStackNamedEmpty(sizer, "Test1");
        });
    }
    SECTION("vstack.named.hstack.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::VStack { "Test3", wxUI::HStack {} }.attachTo(&frame);
        CheckVStackHasOne(frame.GetSizer(), "Test3", [](wxSizer* sizer) {
            CheckHStackEmpty(sizer);
        });
    }
    SECTION("vstack.named.hstack.named.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::VStack { "Test3", wxUI::HStack { "Test2" } }.attachTo(&frame);
        CheckVStackHasOne(frame.GetSizer(), "Test3", [](wxSizer* sizer) {
            CheckHStackNamedEmpty(sizer, "Test2");
        });
    }

    SECTION("hstack.vstack.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::HStack { wxUI::VStack {} }.attachTo(&frame);
        CheckHStackHasOne(frame.GetSizer(), [](wxSizer* sizer) {
            CheckVStackEmpty(sizer);
        });
    }
    SECTION("hstack.vstack.named.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::HStack { wxUI::VStack { "Test1" } }.attachTo(&frame);
        CheckHStackHasOne(frame.GetSizer(), [](wxSizer* sizer) {
            CheckVStackNamedEmpty(sizer, "Test1");
        });
    }
    SECTION("hstack.collapse.hstack.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::HStack { wxUI::HStack {} }.attachTo(&frame);
        CheckHStackEmpty(frame.GetSizer());
    }
    SECTION("hstack.collapse.hstack.named.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::HStack { wxUI::HStack { "Test2" } }.attachTo(&frame);
        CheckHStackNamedEmpty(frame.GetSizer(), "Test2");
    }
    SECTION("hstack.hstack.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::HStack { std::tuple { wxUI::HStack {} } }.attachTo(&frame);
        CheckHStackHasOne(frame.GetSizer(), [](wxSizer* sizer) {
            CheckHStackEmpty(sizer);
        });
    }
    SECTION("hstack.hstack.named.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::HStack { std::tuple { wxUI::HStack { "Test1" } } }.attachTo(&frame);
        CheckHStackHasOne(frame.GetSizer(), [](wxSizer* sizer) {
            CheckHStackNamedEmpty(sizer, "Test1");
        });
    }

    SECTION("hstack.named.vstack.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::HStack { "Test3", wxUI::VStack {} }.attachTo(&frame);
        CheckHStackHasOne(frame.GetSizer(), "Test3", [](wxSizer* sizer) {
            CheckVStackEmpty(sizer);
        });
    }
    SECTION("hstack.named.vstack.named.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::HStack { "Test3", wxUI::VStack { "Test1" } }.attachTo(&frame);
        CheckHStackHasOne(frame.GetSizer(), "Test3", [](wxSizer* sizer) {
            CheckVStackNamedEmpty(sizer, "Test1");
        });
    }
    SECTION("hstack.named.hstack.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::HStack { "Test3", wxUI::HStack {} }.attachTo(&frame);
        CheckHStackHasOne(frame.GetSizer(), "Test3", [](wxSizer* sizer) {
            CheckHStackEmpty(sizer);
        });
    }
    SECTION("hstack.named.hstack.named.empty")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        wxUI::HStack { "Test3", wxUI::HStack { "Test2" } }.attachTo(&frame);
        CheckHStackHasOne(frame.GetSizer(), "Test3", [](wxSizer* sizer) {
            CheckHStackNamedEmpty(sizer, "Test2");
        });
    }
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity)
