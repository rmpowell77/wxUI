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
// wxUI uses Catch2 for testing.
// For more info, refer to https://github.com/catchorg/Catch2

// We need to initialize wxWidgets before running tests since the library
// creates real wx objects (wxSizer, wxMenu, wxSplitterWindow, etc.).
//
// A full wxApp instance is required on all platforms because:
// - Linux/GTK: Needs display connection
// - Windows: Needs window ID management system for wxMenu/wxMenuItem
// - macOS: Standard wxWidgets initialization
//
// We override OnRun() to run Catch2 instead of the wxWidgets event loop,
// so this works fine in headless CI environments.

#define CATCH_CONFIG_RUNNER
#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <wx/app.h>

// Store command line args for Catch2
static int g_argc = 0;
static char** g_argv = nullptr;

// Minimal wxApp for unit testing - no GUI event loop, just initialization
class TestApp : public wxApp {
public:
    bool OnInit() override
    {
        // Don't start event loop - we're running tests, not a GUI app
        return true;
    }

    int OnRun() override
    {
        // Run Catch2 instead of wxApp event loop
        return Catch::Session().run(g_argc, g_argv);
    }
};

wxIMPLEMENT_APP_NO_MAIN(TestApp);

int main(int argc, char* argv[])
{
    // Store command line args for Catch2
    g_argc = argc;
    g_argv = argv;

    // Initialize wxWidgets with a full wxApp instance
    // This properly initializes all subsystems (display connection, ID management, etc.)
    wxApp::SetInstance(new TestApp());
    return wxEntry(argc, argv);
}
