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
#include <wxUI/detail/LayoutDetails.hpp>
#include <wxUI/wxUITypes.hpp>

namespace wxUI {

template <details::SizerItem... Items>
struct VSizer {
    template <details::SizerItem... UItems>
    explicit VSizer(UItems&&... items)
        : details_(wxVERTICAL, std::forward<UItems>(items)...)
    {
    }

    template <details::SizerItem... UItems>
    VSizer(std::string_view caption, UItems&&... items)
        : details_(wxVERTICAL, wxUI_String {}, wxString::FromUTF8(caption.data(), caption.size()), std::forward<UItems>(items)...)
    {
    }

    template <details::SizerItem... UItems>
    VSizer(wxUI_String, wxString caption, UItems&&... items)
        : details_(wxVERTICAL, wxUI_String {}, std::move(caption), std::forward<UItems>(items)...)
    {
    }

    template <details::SizerItem... UItems>
    explicit VSizer(wxSizerFlags const& flags, UItems&&... items)
        : details_(wxVERTICAL, flags, std::forward<UItems>(items)...)
    {
    }

    template <details::SizerItem... UItems>
    VSizer(std::string_view caption, wxSizerFlags const& flags, UItems&&... items)
        : details_(wxVERTICAL, wxUI_String {}, wxString::FromUTF8(caption.data(), caption.size()), flags, std::forward<UItems>(items)...)
    {
    }

    template <details::SizerItem... UItems>
    VSizer(wxUI_String, wxString caption, wxSizerFlags const& flags, UItems&&... items)
        : details_(wxVERTICAL, wxUI_String {}, std::move(caption), flags, std::forward<UItems>(items)...)
    {
    }

    template <typename Parent, typename Sizer>
    auto createAndAdd(Parent* parent, Sizer* parentSizer, wxSizerFlags const& parentFlags)
    {
        return details_.createAndAdd(parent, parentSizer, parentFlags);
    }

    WXUI_FORWARD_TEMPLATEPTR_TO_DETAILS(VSizer, fitTo, Parent, parent)
    WXUI_FORWARD_TO_DETAILS(VSizer, withFlags, wxSizerFlags, flags)

private:
    details::BoxSizer<Items...> details_;
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
        : details_(wxHORIZONTAL, std::forward<UItems>(items)...)
    {
    }

    template <details::SizerItem... UItems>
    HSizer(std::string_view caption, UItems&&... items)
        : details_(wxHORIZONTAL, wxUI_String {}, wxString::FromUTF8(caption.data(), caption.size()), std::forward<UItems>(items)...)
    {
    }

    template <details::SizerItem... UItems>
    HSizer(wxUI_String, wxString caption, UItems&&... items)
        : details_(wxHORIZONTAL, wxUI_String {}, std::move(caption), std::forward<UItems>(items)...)
    {
    }

    template <details::SizerItem... UItems>
    explicit HSizer(wxSizerFlags const& flags, UItems&&... items)
        : details_(wxHORIZONTAL, flags, std::forward<UItems>(items)...)
    {
    }

    template <details::SizerItem... UItems>
    HSizer(std::string_view caption, wxSizerFlags const& flags, UItems&&... items)
        : details_(wxHORIZONTAL, wxUI_String {}, wxString::FromUTF8(caption.data(), caption.size()), flags, std::forward<UItems>(items)...)
    {
    }

    template <details::SizerItem... UItems>
    HSizer(wxUI_String, wxString caption, wxSizerFlags const& flags, UItems&&... items)
        : details_(wxHORIZONTAL, wxUI_String {}, std::move(caption), flags, std::forward<UItems>(items)...)
    {
    }

    template <typename Parent, typename Sizer>
    auto createAndAdd(Parent* parent, Sizer* parentSizer, wxSizerFlags const& parentFlags)
    {
        return details_.createAndAdd(parent, parentSizer, parentFlags);
    }

    WXUI_FORWARD_TEMPLATEPTR_TO_DETAILS(HSizer, fitTo, Parent, parent)
    WXUI_FORWARD_TO_DETAILS(HSizer, withFlags, wxSizerFlags, flags)

private:
    details::BoxSizer<Items...> details_;
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
struct VWrapSizer {
    template <details::SizerItem... UItems>
    explicit LayoutIf(bool enabled, UItems&&... items)
        : items_(std::forward_as_tuple(std::forward<UItems>(items)...))
    {
        if (!enabled) {
            items_.reset();
        }
    }

    template <details::SizerItem... UItems>
    explicit VWrapSizer(wxSizerFlags const& flags, UItems&&... items)
        : details_(details::withWrap {}, wxVERTICAL, flags, std::forward<UItems>(items)...)
    {
    }

    template <typename Parent, typename Sizer>
    auto createAndAdd(Parent* parent, Sizer* parentSizer, wxSizerFlags const& parentFlags)
    {
        return details_.createAndAdd(parent, parentSizer, parentFlags);
    }

    WXUI_FORWARD_TEMPLATEPTR_TO_DETAILS(VWrapSizer, fitTo, Parent, parent)
    WXUI_FORWARD_TO_DETAILS(VWrapSizer, withFlags, wxSizerFlags, flags)

private:
    details::BoxSizer<Items...> details_;
};

template <details::SizerItem... UItems>
VWrapSizer(UItems&&... items) -> VWrapSizer<UItems...>;

template <details::SizerItem... UItems>
VWrapSizer(wxSizerFlags const& flags, UItems&&... items) -> VWrapSizer<UItems...>;

template <details::SizerItem... Items>
struct HWrapSizer {
    template <details::SizerItem... UItems>
    explicit HWrapSizer(UItems&&... items)
        : details_(details::withWrap {}, wxHORIZONTAL, std::forward<UItems>(items)...)
    {
    }

    template <details::SizerItem... UItems>
    explicit HWrapSizer(wxSizerFlags const& flags, UItems&&... items)
        : details_(details::withWrap {}, wxHORIZONTAL, flags, std::forward<UItems>(items)...)
    {
    }

    template <typename Parent, typename Sizer>
    auto createAndAdd(Parent* parent, Sizer* parentSizer, wxSizerFlags const& parentFlags)
    {
        return details_.createAndAdd(parent, parentSizer, parentFlags);
    }

    WXUI_FORWARD_TEMPLATEPTR_TO_DETAILS(HWrapSizer, fitTo, Parent, parent)
    WXUI_FORWARD_TO_DETAILS(HWrapSizer, withFlags, wxSizerFlags, flags)

private:
    details::BoxSizer<Items...> details_;
};

template <details::SizerItem... UItems>
HWrapSizer(UItems&&... items) -> HWrapSizer<UItems...>;

template <details::SizerItem... UItems>
HWrapSizer(wxSizerFlags const& flags, UItems&&... items) -> HWrapSizer<UItems...>;

template <details::SizerItem... Items>
struct GridSizer {
    template <details::SizerItem... UItems>
    explicit GridSizer(int cols, UItems&&... items)
        : details_(std::forward<UItems>(items)...)
        , cols_(cols)
    {
    }

    template <details::SizerItem... UItems>
    GridSizer(int cols, wxSizerFlags const& flags, UItems&&... items)
        : details_(flags, std::forward<UItems>(items)...)
        , cols_(cols)
    {
    }

    template <typename Parent, typename Sizer>
    auto createAndAdd(Parent* parent, Sizer* sizer, wxSizerFlags const& parentFlags)
    {
        return details_.createAndAdd(this->template createImpl<Parent>(), parent, sizer, parentFlags);
    }

    template <typename Parent>
    auto fitTo(Parent* parent) & -> GridSizer
    {
        details_.fitTo(this->template createImpl<Parent>(), parent);
        return *this;
    }

    template <typename Parent>
    auto fitTo(Parent* parent) && -> GridSizer&&
    {
        details_.fitTo(this->template createImpl<Parent>(), parent);
        return std::move(*this);
    }

    WXUI_FORWARD_TO_DETAILS(GridSizer, withFlags, wxSizerFlags, flags)

private:
    template <typename Parent>
    auto createImpl()
    {
        return [this](Parent* parent) {
            using ::wxUI::customizations::GridSizerInfo;
            using ::wxUI::customizations::SizerCreate;
            return SizerCreate(parent, GridSizerInfo { cols_ });
        };
    }

    details::Sizer<Items...> details_;
    int cols_ = 0;
};

template <details::SizerItem... UItems>
GridSizer(int, UItems&&... items) -> GridSizer<UItems...>;

template <details::SizerItem... UItems>
GridSizer(int, wxSizerFlags const& flags, UItems&&... items) -> GridSizer<UItems...>;

template <details::SizerItem... Items>
struct FlexGridSizer {
    template <details::SizerItem... UItems>
    explicit FlexGridSizer(int cols, UItems&&... items)
        : details_(std::forward<UItems>(items)...)
        , cols_(cols)
    {
    }

    template <details::SizerItem... UItems>
    explicit FlexGridSizer(int cols, wxSizerFlags const& flags, UItems&&... items)
        : details_(flags, std::forward<UItems>(items)...)
        , cols_(cols)
    {
    }

    template <typename Parent, typename Sizer>
    auto createAndAdd(Parent* parent, Sizer* sizer, wxSizerFlags const& parentFlags)
    {
        return details_.createAndAdd(this->template createImpl<Parent>(), parent, sizer, parentFlags);
    }

    template <typename Parent>
    auto fitTo(Parent* parent) &
    {
        details_.fitTo(this->template createImpl<Parent>(), parent);
        return *this;
    }

    template <typename Parent>
    auto fitTo(Parent* parent) &&
    {
        details_.fitTo(this->template createImpl<Parent>(), parent);
        return std::move(*this);
    }

    WXUI_FORWARD_TO_DETAILS(GridSizer<Items...>, withFlags, wxSizerFlags, flags)

private:
    template <typename Parent>
    auto createImpl()
    {
        return [this](Parent* parent) {
            using ::wxUI::customizations::FlexGridSizerInfo;
            using ::wxUI::customizations::SizerCreate;
            return SizerCreate(parent, FlexGridSizerInfo { cols_ });
        };
    }

    details::Sizer<Items...> details_;
    int cols_ = 0;
};

template <details::SizerItem... UItems>
FlexGridSizer(int, UItems&&... items) -> FlexGridSizer<UItems...>;

template <details::SizerItem... UItems>
FlexGridSizer(int, wxSizerFlags const& flags, UItems&&... items) -> FlexGridSizer<UItems...>;

}

#include <wxUI/detail/ZapMacros.hpp>
