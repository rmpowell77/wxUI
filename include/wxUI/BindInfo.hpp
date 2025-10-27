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

#include <memory>
#include <optional>
#include <wx/sizer.h>

namespace wxUI::details {

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

// BindInfo uses type erase to allow any binding for any Event type.
struct BindInfo {

    void bindTo(wxWindow* widget) const
    {
        info_->bindTo(widget);
    }

    template <typename Event, typename Function>
    BindInfo(Event event, Function function)
        : info_(std::make_unique<BindInfoDetails<Event, Function>>(event, function))
    {
    }

    ~BindInfo() = default;

    BindInfo(BindInfo const& bindInfo)
        : info_(bindInfo.info_->clone())
    {
    }

    auto operator=(BindInfo const& bindInfo) -> BindInfo&
    {
        if (this == &bindInfo) {
            return *this;
        }
        info_ = bindInfo.info_->clone();
        return *this;
    }

    BindInfo(BindInfo&& bindInfo) noexcept
        : info_(std::move(bindInfo.info_))
    {
    }

    auto operator=(BindInfo&& bindInfo) noexcept -> BindInfo&
    {
        info_ = std::move(bindInfo.info_);
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

    std::unique_ptr<BindInfoDetailsBase> info_;
};

static_assert(std::is_nothrow_move_constructible_v<BindInfo>);
static_assert(std::is_nothrow_move_assignable_v<BindInfo>);

}
