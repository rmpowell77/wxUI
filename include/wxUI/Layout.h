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
#pragma once

#include "Widget.h"
#include <variant>
#include <wx/sizer.h>
#include <wx/statbox.h>

namespace wxUI {

namespace details {

    // clang-format off
    template <typename T>
    concept SizerItem = details::CreateAndAddable<T>
        || (std::is_pointer_v<T> && std::derived_from<std::remove_pointer_t<T>, wxSizer>);
    // clang-format on

    template <details::SizerItem... Items>
    struct Sizer {
        explicit Sizer(Items const&... items)
            : Sizer(std::make_tuple(items...))
        {
        }

        explicit Sizer(wxSizerFlags const& flags, Items const&... items)
            : Sizer(flags, std::make_tuple(items...))
        {
        }

        explicit Sizer(std::tuple<Items...> const& items)
            : items(items)
        {
        }

        Sizer(wxSizerFlags const& flags, std::tuple<Items...> const& items)
            : flags(flags)
            , items(items)
        {
        }

        auto createAndAdd(wxWindow* parent, wxSizer* parentSizer, wxSizerFlags const& parentFlags)
        {
            auto sizer = constructSizer(parent);
            auto currentFlags = flags ? *flags : parentFlags;
            createAndAddWidgets(parent, sizer, currentFlags);

            // and now add our sizer to the parent
            parentSizer->Add(sizer, currentFlags);
            return sizer;
        }

        auto attachToWithoutSizeHints(wxWindow* parent) -> auto&
        {
            auto sizer = constructSizer(parent);
            auto currentFlags = flags ? *flags : wxSizerFlags {};
            createAndAddWidgets(parent, sizer, currentFlags);

            parent->SetSizer(sizer);
            return *this;
        }

        auto attachTo(wxWindow* parent) -> auto&
        {
            attachToWithoutSizeHints(parent);
            auto* sizer = parent->GetSizer();
            sizer->SetSizeHints(parent);
            return *this;
        }

    private:
        virtual auto constructSizer(wxWindow* parent) const -> wxSizer* = 0;

        template <typename T>
        static inline auto createAndAddVisiter(T& arg, wxWindow* parent, wxSizer* sizer, wxSizerFlags const& flags)
        {
            if constexpr (details::CreateAndAddable<T>) {
                arg.createAndAdd(parent, sizer, flags);
            } else {
                sizer->Add(arg, flags);
            }
        }

        void createAndAddWidgets(wxWindow* parent, wxSizer* sizer, wxSizerFlags const& flags)
        {
            std::apply([parent, sizer, flags](auto&&... tupleArg) {
                (createAndAddVisiter(tupleArg, parent, sizer, flags), ...);
            },
                items);
        }

        std::optional<wxSizerFlags> flags {};
        std::tuple<Items...> items {};
    };

    template <wxOrientation orientation, details::SizerItem... Items>
    struct SizerBase : Sizer<Items...> {
        using super = Sizer<Items...>;

        explicit SizerBase(Items const&... items)
            : super(std::make_tuple(items...))
        {
        }

        explicit SizerBase(wxString const& caption, Items const&... items)
            : super(std::make_tuple(items...))
            , caption(caption)
        {
        }

        explicit SizerBase(wxSizerFlags const& flags, Items const&... items)
            : super(flags, std::make_tuple(items...))
        {
        }

        SizerBase(wxString const& caption, wxSizerFlags const& flags, Items const&... items)
            : super(flags, std::make_tuple(items...))
            , caption(caption)
        {
        }

        explicit SizerBase(std::tuple<Items...> const& items)
            : super(items)
        {
        }

        SizerBase(wxString const& caption, std::tuple<Items...> const& items)
            : super(items)
            , caption(caption)
        {
        }

        SizerBase(wxSizerFlags const& flags, std::tuple<Items...> const& items)
            : super(flags, items)
        {
        }

        SizerBase(wxString const& caption, wxSizerFlags const& flags, std::tuple<Items...> const& items)
            : super(flags, items)
            , caption(caption)
        {
        }

    private:
        auto constructSizer(wxWindow* parent) const -> wxSizer* override { return caption ? new wxStaticBoxSizer(new wxStaticBox(parent, wxID_ANY, *caption), orientation) : new wxBoxSizer(orientation); }
        std::optional<wxString> caption {};
    };

}

template <details::SizerItem... Items>
struct VSizer : public details::SizerBase<wxVERTICAL, Items...> {
    using super = details::SizerBase<wxVERTICAL, Items...>;

    explicit VSizer(Items const&... items)
        : super(std::make_tuple(items...))
    {
    }

    explicit VSizer(wxString const& caption, Items const&... items)
        : super(caption, std::make_tuple(items...))
    {
    }

    explicit VSizer(wxSizerFlags const& flags, Items const&... items)
        : super(flags, std::make_tuple(items...))
    {
    }

    VSizer(wxString const& caption, wxSizerFlags const& flags, Items const&... items)
        : super(caption, flags, std::make_tuple(items...))
    {
    }

    explicit VSizer(std::tuple<Items...> const& items)
        : super(items)
    {
    }

    VSizer(wxString const& caption, std::tuple<Items...> const& items)
        : super(caption, items)
    {
    }

    VSizer(wxSizerFlags const& flags, std::tuple<Items...> const& items)
        : super(flags, items)
    {
    }

    VSizer(wxString const& caption, wxSizerFlags const& flags, std::tuple<Items...> const& items)
        : super(caption, flags, items)
    {
    }
};

template <details::SizerItem... Items>
struct HSizer : public details::SizerBase<wxHORIZONTAL, Items...> {
    using super = details::SizerBase<wxHORIZONTAL, Items...>;

    explicit HSizer(Items const&... items)
        : super(std::make_tuple(items...))
    {
    }

    explicit HSizer(wxString const& caption, Items const&... items)
        : super(caption, std::make_tuple(items...))
    {
    }

    explicit HSizer(wxSizerFlags const& flags, Items const&... items)
        : super(flags, std::make_tuple(items...))
    {
    }

    HSizer(wxString const& caption, wxSizerFlags const& flags, Items const&... items)
        : super(caption, flags, std::make_tuple(items...))
    {
    }

    explicit HSizer(std::tuple<Items...> const& items)
        : super(items)
    {
    }

    HSizer(wxString const& caption, std::tuple<Items...> const& items)
        : super(caption, items)
    {
    }

    HSizer(wxSizerFlags const& flags, std::tuple<Items...> const& items)
        : super(flags, items)
    {
    }

    HSizer(wxString const& caption, wxSizerFlags const& flags, std::tuple<Items...> const& items)
        : super(caption, flags, items)
    {
    }
};

}

#include "ZapMacros.h"
