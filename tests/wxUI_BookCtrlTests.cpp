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
#include <wxUI/BookCtrl.hpp>
#include <wxUI/Button.hpp>
#include <wxUI/Layout.hpp>
#include <wxUI/TextCtrl.hpp>
#include <wxUI/wxUITypes.hpp>

#include <wx/wx.h>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity, misc-use-anonymous-namespace, cppcoreguidelines-avoid-do-while)
using namespace wxUITests;
using namespace wxUI;

TEST_CASE("BookCtrl")
{
    SECTION("Notebook.empty")
    {
        TestParent provider;
        auto uut = Notebook {};
        uut.createAndAdd(&provider, provider.add(TestSizer { .type = SizerType::Box, .orientation = wxVERTICAL }), wxSizerFlags {});
        auto dump = provider.dump();
        CHECK(dump == std::vector<std::string> {
                  "Create:Sizer[orientation=wxVERTICAL]",
                  "Create:wxNotebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "controller:wxNotebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "sizer:Sizer[orientation=wxVERTICAL]",
                  "Add:wxNotebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:flags:(0,0x0,0)",
              });
    }

    SECTION("Notebook.single_page")
    {
        TestParent provider;
        auto uut = Notebook {
            BookItem { "Page 1", Button { "Button 1" } }
        };
        uut.createAndAdd(&provider, provider.add(TestSizer { .type = SizerType::Box, .orientation = wxVERTICAL }), wxSizerFlags {});
        auto dump = provider.dump();
        CHECK(dump == std::vector<std::string> {
                  "Create:Sizer[orientation=wxVERTICAL]",
                  "Create:wxNotebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "controller:wxNotebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "Create:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "bookPage:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:Page 1:false",
                  "sizer:Sizer[orientation=wxVERTICAL]",
                  "Add:wxNotebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:flags:(0,0x0,0)",
              });
    }

    SECTION("Notebook.multiple_pages")
    {
        TestParent provider;
        auto uut = Notebook {
            BookItem { "Page 1", Button { "Button 1" } },
            BookItem { "Page 2", Button { "Button 2" } },
        };
        uut.createAndAdd(&provider, provider.add(TestSizer { .type = SizerType::Box, .orientation = wxVERTICAL }), wxSizerFlags {});
        auto dump = provider.dump();
        CHECK(dump == std::vector<std::string> {
                  "Create:Sizer[orientation=wxVERTICAL]",
                  "Create:wxNotebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "controller:wxNotebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "Create:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "Create:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "bookPage:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:Page 1:false",
                  "bookPage:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:Page 2:false",
                  "sizer:Sizer[orientation=wxVERTICAL]",
                  "Add:wxNotebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:flags:(0,0x0,0)",
              });
    }

    SECTION("Notebook.page_with_selection")
    {
        TestParent provider;
        auto uut = Notebook {
            BookItem { "Page 1", false, Button { "Button 1" } },
            BookItem { "Page 2", true, Button { "Button 2" } },
        };
        uut.createAndAdd(&provider, provider.add(TestSizer { .type = SizerType::Box, .orientation = wxVERTICAL }), wxSizerFlags {});
        auto dump = provider.dump();
        CHECK(dump == std::vector<std::string> {
                  "Create:Sizer[orientation=wxVERTICAL]",
                  "Create:wxNotebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "controller:wxNotebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "Create:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "Create:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "bookPage:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:Page 1:false",
                  "bookPage:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:Page 2:true",
                  "sizer:Sizer[orientation=wxVERTICAL]",
                  "Add:wxNotebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:flags:(0,0x0,0)",
              });
    }

    SECTION("Notebook.page_with_layout")
    {
        TestParent provider;
        auto uut = Notebook {
            BookItem { "Page 1",
                VSizer {
                    Button { "Button 1" },
                    Button { "Button 2" },
                } }
        };
        uut.createAndAdd(&provider, provider.add(TestSizer { .type = SizerType::Box, .orientation = wxVERTICAL }), wxSizerFlags {});
        auto dump = provider.dump();
        CHECK(dump == std::vector<std::string> {
                  "Create:Sizer[orientation=wxVERTICAL]",
                  "Create:wxNotebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "controller:wxNotebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "Create:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "bookPage:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:Page 1:false",
                  "sizer:Sizer[orientation=wxVERTICAL]",
                  "Add:wxNotebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:flags:(0,0x0,0)",
              });
    }

    SECTION("Notebook.with_flags")
    {
        TestParent provider;
        auto uut = Notebook {
            wxSizerFlags { 1 }.Expand(),
            BookItem { "Page 1", Button { "Button 1" } }
        };
        uut.createAndAdd(&provider, provider.add(TestSizer { .type = SizerType::Box, .orientation = wxVERTICAL }), wxSizerFlags {});
        auto dump = provider.dump();
        CHECK(dump == std::vector<std::string> {
                  "Create:Sizer[orientation=wxVERTICAL]",
                  "Create:wxNotebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "controller:wxNotebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "Create:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "bookPage:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:Page 1:false",
                  "sizer:Sizer[orientation=wxVERTICAL]",
                  "Add:wxNotebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:flags:(1,0x2000,0)",
              });
    }

    SECTION("BookItem.string_title")
    {
        TestParent provider;
        auto uut = Notebook {
            BookItem { "Unicode 🐨", Button { "Button" } }
        };
        uut.createAndAdd(&provider, provider.add(TestSizer { .type = SizerType::Box, .orientation = wxVERTICAL }), wxSizerFlags {});
        auto dump = provider.dump();
        CHECK(dump == std::vector<std::string> {
                  "Create:Sizer[orientation=wxVERTICAL]",
                  "Create:wxNotebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "controller:wxNotebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "Create:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "bookPage:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:Unicode 🐨:false",
                  "sizer:Sizer[orientation=wxVERTICAL]",
                  "Add:wxNotebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:flags:(0,0x0,0)",
              });
    }

    SECTION("BookItem.wxString_title")
    {
        TestParent provider;
        auto uut = Notebook {
            BookItem { wxUI_String {}, wxString::FromUTF8("Unicode 🐨"), Button { "Button" } }
        };
        uut.createAndAdd(&provider, provider.add(TestSizer { .type = SizerType::Box, .orientation = wxVERTICAL }), wxSizerFlags {});
        auto dump = provider.dump();
        CHECK(dump == std::vector<std::string> {
                  "Create:Sizer[orientation=wxVERTICAL]",
                  "Create:wxNotebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "controller:wxNotebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "Create:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "bookPage:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:Unicode 🐨:false",
                  "sizer:Sizer[orientation=wxVERTICAL]",
                  "Add:wxNotebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:flags:(0,0x0,0)",
              });
    }
}

TEST_CASE("Choicebook")
{
    SECTION("Choicebook.empty")
    {
        TestParent provider;
        auto uut = Choicebook {};
        uut.createAndAdd(&provider, provider.add(TestSizer { .type = SizerType::Box, .orientation = wxVERTICAL }), wxSizerFlags {});
        auto dump = provider.dump();
        CHECK(dump == std::vector<std::string> {
                  "Create:Sizer[orientation=wxVERTICAL]",
                  "Create:wxChoicebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "controller:wxChoicebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "sizer:Sizer[orientation=wxVERTICAL]",
                  "Add:wxChoicebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:flags:(0,0x0,0)",
              });
    }

    SECTION("Choicebook.single_page")
    {
        TestParent provider;
        auto uut = Choicebook {
            BookItem { "Page 1", Button { "Button 1" } }
        };
        uut.createAndAdd(&provider, provider.add(TestSizer { .type = SizerType::Box, .orientation = wxVERTICAL }), wxSizerFlags {});
        auto dump = provider.dump();
        CHECK(dump == std::vector<std::string> {
                  "Create:Sizer[orientation=wxVERTICAL]",
                  "Create:wxChoicebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "controller:wxChoicebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "Create:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "bookPage:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:Page 1:false",
                  "sizer:Sizer[orientation=wxVERTICAL]",
                  "Add:wxChoicebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:flags:(0,0x0,0)",
              });
    }

    SECTION("Choicebook.multiple_pages")
    {
        TestParent provider;
        auto uut = Choicebook {
            BookItem { "Page 1", Button { "Button 1" } },
            BookItem { "Page 2", Button { "Button 2" } },
        };
        uut.createAndAdd(&provider, provider.add(TestSizer { .type = SizerType::Box, .orientation = wxVERTICAL }), wxSizerFlags {});
        auto dump = provider.dump();
        CHECK(dump == std::vector<std::string> {
                  "Create:Sizer[orientation=wxVERTICAL]",
                  "Create:wxChoicebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "controller:wxChoicebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "Create:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "Create:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "bookPage:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:Page 1:false",
                  "bookPage:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:Page 2:false",
                  "sizer:Sizer[orientation=wxVERTICAL]",
                  "Add:wxChoicebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:flags:(0,0x0,0)",
              });
    }

    SECTION("Choicebook.with_flags")
    {
        TestParent provider;
        auto uut = Choicebook {
            wxSizerFlags { 1 }.Expand(),
            BookItem { "Page 1", Button { "Button 1" } }
        };
        uut.createAndAdd(&provider, provider.add(TestSizer { .type = SizerType::Box, .orientation = wxVERTICAL }), wxSizerFlags {});
        auto dump = provider.dump();
        CHECK(dump == std::vector<std::string> {
                  "Create:Sizer[orientation=wxVERTICAL]",
                  "Create:wxChoicebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "controller:wxChoicebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "Create:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "bookPage:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:Page 1:false",
                  "sizer:Sizer[orientation=wxVERTICAL]",
                  "Add:wxChoicebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:flags:(1,0x2000,0)",
              });
    }
}

TEST_CASE("Listbook")
{
    SECTION("Listbook.empty")
    {
        TestParent provider;
        auto uut = Listbook {};
        uut.createAndAdd(&provider, provider.add(TestSizer { .type = SizerType::Box, .orientation = wxVERTICAL }), wxSizerFlags {});
        auto dump = provider.dump();
        CHECK(dump == std::vector<std::string> {
                  "Create:Sizer[orientation=wxVERTICAL]",
                  "Create:wxListbook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "controller:wxListbook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "sizer:Sizer[orientation=wxVERTICAL]",
                  "Add:wxListbook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:flags:(0,0x0,0)",
              });
    }

    SECTION("Listbook.single_page")
    {
        TestParent provider;
        auto uut = Listbook {
            BookItem { "Page 1", Button { "Button 1" } }
        };
        uut.createAndAdd(&provider, provider.add(TestSizer { .type = SizerType::Box, .orientation = wxVERTICAL }), wxSizerFlags {});
        auto dump = provider.dump();
        CHECK(dump == std::vector<std::string> {
                  "Create:Sizer[orientation=wxVERTICAL]",
                  "Create:wxListbook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "controller:wxListbook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "Create:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "bookPage:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:Page 1:false",
                  "sizer:Sizer[orientation=wxVERTICAL]",
                  "Add:wxListbook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:flags:(0,0x0,0)",
              });
    }

    SECTION("Listbook.multiple_pages")
    {
        TestParent provider;
        auto uut = Listbook {
            BookItem { "Page 1", Button { "Button 1" } },
            BookItem { "Page 2", Button { "Button 2" } },
        };
        uut.createAndAdd(&provider, provider.add(TestSizer { .type = SizerType::Box, .orientation = wxVERTICAL }), wxSizerFlags {});
        auto dump = provider.dump();
        CHECK(dump == std::vector<std::string> {
                  "Create:Sizer[orientation=wxVERTICAL]",
                  "Create:wxListbook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "controller:wxListbook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "Create:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "Create:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "bookPage:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:Page 1:false",
                  "bookPage:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:Page 2:false",
                  "sizer:Sizer[orientation=wxVERTICAL]",
                  "Add:wxListbook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:flags:(0,0x0,0)",
              });
    }

    SECTION("Listbook.with_flags")
    {
        TestParent provider;
        auto uut = Listbook {
            wxSizerFlags { 1 }.Expand(),
            BookItem { "Page 1", Button { "Button 1" } }
        };
        uut.createAndAdd(&provider, provider.add(TestSizer { .type = SizerType::Box, .orientation = wxVERTICAL }), wxSizerFlags {});
        auto dump = provider.dump();
        CHECK(dump == std::vector<std::string> {
                  "Create:Sizer[orientation=wxVERTICAL]",
                  "Create:wxListbook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "controller:wxListbook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "Create:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "bookPage:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:Page 1:false",
                  "sizer:Sizer[orientation=wxVERTICAL]",
                  "Add:wxListbook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:flags:(1,0x2000,0)",
              });
    }
}

TEST_CASE("Simplebook")
{
    SECTION("Simplebook.empty")
    {
        TestParent provider;
        auto uut = Simplebook {};
        uut.createAndAdd(&provider, provider.add(TestSizer { .type = SizerType::Box, .orientation = wxVERTICAL }), wxSizerFlags {});
        auto dump = provider.dump();
        CHECK(dump == std::vector<std::string> {
                  "Create:Sizer[orientation=wxVERTICAL]",
                  "Create:wxSimplebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "controller:wxSimplebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "sizer:Sizer[orientation=wxVERTICAL]",
                  "Add:wxSimplebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:flags:(0,0x0,0)",
              });
    }

    SECTION("Simplebook.single_page")
    {
        TestParent provider;
        auto uut = Simplebook {
            BookItem { "Page 1", Button { "Button 1" } }
        };
        uut.createAndAdd(&provider, provider.add(TestSizer { .type = SizerType::Box, .orientation = wxVERTICAL }), wxSizerFlags {});
        auto dump = provider.dump();
        CHECK(dump == std::vector<std::string> {
                  "Create:Sizer[orientation=wxVERTICAL]",
                  "Create:wxSimplebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "controller:wxSimplebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "Create:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "bookPage:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:Page 1:false",
                  "sizer:Sizer[orientation=wxVERTICAL]",
                  "Add:wxSimplebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:flags:(0,0x0,0)",
              });
    }

    SECTION("Simplebook.multiple_pages")
    {
        TestParent provider;
        auto uut = Simplebook {
            BookItem { "Page 1", Button { "Button 1" } },
            BookItem { "Page 2", Button { "Button 2" } },
        };
        uut.createAndAdd(&provider, provider.add(TestSizer { .type = SizerType::Box, .orientation = wxVERTICAL }), wxSizerFlags {});
        auto dump = provider.dump();
        CHECK(dump == std::vector<std::string> {
                  "Create:Sizer[orientation=wxVERTICAL]",
                  "Create:wxSimplebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "controller:wxSimplebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "Create:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "Create:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "bookPage:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:Page 1:false",
                  "bookPage:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:Page 2:false",
                  "sizer:Sizer[orientation=wxVERTICAL]",
                  "Add:wxSimplebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:flags:(0,0x0,0)",
              });
    }

    SECTION("Simplebook.with_flags")
    {
        TestParent provider;
        auto uut = Simplebook {
            wxSizerFlags { 1 }.Expand(),
            BookItem { "Page 1", Button { "Button 1" } }
        };
        uut.createAndAdd(&provider, provider.add(TestSizer { .type = SizerType::Box, .orientation = wxVERTICAL }), wxSizerFlags {});
        auto dump = provider.dump();
        CHECK(dump == std::vector<std::string> {
                  "Create:Sizer[orientation=wxVERTICAL]",
                  "Create:wxSimplebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "controller:wxSimplebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "Create:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "bookPage:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:Page 1:false",
                  "sizer:Sizer[orientation=wxVERTICAL]",
                  "Add:wxSimplebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:flags:(1,0x2000,0)",
              });
    }
}

TEST_CASE("Treebook")
{
    SECTION("Treebook.empty")
    {
        TestParent provider;
        auto uut = Treebook {};
        uut.createAndAdd(&provider, provider.add(TestSizer { .type = SizerType::Box, .orientation = wxVERTICAL }), wxSizerFlags {});
        auto dump = provider.dump();
        CHECK(dump == std::vector<std::string> {
                  "Create:Sizer[orientation=wxVERTICAL]",
                  "Create:wxTreebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "controller:wxTreebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "sizer:Sizer[orientation=wxVERTICAL]",
                  "Add:wxTreebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:flags:(0,0x0,0)",
              });
    }

    SECTION("Treebook.single_page")
    {
        TestParent provider;
        auto uut = Treebook {
            BookItem { "Page 1", Button { "Button 1" } }
        };
        uut.createAndAdd(&provider, provider.add(TestSizer { .type = SizerType::Box, .orientation = wxVERTICAL }), wxSizerFlags {});
        auto dump = provider.dump();
        CHECK(dump == std::vector<std::string> {
                  "Create:Sizer[orientation=wxVERTICAL]",
                  "Create:wxTreebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "controller:wxTreebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "Create:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "bookPage:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:Page 1:false",
                  "sizer:Sizer[orientation=wxVERTICAL]",
                  "Add:wxTreebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:flags:(0,0x0,0)",
              });
    }

    SECTION("Treebook.multiple_pages")
    {
        TestParent provider;
        auto uut = Treebook {
            BookItem { "Page 1", Button { "Button 1" } },
            BookItem { "Page 2", Button { "Button 2" } },
        };
        uut.createAndAdd(&provider, provider.add(TestSizer { .type = SizerType::Box, .orientation = wxVERTICAL }), wxSizerFlags {});
        auto dump = provider.dump();
        CHECK(dump == std::vector<std::string> {
                  "Create:Sizer[orientation=wxVERTICAL]",
                  "Create:wxTreebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "controller:wxTreebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "Create:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "Create:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "bookPage:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:Page 1:false",
                  "bookPage:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:Page 2:false",
                  "sizer:Sizer[orientation=wxVERTICAL]",
                  "Add:wxTreebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:flags:(0,0x0,0)",
              });
    }

    SECTION("Treebook.with_flags")
    {
        TestParent provider;
        auto uut = Treebook {
            wxSizerFlags { 1 }.Expand(),
            BookItem { "Page 1", Button { "Button 1" } }
        };
        uut.createAndAdd(&provider, provider.add(TestSizer { .type = SizerType::Box, .orientation = wxVERTICAL }), wxSizerFlags {});
        auto dump = provider.dump();
        CHECK(dump == std::vector<std::string> {
                  "Create:Sizer[orientation=wxVERTICAL]",
                  "Create:wxTreebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "controller:wxTreebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "Create:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]",
                  "bookPage:wxWindow[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:Page 1:false",
                  "sizer:Sizer[orientation=wxVERTICAL]",
                  "Add:wxTreebook[id=-1, pos=(-1,-1), size=(-1,-1), style=0]:flags:(1,0x2000,0)",
              });
    }
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity, misc-use-anonymous-namespace, cppcoreguidelines-avoid-do-while)
