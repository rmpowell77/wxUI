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

#include <variant>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wxUI/Widget.hpp>
#include <wxUI/detail/HelperMacros.hpp>
#include <wxUI/wxUITypes.hpp>

// forward-declare Wrapper so we can detect it in traits without including its header
namespace wxUI {
template <typename Window>
struct Wrapper;
}

namespace wxUI::details {

template <typename T>
struct is_Wrapper : std::false_type { };
template <typename W>
struct is_Wrapper<::wxUI::Wrapper<W>> : std::true_type { };
template <typename T>
inline constexpr bool is_Wrapper_v = is_Wrapper<T>::value;

template <typename T>
concept SizerItem = details::CreateAndAddable<T>
    || is_Wrapper_v<T>
    || (std::is_pointer_v<T> && std::derived_from<std::remove_pointer_t<T>, wxSizer>);

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
}

namespace wxUI::details {

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

    template <typename CreatorFunction, typename Parent, typename Sizer>
    auto createAndAdd(CreatorFunction creator, Parent* parent, Sizer* parentSizer, wxSizerFlags const& parentFlags)
    {
        auto currentFlags = flags_.value_or(parentFlags);
        auto* sizer = createAndAddWidgets(creator, parent, currentFlags);
        parentSizer->Add(sizer, currentFlags);
        return sizer;
    }

    template <typename CreatorFunction, typename Parent>
    auto fitTo(CreatorFunction creator, Parent* parent)
    {
        auto* sizer = createAndAddWidgets(creator, parent, flags_.value_or(wxSizerFlags {}));
        parent->SetSizer(sizer);
        sizer->SetSizeHints(parent);
    }

private:
    template <typename CreatorFunction, typename Parent>
    auto createAndAddWidgets(CreatorFunction creator, Parent* parent, wxSizerFlags const& flags)
    {
        auto sizer = creator(parent);

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
    std::vector<SizerProxy> proxyHandles_ {};
};

struct withWrap { };
template <details::SizerItem... Items>
struct BoxSizer {
    template <details::SizerItem... UItems>
    explicit BoxSizer(wxOrientation orientation, UItems&&... items)
        : details_(std::forward<UItems>(items)...)
        , orientation_(orientation)
    {
    }

    template <details::SizerItem... UItems>
    explicit BoxSizer(wxOrientation orientation, wxSizerFlags const& flags, UItems&&... items)
        : details_(flags, std::forward<UItems>(items)...)
        , orientation_(orientation)
    {
    }

    template <details::SizerItem... UItems>
    BoxSizer(wxOrientation orientation, std::string_view caption, UItems&&... items)
        : details_(std::forward<UItems>(items)...)
        , orientation_(orientation)
        , caption_(wxString::FromUTF8(caption.data(), caption.size()))
    {
    }

    template <details::SizerItem... UItems>
    BoxSizer(wxOrientation orientation, wxUI_String, wxString caption, UItems&&... items)
        : details_(std::forward<UItems>(items)...)
        , orientation_(orientation)
        , caption_(std::move(caption))
    {
    }

    template <details::SizerItem... UItems>
    BoxSizer(wxOrientation orientation, std::string_view caption, wxSizerFlags const& flags, UItems&&... items)
        : details_(flags, std::forward<UItems>(items)...)
        , orientation_(orientation)
        , caption_(wxString::FromUTF8(caption.data(), caption.size()))
    {
    }

    template <details::SizerItem... UItems>
    BoxSizer(wxOrientation orientation, wxUI_String, wxString caption, wxSizerFlags const& flags, UItems&&... items)
        : details_(flags, std::forward<UItems>(items)...)
        , orientation_(orientation)
        , caption_(std::move(caption))
    {
    }

    template <details::SizerItem... UItems>
    explicit BoxSizer(withWrap, wxOrientation orientation, UItems&&... items)
        : details_(std::forward<UItems>(items)...)
        , orientation_(orientation)
        , wrap_(true)
    {
    }

    template <details::SizerItem... UItems>
    explicit BoxSizer(withWrap, wxOrientation orientation, wxSizerFlags const& flags, UItems&&... items)
        : details_(flags, std::forward<UItems>(items)...)
        , orientation_(orientation)
        , wrap_(true)
    {
    }

    template <typename Parent, typename Sizer>
    auto createAndAdd(Parent* parent, Sizer* sizer, wxSizerFlags const& parentFlags)
    {
        return details_.createAndAdd(this->template createImpl<Parent>(), parent, sizer, parentFlags);
    }

    template <typename Parent>
    auto fitTo(Parent* parent)
    {
        return details_.fitTo(this->template createImpl<Parent>(), parent);
    }

    WXUI_FORWARD_TO_DETAILS(BoxSizer, withFlags, wxSizerFlags, flags)

private:
    template <typename Parent>
    auto createImpl()
    {
        return [this](Parent* parent) {
            using ::wxUI::customizations::BoxSizerInfo;
            using ::wxUI::customizations::WrapSizerInfo;
            using ::wxUI::customizations::SizerCreate;
            if (wrap_) {
                return SizerCreate(parent, WrapSizerInfo { orientation_ });
            }
            return SizerCreate(parent, BoxSizerInfo { caption_, orientation_ });
        };
    }

    details::Sizer<Items...> details_;
    wxOrientation orientation_ {};
    bool wrap_ = false;
    std::optional<wxString> caption_ = std::nullopt;
};

}
