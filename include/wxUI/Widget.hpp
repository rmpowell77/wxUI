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
#include <optional>
#include <stdexcept>
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
    [[nodiscard]] auto control() const -> Underlying*
    {
        if (!controller) {
            throw std::runtime_error("Proxy class has not been attached");
        }
        return controller;
    }

    void setUnderlying(Underlying* control)
    {
        controller = control;
    }

    auto operator->() const { return control(); }

private:
    Underlying* controller {};
};

// The WidgetDetails are the base class of the Controllers.  The common details
// across many controllers are stored in the base class.
// The "recipe" for constructing a controller is pretty straight forward:
// 1. give a name.
// 2. inherit from WidgetDetails.
// 3. implement the create function for constructing the concrete widget.
template <typename ConcreteWidget, typename Underlying>
struct WidgetDetails {
    using Controller = ConcreteWidget;
    using underlying_t = Underlying;

    struct WithStyle {
        int64_t mStyle;
    };

    explicit WidgetDetails(wxWindowID identity = wxID_ANY)
        : identity_(identity)
    {
    }

    WidgetDetails(wxWindowID identity, WithStyle style)
        : identity_(identity)
        , style_(style.mStyle)
    {
    }

    auto withFlags(wxSizerFlags flags) & -> ConcreteWidget&
    {
        flags_ = flags;
        return static_cast<ConcreteWidget&>(*this);
    }

    auto withFlags(wxSizerFlags flags) && -> ConcreteWidget&&
    {
        flags_ = flags;
        return static_cast<ConcreteWidget&&>(std::move(*this));
    }

    auto withPosition(wxPoint pos) & -> ConcreteWidget&
    {
        pos_ = pos;
        return static_cast<ConcreteWidget&>(*this);
    }

    auto withPosition(wxPoint pos) && -> ConcreteWidget&&
    {
        pos_ = pos;
        return static_cast<ConcreteWidget&&>(std::move(*this));
    }

    auto withSize(wxSize size) & -> ConcreteWidget&
    {
        size_ = size;
        return static_cast<ConcreteWidget&>(*this);
    }

    auto withSize(wxSize size) && -> ConcreteWidget&&
    {
        size_ = size;
        return static_cast<ConcreteWidget&&>(std::move(*this));
    }

    auto withWidth(int size) & -> ConcreteWidget&
    {
        size_.SetWidth(size);
        return static_cast<ConcreteWidget&>(*this);
    }

    auto withWidth(int size) && -> ConcreteWidget&&
    {
        size_.SetWidth(size);
        return static_cast<ConcreteWidget&&>(std::move(*this));
    }

    auto withHeight(int size) & -> ConcreteWidget&
    {
        size_.SetHeight(size);
        return static_cast<ConcreteWidget&>(*this);
    }

    auto withHeight(int size) && -> ConcreteWidget&&
    {
        size_.SetHeight(size);
        return static_cast<ConcreteWidget&&>(std::move(*this));
    }

    auto setStyle(int64_t style) & -> ConcreteWidget&
    {
        style_ = style;
        return static_cast<ConcreteWidget&>(*this);
    }

    auto setStyle(int64_t style) && -> ConcreteWidget&&
    {
        style_ = style;
        return static_cast<ConcreteWidget&&>(std::move(*this));
    }

    auto withStyle(int64_t style) & -> ConcreteWidget&
    {
        style_ |= style;
        return static_cast<ConcreteWidget&>(*this);
    }

    auto withStyle(int64_t style) && -> ConcreteWidget&&
    {
        style_ |= style;
        return static_cast<ConcreteWidget&&>(std::move(*this));
    }

    auto withoutStyle(int64_t style) & -> ConcreteWidget&
    {
        style_ &= ~style;
        return static_cast<ConcreteWidget&>(*this);
    }

    auto withoutStyle(int64_t style) && -> ConcreteWidget&&
    {
        style_ &= ~style;
        return static_cast<ConcreteWidget&&>(std::move(*this));
    }

    auto withFont(wxFontInfo const& fontInfo) & -> ConcreteWidget&
    {
        fontInfo_ = fontInfo;
        return static_cast<ConcreteWidget&>(*this);
    }

    auto withFont(wxFontInfo const& fontInfo) && -> ConcreteWidget&&
    {
        fontInfo_ = fontInfo;
        return static_cast<ConcreteWidget&&>(std::move(*this));
    }

    auto setEnabled(bool enabled) & -> ConcreteWidget&
    {
        enabled_ = enabled;
        return static_cast<ConcreteWidget&>(*this);
    }

    auto setEnabled(bool enabled) && -> ConcreteWidget&&
    {
        enabled_ = enabled;
        return static_cast<ConcreteWidget&&>(std::move(*this));
    }

    auto create(wxWindow* parent) -> Underlying*
    {
        auto widget = dynamic_cast<Underlying*>(createImpl(parent));
        if (fontInfo_) {
            widget->SetFont(wxFont(*fontInfo_));
        }
        widget->Enable(enabled_);
        // bind any events
        widget = bindEvents(widget);
        return widget;
    }

    auto createAndAdd(wxWindow* parent, wxSizer* sizer, wxSizerFlags const& parentFlags) -> Underlying*
    {
        auto widget = create(parent);
        // add to parent
        sizer->Add(widget, flags_.value_or(parentFlags));
        return widget;
    }

    auto getIdentity() const { return identity_; }
    auto getPos() const { return pos_; }
    auto getSize() const { return size_; }
    auto getStyle() const { return style_; }
    auto getFlags() const { return flags_; }

    void setProxyHandle(WidgetProxy<Underlying>* proxy)
    {
        proxyHandle_ = proxy;
    }

protected:
    template <typename Function, typename Event = wxCommandEvent>
    auto bind(wxEventTypeTag<Event> event, Function function) & -> ConcreteWidget&
    {
        if constexpr (is_noarg_callable<Function>()) {
            boundedFunctions_.emplace_back(event, [function = function](Event&) {
                function();
            });
        } else {
            boundedFunctions_.emplace_back(event, function);
        }
        return static_cast<ConcreteWidget&>(*this);
    }

    template <typename Function, typename Event = wxCommandEvent>
    auto bind(wxEventTypeTag<Event> event, Function function) && -> ConcreteWidget&&
    {
        if constexpr (is_noarg_callable<Function>()) {
            boundedFunctions_.emplace_back(event, [function = function](Event&) {
                function();
            });
        } else {
            boundedFunctions_.emplace_back(event, function);
        }
        return static_cast<ConcreteWidget&&>(std::move(*this));
    }

    auto setProxy(Underlying* widget) -> Underlying*
    {
        if (proxyHandle_) {
            proxyHandle_->setUnderlying(widget);
        }
        return widget;
    }

private:
    auto bindEvents(Underlying* widget) -> Underlying*
    {
        for (auto& bounded : boundedFunctions_) {
            bounded.bindTo(widget);
        }
        return widget;
    }

    // these should be implemented in the derived classes.
    // aka the Template Pattern
    virtual auto createImpl(wxWindow* parent) -> wxWindow* = 0;

    std::optional<wxSizerFlags> flags_;
    // these are common across the controls
    wxWindowID identity_ = wxID_ANY;
    wxPoint pos_ = wxDefaultPosition;
    wxSize size_ = wxDefaultSize;
    int64_t style_ {};
    bool enabled_ = true;
    std::optional<wxFontInfo> fontInfo_ {};
    details::WidgetProxy<Underlying>* proxyHandle_ {}; // optional WidgetProxy
    std::vector<BindInfo> boundedFunctions_;
};

}
