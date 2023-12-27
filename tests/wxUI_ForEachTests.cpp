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
#include <ranges>
#include <wxUI/Button.h>
#include <wxUI/ForEach.h>

#include <wx/wx.h>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity)

TEST_CASE("ForEach")
{
    SECTION("ForEach.Vector.T")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto sizer = wxBoxSizer(wxHORIZONTAL);
        auto uut = wxUI::ForEach {
            std::vector { "A", "B", "C" },
            [](auto name) {
                return wxUI::Button { name };
            }
        };
        uut.createAndAdd(&frame, &sizer, wxSizerFlags {});
        auto uut2 = wxUI::VForEach(
            std::vector { "A", "B", "C" },
            [](auto name) {
                return wxUI::Button { name };
            });
        uut2.createAndAdd(&frame, &sizer, wxSizerFlags {});
        auto uut3 = wxUI::HForEach(
            std::vector { "A", "B", "C" },
            [](auto name) {
                return wxUI::Button { name };
            });
        uut3.createAndAdd(&frame, &sizer, wxSizerFlags {});
    }
    SECTION("ForEach.lvalue.Vector.T")
    {
        auto data = std::vector { "A", "B", "C" };
        auto builder = [](auto name) {
            return wxUI::Button { name };
        };
        {
            wxFrame frame { nullptr, wxID_ANY, "" };
            auto sizer = wxBoxSizer(wxHORIZONTAL);
            auto uut = wxUI::ForEach {
                data,
                [](auto name) {
                    return wxUI::Button { name };
                }
            };
            uut.createAndAdd(&frame, &sizer, wxSizerFlags {});
            auto uut2 = wxUI::VForEach(
                data,
                [](auto name) {
                    return wxUI::Button { name };
                });
            uut2.createAndAdd(&frame, &sizer, wxSizerFlags {});
            auto uut3 = wxUI::HForEach(
                data,
                [](auto name) {
                    return wxUI::Button { name };
                });
            uut3.createAndAdd(&frame, &sizer, wxSizerFlags {});
        }
        {
            wxFrame frame { nullptr, wxID_ANY, "" };
            auto sizer = wxBoxSizer(wxHORIZONTAL);
            auto uut = wxUI::ForEach {
                std::vector { "A", "B", "C" },
                builder
            };
            uut.createAndAdd(&frame, &sizer, wxSizerFlags {});
            auto uut2 = wxUI::VForEach(
                std::vector { "A", "B", "C" },
                builder);
            uut2.createAndAdd(&frame, &sizer, wxSizerFlags {});
            auto uut3 = wxUI::HForEach(
                std::vector { "A", "B", "C" },
                builder);
            uut3.createAndAdd(&frame, &sizer, wxSizerFlags {});
        }
        {
            wxFrame frame { nullptr, wxID_ANY, "" };
            auto sizer = wxBoxSizer(wxHORIZONTAL);
            auto uut = wxUI::ForEach { data, builder };
            uut.createAndAdd(&frame, &sizer, wxSizerFlags {});
            auto uut2 = wxUI::VForEach(data, builder);
            uut2.createAndAdd(&frame, &sizer, wxSizerFlags {});
            auto uut3 = wxUI::HForEach(data, builder);
            uut3.createAndAdd(&frame, &sizer, wxSizerFlags {});
        }
    }
    SECTION("ForEach.Vector.Tuple")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto sizer = wxBoxSizer(wxHORIZONTAL);
        auto uut = wxUI::ForEach {
            std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" }, { wxID_APPLY, "C" } },
            [](auto name) {
                return wxUI::Button { std::get<0>(name), std::get<1>(name) };
            }
        };
        uut.createAndAdd(&frame, &sizer, wxSizerFlags {});
        auto uut2 = wxUI::VForEach(
            std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" }, { wxID_APPLY, "C" } },
            [](auto name) {
                return wxUI::Button { std::get<0>(name), std::get<1>(name) };
            });
        uut2.createAndAdd(&frame, &sizer, wxSizerFlags {});
        auto uut3 = wxUI::HForEach(
            std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" }, { wxID_APPLY, "C" } },
            [](auto name) {
                return wxUI::Button { std::get<0>(name), std::get<1>(name) };
            });
        uut3.createAndAdd(&frame, &sizer, wxSizerFlags {});
    }
    SECTION("ForEach.Vector.Tuple.Multiarg")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto sizer = wxBoxSizer(wxHORIZONTAL);
        auto uut = wxUI::ForEach {
            std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" }, { wxID_APPLY, "C" } },
            [](auto identity, auto name) {
                return wxUI::Button { identity, name };
            }
        };
        uut.createAndAdd(&frame, &sizer, wxSizerFlags {});
        auto uut2 = wxUI::VForEach(
            std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" }, { wxID_APPLY, "C" } },
            [](auto identity, auto name) {
                return wxUI::Button { identity, name };
            });
        uut2.createAndAdd(&frame, &sizer, wxSizerFlags {});
        auto uut3 = wxUI::HForEach(
            std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" }, { wxID_APPLY, "C" } },
            [](auto identity, auto name) {
                return wxUI::Button { identity, name };
            });
        uut3.createAndAdd(&frame, &sizer, wxSizerFlags {});
    }
    SECTION("ForEach.Range.T")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto sizer = wxBoxSizer(wxHORIZONTAL);
        auto uut = wxUI::ForEach {
            std::vector { "A", "B", "C" } | std::views::filter([](auto str) { return str[0] == 'A'; }),
            [](auto name) {
                return wxUI::Button { name };
            }
        };
        uut.createAndAdd(&frame, &sizer, wxSizerFlags {});
        auto uut2 = wxUI::VForEach(
            std::vector { "A", "B", "C" } | std::views::filter([](auto str) { return str[0] == 'A'; }),
            [](auto name) {
                return wxUI::Button { name };
            });
        uut2.createAndAdd(&frame, &sizer, wxSizerFlags {});
        auto uut3 = wxUI::HForEach(
            std::vector { "A", "B", "C" } | std::views::filter([](auto str) { return str[0] == 'A'; }),
            [](auto name) {
                return wxUI::Button { name };
            });
        uut3.createAndAdd(&frame, &sizer, wxSizerFlags {});
    }
    SECTION("ForEach.lvalue.Range.T")
    {
        auto data = std::vector { "A", "B", "C" } | std::views::filter([](auto str) { return str[0] == 'A'; });
        auto builder = [](auto name) {
            return wxUI::Button { name };
        };
        {
            wxFrame frame { nullptr, wxID_ANY, "" };
            auto sizer = wxBoxSizer(wxHORIZONTAL);
            auto uut = wxUI::ForEach {
                data,
                [](auto name) {
                    return wxUI::Button { name };
                }
            };
            uut.createAndAdd(&frame, &sizer, wxSizerFlags {});
            auto uut2 = wxUI::VForEach(
                data,
                [](auto name) {
                    return wxUI::Button { name };
                });
            uut2.createAndAdd(&frame, &sizer, wxSizerFlags {});
            auto uut3 = wxUI::HForEach(
                data,
                [](auto name) {
                    return wxUI::Button { name };
                });
            uut3.createAndAdd(&frame, &sizer, wxSizerFlags {});
        }
        {
            wxFrame frame { nullptr, wxID_ANY, "" };
            auto sizer = wxBoxSizer(wxHORIZONTAL);
            auto uut = wxUI::ForEach {
                std::vector { "A", "B", "C" },
                builder
            };
            uut.createAndAdd(&frame, &sizer, wxSizerFlags {});
            auto uut2 = wxUI::VForEach(
                std::vector { "A", "B", "C" },
                builder);
            uut2.createAndAdd(&frame, &sizer, wxSizerFlags {});
            auto uut3 = wxUI::HForEach(
                std::vector { "A", "B", "C" },
                builder);
            uut3.createAndAdd(&frame, &sizer, wxSizerFlags {});
        }
        {
            wxFrame frame { nullptr, wxID_ANY, "" };
            auto sizer = wxBoxSizer(wxHORIZONTAL);
            auto uut = wxUI::ForEach { data, builder };
            uut.createAndAdd(&frame, &sizer, wxSizerFlags {});
            auto uut2 = wxUI::VForEach(data, builder);
            uut2.createAndAdd(&frame, &sizer, wxSizerFlags {});
            auto uut3 = wxUI::HForEach(data, builder);
            uut3.createAndAdd(&frame, &sizer, wxSizerFlags {});
        }
    }
    SECTION("ForEach.Range.Tuple")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto sizer = wxBoxSizer(wxHORIZONTAL);
        auto uut = wxUI::ForEach {
            std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" }, { wxID_APPLY, "C" } } | std::views::filter([](auto item) { return std::get<1>(item)[0] == 'A'; }),
            [](auto name) {
                return wxUI::Button { std::get<0>(name), std::get<1>(name) };
            }
        };
        uut.createAndAdd(&frame, &sizer, wxSizerFlags {});
        auto uut2 = wxUI::VForEach(
            std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" }, { wxID_APPLY, "C" } } | std::views::filter([](auto item) { return std::get<1>(item)[0] == 'A'; }),
            [](auto name) {
                return wxUI::Button { std::get<0>(name), std::get<1>(name) };
            });
        uut2.createAndAdd(&frame, &sizer, wxSizerFlags {});
        auto uut3 = wxUI::HForEach(
            std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" }, { wxID_APPLY, "C" } } | std::views::filter([](auto item) { return std::get<1>(item)[0] == 'A'; }),
            [](auto name) {
                return wxUI::Button { std::get<0>(name), std::get<1>(name) };
            });
        uut3.createAndAdd(&frame, &sizer, wxSizerFlags {});
    }
    SECTION("ForEach.Range.Tuple.Multiarg")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto sizer = wxBoxSizer(wxHORIZONTAL);
        auto uut = wxUI::ForEach {
            std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" }, { wxID_APPLY, "C" } } | std::views::filter([](auto item) { return std::get<1>(item)[0] == 'A'; }),
            [](auto identity, auto name) {
                return wxUI::Button { identity, name };
            }
        };
        uut.createAndAdd(&frame, &sizer, wxSizerFlags {});
        auto uut2 = wxUI::VForEach(
            std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" }, { wxID_APPLY, "C" } } | std::views::filter([](auto item) { return std::get<1>(item)[0] == 'A'; }),
            [](auto identity, auto name) {
                return wxUI::Button { identity, name };
            });
        uut2.createAndAdd(&frame, &sizer, wxSizerFlags {});
        auto uut3 = wxUI::HForEach(
            std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" }, { wxID_APPLY, "C" } } | std::views::filter([](auto item) { return std::get<1>(item)[0] == 'A'; }),
            [](auto identity, auto name) {
                return wxUI::Button { identity, name };
            });
        uut3.createAndAdd(&frame, &sizer, wxSizerFlags {});
    }
    SECTION("ForEach.InitializerList.T")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto sizer = wxBoxSizer(wxHORIZONTAL);
        auto uut = wxUI::ForEach {
            { "A", "B", "C" },
            [](auto name) {
                return wxUI::Button { name };
            }
        };
        uut.createAndAdd(&frame, &sizer, wxSizerFlags {});
        auto uut2 = wxUI::VForEach(
            { "A", "B", "C" },
            [](auto name) {
                return wxUI::Button { name };
            });
        uut2.createAndAdd(&frame, &sizer, wxSizerFlags {});
        auto uut3 = wxUI::HForEach(
            { "A", "B", "C" },
            [](auto name) {
                return wxUI::Button { name };
            });
        uut3.createAndAdd(&frame, &sizer, wxSizerFlags {});
    }
    SECTION("ForEach.lvalue.InitializerList.T")
    {
        auto data = { "A", "B", "C" };
        auto builder = [](auto name) {
            return wxUI::Button { name };
        };
        {
            wxFrame frame { nullptr, wxID_ANY, "" };
            auto sizer = wxBoxSizer(wxHORIZONTAL);
            auto uut = wxUI::ForEach {
                data,
                [](auto name) {
                    return wxUI::Button { name };
                }
            };
            uut.createAndAdd(&frame, &sizer, wxSizerFlags {});
            auto uut2 = wxUI::VForEach(
                data,
                [](auto name) {
                    return wxUI::Button { name };
                });
            uut2.createAndAdd(&frame, &sizer, wxSizerFlags {});
            auto uut3 = wxUI::HForEach(
                data,
                [](auto name) {
                    return wxUI::Button { name };
                });
            uut3.createAndAdd(&frame, &sizer, wxSizerFlags {});
        }
        {
            wxFrame frame { nullptr, wxID_ANY, "" };
            auto sizer = wxBoxSizer(wxHORIZONTAL);
            auto uut = wxUI::ForEach {
                std::vector { "A", "B", "C" },
                builder
            };
            uut.createAndAdd(&frame, &sizer, wxSizerFlags {});
            auto uut2 = wxUI::VForEach(
                std::vector { "A", "B", "C" },
                builder);
            uut2.createAndAdd(&frame, &sizer, wxSizerFlags {});
            auto uut3 = wxUI::HForEach(
                std::vector { "A", "B", "C" },
                builder);
            uut3.createAndAdd(&frame, &sizer, wxSizerFlags {});
        }
        {
            wxFrame frame { nullptr, wxID_ANY, "" };
            auto sizer = wxBoxSizer(wxHORIZONTAL);
            auto uut = wxUI::ForEach { data, builder };
            uut.createAndAdd(&frame, &sizer, wxSizerFlags {});
            auto uut2 = wxUI::VForEach(data, builder);
            uut2.createAndAdd(&frame, &sizer, wxSizerFlags {});
            auto uut3 = wxUI::HForEach(data, builder);
            uut3.createAndAdd(&frame, &sizer, wxSizerFlags {});
        }
    }
    SECTION("ForEach.InitializerList.Tuple")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto sizer = wxBoxSizer(wxHORIZONTAL);
        auto uut = wxUI::ForEach {
            { std::tuple<wxWindowID, std::string> { wxID_CANCEL, "A" }, std::tuple<wxWindowID, std::string> { wxID_OK, "B" }, std::tuple<wxWindowID, std::string> { wxID_APPLY, "C" } },
            [](auto name) {
                return wxUI::Button { std::get<0>(name), std::get<1>(name) };
            }
        };
        uut.createAndAdd(&frame, &sizer, wxSizerFlags {});
        auto uut2 = wxUI::VForEach(
            { std::tuple<wxWindowID, std::string> { wxID_CANCEL, "A" }, std::tuple<wxWindowID, std::string> { wxID_OK, "B" }, std::tuple<wxWindowID, std::string> { wxID_APPLY, "C" } },
            [](auto name) {
                return wxUI::Button { std::get<0>(name), std::get<1>(name) };
            });
        uut2.createAndAdd(&frame, &sizer, wxSizerFlags {});
        auto uut3 = wxUI::HForEach(
            { std::tuple<wxWindowID, std::string> { wxID_CANCEL, "A" }, std::tuple<wxWindowID, std::string> { wxID_OK, "B" }, std::tuple<wxWindowID, std::string> { wxID_APPLY, "C" } },
            [](auto name) {
                return wxUI::Button { std::get<0>(name), std::get<1>(name) };
            });
        uut3.createAndAdd(&frame, &sizer, wxSizerFlags {});
    }
    SECTION("ForEach.InitializerList.Tuple.Multiarg")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto sizer = wxBoxSizer(wxHORIZONTAL);
        auto uut = wxUI::ForEach {
            { std::tuple<wxWindowID, std::string> { wxID_CANCEL, "A" }, std::tuple<wxWindowID, std::string> { wxID_OK, "B" }, std::tuple<wxWindowID, std::string> { wxID_APPLY, "C" } },
            [](auto identity, auto name) {
                return wxUI::Button { identity, name };
            }
        };
        uut.createAndAdd(&frame, &sizer, wxSizerFlags {});
        auto uut2 = wxUI::VForEach(
            { std::tuple<wxWindowID, std::string> { wxID_CANCEL, "A" }, std::tuple<wxWindowID, std::string> { wxID_OK, "B" }, std::tuple<wxWindowID, std::string> { wxID_APPLY, "C" } },
            [](auto identity, auto name) {
                return wxUI::Button { identity, name };
            });
        uut2.createAndAdd(&frame, &sizer, wxSizerFlags {});
        auto uut3 = wxUI::HForEach(
            { std::tuple<wxWindowID, std::string> { wxID_CANCEL, "A" }, std::tuple<wxWindowID, std::string> { wxID_OK, "B" }, std::tuple<wxWindowID, std::string> { wxID_APPLY, "C" } },
            [](auto identity, auto name) {
                return wxUI::Button { identity, name };
            });
        uut3.createAndAdd(&frame, &sizer, wxSizerFlags {});
    }
    SECTION("ForEach.Flags.Vector.T")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto sizer = wxBoxSizer(wxHORIZONTAL);
        auto uut = wxUI::ForEach {
            wxSizerFlags {},
            std::vector { "A", "B", "C" },
            [](auto name) {
                return wxUI::Button { name };
            }
        };
        uut.createAndAdd(&frame, &sizer, wxSizerFlags {});
        auto uut2 = wxUI::VForEach(
            wxSizerFlags {},
            std::vector { "A", "B", "C" },
            [](auto name) {
                return wxUI::Button { name };
            });
        uut2.createAndAdd(&frame, &sizer, wxSizerFlags {});
        auto uut3 = wxUI::HForEach(
            wxSizerFlags {},
            std::vector { "A", "B", "C" },
            [](auto name) {
                return wxUI::Button { name };
            });
        uut3.createAndAdd(&frame, &sizer, wxSizerFlags {});
    }
    SECTION("ForEach.Flags.lvalue.Vector.T")
    {
        auto data = std::vector { "A", "B", "C" };
        auto builder = [](auto name) {
            return wxUI::Button { name };
        };
        {
            wxFrame frame { nullptr, wxID_ANY, "" };
            auto sizer = wxBoxSizer(wxHORIZONTAL);
            auto uut = wxUI::ForEach {
                wxSizerFlags {},
                data,
                [](auto name) {
                    return wxUI::Button { name };
                }
            };
            uut.createAndAdd(&frame, &sizer, wxSizerFlags {});
            auto uut2 = wxUI::VForEach(
                wxSizerFlags {},
                data,
                [](auto name) {
                    return wxUI::Button { name };
                });
            uut2.createAndAdd(&frame, &sizer, wxSizerFlags {});
            auto uut3 = wxUI::HForEach(
                wxSizerFlags {},
                data,
                [](auto name) {
                    return wxUI::Button { name };
                });
            uut3.createAndAdd(&frame, &sizer, wxSizerFlags {});
        }
        {
            wxFrame frame { nullptr, wxID_ANY, "" };
            auto sizer = wxBoxSizer(wxHORIZONTAL);
            auto uut = wxUI::ForEach {
                wxSizerFlags {},
                std::vector { "A", "B", "C" },
                builder
            };
            uut.createAndAdd(&frame, &sizer, wxSizerFlags {});
            auto uut2 = wxUI::VForEach(
                wxSizerFlags {},
                std::vector { "A", "B", "C" },
                builder);
            uut2.createAndAdd(&frame, &sizer, wxSizerFlags {});
            auto uut3 = wxUI::HForEach(
                wxSizerFlags {},
                std::vector { "A", "B", "C" },
                builder);
            uut3.createAndAdd(&frame, &sizer, wxSizerFlags {});
        }
        {
            wxFrame frame { nullptr, wxID_ANY, "" };
            auto sizer = wxBoxSizer(wxHORIZONTAL);
            auto uut = wxUI::ForEach { wxSizerFlags {}, data, builder };
            uut.createAndAdd(&frame, &sizer, wxSizerFlags {});
            auto uut2 = wxUI::VForEach(wxSizerFlags {}, data, builder);
            uut2.createAndAdd(&frame, &sizer, wxSizerFlags {});
            auto uut3 = wxUI::HForEach(wxSizerFlags {}, data, builder);
            uut3.createAndAdd(&frame, &sizer, wxSizerFlags {});
        }
    }
    SECTION("ForEach.Flags.Vector.Tuple")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto sizer = wxBoxSizer(wxHORIZONTAL);
        auto uut = wxUI::ForEach {
            wxSizerFlags {},
            std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" }, { wxID_APPLY, "C" } },
            [](auto name) {
                return wxUI::Button { std::get<0>(name), std::get<1>(name) };
            }
        };
        uut.createAndAdd(&frame, &sizer, wxSizerFlags {});
        auto uut2 = wxUI::VForEach(
            wxSizerFlags {},
            std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" }, { wxID_APPLY, "C" } },
            [](auto name) {
                return wxUI::Button { std::get<0>(name), std::get<1>(name) };
            });
        uut2.createAndAdd(&frame, &sizer, wxSizerFlags {});
        auto uut3 = wxUI::HForEach(
            wxSizerFlags {},
            std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" }, { wxID_APPLY, "C" } },
            [](auto name) {
                return wxUI::Button { std::get<0>(name), std::get<1>(name) };
            });
        uut3.createAndAdd(&frame, &sizer, wxSizerFlags {});
    }
    SECTION("ForEach.Flags.Vector.Tuple.Multiarg")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto sizer = wxBoxSizer(wxHORIZONTAL);
        auto uut = wxUI::ForEach {
            wxSizerFlags {},
            std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" }, { wxID_APPLY, "C" } },
            [](auto identity, auto name) {
                return wxUI::Button { identity, name };
            }
        };
        uut.createAndAdd(&frame, &sizer, wxSizerFlags {});
        auto uut2 = wxUI::VForEach(
            wxSizerFlags {},
            std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" }, { wxID_APPLY, "C" } },
            [](auto identity, auto name) {
                return wxUI::Button { identity, name };
            });
        uut2.createAndAdd(&frame, &sizer, wxSizerFlags {});
        auto uut3 = wxUI::HForEach(
            wxSizerFlags {},
            std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" }, { wxID_APPLY, "C" } },
            [](auto identity, auto name) {
                return wxUI::Button { identity, name };
            });
        uut3.createAndAdd(&frame, &sizer, wxSizerFlags {});
    }
    SECTION("ForEach.Flags.Range.T")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto sizer = wxBoxSizer(wxHORIZONTAL);
        auto uut = wxUI::ForEach {
            wxSizerFlags {},
            std::vector { "A", "B", "C" } | std::views::filter([](auto str) { return str[0] == 'A'; }),
            [](auto name) {
                return wxUI::Button { name };
            }
        };
        uut.createAndAdd(&frame, &sizer, wxSizerFlags {});
        auto uut2 = wxUI::VForEach(
            wxSizerFlags {},
            std::vector { "A", "B", "C" } | std::views::filter([](auto str) { return str[0] == 'A'; }),
            [](auto name) {
                return wxUI::Button { name };
            });
        uut2.createAndAdd(&frame, &sizer, wxSizerFlags {});
        auto uut3 = wxUI::HForEach(
            wxSizerFlags {},
            std::vector { "A", "B", "C" } | std::views::filter([](auto str) { return str[0] == 'A'; }),
            [](auto name) {
                return wxUI::Button { name };
            });
        uut3.createAndAdd(&frame, &sizer, wxSizerFlags {});
    }
    SECTION("ForEach.Flags.lvalue.Range.T")
    {
        auto data = std::vector { "A", "B", "C" } | std::views::filter([](auto str) { return str[0] == 'A'; });
        auto builder = [](auto name) {
            return wxUI::Button { name };
        };
        {
            wxFrame frame { nullptr, wxID_ANY, "" };
            auto sizer = wxBoxSizer(wxHORIZONTAL);
            auto uut = wxUI::ForEach {
                wxSizerFlags {},
                data,
                [](auto name) {
                    return wxUI::Button { name };
                }
            };
            uut.createAndAdd(&frame, &sizer, wxSizerFlags {});
            auto uut2 = wxUI::VForEach(
                wxSizerFlags {},
                data,
                [](auto name) {
                    return wxUI::Button { name };
                });
            uut2.createAndAdd(&frame, &sizer, wxSizerFlags {});
            auto uut3 = wxUI::HForEach(
                wxSizerFlags {},
                data,
                [](auto name) {
                    return wxUI::Button { name };
                });
            uut3.createAndAdd(&frame, &sizer, wxSizerFlags {});
        }
        {
            wxFrame frame { nullptr, wxID_ANY, "" };
            auto sizer = wxBoxSizer(wxHORIZONTAL);
            auto uut = wxUI::ForEach {
                wxSizerFlags {},
                std::vector { "A", "B", "C" },
                builder
            };
            uut.createAndAdd(&frame, &sizer, wxSizerFlags {});
            auto uut2 = wxUI::VForEach(
                wxSizerFlags {},
                std::vector { "A", "B", "C" },
                builder);
            uut2.createAndAdd(&frame, &sizer, wxSizerFlags {});
            auto uut3 = wxUI::HForEach(
                wxSizerFlags {},
                std::vector { "A", "B", "C" },
                builder);
            uut3.createAndAdd(&frame, &sizer, wxSizerFlags {});
        }
        {
            wxFrame frame { nullptr, wxID_ANY, "" };
            auto sizer = wxBoxSizer(wxHORIZONTAL);
            auto uut = wxUI::ForEach { wxSizerFlags {}, data, builder };
            uut.createAndAdd(&frame, &sizer, wxSizerFlags {});
            auto uut2 = wxUI::VForEach(wxSizerFlags {}, data, builder);
            uut2.createAndAdd(&frame, &sizer, wxSizerFlags {});
            auto uut3 = wxUI::HForEach(wxSizerFlags {}, data, builder);
            uut3.createAndAdd(&frame, &sizer, wxSizerFlags {});
        }
    }
    SECTION("ForEach.Flags.Range.Tuple")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto sizer = wxBoxSizer(wxHORIZONTAL);
        auto uut = wxUI::ForEach {
            wxSizerFlags {},
            std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" }, { wxID_APPLY, "C" } } | std::views::filter([](auto item) { return std::get<1>(item)[0] == 'A'; }),
            [](auto name) {
                return wxUI::Button { std::get<0>(name), std::get<1>(name) };
            }
        };
        uut.createAndAdd(&frame, &sizer, wxSizerFlags {});
        auto uut2 = wxUI::VForEach(
            wxSizerFlags {},
            std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" }, { wxID_APPLY, "C" } } | std::views::filter([](auto item) { return std::get<1>(item)[0] == 'A'; }),
            [](auto name) {
                return wxUI::Button { std::get<0>(name), std::get<1>(name) };
            });
        uut2.createAndAdd(&frame, &sizer, wxSizerFlags {});
        auto uut3 = wxUI::HForEach(
            wxSizerFlags {},
            std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" }, { wxID_APPLY, "C" } } | std::views::filter([](auto item) { return std::get<1>(item)[0] == 'A'; }),
            [](auto name) {
                return wxUI::Button { std::get<0>(name), std::get<1>(name) };
            });
        uut3.createAndAdd(&frame, &sizer, wxSizerFlags {});
    }
    SECTION("ForEach.Flags.Range.Tuple.Multiarg")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto sizer = wxBoxSizer(wxHORIZONTAL);
        auto uut = wxUI::ForEach {
            wxSizerFlags {},
            std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" }, { wxID_APPLY, "C" } } | std::views::filter([](auto item) { return std::get<1>(item)[0] == 'A'; }),
            [](auto identity, auto name) {
                return wxUI::Button { identity, name };
            }
        };
        uut.createAndAdd(&frame, &sizer, wxSizerFlags {});
        auto uut2 = wxUI::VForEach(
            wxSizerFlags {},
            std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" }, { wxID_APPLY, "C" } } | std::views::filter([](auto item) { return std::get<1>(item)[0] == 'A'; }),
            [](auto identity, auto name) {
                return wxUI::Button { identity, name };
            });
        uut2.createAndAdd(&frame, &sizer, wxSizerFlags {});
        auto uut3 = wxUI::HForEach(
            wxSizerFlags {},
            std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" }, { wxID_APPLY, "C" } } | std::views::filter([](auto item) { return std::get<1>(item)[0] == 'A'; }),
            [](auto identity, auto name) {
                return wxUI::Button { identity, name };
            });
        uut3.createAndAdd(&frame, &sizer, wxSizerFlags {});
    }
    SECTION("ForEach.Flags.InitializerList.T")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto sizer = wxBoxSizer(wxHORIZONTAL);
        auto uut = wxUI::ForEach {
            wxSizerFlags {},
            { "A", "B", "C" },
            [](auto name) {
                return wxUI::Button { name };
            }
        };
        uut.createAndAdd(&frame, &sizer, wxSizerFlags {});
        auto uut2 = wxUI::VForEach(
            wxSizerFlags {},
            { "A", "B", "C" },
            [](auto name) {
                return wxUI::Button { name };
            });
        uut2.createAndAdd(&frame, &sizer, wxSizerFlags {});
        auto uut3 = wxUI::HForEach(
            wxSizerFlags {},
            { "A", "B", "C" },
            [](auto name) {
                return wxUI::Button { name };
            });
        uut3.createAndAdd(&frame, &sizer, wxSizerFlags {});
    }
    SECTION("ForEach.Flags.lvalue.InitializerList.T")
    {
        auto data = { "A", "B", "C" };
        auto builder = [](auto name) {
            return wxUI::Button { name };
        };
        {
            wxFrame frame { nullptr, wxID_ANY, "" };
            auto sizer = wxBoxSizer(wxHORIZONTAL);
            auto uut = wxUI::ForEach {
                wxSizerFlags {},
                data,
                [](auto name) {
                    return wxUI::Button { name };
                }
            };
            uut.createAndAdd(&frame, &sizer, wxSizerFlags {});
            auto uut2 = wxUI::VForEach(
                wxSizerFlags {},
                data,
                [](auto name) {
                    return wxUI::Button { name };
                });
            uut2.createAndAdd(&frame, &sizer, wxSizerFlags {});
            auto uut3 = wxUI::HForEach(
                wxSizerFlags {},
                data,
                [](auto name) {
                    return wxUI::Button { name };
                });
            uut3.createAndAdd(&frame, &sizer, wxSizerFlags {});
        }
        {
            wxFrame frame { nullptr, wxID_ANY, "" };
            auto sizer = wxBoxSizer(wxHORIZONTAL);
            auto uut = wxUI::ForEach {
                wxSizerFlags {},
                std::vector { "A", "B", "C" },
                builder
            };
            uut.createAndAdd(&frame, &sizer, wxSizerFlags {});
            auto uut2 = wxUI::VForEach(
                wxSizerFlags {},
                std::vector { "A", "B", "C" },
                builder);
            uut2.createAndAdd(&frame, &sizer, wxSizerFlags {});
            auto uut3 = wxUI::HForEach(
                wxSizerFlags {},
                std::vector { "A", "B", "C" },
                builder);
            uut3.createAndAdd(&frame, &sizer, wxSizerFlags {});
        }
        {
            wxFrame frame { nullptr, wxID_ANY, "" };
            auto sizer = wxBoxSizer(wxHORIZONTAL);
            auto uut = wxUI::ForEach { wxSizerFlags {}, data, builder };
            uut.createAndAdd(&frame, &sizer, wxSizerFlags {});
            auto uut2 = wxUI::VForEach(wxSizerFlags {}, data, builder);
            uut2.createAndAdd(&frame, &sizer, wxSizerFlags {});
            auto uut3 = wxUI::HForEach(wxSizerFlags {}, data, builder);
            uut3.createAndAdd(&frame, &sizer, wxSizerFlags {});
        }
    }
    SECTION("ForEach.Flags.InitializerList.Tuple")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto sizer = wxBoxSizer(wxHORIZONTAL);
        auto uut = wxUI::ForEach {
            wxSizerFlags {},
            { std::tuple<wxWindowID, std::string> { wxID_CANCEL, "A" }, std::tuple<wxWindowID, std::string> { wxID_OK, "B" }, std::tuple<wxWindowID, std::string> { wxID_APPLY, "C" } },
            [](auto name) {
                return wxUI::Button { std::get<0>(name), std::get<1>(name) };
            }
        };
        uut.createAndAdd(&frame, &sizer, wxSizerFlags {});
        auto uut2 = wxUI::VForEach(
            wxSizerFlags {},
            { std::tuple<wxWindowID, std::string> { wxID_CANCEL, "A" }, std::tuple<wxWindowID, std::string> { wxID_OK, "B" }, std::tuple<wxWindowID, std::string> { wxID_APPLY, "C" } },
            [](auto name) {
                return wxUI::Button { std::get<0>(name), std::get<1>(name) };
            });
        uut2.createAndAdd(&frame, &sizer, wxSizerFlags {});
        auto uut3 = wxUI::HForEach(
            wxSizerFlags {},
            { std::tuple<wxWindowID, std::string> { wxID_CANCEL, "A" }, std::tuple<wxWindowID, std::string> { wxID_OK, "B" }, std::tuple<wxWindowID, std::string> { wxID_APPLY, "C" } },
            [](auto name) {
                return wxUI::Button { std::get<0>(name), std::get<1>(name) };
            });
        uut3.createAndAdd(&frame, &sizer, wxSizerFlags {});
    }
    SECTION("ForEach.Flags.InitializerList.Tuple.Multiarg")
    {
        wxFrame frame { nullptr, wxID_ANY, "" };
        auto sizer = wxBoxSizer(wxHORIZONTAL);
        auto uut = wxUI::ForEach {
            wxSizerFlags {},
            { std::tuple<wxWindowID, std::string> { wxID_CANCEL, "A" }, std::tuple<wxWindowID, std::string> { wxID_OK, "B" }, std::tuple<wxWindowID, std::string> { wxID_APPLY, "C" } },
            [](auto identity, auto name) {
                return wxUI::Button { identity, name };
            }
        };
        uut.createAndAdd(&frame, &sizer, wxSizerFlags {});
        auto uut2 = wxUI::VForEach(
            wxSizerFlags {},
            { std::tuple<wxWindowID, std::string> { wxID_CANCEL, "A" }, std::tuple<wxWindowID, std::string> { wxID_OK, "B" }, std::tuple<wxWindowID, std::string> { wxID_APPLY, "C" } },
            [](auto identity, auto name) {
                return wxUI::Button { identity, name };
            });
        uut2.createAndAdd(&frame, &sizer, wxSizerFlags {});
        auto uut3 = wxUI::HForEach(
            wxSizerFlags {},
            { std::tuple<wxWindowID, std::string> { wxID_CANCEL, "A" }, std::tuple<wxWindowID, std::string> { wxID_OK, "B" }, std::tuple<wxWindowID, std::string> { wxID_APPLY, "C" } },
            [](auto identity, auto name) {
                return wxUI::Button { identity, name };
            });
        uut3.createAndAdd(&frame, &sizer, wxSizerFlags {});
    }
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity)
