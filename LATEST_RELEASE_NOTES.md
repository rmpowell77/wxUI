# wxUI release notes for v0.4.0

Bugs addressed in this release:

* [#255](../../issues/255) Missed some std::string that should be std::string_view
* [#274](../../issues/274) ASAN failure with when using ComboBox { { hello, goodbye } }

Other changes:

* [#23](../../issues/23) Add a cmake alias
* [#32](../../issues/32) Missing FlexGridStack in layouts.
* [#42](../../issues/42) Why does clang-format not like concepts?
* [#233](../../issues/233) We should figure out the best long term header layout
* [#254](../../issues/254) Generic is too generic. Split into Wrapper and Factory
* [#256](../../issues/256) Proxy should follow controller_ convention
* [#271](../../issues/271) Change example from HelloWorld to HelloWidgets
* [#276](../../issues/276) ASAN crash when we're adding items to a combobox

