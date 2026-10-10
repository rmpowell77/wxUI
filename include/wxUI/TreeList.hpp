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

#include <wx/treelist.h>
#include <wxUI/GetterSetter.hpp>
#include <wxUI/Widget.hpp>

#include <set>
#include <wxUI/detail/Column.hpp>
#include <wxUI/detail/HelperMacros.hpp>
#include <wxUI/detail/Images.hpp>
#include <wxUI/detail/TreeListDetails.hpp>

namespace wxUI {

struct TreeListItems {
    wxTreeListItem parent {};
    std::vector<TreeListItems> children = {};
};

// https://docs.wxwidgets.org/latest/classwx_tree_list_ctrl.html
template <std::size_t N>
struct TreeList {
    using underlying_t = wxTreeListCtrl;

    static auto GetAllItems(wxTreeListCtrl* tree, wxTreeListItem parent) -> TreeListItems
    {
        TreeListItems result {
            .parent = parent,
        };
        auto child = tree->GetFirstChild(parent);
        while (child.IsOk()) {
            result.children.push_back(GetAllItems(tree, child));
            // Process the child here if needed
            child = tree->GetNextSibling(child);
        }
        return result;
    }

    static auto GetAllItems(wxTreeListCtrl* tree) -> std::vector<TreeListItems>
    {
        std::vector<TreeListItems> result;
        auto root = tree->GetRootItem();
        auto child = tree->GetFirstChild(root);
        while (child.IsOk()) {
            result.push_back(GetAllItems(tree, child));
            child = tree->GetNextSibling(child);
        }
        return result;
    }

    template <typename Tree>
    static auto SetAllItems(Tree* tree, wxTreeListItem parent, std::vector<wxArtID> const& normalImages, std::vector<Entry<N>> const& items) -> std::vector<TreeListItems>
    {
        std::vector<TreeListItems> result;
        std::transform(items.begin(), items.end(), std::back_inserter(result), [&normalImages, tree, parent](auto const& it) {
            auto imageClosed = it.imageClosed.has_value() ? static_cast<int>(std::distance(normalImages.begin(), std::find(normalImages.begin(), normalImages.end(), *it.imageClosed))) : -1;
            auto imageOpened = it.imageOpened.has_value() ? static_cast<int>(std::distance(normalImages.begin(), std::find(normalImages.begin(), normalImages.end(), *it.imageOpened))) : -1;

            TreeListItems item {
                .parent = tree->AppendItem(parent, it.parent.cells[0], imageClosed, imageOpened),
            };
            for (std::size_t c = 1; c < N; ++c) {
                tree->SetItemText(item.parent, static_cast<unsigned>(c), it.parent.cells[c]);
            }
            item.children = SetAllItems(tree, item.parent, normalImages, it.children);
            return item;
        });
        return result;
    }

    template <typename Tree>
    static auto SetAllItems(Tree* tree, std::vector<Entry<N>> const& items) -> std::vector<TreeListItems>
    {
        // first we go through the list of all the items
        auto setOfImages = details::FindAllArt(items);
        auto normalImages = std::vector<wxArtID> { setOfImages.begin(), setOfImages.end() };

        tree->SetImageList(details::IconList(wxSize(16, 16), normalImages).release());

        return SetAllItems(tree, tree->GetRootItem(), normalImages, items);
    }

    explicit TreeList(Columns<N> columns)
        : TreeList(wxID_ANY, std::move(columns))
    {
    }

    TreeList(Columns<N> columns, std::vector<Entry<N>> items)
        : TreeList(wxID_ANY, std::move(columns), std::move(items))
    {
    }

    TreeList(wxWindowID identity, Columns<N> columns)
        : details_(identity)
        , columns_(std::move(columns))
    {
    }

    TreeList(wxWindowID identity, Columns<N> columns, std::vector<Entry<N>> items)
        : details_(identity)
        , columns_(std::move(columns))
        , items_(std::move(items))
    {
    }

    template <typename Function>
    auto bind(Function func) & -> TreeList&
    {
        details_.bind(wxEVT_TREELIST_SELECTION_CHANGED, func);
        return *this;
    }

    template <typename Function>
    auto bind(Function func) && -> TreeList&&
    {
        details_.bind(wxEVT_TREELIST_SELECTION_CHANGED, func);
        return std::move(*this);
    }

    template <typename Function>
    auto bindActivated(Function func) & -> TreeList&
    {
        details_.bind(wxEVT_TREELIST_ITEM_ACTIVATED, func);
        return *this;
    }

    template <typename Function>
    auto bindActivated(Function func) && -> TreeList&&
    {
        details_.bind(wxEVT_TREELIST_ITEM_ACTIVATED, func);
        return std::move(*this);
    }

    struct Proxy : details::Proxy<underlying_t> {
        [[nodiscard]] auto getAllItems() const
        {
            return TreeList<N>::GetAllItems(control());
        }
        auto setAllItems(std::vector<Entry<N>> const& items)
        {
            auto* controller = control();
            controller->DeleteAllItems();
            return TreeList<N>::SetAllItems(controller, items);
        }

        [[nodiscard]] auto selections() const
        {
            auto* controller = control();
            return details::GetterSetter {
                [controller]() -> std::vector<wxTreeListItem> {
                    wxTreeListItems selected {};
                    controller->GetSelections(selected);
                    return std::vector<wxTreeListItem> { selected.begin(), selected.end() };
                },
                [controller](std::vector<wxTreeListItem> const& selections) {
                    controller->UnselectAll();
                    for (auto&& selection : selections) {
                        controller->Select(selection);
                    }
                }
            };
        }

        auto
        operator*() const
        {
            return selections();
        }
    };

private:
    details::WidgetDetails<TreeList, underlying_t> details_;
    Columns<N> columns_ {};
    std::vector<Entry<N>> items_ {};
    std::optional<int> ensureVisible_ {};

    template <typename Parent>
    auto createImpl()
    {
        return [&columns = columns_, &items = items_](Parent* parent, wxWindowID id, wxPoint pos, wxSize size, int64_t style) {
            auto* widget = customizations::ParentCreate<underlying_t>(parent, id, pos, size, style);

            for (auto const& c : columns.columns) {
                widget->AppendColumn(c.title, c.width, c.align, c.flags);
            }

            TreeList<N>::SetAllItems(widget, items);
            return widget;
        };
    }

public:
    WXUI_FORWARD_ALL_TO_DETAILS(TreeList)
};

// CTAD (Class Template Argument Deduction)
template <std::size_t N>
TreeList(Columns<N>) -> TreeList<N>;
template <std::size_t N>
TreeList(Columns<N> columns, std::vector<Entry<N>> items) -> TreeList<N>;
template <std::size_t N>
TreeList(wxWindowID, Columns<N>) -> TreeList<N>;
template <std::size_t N>
TreeList(wxWindowID, Columns<N> columns, std::vector<Entry<N>> items) -> TreeList<N>;

}

#include <wxUI/detail/ZapMacros.hpp>
