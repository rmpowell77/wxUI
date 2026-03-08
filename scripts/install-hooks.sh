#!/bin/bash
# Copyright (c) 2025 Richard Powell
#
# Install git hooks for wxUI project
# This script sets up automatic version.hpp updates when tags are created

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
HOOKS_SOURCE="$PROJECT_ROOT/scripts/git-hooks"
HOOKS_TARGET="$PROJECT_ROOT/.git/hooks"

echo "Installing wxUI git hooks..."

# Check if we're in a git repository
if [ ! -d "$PROJECT_ROOT/.git" ]; then
    echo "Error: Not in a git repository"
    exit 1
fi

# Create hooks directory if it doesn't exist
mkdir -p "$HOOKS_TARGET"

# Install each hook
for hook in "$HOOKS_SOURCE"/*; do
    if [ -f "$hook" ]; then
        hook_name=$(basename "$hook")
        target="$HOOKS_TARGET/$hook_name"
        
        # Backup existing hook if present
        if [ -f "$target" ]; then
            echo "  Backing up existing $hook_name to ${hook_name}.backup"
            mv "$target" "${target}.backup"
        fi
        
        # Copy hook and make executable
        cp "$hook" "$target"
        chmod +x "$target"
        echo "  ✓ Installed $hook_name"
    fi
done

# Make update-version.sh executable
chmod +x "$PROJECT_ROOT/scripts/update-version.sh"

echo ""
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "✓ Git hooks installed successfully!"
echo ""
echo "Installed hooks:"
echo "  • pre-commit: Detects version.hpp changes and schedules tag creation"
echo "  • post-commit: Creates the version tag after commit completes"
echo ""
echo "Release workflow:"
echo "  1. Run: ./scripts/update-version.sh v1.2.3"
echo "  2. Stage: git add include/wxUI/version.hpp"
echo "  3. Commit: git commit -m 'Bump version to 1.2.3'"
echo "  4. Hooks automatically create tag v1.2.3"
echo "  5. Push: git push origin main --tags"
echo ""
echo "You can also manually update version.hpp anytime by running:"
echo "  ./scripts/update-version.sh"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
