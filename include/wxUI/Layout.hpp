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

using Spacer = details::Spacer;
using StretchSpacer = details::StretchSpacer;

template <details::SizerItem... Items>
struct VSizer {
    explicit VSizer(Items&&... items)
        : details_(wxVERTICAL, std::forward<Items>(items)...)
    {
    }

    explicit VSizer(std::string_view caption, Items&&... items)
        : details_(wxVERTICAL, wxUI_String {}, details::toWxString(caption), std::forward<Items>(items)...)
    {
    }

    VSizer(wxUI_String, wxString caption, Items&&... items)
        : details_(wxVERTICAL, wxUI_String {}, std::move(caption), std::forward<Items>(items)...)
    {
    }

    explicit VSizer(wxSizerFlags const& flags, Items&&... items)
        : details_(wxVERTICAL, flags, std::forward<Items>(items)...)
    {
    }

    VSizer(std::string_view caption, wxSizerFlags const& flags, Items&&... items)
        : details_(wxVERTICAL, wxUI_String {}, details::toWxString(caption), flags, std::forward<Items>(items)...)
    {
    }

    VSizer(wxUI_String, wxString caption, wxSizerFlags const& flags, Items&&... items)
        : details_(wxVERTICAL, wxUI_String {}, std::move(caption), flags, std::forward<Items>(items)...)
    {
    }

    template <typename Parent, typename Sizer>
    auto createAndAdd(Parent* parent, Sizer* parentSizer, wxSizerFlags const& parentFlags)
    {
        return details_.createAndAdd(parent, parentSizer, parentFlags);
    }

    WXUI_FITTO_DETAILS()
    WXUI_FORWARD_TO_DETAILS(VSizer, withFlags, wxSizerFlags, flags)
    WXUI_FORWARD_TO_DETAILS(VSizer, withProxy, SizerProxy, proxy)
    WXUI_FORWARD_SCROLL_TO_DETAILS(VSizer)

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
    explicit HSizer(Items&&... items)
        : details_(wxHORIZONTAL, std::forward<Items>(items)...)
    {
    }

    explicit HSizer(std::string_view caption, Items&&... items)
        : details_(wxHORIZONTAL, wxUI_String {}, details::toWxString(caption), std::forward<Items>(items)...)
    {
    }

    HSizer(wxUI_String, wxString caption, Items&&... items)
        : details_(wxHORIZONTAL, wxUI_String {}, std::move(caption), std::forward<Items>(items)...)
    {
    }

    explicit HSizer(wxSizerFlags const& flags, Items&&... items)
        : details_(wxHORIZONTAL, flags, std::forward<Items>(items)...)
    {
    }

    HSizer(std::string_view caption, wxSizerFlags const& flags, Items&&... items)
        : details_(wxHORIZONTAL, wxUI_String {}, details::toWxString(caption), flags, std::forward<Items>(items)...)
    {
    }

    HSizer(wxUI_String, wxString caption, wxSizerFlags const& flags, Items&&... items)
        : details_(wxHORIZONTAL, wxUI_String {}, std::move(caption), flags, std::forward<Items>(items)...)
    {
    }

    template <typename Parent, typename Sizer>
    auto createAndAdd(Parent* parent, Sizer* parentSizer, wxSizerFlags const& parentFlags)
    {
        return details_.createAndAdd(parent, parentSizer, parentFlags);
    }

    WXUI_FITTO_DETAILS()
    WXUI_FORWARD_TO_DETAILS(HSizer, withFlags, wxSizerFlags, flags)
    WXUI_FORWARD_TO_DETAILS(HSizer, withProxy, SizerProxy, proxy)
    WXUI_FORWARD_SCROLL_TO_DETAILS(HSizer)

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
    explicit VWrapSizer(Items&&... items)
        : details_(details::withWrap {}, wxVERTICAL, std::forward<Items>(items)...)
    {
    }

    explicit VWrapSizer(wxSizerFlags const& flags, Items&&... items)
        : details_(details::withWrap {}, wxVERTICAL, flags, std::forward<Items>(items)...)
    {
    }

    template <typename Parent, typename Sizer>
    auto createAndAdd(Parent* parent, Sizer* parentSizer, wxSizerFlags const& parentFlags)
    {
        return details_.createAndAdd(parent, parentSizer, parentFlags);
    }

    WXUI_FITTO_DETAILS()
    WXUI_FORWARD_TO_DETAILS(VWrapSizer, withFlags, wxSizerFlags, flags)
    WXUI_FORWARD_TO_DETAILS(VWrapSizer, withProxy, SizerProxy, proxy)
    WXUI_FORWARD_SCROLL_TO_DETAILS(VWrapSizer)

private:
    details::BoxSizer<Items...> details_;
};

template <details::SizerItem... UItems>
VWrapSizer(UItems&&... items) -> VWrapSizer<UItems...>;

template <details::SizerItem... UItems>
VWrapSizer(wxSizerFlags const& flags, UItems&&... items) -> VWrapSizer<UItems...>;

template <details::SizerItem... Items>
struct HWrapSizer {
    explicit HWrapSizer(Items&&... items)
        : details_(details::withWrap {}, wxHORIZONTAL, std::forward<Items>(items)...)
    {
    }

    explicit HWrapSizer(wxSizerFlags const& flags, Items&&... items)
        : details_(details::withWrap {}, wxHORIZONTAL, flags, std::forward<Items>(items)...)
    {
    }

    template <typename Parent, typename Sizer>
    auto createAndAdd(Parent* parent, Sizer* parentSizer, wxSizerFlags const& parentFlags)
    {
        return details_.createAndAdd(parent, parentSizer, parentFlags);
    }

    WXUI_FITTO_DETAILS()
    WXUI_FORWARD_TO_DETAILS(HWrapSizer, withFlags, wxSizerFlags, flags)
    WXUI_FORWARD_TO_DETAILS(HWrapSizer, withProxy, SizerProxy, proxy)
    WXUI_FORWARD_SCROLL_TO_DETAILS(HWrapSizer)

private:
    details::BoxSizer<Items...> details_;
};

template <details::SizerItem... UItems>
HWrapSizer(UItems&&... items) -> HWrapSizer<UItems...>;

template <details::SizerItem... UItems>
HWrapSizer(wxSizerFlags const& flags, UItems&&... items) -> HWrapSizer<UItems...>;

template <details::SizerItem... Items>
struct GridSizer {
    explicit GridSizer(int cols, Items&&... items)
        : details_(std::forward<Items>(items)...)
        , cols_(cols)
    {
    }

    GridSizer(int cols, wxSizerFlags const& flags, Items&&... items)
        : details_(flags, std::forward<Items>(items)...)
        , cols_(cols)
    {
    }

    template <typename Parent, typename Sizer>
    auto createAndAdd(Parent* parent, Sizer* sizer, wxSizerFlags const& parentFlags)
    {
        return details_.createAndAdd(this->template createImpl<Parent>(), parent, sizer, parentFlags);
    }

    template <typename Parent>
    auto fitTo(Parent* parent) -> Parent*
    {
        return details_.fitTo(this->template createImpl<Parent>(), parent);
    }

    WXUI_FORWARD_TO_DETAILS(GridSizer, withFlags, wxSizerFlags, flags)
    WXUI_FORWARD_TO_DETAILS(GridSizer, withProxy, SizerProxy, proxy)
    WXUI_FORWARD_SCROLL_TO_DETAILS(GridSizer)

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
    explicit FlexGridSizer(int cols, Items&&... items)
        : details_(std::forward<Items>(items)...)
        , cols_(cols)
    {
    }

    FlexGridSizer(int cols, wxSizerFlags const& flags, Items&&... items)
        : details_(flags, std::forward<Items>(items)...)
        , cols_(cols)
    {
    }

    auto withFlexibleRow(size_t row, int proportion) & -> FlexGridSizer&
    {
        flexibleRows_.emplace_back(row, proportion);
        return *this;
    }

    auto withFlexibleRow(size_t row, int proportion) && -> FlexGridSizer&&
    {
        flexibleRows_.emplace_back(row, proportion);
        return std::move(*this);
    }

    auto withFlexibleCol(size_t col, int proportion) & -> FlexGridSizer&
    {
        flexibleCols_.emplace_back(col, proportion);
        return *this;
    }

    auto withFlexibleCol(size_t col, int proportion) && -> FlexGridSizer&&
    {
        flexibleCols_.emplace_back(col, proportion);
        return std::move(*this);
    }

    template <typename Parent, typename Sizer>
    auto createAndAdd(Parent* parent, Sizer* sizer, wxSizerFlags const& parentFlags)
    {
        return details_.createAndAdd(this->template createImpl<Parent>(), parent, sizer, parentFlags);
    }

    template <typename Parent>
    auto fitTo(Parent* parent) -> Parent*
    {
        return details_.fitTo(this->template createImpl<Parent>(), parent);
    }

    WXUI_FORWARD_TO_DETAILS(FlexGridSizer, withFlags, wxSizerFlags, flags)
    WXUI_FORWARD_TO_DETAILS(FlexGridSizer, withProxy, SizerProxy, proxy)
    WXUI_FORWARD_SCROLL_TO_DETAILS(FlexGridSizer)

private:
    template <typename Parent>
    auto createImpl()
    {
        return [this](Parent* parent) {
            using ::wxUI::customizations::FlexGridSizerInfo;
            using ::wxUI::customizations::SizerCreate;
            return SizerCreate(parent, FlexGridSizerInfo { cols_, flexibleRows_, flexibleCols_ });
        };
    }

    details::Sizer<Items...> details_;
    int cols_ = 0;
    std::vector<std::pair<size_t, int>> flexibleRows_;
    std::vector<std::pair<size_t, int>> flexibleCols_;
};

template <details::SizerItem... UItems>
FlexGridSizer(int, UItems&&... items) -> FlexGridSizer<UItems...>;

template <details::SizerItem... UItems>
FlexGridSizer(int, wxSizerFlags const& flags, UItems&&... items) -> FlexGridSizer<UItems...>;
}

#include <wxUI/detail/ZapMacros.hpp>
