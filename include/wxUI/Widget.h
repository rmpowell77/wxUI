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

#include "BindInfo.h"
#include <optional>
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

// clang-format on

template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

template <typename Controller, typename Underlying>
struct WidgetProxy;

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

    struct WidgetProxy {
        using Controller = ConcreteWidget;
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

    private:
        Underlying* controller {};
    };

    struct WithStyle {
        int64_t mStyle;
    };

    explicit WidgetDetails(wxWindowID identity = wxID_ANY)
        : identity(identity)
    {
    }

    WidgetDetails(wxSizerFlags const& flags, wxWindowID identity)
        : flags(flags)
        , identity(identity)
    {
    }

    explicit WidgetDetails(wxWindowID identity, WithStyle style)
        : identity(identity)
        , style(style.mStyle)
    {
    }

    WidgetDetails(wxSizerFlags const& flags, wxWindowID identity, WithStyle style)
        : flags(flags)
        , identity(identity)
        , style(style.mStyle)
    {
    }

    auto withPosition(wxPoint pos_) -> ConcreteWidget&
    {
        pos = pos_;
        return static_cast<ConcreteWidget&>(*this);
    }

    auto withSize(wxSize size_) & -> ConcreteWidget&
    {
        size = size_;
        return static_cast<ConcreteWidget&>(*this);
    }

    auto withSize(wxSize size_) && -> ConcreteWidget&&
    {
        size = size_;
        return static_cast<ConcreteWidget&&>(std::move(*this));
    }

    auto withWidthSize(int size_) -> ConcreteWidget&
    {
        size.SetWidth(size_);
        return static_cast<ConcreteWidget&>(*this);
    }

    auto withHeightSize(int size_) -> ConcreteWidget&
    {
        size.SetHeight(size_);
        return static_cast<ConcreteWidget&>(*this);
    }

    auto withStyle(int64_t style_) -> ConcreteWidget&
    {
        style |= style_;
        return static_cast<ConcreteWidget&>(*this);
    }

    auto withoutStyle(int64_t style_) -> ConcreteWidget&
    {
        style &= ~style_;
        return static_cast<ConcreteWidget&>(*this);
    }

    auto withFont(wxFontInfo const& fontInfo_) -> ConcreteWidget&
    {
        fontInfo = fontInfo_;
        return static_cast<ConcreteWidget&>(*this);
    }

    template <typename Function, typename Event = wxCommandEvent>
    auto bind(wxEventTypeTag<Event> event, Function function)
    {
        if constexpr (is_noarg_callable<Function>()) {
            boundedFunctions.emplace_back(event, [function = function](Event&) {
                function();
            });
        } else {
            boundedFunctions.emplace_back(event, function);
        }
        return static_cast<ConcreteWidget&>(*this);
    }

    auto bindEvents(Underlying* widget) -> Underlying*
    {
        for (auto& bounded : boundedFunctions) {
            bounded.bindTo(widget);
        }
        return widget;
    }

    auto create(wxWindow* parent) -> Underlying*
    {
        auto widget = dynamic_cast<Underlying*>(createImpl(parent));
        if (fontInfo) {
            widget->SetFont(wxFont(*fontInfo));
        }
        // bind any events
        widget = bindEvents(widget);
        return widget;
    }

    auto createAndAdd(wxWindow* parent, wxSizer* sizer, wxSizerFlags const& parentFlags) -> Underlying*
    {
        auto widget = create(parent);
        // add to parent
        sizer->Add(widget, flags ? *flags : parentFlags);
        return widget;
    }

    auto getIdentity() const { return identity; }
    auto getPos() const { return pos; }
    auto getSize() const { return size; }
    auto getStyle() const { return style; }

    void setProxyHandle(WidgetProxy* proxy)
    {
        proxyHandle = proxy;
    }

protected:
    auto setProxy(Underlying* widget) -> Underlying*
    {
        if (proxyHandle) {
            (*proxyHandle)->setUnderlying(widget);
        }
        return widget;
    }

private:
    // these should be implemented in the derived classes.
    // aka the Template Pattern
    virtual auto createImpl(wxWindow* parent) -> wxWindow* = 0;

    std::optional<wxSizerFlags> flags;
    // these are common across the controls
    wxWindowID identity = wxID_ANY;
    wxPoint pos = wxDefaultPosition;
    wxSize size = wxDefaultSize;
    int64_t style {};
    std::optional<wxFontInfo> fontInfo {};
    std::optional<WidgetProxy*> proxyHandle {};
    std::vector<BindInfo> boundedFunctions;
};

}
