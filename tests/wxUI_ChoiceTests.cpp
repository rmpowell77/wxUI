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
#include <wxUI/Choice.h>

#include <wx/wx.h>

using TypeUnderTest = wxUI::Choice;

TEST_CASE("Choice")
{
    SECTION("noargs.1")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        TypeUnderTest uut {};
        auto window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(0 == window->GetCount());
    }
    SECTION("noargs")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest {};
        auto window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(0 == window->GetCount());
    }

    SECTION("choice")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { { "Hello", "Goodbye" } };
        auto window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(2 == window->GetCount());
        CHECK(0 == window->GetSelection());
        CHECK("Hello" == window->GetString(0));
        CHECK("Goodbye" == window->GetString(1));
    }

    SECTION("id")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { 10000 };
        auto window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(10000 == window->GetId());
        CHECK(0 == window->GetCount());
    }

    SECTION("id.choice")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { 10000, { "Hello", "Goodbye" } };
        auto window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(10000 == window->GetId());
        CHECK(2 == window->GetCount());
        CHECK(0 == window->GetSelection());
        CHECK("Hello" == window->GetString(0));
        CHECK("Goodbye" == window->GetString(1));
    }

    SECTION("size")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { wxSizerFlags(1) };
        auto window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(0 == window->GetCount());
    }

    SECTION("size.choice")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { wxSizerFlags(1), { "Hello", "Goodbye" } };
        auto window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(2 == window->GetCount());
        CHECK(0 == window->GetSelection());
        CHECK("Hello" == window->GetString(0));
        CHECK("Goodbye" == window->GetString(1));
    }

    SECTION("size.id")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { wxSizerFlags(1), 10000 };
        auto window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(10000 == window->GetId());
        CHECK(0 == window->GetCount());
    }

    SECTION("size.id.choice")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto uut = TypeUnderTest { wxSizerFlags(1), 10000, { "Hello", "Goodbye" } };
        auto window = dynamic_cast<TypeUnderTest::underlying_t*>(uut.create(&frame));
        CHECK(10000 == window->GetId());
        CHECK(2 == window->GetCount());
        CHECK(0 == window->GetSelection());
        CHECK("Hello" == window->GetString(0));
        CHECK("Goodbye" == window->GetString(1));
    }
}
