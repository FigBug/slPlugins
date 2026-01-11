#!/bin/bash -e

cd "$(dirname "$0")"
ROOT=$(pwd)

if [ -z "$1" ]; then
    echo "Usage: ./tag.sh <PluginName>"
    echo "Example: ./tag.sh ABTester"
    exit 1
fi

PLUGIN="$1"
PLUGIN_DIR="$ROOT/plugins/$PLUGIN"

if [ ! -d "$PLUGIN_DIR" ]; then
    echo "Error: Plugin directory not found: $PLUGIN_DIR"
    exit 1
fi

if [ ! -f "$PLUGIN_DIR/VERSION" ]; then
    echo "Error: VERSION file not found: $PLUGIN_DIR/VERSION"
    exit 1
fi

VER=$(cat "$PLUGIN_DIR/VERSION")

# Check that the version exists in the Changelist.txt
if [ -f "$PLUGIN_DIR/Changelist.txt" ]; then
    if ! grep -q "^${VER}:*$" "$PLUGIN_DIR/Changelist.txt"; then
        echo "Error: Version $VER not found in $PLUGIN_DIR/Changelist.txt"
        exit 1
    fi
fi

# Convert plugin name to lowercase for tag
TAG_PREFIX=$(echo "$PLUGIN" | tr '[:upper:]' '[:lower:]')
TAG="${TAG_PREFIX}_v${VER}"

echo "Tagging $PLUGIN [$TAG]"
git tag "$TAG" && git push origin "$TAG"
