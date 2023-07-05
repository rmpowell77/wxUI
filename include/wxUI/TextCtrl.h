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

#include "GetterSetter.h"
#include "Widget.h"
#include <wx/textctrl.h>

#include "HelperMacros.h"

namespace wxUI {

// https://docs.wxwidgets.org/latest/classwx_text_ctrl.html
struct TextCtrl : public details::WidgetDetails<TextCtrl, wxTextCtrl> {
    using super = details::WidgetDetails<TextCtrl, wxTextCtrl>;

    explicit TextCtrl(wxWindowID identity, std::string text = "")
        : super(identity)
        , text(std::move(text))
    {
    }

    explicit TextCtrl(std::string text = "")
        : TextCtrl(wxID_ANY, std::move(text))
    {
    }

    TextCtrl(wxSizerFlags const& flags, wxWindowID identity, std::string text = "")
        : super(flags, identity)
        , text(std::move(text))
    {
    }

    explicit TextCtrl(wxSizerFlags const& flags, std::string text = "")
        : TextCtrl(flags, wxID_ANY, std::move(text))
    {
    }

    auto create(wxWindow* parent) -> wxWindow* override
    {
        return setProxy(new underlying_t(parent, getIdentity(), text, getPos(), getSize(), getStyle()));
    }

    // Bind
    using super::bind;
    template <typename Function>
    auto bind(Function func)
    {
        return super::bind(wxEVT_TEXT, func);
    }

    struct Proxy : super::WidgetProxy {
        PROXY_BOILERPLATE();

        [[nodiscard]] auto label() const
        {
            auto* controller = control();
            return details::GetterSetter {
                [controller] { return static_cast<std::string>(controller->GetLabel()); },
                [controller](std::string label) { controller->SetLabel(label); }
            };
        }

        auto operator->() const { return label(); }
        auto operator*() const { return label(); }
    };

    RULE_OF_SIX_BOILERPLATE(TextCtrl);

private:
    std::string text;
};

WIDGET_STATIC_ASSERT_BOILERPLATE(TextCtrl);
}

#include "ZapMacros.h"
