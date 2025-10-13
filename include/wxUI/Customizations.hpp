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

namespace wxUI::customizations {

template <typename>
inline constexpr bool always_false_v = false;

// Customization points for creating underlying widgets via different
// "parents". By default this will construct the real wxWidgets object
// when the parent is a wxWindow (or derived). Tests can provide their own
// specializations of ParentCreateImpl in the wxUI::details namespace to
// capture calls without needing an actual wxWindow.
template <typename Underlying, typename Parent>
struct ParentCreateImpl {
    template <typename... Args>
    static auto create(Parent* parent, Args&&... args) -> Underlying*
    {
        if constexpr (std::is_convertible_v<Parent*, wxWindow*>) {
            return new Underlying(parent, std::forward<Args>(args)...);
        } else {
            static_assert(std::is_convertible_v<Parent*, wxWindow*>, "ParentCreateImpl must be specialized for non-wx parents");
            return nullptr; // unreachable
        }
    }
};

// Forwarding convenience function used by controllers. It simply forwards
// the call to the implementation class above — allowing callers to use a
// simple function-style API while tests can partially-specialize the
// class template.
template <typename Underlying, typename Parent, typename... Args>
inline auto ParentCreate(Parent* parent, Args&&... args)
{
    return ParentCreateImpl<Underlying, Parent>::create(parent, std::forward<Args>(args)...);
}

//--- Customization points for controllers ---//
// clang-format off

template <typename Controller>
inline void ControllerSetDefault(Controller* controller)
{
    if constexpr (requires(Controller* w) { w->SetDefault(); }) {
        controller->SetDefault();
    } else {
        static_assert(always_false_v<Controller>, "ControllerSetDefault: Provide a customization in namespace wxUI::customizations.");
    }
}

template <typename Controller>
inline void ControllerSetFont(Controller* controller, wxFont const& font)
{
    if constexpr (requires(Controller* w) { w->SetFont(font); }) {
        controller->SetFont(font);
    } else {
        static_assert(always_false_v<Controller>, "ControllerSetFont: Provide a customization in namespace wxUI::customizations.");
    }
}

template <typename Controller>
inline void ControllerSetEnabled(Controller* controller, bool enabled)
{
    if constexpr (requires(Controller* w) { w->Enable(enabled); }) {
        controller->Enable(enabled);
    } else {
        static_assert(always_false_v<Controller>, "ControllerSetEnabled: Provide a customization in namespace wxUI::customizations.");
    }
}

template <typename Controller>
inline void ControllerEnsureVisible(Controller* controller, bool visible)
{
    if constexpr (requires(Controller* w) { w->EnsureVisible(visible); }) {
        controller->EnsureVisible(visible);
    } else {
        static_assert(always_false_v<Controller>, "ControllerEnsureVisible: Provide a customization in namespace wxUI::customizations.");
    }
}

template <typename Controller>
inline void ControllerWrap(Controller* controller, bool wrap)
{
    if constexpr (requires(Controller* w) { w->Wrap(wrap); }) {
        controller->Wrap(wrap);
    } else {
        static_assert(always_false_v<Controller>, "ControllerWrap: Provide a customization in namespace wxUI::customizations.");
    }
}

template <typename Controller>
inline void ControllerBindEvent(Controller* controller, details::BindInfo const& boundedFunction)
{
    if constexpr (std::is_convertible_v<Controller*, wxWindow*>) {
        boundedFunction.bindTo(controller);
    } else {
        static_assert(always_false_v<Controller>, "ControllerBindEvent: Provide a customization in namespace wxUI::customizations..");
    }
}

template <typename Controller, typename Proxy>
inline void ControllerBindProxy(Controller* controller, Proxy& proxyHandle)
{
    if constexpr (requires(Proxy proxy, Controller* c) { proxy.setUnderlying(c); }) {
        proxyHandle.setUnderlying(controller);
    } else {
        static_assert(always_false_v<Proxy>, "ControllerBindProxy: Provide a customization in namespace wxUI::customizations.");
    }
}

template <typename Controller>
inline void ControllerSetValue(Controller* controller, bool value)
{
    if constexpr (requires(Controller* w) { w->SetValue(value); }) {
        controller->SetValue(value);
    } else {
        static_assert(always_false_v<Controller>, "ControllerSetValue: Provide a customization in namespace wxUI::customizations.");
    }
}

template <typename Controller>
inline void ControllerSetSelection(Controller* controller, int selection)
{
    if constexpr (requires(Controller* w) { w->SetSelection(selection); }) {
        controller->SetSelection(selection);
    } else {
        static_assert(always_false_v<Controller>, "ControllerSetSelection: Provide a customization in namespace wxUI::customizations.");
    }
}

template <typename Controller, typename Bitmap>
inline void ControllerSetItemBitmap(Controller* controller, unsigned int n, Bitmap const& bitmap)
{
    if constexpr (requires(Controller* w) { w->SetItemBitmap(n, bitmap); }) {
        controller->SetItemBitmap(n, bitmap);
    } else {
        static_assert(always_false_v<Controller>, "ControllerSetItemBitmap: Provide a customization in namespace wxUI::customizations.");
    }
}

template <typename Controller, typename Bitmap>
inline void ControllerSetBitmapPressed(Controller* controller, Bitmap const& bitmap)
{
    if constexpr (requires(Controller* w) { w->SetBitmapPressed(bitmap); }) {
        controller->SetBitmapPressed(bitmap);
    } else {
        static_assert(always_false_v<Controller>, "ControllerSetBitmapPressed: Provide a customization in namespace wxUI::customizations.");
    }
}

template <typename Controller, typename W1, typename W2>
inline void ControllerSplitHorizontal(Controller* controller, W1* window1, W2* window2)
{
    if constexpr (requires(Controller* w) { w->SplitHorizontally(window1, window2); }) {
        controller->SplitHorizontally(window1, window2);
    } else {
        static_assert(always_false_v<Controller>, "ControllerSplitHorizontal: Provide a customization in namespace wxUI::customizations..");
    }
}

template <typename Controller, typename W1, typename W2>
inline void ControllerSplitVertical(Controller* controller, W1* window1, W2* window2)
{
    if constexpr (requires(Controller* w) { w->SplitVertically(window1, window2); }) {
        controller->SplitVertically(window1, window2);
    } else {
        static_assert(always_false_v<Controller>, "ControllerSplitVertically: Provide a customization in namespace wxUI::customizations.");
    }
}

template <typename Controller>
inline void ControllerSetSashGravity(Controller* controller, double gravity)
{
    if constexpr (requires(Controller* w) { w->SetSashGravity(gravity); }) {
        controller->SetSashGravity(gravity);
    } else {
        static_assert(always_false_v<Controller>, "ControllerSetSashGravity: Provide a customization in namespace wxUI::customizations.");
    }
}
// clang-format on

}
