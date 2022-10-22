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

#include <wxUI/Widget.h>

namespace wxUI {

namespace details {

    template <details::Widget... W>
    struct Sizer {
        Sizer(W const&... widgets)
            : Sizer(std::make_tuple(widgets...))
        {
        }

        Sizer(wxSizerFlags const& flags, W const&... widgets)
            : Sizer(flags, std::make_tuple(widgets...))
        {
        }

        Sizer(std::tuple<W...> const& widgets)
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

        auto& asTopLevel(wxWindow* parent)
        {
            auto sizer = constructSizer(parent);
            auto currentFlags = flags ? *flags : wxSizerFlags {};
            createAndAddWidgets(parent, sizer, currentFlags);

            parent->SetSizerAndFit(sizer);
            return *this;
        }

    private:
        virtual wxSizer* constructSizer(wxWindow* parent) const = 0;

        void createAndAddWidgets(wxWindow* parent, wxSizer* sizer, wxSizerFlags const& flags)
        {
            std::apply([parent, sizer, flags](auto&&... tupleArg) {
                (tupleArg.createAndAdd(parent, sizer, flags), ...);
            },
                widgets);
        }

    public:
        std::optional<wxSizerFlags> flags {};
        std::tuple<W...> widgets;
    };

    template <wxOrientation orientation, details::Widget... W>
    struct Stack : Sizer<W...> {
        using super = Sizer<W...>;

        Stack(W const&... widgets)
            : super(std::make_tuple(widgets...))
        {
        }

        Stack(wxString const& caption, W const&... widgets)
            : super(std::make_tuple(widgets...))
            , caption(caption)
        {
        }

        Stack(wxSizerFlags const& flags, W const&... widgets)
            : super(flags, std::make_tuple(widgets...))
        {
        }

        Stack(wxString const& caption, wxSizerFlags const& flags, W const&... widgets)
            : super(flags, std::make_tuple(widgets...))
            , caption(caption)
        {
        }

        Stack(std::tuple<W...> const& widgets)
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
        wxSizer* constructSizer(wxWindow* parent) const override { return caption ? new wxStaticBoxSizer(new wxStaticBox(parent, wxID_ANY, *caption), orientation) : new wxBoxSizer(orientation); }
        std::optional<wxString> caption;
    };

}

template <details::Widget... W>
struct VStack : public details::Stack<wxVERTICAL, W...> {
    using super = details::Stack<wxVERTICAL, W...>;

    VStack(W const&... widgets)
        : super(std::make_tuple(widgets...))
    {
    }

    VStack(wxString const& caption, W const&... widgets)
        : super(caption, std::make_tuple(widgets...))
    {
    }

    VStack(wxSizerFlags const& flags, W const&... widgets)
        : super(flags, std::make_tuple(widgets...))
    {
    }

    VStack(wxString const& caption, wxSizerFlags const& flags, W const&... widgets)
        : super(caption, flags, std::make_tuple(widgets...))
    {
    }

    VStack(std::tuple<W...> const& widgets)
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

    HStack(W const&... widgets)
        : super(std::make_tuple(widgets...))
    {
    }

    HStack(wxString const& caption, W const&... widgets)
        : super(caption, std::make_tuple(widgets...))
    {
    }

    HStack(wxSizerFlags const& flags, W const&... widgets)
        : super(flags, std::make_tuple(widgets...))
    {
    }

    HStack(wxString const& caption, wxSizerFlags const& flags, W const&... widgets)
        : super(caption, flags, std::make_tuple(widgets...))
    {
    }

    HStack(std::tuple<W...> const& widgets)
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

}
