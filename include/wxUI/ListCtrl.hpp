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

#include <wx/listctrl.h>
#include <wxUI/GetterSetter.hpp>
#include <wxUI/Widget.hpp>

#include <wxUI/detail/HelperMacros.hpp>

namespace wxUI::details {

inline auto toWxListItem(auto&& text) -> wxListItem 
{
    auto item = wxListItem {};
    item.SetText(std::forward<decltype(text)>(text));
    return item;
}

}

namespace wxUI::details::Ranges {

template <details::utf8_text_choice T>
inline auto convertToListItems(std::initializer_list<T> choices) -> std::vector<wxListItem>
{
    std::vector<wxListItem> result;
    result.reserve(choices.size());
    for (auto const& choice : choices) {
        result.push_back(details::toWxListItem(choice));
    }
    return result;
}

inline auto flattenToListItems(std::initializer_list<std::initializer_list<char const*>> choices) -> std::vector<wxListItem>
{
    auto count = static_cast<size_t>(0);
    for (auto const& group : choices) {
        count += group.size();
    }
    auto result = std::vector<wxListItem> {};
    result.reserve(count);
    for (auto const& group : choices) {
        for (auto const* choice : group) {
            result.push_back(details::toWxListItem(choice));
        }
    }
    return result;
}

// Convert variadic arguments to vector of wxString
template <details::utf8_text_choice... Strings>
inline auto toVectorListItems(Strings&&... strings) -> std::vector<wxListItem>
{
    auto result = std::vector<wxListItem> {};
    result.reserve(sizeof...(strings));
    (result.push_back(details::toWxListItem(std::forward<Strings>(strings))), ...);
    return result;
}

template <utf8_text_input_range Range>
inline auto ToVectorListItems(Range&& range) -> std::vector<wxListItem>
{
    auto result = std::vector<wxListItem> {};
    if constexpr (std::ranges::sized_range<Range>) {
        result.reserve(std::ranges::size(range));
    }
    std::ranges::transform(std::forward<Range>(range), std::back_inserter(result), [](auto const& choice) {
        return details::toWxListItem(choice);
    });
    return result;
}

}

namespace wxUI {

// https://docs.wxwidgets.org/latest/classwx_list_ctrl.html
struct ListCtrl {
    using underlying_t = wxListCtrl;

    ListCtrl() = default;

    template <typename String>
    requires details::utf8_text_choice<String>
    explicit ListCtrl(std::initializer_list<String> choices)
        : ListCtrl(wxID_ANY, choices)
    {
    }

    explicit ListCtrl(std::initializer_list<std::initializer_list<char const*>> choices)
        : ListCtrl(wxID_ANY, choices)
    {
    }

    explicit ListCtrl(details::Ranges::utf8_text_input_range auto&& choices)
        : ListCtrl(wxID_ANY, std::forward<decltype(choices)>(choices))
    {
    }

    template <typename... Strings>
    requires(sizeof...(Strings) > 0) && (details::utf8_text_choice<Strings> && ...)
    ListCtrl(Strings&&... choices)
        : ListCtrl(wxID_ANY, std::forward<Strings>(choices)...)
    {
    }

    explicit ListCtrl(wxWindowID identity)
        : ListCtrl(identity, std::initializer_list<char const*> {})
    {
    }

    template <typename String>
    requires details::utf8_text_choice<String>
    ListCtrl(wxWindowID identity, std::initializer_list<String> choices)
        : details_(identity)
        , items_(details::Ranges::convertToListItems(choices))
    {
    }

    ListCtrl(wxWindowID identity, std::initializer_list<std::initializer_list<char const*>> choices)
        : details_(identity)
        , items_(details::Ranges::flattenToListItems(choices))
    {
    }

    ListCtrl(wxWindowID identity, details::Ranges::utf8_text_input_range auto&& choices)
        : details_(identity)
        , items_(details::Ranges::ToVectorListItems(std::forward<decltype(choices)>(choices)))
    {
    }

    template <typename... Strings>
    requires(sizeof...(Strings) > 0) && (details::utf8_text_choice<Strings> && ...)
    ListCtrl(wxWindowID identity, Strings&&... choices)
        : details_(identity)
        , items_(details::Ranges::toVectorListItems(std::forward<Strings>(choices)...))
    {
    }

    auto withItems(std::vector<wxListItem> items) & -> ListCtrl&
    {
        items_.insert(items_.end(), std::make_move_iterator(items.begin()), std::make_move_iterator(items.end()));
        return *this;
    }

    auto withItems(std::vector<wxListItem> items) && -> ListCtrl&&
    {
        items_.insert(items_.end(), std::make_move_iterator(items.begin()), std::make_move_iterator(items.end()));
        return std::move(*this);
    }

    auto withAddedColumn(std::string_view title, wxListColumnFormat format = wxLIST_FORMAT_LEFT, int width = -1) & -> ListCtrl&
    {
        wxListItem column;

        column.SetColumn(static_cast<int>(columns_.size()));
        column.SetText(details::toWxString(title));
        column.SetAlign(format);
        column.SetWidth(width);

        columns_.push_back(std::move(column));
        return *this;
    }

    auto withAddedColumn(std::string_view title, wxListColumnFormat format = wxLIST_FORMAT_LEFT, int width = -1) && -> ListCtrl&&
    {
        wxListItem column;

        column.SetColumn(static_cast<int>(columns_.size()));
        column.SetText(details::toWxString(title));
        column.SetAlign(format);
        column.SetWidth(width);

        columns_.push_back(std::move(column));
        return std::move(*this);
    }

    auto withColumns(std::vector<wxListItem> columns) & -> ListCtrl&
    {
        columns_.insert(columns_.end(), std::make_move_iterator(columns.begin()), std::make_move_iterator(columns.end()));
        return *this;
    }

    auto withColumns(std::vector<wxListItem> columns) && -> ListCtrl&&
    {
        columns_.insert(columns_.end(), std::make_move_iterator(columns.begin()), std::make_move_iterator(columns.end()));
        return std::move(*this);
    }

    auto withNormalImages(wxVector<wxBitmapBundle> images) & -> ListCtrl&
    {
        normalImages_ = std::move(images);
        return *this;
    }

    auto withNormalImages(wxVector<wxBitmapBundle> images) && -> ListCtrl&&
    {
        normalImages_ = std::move(images);
        return std::move(*this);
    }

    auto withSmallImages(wxVector<wxBitmapBundle> images) & -> ListCtrl&
    {
        smallImages_ = std::move(images);
        return *this;
    }

    auto withSmallImages(wxVector<wxBitmapBundle> images) && -> ListCtrl&&
    {
        smallImages_ = std::move(images);
        return std::move(*this);
    }

    auto withEnsureVisible(int which) & -> ListCtrl&
    {
        ensureVisible_ = which;
        return *this;
    }

    auto withEnsureVisible(int which) && -> ListCtrl&&
    {
        ensureVisible_ = which;
        return std::move(*this);
    }

    template <typename Function>
    auto bind(Function func) & -> ListCtrl&
    {
        details_.bind(wxEVT_LIST_ITEM_SELECTED, func);
        return *this;
    }

    template <typename Function>
    auto bind(Function func) && -> ListCtrl&&
    {
        details_.bind(wxEVT_LIST_ITEM_SELECTED, func);
        return std::move(*this);
    }

    template <typename Function>
    auto bindDClick(Function func) & -> ListCtrl&
    {
        details_.bind(wxEVT_LIST_ITEM_ACTIVATED, func);
        return *this;
    }

    template <typename Function>
    auto bindDClick(Function func) && -> ListCtrl&&
    {
        details_.bind(wxEVT_LIST_ITEM_ACTIVATED, func);
        return std::move(*this);
    }

    struct Proxy : details::Proxy<underlying_t> { 
    private:
        [[nodiscard]] static auto selected(underlying_t* controller)
        {
            std::vector<int> selectedItems;
            long item = -1;
            while (true) {
                item = controller->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

                if (item == -1) {
                    break;
                }

                selectedItems.push_back(item);
            }
            return selectedItems;
        }

        static void select(underlying_t* controller, long index, bool selected) 
        {
            wxListItem item;

            item.SetId(index);
            item.SetMask(wxLIST_MASK_STATE);
            item.SetStateMask(wxLIST_STATE_SELECTED);
            item.SetState(selected ? wxLIST_STATE_SELECTED : 0);
            
            controller->SetItem(item);
        }

    public:
        [[nodiscard]] auto selection() const
        {
            auto* controller = control();
            return details::GetterSetter {
                [controller] { return controller->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED); },
                [controller](int selection) { 
                    auto existingSelections = selected(controller);
                    
                    for (auto&& existing : existingSelections) {
                        select(controller, existing, false);
                    }

                    if (selection >= 0) {
                        select(controller, selection, true);
                    }
                }
            };
        }

        [[nodiscard]] auto selections() const
        {
            auto* controller = control();
            return details::GetterSetter {
                [controller]() -> std::vector<int> {
                    return selected(controller);
                },
                [controller](std::vector<int> const& selections) {
                    auto existingSelections = selected(controller);
                    
                    for (auto&& existing : existingSelections) {
                        select(controller, existing, false);
                    }
                    
                    for (auto&& selection : selections) {
                        select(controller, selection, true);
                    }
                }
            };
        }

        auto
        operator*() const
        {
            return selection();
        }
    };

private:
    details::WidgetDetails<ListCtrl, underlying_t> details_;
    std::vector<wxListItem> columns_ {};
    std::vector<wxListItem> items_ {};
    std::optional<int> ensureVisible_ {};
    std::optional<wxVector<wxBitmapBundle>> normalImages_ {};
    std::optional<wxVector<wxBitmapBundle>> smallImages_ {};

    template <typename Parent>
    auto createImpl()
    {
        return [&columns = columns_, &items = items_, &normalImages = normalImages_, &smallImages = smallImages_, &ensureVisible = ensureVisible_](Parent* parent, wxWindowID id, wxPoint pos, wxSize size, int64_t style) {
            auto* widget = customizations::ParentCreate<underlying_t>(parent, id, pos, size, style);

            if (normalImages) {
                widget->SetNormalImages(*normalImages);
            }

            if (smallImages) {
                widget->SetSmallImages(*smallImages);
            }

            for (long index {}; auto&& column : columns) {
                widget->InsertColumn(index++, column);
            }

            for (auto&& item : items) {
                if (item.GetColumn() == 0) {
                    widget->InsertItem(item);
                } else {
                    widget->SetItem(item);
                }
            }

            if (ensureVisible) {
                widget->EnsureVisible(*ensureVisible);
            }

            return widget;
        };
    }

public:
    WXUI_FORWARD_ALL_TO_DETAILS(ListCtrl)
};

WXUI_WIDGET_STATIC_ASSERT_BOILERPLATE(ListCtrl);
}

#include <wxUI/detail/ZapMacros.hpp>
