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

#include <wx/sizer.h>
#include <wxUI/Widget.hpp>

#include <wxUI/detail/HelperMacros.hpp>

namespace wxUI {

// https://docs.wxwidgets.org/latest/classwx_std_dialog_button_sizer.html
struct StdDialogButtons {
    using underlying_t = wxStdDialogButtonSizer;

    explicit StdDialogButtons(wxDialog* dlg, long flags)
        : dlg_(dlg)
        , flags_(flags)
    {
    }

    struct Proxy : details::Proxy<underlying_t> {
    };

    template <typename Parent>
    auto create(Parent* parent)
    {
        return createImpl(parent);
    }

    template <typename Parent, typename Sizer>
    auto createAndAdd(Parent* parent, Sizer* parentSizer, wxSizerFlags const& parentFlags)
    {
        auto* window = create(parent);
        using Window = std::remove_pointer_t<decltype(window)>;
        if constexpr (requires(Sizer* sizer, Window* window, wxSizerFlags flags) { sizer->Add(window, flags); }) {
            parentSizer->Add(window, parentFlags);
        }
        return window;
    }

private:
    wxDialog* dlg_;
    long flags_;

    template <typename Parent>
    auto createImpl(Parent* parent)
    {
        auto* sizer = new wxStdDialogButtonSizer();

        wxButton* ok = nullptr;
        wxButton* yes = nullptr;
        wxButton* no = nullptr;

        if (flags_ & wxOK) {
            ok = customizations::ParentCreate<wxButton>(parent, wxID_OK);
            sizer->AddButton(ok);
        }

        if (flags_ & wxCANCEL) {
            auto* cancel = customizations::ParentCreate<wxButton>(parent, wxID_CANCEL);
            sizer->AddButton(cancel);
        }

        if (flags_ & wxYES) {
            yes = customizations::ParentCreate<wxButton>(parent, wxID_YES);
            sizer->AddButton(yes);
        }

        if (flags_ & wxNO) {
            no = customizations::ParentCreate<wxButton>(parent, wxID_NO);
            sizer->AddButton(no);
        }

        if (flags_ & wxAPPLY) {
            auto* apply = customizations::ParentCreate<wxButton>(parent, wxID_APPLY);
            sizer->AddButton(apply);
        }

        if (flags_ & wxCLOSE) {
            auto* close = customizations::ParentCreate<wxButton>(parent, wxID_CLOSE);
            sizer->AddButton(close);
        }

        if (flags_ & wxHELP) {
            auto* help = customizations::ParentCreate<wxButton>(parent, wxID_HELP);
            sizer->AddButton(help);
        }

        if (flags_ & wxNO_DEFAULT) {
            if (no) {
                no->SetDefault();
                no->SetFocus();
            }
        } else {
            if (ok) {
                ok->SetDefault();
                ok->SetFocus();
            } else if (yes) {
                yes->SetDefault();
                yes->SetFocus();
            }
        }

        auto setAffirmatives = [](auto* dlg, long flags) {
            if (dlg == nullptr) {
                return;
            }
            if (flags & wxOK) {
                dlg->SetAffirmativeId(wxID_OK);
            } else if (flags & wxYES) {
                dlg->SetAffirmativeId(wxID_YES);
            } else if (flags & wxCLOSE) {
                dlg->SetAffirmativeId(wxID_CLOSE);
            }
        };
        if (dlg_) {
            setAffirmatives(dlg_, flags_);
        } else {
            if constexpr (requires(Parent* p) { p->SetAffirmativeId(wxID_OK); }) {
                setAffirmatives(dlg_, flags_);
            }
        }

        sizer->Realize();

        return sizer;
    }

public:
    WXUI_RULE_OF_SIX_BOILERPLATE(StdDialogButtons)
};

WXUI_WIDGET_STATIC_ASSERT_BOILERPLATE(StdDialogButtons);
}

#include <wxUI/detail/ZapMacros.hpp>
