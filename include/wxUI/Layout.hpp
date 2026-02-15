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
#pragma once

#include "HelperMacros.hpp"
#include "Widget.hpp"
#include "wxUITypes.hpp"
#include <variant>
#include <wx/sizer.h>
#include <wx/statbox.h>

// forward-declare Wrapper so we can detect it in traits without including its header
namespace wxUI {
template <typename Window>
struct Wrapper;
}

namespace wxUI::details {

// clang-format off
template <typename T>
struct is_Wrapper : std::false_type {};
template <typename W>
struct is_Wrapper<::wxUI::Wrapper<W>> : std::true_type {};
template <typename T>
inline constexpr bool is_Wrapper_v = is_Wrapper<T>::value;

template <typename T>
concept SizerItem = details::CreateAndAddable<T>
    || is_Wrapper_v<T>
    || (std::is_pointer_v<T> && std::derived_from<std::remove_pointer_t<T>, wxSizer>);
// clang-format on

template <typename T, typename Parent, typename Sizer>
static inline auto createAndAddVisiter(T& arg, Parent* parent, Sizer* sizer, wxSizerFlags const& flags)
{
    if constexpr (details::CreateAndAddable<T>) {
        arg.createAndAdd(parent, sizer, flags);
    } else if constexpr (std::is_pointer_v<T> && std::derived_from<std::remove_pointer_t<T>, wxSizer>) {
        sizer->Add(arg, flags);
    } else if constexpr (is_Wrapper_v<T>) {
        sizer->Add(arg.create(), flags);
    } else {
        static_assert(always_false_v<T>, "createAndAdd not available for this item with these Parent/Sizer types; provide a customization");
    }
}
}

namespace wxUI {

using SizerProxy = details::Proxy<wxSizer>;

template <wxOrientation orientation, details::SizerItem... Items>
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

    template <details::SizerItem... UItems>
    explicit Sizer(std::string_view caption, UItems&&... items)
        : items_(std::forward_as_tuple(std::forward<UItems>(items)...))
        , caption(wxString::FromUTF8(caption.data(), caption.size()))
    {
    }

    template <details::SizerItem... UItems>
    explicit Sizer(wxUI_String, wxString caption, UItems&&... items)
        : items_(std::forward_as_tuple(std::forward<UItems>(items)...))
        , caption(std::move(caption))
    {
    }

    template <details::SizerItem... UItems>
    Sizer(std::string_view caption, wxSizerFlags const& flags, UItems&&... items)
        : flags_(flags)
        , items_(std::forward_as_tuple(std::forward<UItems>(items)...))
        , caption(wxString::FromUTF8(caption.data(), caption.size()))
    {
    }

    template <details::SizerItem... UItems>
    Sizer(wxUI_String, wxString caption, wxSizerFlags const& flags, UItems&&... items)
        : flags_(flags)
        , items_(std::forward_as_tuple(std::forward<UItems>(items)...))
        , caption(std::move(caption))
    {
    }

    auto withFlags(wxSizerFlags flags) & -> Sizer&
    {
        flags_ = flags;
        return *this;
    }

    auto withFlags(wxSizerFlags flags) && -> Sizer&&
    {
        flags_ = flags;
        return std::move(*this);
    }

    auto withProxy(SizerProxy const& proxy) & -> Sizer&
    {
        proxyHandles_.push_back(proxy);
        return *this;
    }

    auto withProxy(SizerProxy const& proxy) && -> Sizer&&
    {
        proxyHandles_.push_back(proxy);
        return std::move(*this);
    }

    template <typename Parent, typename Sizer>
    auto createAndAdd(Parent* parent, Sizer* parentSizer, wxSizerFlags const& parentFlags)
    {
        auto currentFlags = flags_.value_or(parentFlags);
        auto* sizer = createAndAddWidgets(parent, currentFlags);
        parentSizer->Add(sizer, currentFlags);
        return sizer;
    }

    template <typename Parent>
    auto fitTo(Parent* parent)
    {
        auto* sizer = createAndAddWidgets(parent, flags_.value_or(wxSizerFlags {}));
        parent->SetSizer(sizer);
        sizer->SetSizeHints(parent);
    }

private:
    template <typename Parent>
    auto constructSizer(Parent* parent) const
    {
        using ::wxUI::customizations::SizerCreate;
        return SizerCreate(parent, caption, orientation);
    }

    template <typename Parent>
    auto createAndAddWidgets(Parent* parent, wxSizerFlags const& flags)
    {
        auto sizer = constructSizer(parent);

        std::apply([parent, sizer, flags](auto&&... tupleArg) {
            (details::createAndAddVisiter(tupleArg, parent, sizer, flags), ...);
        },
            items_);
        return bindProxy(sizer);
    }

    template <typename Sizer>
    auto bindProxy(Sizer* sizer)
    {
        for (auto& proxyHandle : proxyHandles_) {
            using ::wxUI::customizations::SizerBindProxy;
            SizerBindProxy(sizer, proxyHandle);
        }
        return sizer;
    }

    std::optional<wxSizerFlags> flags_ {};
    std::tuple<Items...> items_ {};
    std::optional<wxString> caption {};
    std::vector<SizerProxy> proxyHandles_;
};
}

namespace wxUI {

template <details::SizerItem... Items>
struct VSizer {
    template <details::SizerItem... UItems>
    explicit VSizer(UItems&&... items)
        : details_(std::forward<UItems>(items)...)
    {
    }

    template <details::SizerItem... UItems>
    explicit VSizer(std::string_view caption, UItems&&... items)
        : details_(wxUI_String {}, wxString::FromUTF8(caption.data(), caption.size()), std::forward<UItems>(items)...)
    {
    }

    template <details::SizerItem... UItems>
    explicit VSizer(wxUI_String, wxString caption, UItems&&... items)
        : details_(wxUI_String {}, std::move(caption), std::forward<UItems>(items)...)
    {
    }

    template <details::SizerItem... UItems>
    explicit VSizer(wxSizerFlags const& flags, UItems&&... items)
        : details_(flags, std::forward<UItems>(items)...)
    {
    }

    template <details::SizerItem... UItems>
    VSizer(std::string_view caption, wxSizerFlags const& flags, UItems&&... items)
        : details_(wxUI_String {}, wxString::FromUTF8(caption.data(), caption.size()), flags, std::forward<UItems>(items)...)
    {
    }

    template <details::SizerItem... UItems>
    VSizer(wxUI_String, wxString caption, wxSizerFlags const& flags, UItems&&... items)
        : details_(wxUI_String {}, std::move(caption), flags, std::forward<UItems>(items)...)
    {
    }

    template <typename Parent, typename Sizer>
    auto createAndAdd(Parent* parent, Sizer* parentSizer, wxSizerFlags const& parentFlags)
    {
        return details_.createAndAdd(parent, parentSizer, parentFlags);
    }

    template <typename Parent>
    auto fitTo(Parent* parent) -> VSizer&
    {
        details_.fitTo(parent);
        return *this;
    }

    WXUI_FORWARD_TO_DETAILS(VSizer<Items...>, withFlags, wxSizerFlags, flags)

private:
    Sizer<wxVERTICAL, Items...> details_;
};

template <details::SizerItem... UItems>
VSizer(UItems&&... items) -> VSizer<UItems...>;

template <details::SizerItem... UItems>
VSizer(std::string_view caption, UItems&&... items) -> VSizer<UItems...>;

template <details::SizerItem... UItems>
VSizer(wxUI_String, wxString caption, UItems&&... items) -> VSizer<UItems...>;

template <details::SizerItem... UItems>
VSizer(wxSizerFlags const& flags, UItems&&... items) -> VSizer<UItems...>;

template <details::SizerItem... UItems>
VSizer(std::string_view caption, wxSizerFlags const& flags, UItems&&... items) -> VSizer<UItems...>;

template <details::SizerItem... UItems>
VSizer(wxUI_String, wxString caption, wxSizerFlags const& flags, UItems&&... items) -> VSizer<UItems...>;

template <details::SizerItem... Items>
struct HSizer {
    template <details::SizerItem... UItems>
    explicit HSizer(UItems&&... items)
        : details_(std::forward<UItems>(items)...)
    {
    }

    template <details::SizerItem... UItems>
    explicit HSizer(std::string_view caption, UItems&&... items)
        : details_(wxUI_String {}, wxString::FromUTF8(caption.data(), caption.size()), std::forward<UItems>(items)...)
    {
    }

    template <details::SizerItem... UItems>
    explicit HSizer(wxUI_String, wxString caption, UItems&&... items)
        : details_(wxUI_String {}, std::move(caption), std::forward<UItems>(items)...)
    {
    }

    template <details::SizerItem... UItems>
    explicit HSizer(wxSizerFlags const& flags, UItems&&... items)
        : details_(flags, std::forward<UItems>(items)...)
    {
    }

    template <details::SizerItem... UItems>
    HSizer(std::string_view caption, wxSizerFlags const& flags, UItems&&... items)
        : details_(wxUI_String {}, wxString::FromUTF8(caption.data(), caption.size()), flags, std::forward<UItems>(items)...)
    {
    }

    template <details::SizerItem... UItems>
    HSizer(wxUI_String, wxString caption, wxSizerFlags const& flags, UItems&&... items)
        : details_(wxUI_String {}, std::move(caption), flags, std::forward<UItems>(items)...)
    {
    }

    template <typename Parent, typename Sizer>
    auto createAndAdd(Parent* parent, Sizer* parentSizer, wxSizerFlags const& parentFlags)
    {
        return details_.createAndAdd(parent, parentSizer, parentFlags);
    }

    // need rvalues versions of these.
    template <typename Parent>
    auto fitTo(Parent* parent) -> HSizer&
    {
        details_.fitTo(parent);
        return *this;
    }

    WXUI_FORWARD_TO_DETAILS(HSizer<Items...>, withFlags, wxSizerFlags, flags)

private:
    Sizer<wxHORIZONTAL, Items...> details_;
};

template <details::SizerItem... UItems>
HSizer(UItems&&... items) -> HSizer<UItems...>;

template <details::SizerItem... UItems>
HSizer(std::string_view caption, UItems&&... items) -> HSizer<UItems...>;

template <details::SizerItem... UItems>
HSizer(wxUI_String, wxString const& caption, UItems&&... items) -> HSizer<UItems...>;

template <details::SizerItem... UItems>
HSizer(wxSizerFlags const& flags, UItems&&... items) -> HSizer<UItems...>;

template <details::SizerItem... UItems>
HSizer(std::string_view caption, wxSizerFlags const& flags, UItems&&... items) -> HSizer<UItems...>;

template <details::SizerItem... UItems>
HSizer(wxUI_String, wxString const& caption, wxSizerFlags const& flags, UItems&&... items) -> HSizer<UItems...>;

template <details::SizerItem... Items>
struct LayoutIf {
    template <details::SizerItem... UItems>
    explicit LayoutIf(bool enabled, UItems&&... items)
    {
        if (enabled) {
            items_ = std::forward_as_tuple(std::forward<UItems>(items)...);
        }
    }
    template <typename Parent, typename Sizer>
    auto createAndAdd(Parent* parent, Sizer* parentSizer, wxSizerFlags const& parentFlags)
    {
        if (!items_) {
            return;
        }
        createAndAddWidgets(parent, parentSizer, parentFlags);
    }

private:
    template <typename Parent, typename Sizer>
    void createAndAddWidgets(Parent* parent, Sizer* sizer, wxSizerFlags const& flags)
    {
        std::apply([parent, sizer, flags](auto&&... tupleArg) {
            (details::createAndAddVisiter(tupleArg, parent, sizer, flags), ...);
        },
            *items_);
    }

    std::optional<std::tuple<Items...>> items_ {};
};

template <details::SizerItem... Item>
LayoutIf(bool, Item... item) -> LayoutIf<Item...>;
}

#include "ZapMacros.hpp"
