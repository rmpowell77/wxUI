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
// wxUI Extended Dialog Example

#include <wx/wx.h>
#include <wxUI/wxUI.hpp>

class ExtendedExample : public wxDialog {
public:
    explicit ExtendedExample(wxWindow* parent);

private:
    wxUI::Text::Proxy textProxy;
    wxUI::SpinCtrl::Proxy spinProxy;
};

class MultibindExample : public wxDialog {
public:
    explicit MultibindExample(wxWindow* parent);
};

class SplitterExample : public wxDialog {
public:
    explicit SplitterExample(wxWindow* parent);

private:
    wxUI::TextCtrl::Proxy rightUpper;
};

class GenericExample : public wxDialog {
public:
    explicit GenericExample(wxWindow* parent);

private:
};

class ForEachExample : public wxDialog {
public:
    explicit ForEachExample(wxWindow* parent);
};

class ListExample : public wxDialog {
public:
    explicit ListExample(wxWindow* parent);
};

class LayoutIfExample : public wxDialog {
public:
    explicit LayoutIfExample(wxWindow* parent);
};
