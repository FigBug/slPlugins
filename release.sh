#!/bin/bash

set -e

if [ -z "$1" ]; then
    echo "Usage: ./release.sh <PluginName>"
    echo "Example: ./release.sh SimpleVerb"
    exit 1
fi

PLUGIN_NAME="$1"
PLUGIN_DIR="plugins/$PLUGIN_NAME"
VERSION_FILE="$PLUGIN_DIR/VERSION"

if [ ! -d "$PLUGIN_DIR" ]; then
    echo "Error: Plugin directory '$PLUGIN_DIR' not found"
    exit 1
fi

if [ ! -f "$VERSION_FILE" ]; then
    echo "Error: VERSION file not found at '$VERSION_FILE'"
    exit 1
fi

VERSION=$(head -n 1 "$VERSION_FILE" | tr -d '[:space:]')

if [ -z "$VERSION" ]; then
    echo "Error: VERSION file is empty"
    exit 1
fi

PLUGIN_LOWER=$(echo "$PLUGIN_NAME" | tr '[:upper:]' '[:lower:]')
TAG_NAME="${PLUGIN_LOWER}_v${VERSION}"

echo "Plugin: $PLUGIN_NAME"
echo "Version: $VERSION"
echo "Tag: $TAG_NAME"
echo ""

if git rev-parse "$TAG_NAME" >/dev/null 2>&1; then
    echo "Error: Tag '$TAG_NAME' already exists"
    exit 1
fi

read -p "Create and push tag '$TAG_NAME'? [y/N] " -n 1 -r
echo ""

if [[ $REPLY =~ ^[Yy]$ ]]; then
    git tag "$TAG_NAME"
    git push origin "$TAG_NAME"
    echo "Tag '$TAG_NAME' created and pushed"
else
    echo "Aborted"
    exit 0
fi
