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

#include "BindInfo.hpp"
#include "Customizations.hpp"
#include <optional>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <wx/sizer.h>

namespace wxUI::details {

// clang-format off

// A CreateAndAdd function takes a Window, Sizer, and Size flags
// It is expected that the function does something meaningful, such as
// create a wxWindow object and insert it into the sizer
template <typename T>
concept CreateAndAddFunction = requires(T function, wxWindow* window, wxSizer* sizer)
{
    function(window, sizer, wxSizerFlags {});
};

template <typename T>
concept CreateAndAddable = requires(T widget, wxWindow* window, wxSizer* sizer)
{
    widget.createAndAdd(window, sizer, wxSizerFlags {});
};

template <typename T>
concept Createable = requires(T widget, wxWindow* window)
{
    widget.create(window);
};

namespace Ranges {

template <typename T>
concept ConvertTowxString = std::is_convertible_v<T, wxString>;

template <ConvertTowxString T>
inline auto convertTo(std::initializer_list<T> choices) -> std::vector<wxString>
{
    std::vector<wxString> result;
    std::ranges::copy(choices, std::back_inserter(result));
    return result;
}


template <typename R, typename T>
concept input_range_of = std::ranges::input_range<R> && std::convertible_to<std::ranges::range_value_t<R>, T>;

// This is due to missing support for ranges::to()
template <typename T, std::ranges::range Range>
    requires std::convertible_to<std::ranges::range_value_t<Range>, T>
inline auto ToVector(Range&& range) -> std::vector<T>
{
    auto result = std::vector<T>{};
    std::ranges::copy(std::forward<Range>(range), std::back_inserter(result));
    return result;
}
}

// clang-format on

template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

template <typename Underlying>
struct WidgetProxy {
    WidgetProxy()
        : controller(std::make_shared<Underlying*>())
    {
    }

    [[nodiscard]] auto control() const -> Underlying*
    {
        if (!controller) {
            throw std::runtime_error("Proxy class has not been attached");
        }
        return *controller;
    }

    void setUnderlying(Underlying* control)
    {
        *controller = control;
    }

    auto operator->() const { return control(); }

    explicit operator bool() const noexcept { return controller != nullptr; }

private:
    std::shared_ptr<Underlying*> controller {};
};

// The WidgetDetails holds the common details across many controllers.
// The "recipe" for constructing a controller is pretty straight forward:
// 1. give a name.
// 2. Have a WidgetDetails member.
// 3. Include the WXUI_FORWARD_ALL_TO_DETAILS macro.
template <typename ConcreteWidget, typename Underlying>
struct WidgetDetails {
    using underlying_t = Underlying;

    explicit WidgetDetails(wxWindowID identity = wxID_ANY)
        : identity_(identity)
    {
    }

    auto withFlags(wxSizerFlags flags)
    {
        flags_ = flags;
    }

    auto withPosition(wxPoint pos)
    {
        pos_ = pos;
    }

    auto withSize(wxSize size)
    {
        size_ = size;
    }

    auto withWidth(int size)
    {
        size_.SetWidth(size);
    }

    auto withHeight(int size)
    {
        size_.SetHeight(size);
    }

    auto setStyle(int64_t style)
    {
        style_ = style;
    }

    auto withStyle(int64_t style)
    {
        style_ |= style;
    }

    auto withoutStyle(int64_t style)
    {
        style_ &= ~style;
    }

    auto withFont(wxFontInfo const& fontInfo)
    {
        fontInfo_ = fontInfo;
    }

    auto setEnabled(bool enabled)
    {
        enabled_ = enabled;
    }

    auto withProxy(WidgetProxy<Underlying> const& proxy)
    {
        proxyHandles_.push_back(proxy);
    }

    // create uses the stored Creator to construct the underlying widget,
    // then performs the same common post-processing that WidgetDetails did.
    template <typename CreatorFunction, typename Parent>
    auto create(CreatorFunction creator, Parent* parent)
    {
        auto* widget = creator(parent, identity_, pos_, size_, style_);
        if (fontInfo_) {
            widget->SetFont(wxFont(*fontInfo_));
        }
        widget->Enable(enabled_);
        bindEvents(widget);
        bindProxy(widget);

        return widget;
    }

    template <typename Creator, typename Parent, typename Sizer>
    auto createAndAdd(Creator creator, Parent* parent, Sizer* sizer, wxSizerFlags const& parentFlags)
    {
        auto* widget = create(creator, parent);
        sizer->Add(widget, flags_.value_or(parentFlags));
        return widget;
    }

    auto getIdentity() const { return identity_; }
    auto getPos() const { return pos_; }
    auto getSize() const { return size_; }
    auto getStyle() const { return style_; }
    auto getFlags() const { return flags_; }

    // Bind API (made public here so controllers can forward easily)
    template <typename Function, typename Event = wxCommandEvent>
    auto bind(wxEventTypeTag<Event> event, Function function)
    {
        if constexpr (is_noarg_callable<Function>()) {
            boundedFunctions_.emplace_back(event, [function = function](Event&) {
                function();
            });
        } else {
            boundedFunctions_.emplace_back(event, function);
        }
    }

    template <typename Controller>
    auto bindProxy(Controller* widget)
    {
        for (auto& proxyHandle : proxyHandles_) {
            using ::wxUI::customizations::ControllerBindProxy;
            ControllerBindProxy(widget, proxyHandle);
        }
        return widget;
    }

private:
    template <typename Controller>
    auto bindEvents(Controller* widget)
    {
        for (auto& bounded : boundedFunctions_) {
            using ::wxUI::customizations::ControllerBindEvent;
            ControllerBindEvent(widget, bounded);
        }
        return widget;
    }

    std::optional<wxSizerFlags> flags_;
    wxWindowID identity_ = wxID_ANY;
    wxPoint pos_ = wxDefaultPosition;
    wxSize size_ = wxDefaultSize;
    int64_t style_ {};
    bool enabled_ = true;
    std::optional<wxFontInfo> fontInfo_ {};
    std::vector<details::WidgetProxy<Underlying>> proxyHandles_ {};
    std::vector<BindInfo> boundedFunctions_;
};

}
