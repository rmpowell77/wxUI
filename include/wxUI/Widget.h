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

#include <optional>
#include <wx/sizer.h>

namespace wxUI::details {

// A widget is anything that supports the createAndAdd function.

// clang-format off
template <typename T>
concept Widget = requires(T widget, wxWindow* window, wxSizer* sizer)
{
    widget.createAndAdd(window, sizer, wxSizerFlags {});
};

template <typename T>
concept CreateAndAddFunction = requires(T function, wxWindow* window, wxSizer* sizer)
{
    function(window, sizer, wxSizerFlags {});
};

// clang-format on

// https://stackoverflow.com/questions/27866909/get-function-arity-from-template-parameter
template <typename T>
constexpr bool noarg_callable_impl(
    typename std::enable_if<bool(sizeof((std::declval<T>()(), 0)))>::type*)
{
    return true;
}

template <typename T>
constexpr bool noarg_callable_impl(...)
{
    return false;
}

template <typename T>
constexpr bool is_noarg_callable()
{
    return noarg_callable_impl<T>(nullptr);
}

template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

template <typename Controller, typename Underlying>
struct WidgetProxy;

// BindInfo uses type erase to allow any binding for any Event type.
struct BindInfo {

    void bindTo([[maybe_unused]] wxWindow* widget) const
    {
        mInfo->bindTo(widget);
    }

    template <typename Event, typename Function>
    BindInfo([[maybe_unused]] Event event, [[maybe_unused]] Function function)
        : mInfo(std::make_unique<BindInfoDetails<Event, Function>>(event, function))
    {
    }

    ~BindInfo() = default;

    BindInfo(BindInfo const& bindInfo)
        : mInfo(bindInfo.mInfo->clone())
    {
    }

    auto operator=(BindInfo const& bindInfo) -> BindInfo&
    {
        if (this == &bindInfo) {
            return *this;
        }
        mInfo = bindInfo.mInfo->clone();
        return *this;
    }

    BindInfo(BindInfo&& bindInfo) noexcept
        : mInfo(std::move(bindInfo.mInfo))
    {
    }

    auto operator=(BindInfo&& bindInfo) noexcept -> BindInfo&
    {
        mInfo = std::move(bindInfo.mInfo);
        return *this;
    }

private:
    struct BindInfoDetailsBase {
        virtual ~BindInfoDetailsBase() = default;
        virtual void bindTo(wxWindow* widget) const = 0;
        [[nodiscard]] virtual auto clone() const -> std::unique_ptr<BindInfoDetailsBase> = 0;
    };

    template <typename Event, typename Function>
    struct BindInfoDetails : BindInfoDetailsBase {
        BindInfoDetails(Event event, Function function)
            : event(event)
            , function(function)
        {
        }
        Event event;
        Function function;
        void bindTo(wxWindow* widget) const override
        {
            widget->Bind(event, function);
        }
        [[nodiscard]] auto clone() const -> std::unique_ptr<BindInfoDetailsBase> override
        {
            return std::make_unique<BindInfoDetails>(event, function);
        }
    };

    std::unique_ptr<BindInfoDetailsBase> mInfo;
};

static_assert(std::is_nothrow_move_constructible_v<BindInfo>);
static_assert(std::is_nothrow_move_assignable_v<BindInfo>);

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

    auto withSize(wxSize size_) -> ConcreteWidget&
    {
        size = size_;
        return static_cast<ConcreteWidget&>(*this);
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

    auto createAndAdd(wxWindow* parent, wxSizer* sizer, wxSizerFlags const& parentFlags) -> Underlying*
    {
        auto widget = dynamic_cast<Underlying*>(create(parent));
        if (fontInfo) {
            widget->SetFont(wxFont(*fontInfo));
        }
        widget = bindEvents(widget);
        // bind any events
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
    virtual auto create(wxWindow* parent) -> wxWindow* = 0;

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

// clang-format off

#define RULE_OF_SIX_BOILERPLATE(WIDGET)               \
    virtual ~WIDGET() = default;                      \
    WIDGET(WIDGET const&) = default;                  \
    WIDGET(WIDGET&&) noexcept = default;              \
    auto operator=(WIDGET const&)->WIDGET& = default; \
    auto operator=(WIDGET&&) noexcept -> WIDGET& = default;

// clang-format on

#define PROXY_BOILERPLATE()                                                           \
    template <typename W>                                                             \
    auto operator=(W&& controller)->W&& { return bind(std::forward<W>(controller)); } \
    template <typename W>                                                             \
    auto bind(W&& widget)->W&&                                                        \
    {                                                                                 \
        widget.setProxyHandle(this);                                                  \
        return std::forward<W>(widget);                                               \
    }

#define WIDGET_STATIC_ASSERT_BOILERPLATE(WIDGET)                 \
    static_assert(details::Widget<WIDGET>);                      \
    static_assert(std::is_nothrow_move_constructible_v<WIDGET>); \
    static_assert(std::is_nothrow_move_assignable_v<WIDGET>)

}
