# wxWidgets Compatibility Testing

## Overview

wxUI is designed to work with multiple versions of wxWidgets. To ensure compatibility across different wxWidgets releases, we maintain an automated compatibility testing matrix that runs regularly and on each wxUI release.

## Tested Versions

Testing is performed on:
- **Ubuntu Latest** (GCC)
- **macOS Latest** (Clang)
- **Windows Latest** (MSVC)

wxUI is actively tested against the wxWidgets listed in [wxwidgets_compatibility.yml](../.github/wxwidgets-versions.txt).

### Testing Schedule

The compatibility matrix runs:

- **On every tagged release** (tags matching `v*`)
- **Weekly** (every Monday at 00:00 UTC)
- **On demand** via manual workflow dispatch
- **On test branches** named `test-compat-*`

### Testing Schedule

The compatibility matrix runs:

- **On every tagged release** (tags matching `v*`)
- **Weekly** (every Monday at 00:00 UTC)
- **On demand** via manual workflow dispatch
- **On test branches** named `test-compat-*`

## Testing Locally (For Developers)

### Test Against a Specific wxWidgets Version

You can test wxUI against any wxWidgets version locally:

```bash
# Test against wxWidgets v3.2.5
cmake -B build-wx325 \
  -DWXUI_FETCH_WXWIDGETS=ON \
  -DWXUI_WXWIDGETS_GIT_TAG=v3.2.5 \
  -DWXUI_WITH_TESTS=ON \
  -DWXUI_WITH_EXAMPLE=ON

cmake --build build-wx325
ctest --test-dir build-wx325 --output-on-failure
```

### Test Against a Development Branch

```bash
cmake -B build-master \
  -DWXUI_FETCH_WXWIDGETS=ON \
  -DWXUI_WXWIDGETS_GIT_TAG=master \
  -DWXUI_WITH_TESTS=ON

cmake --build build-master
```

### Normal Development (vcpkg)

By default, wxUI uses vcpkg to manage dependencies:

```bash
cmake -B build \
  -DCMAKE_TOOLCHAIN_FILE=vcpkg/scripts/buildsystems/vcpkg.cmake \
  -DWXUI_WITH_TESTS=ON

cmake --build build
```

This uses whatever wxWidgets version is specified in `vcpkg.json`.

### CMake Configuration Options

| Option | Description | Default |
|--------------|-------------|---------|
| `WXUI_FETCH_WXWIDGETS` | Use FetchContent instead of find_package | `OFF` |
| `WXUI_WXWIDGETS_GIT_TAG` | wxWidgets git tag/hash to fetch | `v3.2.6` |
| `WXUI_WITH_TESTS` | Build tests | `ON` (if top-level) |
| `WXUI_WITH_EXAMPLE` | Build examples | `ON` (if top-level) |

## Maintaining the Compatibility Matrix (For Maintainers)

### When to Update

Update the tested wxWidgets versions when:

1. **A new stable wxWidgets version is released** - Add it and optionally remove the oldest to keep ~5 versions
2. **A wxWidgets version reaches end-of-life** - Remove it from the list
3. **Dropping support** - Update minimum version in docs and remove from list

### How to Update Versions

The wxWidgets versions are defined in: [`.github/wxwidgets-versions.txt`](../.github/wxwidgets-versions.txt)

**Steps:**

1. **Check for new releases:** https://github.com/wxWidgets/wxWidgets/releases

2. **Edit the versions file:**

```bash
vi .github/wxwidgets-versions.txt
```

3. **Update the list** (one version per line):

```text
v3.2.7
v3.2.6
v3.2.5
v3.2.4
v3.2.3
```

4. **Update the default in `cmake/dependencies.cmake`:**

```cmake
if(NOT DEFINED WXUI_WXWIDGETS_GIT_TAG)
  set(WXUI_WXWIDGETS_GIT_TAG "v3.2.7" CACHE STRING ...)
endif()
```

5. **Update this documentation:**

Update minimum requirements if needed:

```markdown
- **wxWidgets**: 3.2.5 or later
```

**Note:** If adding a new major/minor version (e.g., wxWidgets 3.4.0), consider creating a version-specific workflow like `.github/workflows/wxwidgets_3.4_compat.yml` for a dedicated badge. See the [Workflow Architecture](#workflow-architecture) section.

6. **Commit and test:**

```bash
git add .github/wxwidgets-versions.txt cmake/dependencies.cmake docs/wxWidgetsCompatibility.md
git commit -m "Update wxWidgets compatibility matrix to include v3.2.7"
git push origin main
```

Then manually trigger the workflow to verify.

## CI/CD Details

### Workflow Architecture

The compatibility testing uses a **reusable workflow** architecture:

- **[`wxwidgets_compat_reusable.yml`](../.github/workflows/wxwidgets_compat_reusable.yml)** - Core test logic (reusable)
- **[`wxwidgets_compatibility.yml`](../.github/workflows/wxwidgets_compatibility.yml)** - Comprehensive testing (all versions)
- **[`wxwidgets_3.2_compat.yml`](../.github/workflows/wxwidgets_3.2_compat.yml)** - 3.2.x specific testing (for badge)
- **[`wxwidgets_3.3_compat.yml`](../.github/workflows/wxwidgets_3.3_compat.yml)** - 3.3.x testing (when released)

**Benefits:**
- ✅ No code duplication - all workflows use the same test logic
- ✅ Version-specific badges (e.g., "wxWidgets 3.2.x")
- ✅ Easy to add new version families
- ✅ Can reuse in other workflows (like regular CI)

**Example: Using in your own workflow**
```yaml
jobs:
  test-custom-versions:
    uses: ./.github/workflows/wxwidgets_compat_reusable.yml
    with:
      wxwidgets_versions: '["v3.2.6", "v3.2.5"]'
      workflow_name: 'My Custom Test'
```

**Example: Adding to regular CI (ci_automation.yml)**

You can add a job to your regular CI to test against a specific wxWidgets version alongside vcpkg:

```yaml
jobs:
  build:
    # ... your existing vcpkg-based build ...

  test-specific-wxwidgets:
    uses: ./.github/workflows/wxwidgets_compat_reusable.yml
    with:
      wxwidgets_versions: '["v3.2.6"]'
      workflow_name: 'CI with wxWidgets 3.2.6'
```

This ensures PRs are tested against both the vcpkg version and a pinned version like 3.2.6.

### Manual Triggering

To run the compatibility tests manually:

1. Go to [Actions → wxWidgets Compatibility Matrix](https://github.com/rmpowell77/wxUI/actions/workflows/wxwidgets_compatibility.yml)
2. Click **"Run workflow"**
3. Select your branch
4. Optionally specify custom versions (comma-separated, e.g., `v3.2.6,v3.2.5`)
5. Click **"Run workflow"**

**Quick test with single version:**
```bash
gh workflow run wxwidgets_compatibility.yml -f wxwidgets_versions="v3.2.6"
```

### Viewing Results

Compatibility reports are available as workflow artifacts:

1. Visit the [Actions tab](https://github.com/rmpowell77/wxUI/actions/workflows/wxwidgets_compatibility.yml)
2. Click on a workflow run
3. Download the `wxwidgets-compatibility-report` artifact
4. Open `index.html` in your browser

Each report includes:
- Test results for each wxWidgets version × platform combination
- Build logs and test output
- Timestamp and wxUI version information

### Caching

The workflow caches wxWidgets builds to speed up testing:

- **Cache key:** `wxwidgets-{OS}-{version}-{build-type}-v2`
- **First run:** Builds wxWidgets from scratch (~30-45 min)
- **Subsequent runs:** Reuses cached build (~10-15 min)
- **Retention:** GitHub caches expire after 7 days of inactivity

To clear a corrupted cache: Settings → Actions → Caches → Search and delete

## How It Works

## How It Works

### Development Mode (Default)

Uses vcpkg's `find_package`:

```cmake
find_package(wxWidgets CONFIG REQUIRED COMPONENTS net core base gl aui html)
```

### CI Compatibility Testing Mode

Uses CMake's `FetchContent` to download and build specific wxWidgets versions:

```cmake
FetchContent_Declare(
  wxWidgets
  GIT_REPOSITORY https://github.com/wxWidgets/wxWidgets.git
  GIT_TAG ${WXUI_WXWIDGETS_GIT_TAG}
)
```

This allows testing against any wxWidgets git tag or commit hash without requiring vcpkg.

## Interpreting Results

### Success Criteria

A compatibility test passes if:
- ✅ wxUI builds without errors
- ✅ All unit tests pass
- ✅ Examples build successfully

### Common Failure Modes

| Issue | Possible Cause | Resolution |
|-------|---------------|------------|
| Build failure | API changes in wxWidgets | Update wxUI to handle API differences |
| Test failure | Behavior changes in wxWidgets | Update tests or fix compatibility issue |
| Link failure | ABI incompatibility | Check compiler/platform compatibility |
| Timeout | Slow build or deadlock | Check for infinite loops; adjust timeout |

### Troubleshooting

**Build fails with new wxWidgets version:**
- Check wxWidgets release notes for breaking changes
- Test locally first using the FetchContent method
- May require wxUI code updates for new APIs

**Cache issues:**
- Clear specific cache from Settings → Actions → Caches
- Increment cache version suffix in workflow (v2 → v3)

**Tests timeout:**
- Default timeout is 60 seconds
- Check for race conditions in GUI tests (especially on Linux/xvfb)

## Minimum Requirements

- **wxWidgets**: 3.2.5 or later
- **C++ Standard**: C++20
- **CMake**: 3.23 or later
- **Compilers**: GCC 11+, Clang 14+, MSVC 2019+

## Contributing

When making changes to wxUI:

1. Ensure changes work with the minimum supported wxWidgets version
2. Test locally against multiple versions when possible
3. CI will automatically test PRs against the current vcpkg wxWidgets version
4. On release, the full compatibility matrix runs automatically

## Future Plans

- Add testing against wxWidgets development branch (`master`)
- Publish compatibility report to GitHub Pages
- Add performance benchmarking across versions
- Expand platform coverage (older OS versions, ARM)
- Test additional compiler versions

## Questions?

If you encounter compatibility issues or have questions about supported versions, please [open an issue](https://github.com/rmpowell77/wxUI/issues).
