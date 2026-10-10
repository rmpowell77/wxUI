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

#include <wx/artprov.h>
#include <wx/dcmemory.h>
#include <wx/graphics.h>
#include <wx/image.h>
#include <wx/wx.h>

#include <cstring>
#include <memory>
#include <stdexcept>

namespace wxUI::details {

// Render a single emoji using the system font renderer.
// Throws if no graphics context is available or nothing visible was drawn
// (e.g. no emoji font installed and the glyph came out blank).
inline auto ImageFromEmoji(const wxString& emoji, size_t size) -> wxImage
{
    if (emoji.empty()) {
        throw std::runtime_error("ImageFromEmoji: empty string");
    }

    auto bmp = wxBitmap { static_cast<int>(size), static_cast<int>(size), 32 };
    bmp.UseAlpha();

    wxMemoryDC dc(bmp);
    std::unique_ptr<wxGraphicsContext> gc;

#if defined(__WXMSW__) && wxUSE_GRAPHICS_DIRECT2D
    // Direct2D is the route to color emoji on Windows; falls back below.
    if (wxGraphicsRenderer* d2d = wxGraphicsRenderer::GetDirect2DRenderer()) {
        gc.reset(d2d->CreateContext(dc));
    }
#endif
    if (!gc) {
        gc.reset(wxGraphicsContext::Create(dc));
    }
    if (!gc) {
        throw std::runtime_error("ImageFromEmoji: no graphics context");
    }

    // Clear to fully transparent.
    gc->SetCompositionMode(wxCOMPOSITION_SOURCE);
    gc->SetBrush(wxBrush(wxColour { 0, 0, 0, 0 }));
    gc->SetPen(*wxTRANSPARENT_PEN);
    gc->DrawRectangle(0, 0, size, size);
    gc->SetCompositionMode(wxCOMPOSITION_OVER);

    gc->SetFont(wxFont(wxFontInfo(wxSize { 0, static_cast<int>(size * 0.75) })), *wxBLACK);
    auto w = 0.0;
    auto h = 0.0;
    gc->GetTextExtent(emoji, &w, &h);
    gc->DrawText(emoji, (size - w) / 2.0, (size - h) / 2.0);

    gc.reset();
    dc.SelectObject(wxNullBitmap);

    auto img = bmp.ConvertToImage();
    if (!img.IsOk() || !img.HasAlpha()) {
        throw std::runtime_error("ImageFromEmoji: conversion failed");
    }

    // Make sure something actually got drawn.
    auto const* a = img.GetAlpha();
    auto visible = false;
    for (auto i = 0UL; i < size * size && !visible; ++i) {
        visible = a[i] != 0;
    }
    if (!visible) {
        throw std::runtime_error("ImageFromEmoji: glyph rendered blank");
    }

    return img;
}

// Get a stock icon from wxArtProvider, scaled to size.
inline auto ImageFromArt(wxArtID const& id, int size, wxArtClient const& client = wxART_OTHER) -> wxImage
{
    auto bmp = wxArtProvider::GetBitmap(id, client, wxSize(size, size));
    if (!bmp.IsOk()) {
        throw std::runtime_error("ImageFromArt: no art for id");
    }

    auto img = bmp.ConvertToImage();
    if (!img.IsOk()) {
        throw std::runtime_error("ImageFromArt: conversion failed");
    }
    if (img.GetWidth() != size || img.GetHeight() != size) {
        img.Rescale(size, size, wxIMAGE_QUALITY_HIGH);
    }
    return img;
}

// Solid color square. Honors the colour's alpha.
inline auto ImageFromColour(wxColour const& colour, int size) -> wxImage
{
    if (!colour.IsOk()) {
        throw std::runtime_error("ImageFromColour: invalid colour");
    }
    auto img = wxImage { size, size, false };
    img.SetRGB(wxRect(0, 0, size, size), colour.Red(), colour.Green(), colour.Blue());

    if (colour.Alpha() != wxALPHA_OPAQUE) {
        img.InitAlpha();
        std::memset(img.GetAlpha(), colour.Alpha(), size_t(size) * size);
    }
    return img;
}

template <std::ranges::input_range R>
requires std::convertible_to<std::ranges::range_reference_t<R>, wxArtID>
auto IconList(wxSize size, R&& ids) -> std::unique_ptr<wxImageList>
{
    if (size == wxDefaultSize) {
        size = wxArtProvider::GetSizeHint(wxART_LIST);
        if (size == wxDefaultSize) {
            size = wxSize(16, 16);
        }
    }
    auto list = std::make_unique<wxImageList>(size.x, size.y);
    for (auto const& id : ids) {
        list->Add(wxArtProvider::GetIcon(id, wxART_LIST, size));
    }
    return list;
}

template <typename... Ids>
requires(std::convertible_to<Ids, wxArtID> && ...)
auto IconList(wxSize size, Ids&&... ids) -> std::unique_ptr<wxImageList>
{
    return IconList(size, std::vector<wxArtID> { wxArtID(ids)... });
}
} // namespace wxUI