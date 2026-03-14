# wxUI release notes for v0.4.0

Bugs addressed in this release:

* [#255](../../issues/255) Missed some std::string that should be std::string_view
* [#274](../../issues/274) ASAN failure with when using ComboBox { { hello, goodbye } }
* [#279](../../issues/279) We have made it more difficult to use include dir
* [#297](../../issues/297) Make sure that mutable lambdas are usable in places that take lambdas

Other changes:

* [#23](../../issues/23) Add a cmake alias
* [#32](../../issues/32) Missing FlexGridStack in layouts.
* [#42](../../issues/42) Why does clang-format not like concepts?
* [#81](../../issues/81) need wxNotebook
* [#233](../../issues/233) We should figure out the best long term header layout
* [#254](../../issues/254) Generic is too generic. Split into Wrapper and Factory
* [#256](../../issues/256) Proxy should follow controller_ convention
* [#263](../../issues/263) Will Factory with move-only lambda work?
* [#271](../../issues/271) Change example from HelloWorld to HelloWidgets
* [#276](../../issues/276) ASAN crash when we're adding items to a combobox
* [#285](../../issues/285) We should have some unit tests for Notebook
* [#289](../../issues/289) We should allow Splitter to take in any types of sizers and layouts
* [#292](../../issues/292) FlexGridSizer does not support growable columns and will never increase to fill space
* [#295](../../issues/295) make sure all files have a BuildTests
* [#302](../../issues/302) Need better defaults for WXUI_WITH_TESTS, WXUI_WITH_EXAMPLE

