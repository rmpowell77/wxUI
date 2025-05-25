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
#pragma once

#include "Widget.hpp"
#include <variant>
#include <wx/sizer.h>
#include <wx/statbox.h>

namespace wxUI {

namespace details {

    template <typename T>
    concept SizerItem = details::CreateAndAddable<T>
        || (std::is_pointer_v<T> && std::derived_from<std::remove_pointer_t<T>, wxSizer>);

    template <typename T>
    static inline auto createAndAddVisiter(T& arg, wxWindow* parent, wxSizer* sizer, wxSizerFlags const& flags)
    {
        if constexpr (details::CreateAndAddable<T>) {
            arg.createAndAdd(parent, sizer, flags);
        } else {
            sizer->Add(arg, flags);
        }
    }

    template <details::SizerItem... Items>
    struct Sizer {
        template <details::SizerItem... UItems>
        explicit Sizer(UItems&&... items)
            : items_(std::forward_as_tuple(std::forward<UItems>(items)...))
        {
        }

        template <details::SizerItem... UItems>
        explicit Sizer(wxSizerFlags const& flags, UItems&&... items)
            : flags_(flags)
            , items_(std::forward_as_tuple(std::forward<UItems>(items)...))
        {
        }

        auto createAndAdd(wxWindow* parent, wxSizer* parentSizer, wxSizerFlags const& parentFlags)
        {
            auto sizer = constructSizer(parent);
            auto currentFlags = flags_.value_or(parentFlags);
            createAndAddWidgets(parent, sizer, currentFlags);

            // and now add our sizer to the parent
            parentSizer->Add(sizer, currentFlags);
            return sizer;
        }

        auto fitToWithoutSizeHints(wxWindow* parent) -> auto&
        {
            auto sizer = constructSizer(parent);
            auto currentFlags = flags_.value_or(wxSizerFlags {});
            createAndAddWidgets(parent, sizer, currentFlags);

            parent->SetSizer(sizer);
            return *this;
        }

        auto fitTo(wxWindow* parent) -> auto&
        {
            fitToWithoutSizeHints(parent);
            auto* sizer = parent->GetSizer();
            sizer->SetSizeHints(parent);
            return *this;
        }

    private:
        virtual auto constructSizer(wxWindow* parent) const -> wxSizer* = 0;

        void createAndAddWidgets(wxWindow* parent, wxSizer* sizer, wxSizerFlags const& flags)
        {
            std::apply([parent, sizer, flags](auto&&... tupleArg) {
                (createAndAddVisiter(tupleArg, parent, sizer, flags), ...);
            },
                items_);
        }

        std::optional<wxSizerFlags> flags_ {};
        std::tuple<Items...> items_ {};
    };

    template <wxOrientation orientation, details::SizerItem... Items>
    struct SizerBase : Sizer<Items...> {
        using super = Sizer<Items...>;

        template <details::SizerItem... UItems>
        explicit SizerBase(UItems&&... items)
            : super(std::forward<UItems>(items)...)
        {
        }

        template <details::SizerItem... UItems>
        explicit SizerBase(wxString const& caption, UItems&&... items)
            : super(std::forward<UItems>(items)...)
            , caption(caption)
        {
        }

        template <details::SizerItem... UItems>
        explicit SizerBase(wxSizerFlags const& flags, UItems&&... items)
            : super(flags, std::forward<UItems>(items)...)
        {
        }

        template <details::SizerItem... UItems>
        SizerBase(wxString const& caption, wxSizerFlags const& flags, UItems&&... items)
            : super(flags, std::forward<UItems>(items)...)
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

    template <details::SizerItem... UItems>
    explicit VSizer(UItems&&... items)
        : super(std::forward<UItems>(items)...)
    {
    }

    template <details::SizerItem... UItems>
    explicit VSizer(wxString const& caption, UItems&&... items)
        : super(caption, std::forward<UItems>(items)...)
    {
    }

    template <details::SizerItem... UItems>
    explicit VSizer(wxSizerFlags const& flags, UItems&&... items)
        : super(flags, std::forward<UItems>(items)...)
    {
    }

    template <details::SizerItem... UItems>
    VSizer(wxString const& caption, wxSizerFlags const& flags, UItems&&... items)
        : super(caption, flags, std::forward<UItems>(items)...)
    {
    }
};

template <details::SizerItem... UItems>
VSizer(UItems&&... items) -> VSizer<UItems...>;

template <details::SizerItem... UItems>
VSizer(wxString const& caption, UItems&&... items) -> VSizer<UItems...>;

template <details::SizerItem... UItems>
VSizer(wxSizerFlags const& flags, UItems&&... items) -> VSizer<UItems...>;

template <details::SizerItem... UItems>
VSizer(wxString const& caption, wxSizerFlags const& flags, UItems&&... items) -> VSizer<UItems...>;

template <details::SizerItem... Items>
struct HSizer : public details::SizerBase<wxHORIZONTAL, Items...> {
    using super = details::SizerBase<wxHORIZONTAL, Items...>;

    template <details::SizerItem... UItems>
    explicit HSizer(UItems&&... items)
        : super(std::forward<UItems>(items)...)
    {
    }

    template <details::SizerItem... UItems>
    explicit HSizer(wxString const& caption, UItems&&... items)
        : super(caption, std::forward<UItems>(items)...)
    {
    }

    template <details::SizerItem... UItems>
    explicit HSizer(wxSizerFlags const& flags, UItems&&... items)
        : super(flags, std::forward<UItems>(items)...)
    {
    }

    template <details::SizerItem... UItems>
    HSizer(wxString const& caption, wxSizerFlags const& flags, UItems&&... items)
        : super(caption, flags, std::forward<UItems>(items)...)
    {
    }
};

template <details::SizerItem... UItems>
HSizer(UItems&&... items) -> HSizer<UItems...>;

template <details::SizerItem... UItems>
HSizer(wxString const& caption, UItems&&... items) -> HSizer<UItems...>;

template <details::SizerItem... UItems>
HSizer(wxSizerFlags const& flags, UItems&&... items) -> HSizer<UItems...>;

template <details::SizerItem... UItems>
HSizer(wxString const& caption, wxSizerFlags const& flags, UItems&&... items) -> HSizer<UItems...>;

template <details::SizerItem... Items>
struct LayoutIf {
    template <details::SizerItem... UItems>
    explicit LayoutIf(bool enabled, UItems&&... items)
    {
        if (enabled) {
            items_ = std::forward_as_tuple(std::forward<UItems>(items)...);
        }
    }
    auto createAndAdd(wxWindow* parent, wxSizer* parentSizer, wxSizerFlags const& parentFlags)
    {
        if (!items_) {
            return;
        }
        createAndAddWidgets(parent, parentSizer, parentFlags);
    }

private:
    void createAndAddWidgets(wxWindow* parent, wxSizer* sizer, wxSizerFlags const& flags)
    {
        std::apply([parent, sizer, flags](auto&&... tupleArg) {
            (createAndAddVisiter(tupleArg, parent, sizer, flags), ...);
        },
            *items_);
    }

    std::optional<std::tuple<Items...>> items_ {};
};

template <details::SizerItem... Item>
LayoutIf(bool, Item... item) -> LayoutIf<Item...>;

}

#include "ZapMacros.hpp"
