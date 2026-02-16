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

#include <functional>
#include <istream>
#include <ostream>

namespace wxUI::details {

// why is this Enable here?  Because we need to specialize on the type without over constraining.
template <typename Type, typename Getter, typename Setter, class Enable = void>
struct GetterSetter {

    static_assert(std::is_invocable_v<Getter>);
    static_assert(std::is_invocable_v<Setter, Type>);
    static_assert(std::is_invocable_v<Setter, std::invoke_result_t<Getter>>);

    explicit GetterSetter(Getter getter, Setter setter)
        : getter_(getter)
        , setter_(setter)
    {
    }

    [[nodiscard]] auto get() const -> Type
    {
        return getter_();
    }

    // we specifically want implicit conversions for ergonomics
    // NOLINTNEXTLINE (hicpp-explicit-conversions)
    operator Type() const
    {
        return getter_();
    }

    void set(Type const& value)
    {
        setter_(value);
    }

    auto operator=(Type const& value) -> GetterSetter&
    {
        setter_(value);
        return *this;
    }

    auto operator->() -> GetterSetter*
    {
        return this;
    }

    // NOLINTNEXTLINE (readability-identifier-length)
    friend auto operator<<(std::ostream& os, GetterSetter const& v) -> std::ostream&
    {
        return os << v.get();
    }

    // NOLINTNEXTLINE (readability-identifier-length)
    friend auto operator>>(std::istream& is, GetterSetter& v) -> std::istream&
    {
        auto temp = Type {};
        is >> temp;
        v.set(std::move(temp));
        return is;
    }

private:
    Getter getter_;
    Setter setter_;
};

// CTAD for GetterSetter
template <typename Getter, typename Setter>
GetterSetter(Getter, Setter) -> GetterSetter<std::invoke_result_t<Getter>, Getter, Setter>;

// Specialize for bool

// This implemention supports logic operations for bools only
template <typename Getter, typename Setter>
struct GetterSetter<bool, Getter, Setter> {

    using Type = bool;
    static_assert(std::is_invocable_v<Getter>);
    static_assert(std::is_invocable_v<Setter, Type>);
    static_assert(std::is_invocable_v<Setter, Type>);

    explicit GetterSetter(Getter getter, Setter setter)
        : getter_(getter)
        , setter_(setter)
    {
    }

    [[nodiscard]] auto get() const -> Type
    {
        return getter_();
    }

    // we specifically want implicit conversions for ergonomics
    // NOLINTNEXTLINE (hicpp-explicit-conversions)
    operator Type() const
    {
        return getter_();
    }

    void set(Type value)
    {
        setter_(value);
    }

    auto operator=(Type value) -> GetterSetter&
    {
        setter_(value);
        return *this;
    }

    auto operator&=(Type value) -> GetterSetter&
    {
        setter_(getter_() & value);
        return *this;
    }

    auto operator^=(Type value) -> GetterSetter&
    {
        setter_(getter_() ^ value);
        return *this;
    }

    auto operator|=(Type value) -> GetterSetter&
    {
        setter_(getter_() | value);
        return *this;
    }

    auto operator->() -> GetterSetter*
    {
        return this;
    }

    // NOLINTNEXTLINE (readability-identifier-length)
    friend auto operator<<(std::ostream& os, GetterSetter const& v) -> std::ostream&
    {
        return os << v.get();
    }

    // NOLINTNEXTLINE (readability-identifier-length)
    friend auto operator>>(std::istream& is, GetterSetter& v) -> std::istream&
    {
        auto temp = Type {};
        is >> temp;
        v.set(temp);
        return is;
    }

private:
    Getter getter_;
    Setter setter_;
};

// Specialize for integral types
template <class T>
concept Integral = std::is_integral_v<T>;

template <Integral Type, typename Getter, typename Setter>
struct GetterSetter<Type, Getter, Setter> {

    static_assert(std::is_invocable_v<Getter>);
    static_assert(std::is_invocable_v<Setter, Type>);
    static_assert(std::is_invocable_v<Setter, std::invoke_result_t<Getter>>);

    explicit GetterSetter(Getter getter, Setter setter)
        : getter_(getter)
        , setter_(setter)
    {
    }

    [[nodiscard]] auto get() const -> Type
    {
        return getter_();
    }

    // we specifically want implicit conversions for ergonomics
    // NOLINTNEXTLINE (hicpp-explicit-conversions)
    operator Type() const
    {
        return getter_();
    }

    void set(Type value)
    {
        setter_(value);
    }

    auto operator=(Type value) -> GetterSetter&
    {
        setter_(value);
        return *this;
    }

    auto operator+=(Type value) -> GetterSetter&
    {
        setter_(getter_() + value);
        return *this;
    }

    auto operator-=(Type value) -> GetterSetter&
    {
        setter_(getter_() - value);
        return *this;
    }

    auto operator*=(Type value) -> GetterSetter&
    {
        setter_(getter_() * value);
        return *this;
    }

    auto operator/=(Type value) -> GetterSetter&
    {
        setter_(getter_() / value);
        return *this;
    }

    auto operator%=(Type value) -> GetterSetter&
    {
        setter_(getter_() % value);
        return *this;
    }

    auto operator<<=(Type value) -> GetterSetter&
    {
        setter_(getter_() << value);
        return *this;
    }

    auto operator>>=(Type value) -> GetterSetter&
    {
        setter_(getter_() >> value);
        return *this;
    }

    auto operator&=(Type value) -> GetterSetter&
    {
        setter_(getter_() & value);
        return *this;
    }

    auto operator^=(Type value) -> GetterSetter&
    {
        setter_(getter_() ^ value);
        return *this;
    }

    auto operator|=(Type value) -> GetterSetter&
    {
        setter_(getter_() | value);
        return *this;
    }

    auto operator++() -> GetterSetter&
    {
        setter_(getter_() + 1);
        return *this;
    }

    auto operator++(int) -> Type
    {
        auto result = getter_();
        setter_(result + 1);
        return result;
    }

    auto operator--() -> GetterSetter&
    {
        setter_(getter_() - 1);
        return *this;
    }

    auto operator--(int) -> Type
    {
        auto result = getter_();
        setter_(result - 1);
        return result;
    }

    auto operator->() -> GetterSetter*
    {
        return this;
    }

    // NOLINTNEXTLINE (readability-identifier-length)
    friend auto operator<<(std::ostream& os, GetterSetter const& v) -> std::ostream&
    {
        return os << v.get();
    }

    // NOLINTNEXTLINE (readability-identifier-length)
    friend auto operator>>(std::istream& is, GetterSetter& v) -> std::istream&
    {
        auto temp = Type {};
        is >> temp;
        v.set(std::move(temp));
        return is;
    }

private:
    Getter getter_;
    Setter setter_;
};

// Specialize for Float types
template <class T>
concept FloatingPoint = std::is_floating_point_v<T>;

template <FloatingPoint Type, typename Getter, typename Setter>
struct GetterSetter<Type, Getter, Setter> {

    static_assert(std::is_invocable_v<Getter>);
    static_assert(std::is_invocable_v<Setter, Type>);
    static_assert(std::is_invocable_v<Setter, std::invoke_result_t<Getter>>);

    explicit GetterSetter(Getter getter, Setter setter)
        : getter_(getter)
        , setter_(setter)
    {
    }

    [[nodiscard]] auto get() const -> Type
    {
        return getter_();
    }

    // we specifically want implicit conversions for ergonomics
    // NOLINTNEXTLINE (hicpp-explicit-conversions)
    operator Type() const
    {
        return getter_();
    }

    void set(Type value)
    {
        setter_(value);
    }

    auto operator=(Type value) -> GetterSetter&
    {
        setter_(value);
        return *this;
    }

    auto operator+=(Type value) -> GetterSetter&
    {
        setter_(getter_() + value);
        return *this;
    }

    auto operator-=(Type value) -> GetterSetter&
    {
        setter_(getter_() - value);
        return *this;
    }

    auto operator*=(Type value) -> GetterSetter&
    {
        setter_(getter_() * value);
        return *this;
    }

    auto operator/=(Type value) -> GetterSetter&
    {
        setter_(getter_() / value);
        return *this;
    }

    auto operator++() -> GetterSetter&
    {
        setter_(getter_() + 1);
        return *this;
    }

    auto operator++(int) -> Type
    {
        auto result = getter_();
        setter_(result + 1);
        return result;
    }

    auto operator--() -> GetterSetter&
    {
        setter_(getter_() - 1);
        return *this;
    }

    auto operator--(int) -> Type
    {
        auto result = getter_();
        setter_(result - 1);
        return result;
    }

    auto operator->() -> GetterSetter*
    {
        return this;
    }

    // NOLINTNEXTLINE (readability-identifier-length)
    friend auto operator<<(std::ostream& os, GetterSetter const& v) -> std::ostream&
    {
        return os << v.get();
    }

    // NOLINTNEXTLINE (readability-identifier-length)
    friend auto operator>>(std::istream& is, GetterSetter& v) -> std::istream&
    {
        auto temp = Type {};
        is >> temp;
        v.set(std::move(temp));
        return is;
    }

private:
    Getter getter_;
    Setter setter_;
};

}

#include <wxUI/detail/ZapMacros.hpp>
