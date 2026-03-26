#!/bin/bash
# Copyright (c) 2025 Richard Powell
#
# Install git hooks for wxUI project
# This script sets up automatic documentation regeneration when source files change

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

# Remove obsolete hooks that are no longer needed
OBSOLETE_HOOKS="post-commit"
for hook_name in $OBSOLETE_HOOKS; do
    target="$HOOKS_TARGET/$hook_name"
    if [ -f "$target" ]; then
        echo "  Removing obsolete hook: $hook_name"
        rm -f "$target"
    fi
done

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
echo "  • pre-commit: Auto-regenerates documentation when source files change"
echo ""
echo "The hooks will automatically update README.md and docs/ProgrammersGuide.md"
echo "when you commit changes to source files in docs/src/ or examples/."
echo ""
echo "To manually update documentation, run:"
echo "  ./checklists/do_md"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
