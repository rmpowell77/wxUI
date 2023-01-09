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

#include "Widget.h"
#include <variant>
#include <wx/sizer.h>
#include <wx/statbox.h>

namespace wxUI {

namespace details {

    template <details::Widget... W>
    struct Sizer {
        explicit Sizer(W const&... widgets)
            : Sizer(std::make_tuple(widgets...))
        {
        }

        explicit Sizer(wxSizerFlags const& flags, W const&... widgets)
            : Sizer(flags, std::make_tuple(widgets...))
        {
        }

        explicit Sizer(std::tuple<W...> const& widgets)
            : widgets(widgets)
        {
        }

        Sizer(wxSizerFlags const& flags, std::tuple<W...> const& widgets)
            : flags(flags)
            , widgets(widgets)
        {
        }

        auto createAndAdd(wxWindow* parent, wxSizer* parentSizer, wxSizerFlags const& parentFlags)
        {
            auto sizer = constructSizer(parent);
            auto currentFlags = flags ? *flags : parentFlags;
            createAndAddWidgets(parent, sizer, currentFlags);

            // and now add our sizer to the parent
            parentSizer->Add(sizer, currentFlags);
            return sizer;
        }

        auto attachTo(wxWindow* parent) -> auto&
        {
            auto sizer = constructSizer(parent);
            auto currentFlags = flags ? *flags : wxSizerFlags {};
            createAndAddWidgets(parent, sizer, currentFlags);

            parent->SetSizerAndFit(sizer);
            return *this;
        }

    private:
        virtual auto constructSizer(wxWindow* parent) const -> wxSizer* = 0;

        void createAndAddWidgets(wxWindow* parent, wxSizer* sizer, wxSizerFlags const& flags)
        {
            std::apply([parent, sizer, flags](auto&&... tupleArg) {
                (tupleArg.createAndAdd(parent, sizer, flags), ...);
            },
                widgets);
        }

    public:
        std::optional<wxSizerFlags> flags {};
        std::tuple<W...> widgets {};
    };

    template <wxOrientation orientation, details::Widget... W>
    struct Stack : Sizer<W...> {
        using super = Sizer<W...>;

        explicit Stack(W const&... widgets)
            : super(std::make_tuple(widgets...))
        {
        }

        explicit Stack(wxString const& caption, W const&... widgets)
            : super(std::make_tuple(widgets...))
            , caption(caption)
        {
        }

        explicit Stack(wxSizerFlags const& flags, W const&... widgets)
            : super(flags, std::make_tuple(widgets...))
        {
        }

        Stack(wxString const& caption, wxSizerFlags const& flags, W const&... widgets)
            : super(flags, std::make_tuple(widgets...))
            , caption(caption)
        {
        }

        explicit Stack(std::tuple<W...> const& widgets)
            : super(widgets)
        {
        }

        Stack(wxString const& caption, std::tuple<W...> const& widgets)
            : super(widgets)
            , caption(caption)
        {
        }

        Stack(wxSizerFlags const& flags, std::tuple<W...> const& widgets)
            : super(flags, widgets)
        {
        }

        Stack(wxString const& caption, wxSizerFlags const& flags, std::tuple<W...> const& widgets)
            : super(flags, widgets)
            , caption(caption)
        {
        }

    private:
        auto constructSizer(wxWindow* parent) const -> wxSizer* override { return caption ? new wxStaticBoxSizer(new wxStaticBox(parent, wxID_ANY, *caption), orientation) : new wxBoxSizer(orientation); }
        std::optional<wxString> caption {};
    };

}

template <details::Widget... W>
struct VStack : public details::Stack<wxVERTICAL, W...> {
    using super = details::Stack<wxVERTICAL, W...>;

    explicit VStack(W const&... widgets)
        : super(std::make_tuple(widgets...))
    {
    }

    explicit VStack(wxString const& caption, W const&... widgets)
        : super(caption, std::make_tuple(widgets...))
    {
    }

    explicit VStack(wxSizerFlags const& flags, W const&... widgets)
        : super(flags, std::make_tuple(widgets...))
    {
    }

    VStack(wxString const& caption, wxSizerFlags const& flags, W const&... widgets)
        : super(caption, flags, std::make_tuple(widgets...))
    {
    }

    explicit VStack(std::tuple<W...> const& widgets)
        : super(widgets)
    {
    }

    VStack(wxString const& caption, std::tuple<W...> const& widgets)
        : super(caption, widgets)
    {
    }

    VStack(wxSizerFlags const& flags, std::tuple<W...> const& widgets)
        : super(flags, widgets)
    {
    }

    VStack(wxString const& caption, wxSizerFlags const& flags, std::tuple<W...> const& widgets)
        : super(caption, flags, widgets)
    {
    }
};

template <details::Widget... W>
struct HStack : public details::Stack<wxHORIZONTAL, W...> {
    using super = details::Stack<wxHORIZONTAL, W...>;

    explicit HStack(W const&... widgets)
        : super(std::make_tuple(widgets...))
    {
    }

    explicit HStack(wxString const& caption, W const&... widgets)
        : super(caption, std::make_tuple(widgets...))
    {
    }

    explicit HStack(wxSizerFlags const& flags, W const&... widgets)
        : super(flags, std::make_tuple(widgets...))
    {
    }

    HStack(wxString const& caption, wxSizerFlags const& flags, W const&... widgets)
        : super(caption, flags, std::make_tuple(widgets...))
    {
    }

    explicit HStack(std::tuple<W...> const& widgets)
        : super(widgets)
    {
    }

    HStack(wxString const& caption, std::tuple<W...> const& widgets)
        : super(caption, widgets)
    {
    }

    HStack(wxSizerFlags const& flags, std::tuple<W...> const& widgets)
        : super(flags, widgets)
    {
    }

    HStack(wxString const& caption, wxSizerFlags const& flags, std::tuple<W...> const& widgets)
        : super(caption, flags, widgets)
    {
    }
};

struct Generic {
    std::optional<wxSizerFlags> flags {};
    std::variant<wxSizer*, wxWindow*> child;

    Generic(wxSizerFlags const& flags, wxSizer* sizer)
        : flags(flags)
        , child(sizer)
    {
    }

    explicit Generic(wxSizer* sizer)
        : child(sizer)
    {
    }

    Generic(wxSizerFlags const& flags, wxWindow* window)
        : flags(flags)
        , child(window)
    {
    }

    explicit Generic(wxWindow* window)
        : child(window)
    {
    }

    void createAndAdd([[maybe_unused]] wxWindow* parent, wxSizer* parentSizer, wxSizerFlags const& parentFlags) const
    {
        return std::visit(details::overloaded {
                              [this, parentSizer, parentFlags](auto arg) {
                                  parentSizer->Add(arg, flags ? *flags : parentFlags);
                              },
                          },
            child);
    }
};
}
