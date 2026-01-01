#!/bin/bash
set -e

# Configuration
GITHUB_ORG="FigBug"
TEMP_DIR=$(mktemp -d)
OUTPUT_DIR="$TEMP_DIR/output"

# Standalone plugin repos
STANDALONE_REPOS=(
    "Organ"
    "PAPU"
    "Piano"
    "RP2A03"
    "SID"
    "SN76489"
    "Voc"
    "Wavetable"
    "mverb"
)

# slPlugins individual plugins (each has its own release)
SLPLUGINS=(
    "ABTester"
    "AddInvert"
    "ChannelMute"
    "CompensatedDelay"
    "Compressor"
    "Crossfeed"
    "Delay"
    "Expander"
    "Gate"
    "HugeGain"
    "Limiter"
    "Maths"
    "MidiLooper"
    "Oscilloscope"
    "PitchTrack"
    "SFX8"
    "SampleDelay"
    "SimpleVerb"
    "SpectrumAnalyzer"
    "StereoEnhancer"
    "StereoProcessor"
    "ToneGenerator"
    "WaveLooper"
    "XYScope"
)

# Create output directories
mkdir -p "$OUTPUT_DIR/mac"
mkdir -p "$OUTPUT_DIR/win"
mkdir -p "$OUTPUT_DIR/linux"

cleanup() {
    echo "Cleaning up temp directory..."
    rm -rf "$TEMP_DIR"
}
trap cleanup EXIT

download_release() {
    local repo=$1
    local plugin_name=$2
    local tag=$3

    echo "Downloading $plugin_name from $repo (tag: $tag)..."

    local api_url="https://api.github.com/repos/$GITHUB_ORG/$repo/releases/tags/$tag"
    local release_info=$(curl -s "$api_url")

    if echo "$release_info" | grep -q '"message": "Not Found"'; then
        echo "  Warning: Release $tag not found for $repo"
        return 1
    fi

    # Download each platform's zip
    for platform in Mac Win Linux; do
        local asset_name="${plugin_name}_${platform}.zip"
        local download_url=$(echo "$release_info" | python3 -c "
import json,sys
d=json.load(sys.stdin)
for a in d.get('assets',[]):
    if a['name'] == '$asset_name':
        print(a['browser_download_url'])
        break
" 2>/dev/null)

        if [ -n "$download_url" ]; then
            echo "  Downloading $asset_name..."
            curl -sL "$download_url" -o "$TEMP_DIR/$asset_name"

            # Unzip to appropriate directory
            local target_dir="$OUTPUT_DIR/$(echo $platform | tr '[:upper:]' '[:lower:]')"
            unzip -q -o "$TEMP_DIR/$asset_name" -d "$target_dir/"
            rm "$TEMP_DIR/$asset_name"
        else
            echo "  Warning: $asset_name not found in release"
        fi
    done
}

get_latest_tag() {
    local repo=$1
    curl -s "https://api.github.com/repos/$GITHUB_ORG/$repo/releases/latest" | python3 -c "
import json,sys
d=json.load(sys.stdin)
print(d.get('tag_name',''))
" 2>/dev/null
}

get_plugin_tag() {
    local repo=$1
    local plugin=$2
    local plugin_lower=$(echo "$plugin" | tr '[:upper:]' '[:lower:]')

    # Get list of releases and find the latest one for this plugin
    curl -s "https://api.github.com/repos/$GITHUB_ORG/$repo/releases?per_page=100" | python3 -c "
import json,sys
releases=json.load(sys.stdin)
prefix='${plugin_lower}_v'
for r in releases:
    tag = r.get('tag_name','')
    if tag.startswith(prefix):
        print(tag)
        break
" 2>/dev/null
}

echo "=== Downloading Standalone Plugins ==="
for repo in "${STANDALONE_REPOS[@]}"; do
    # Plugin name is same as repo name for standalone plugins
    # Special case for mverb -> Mverb2020
    if [ "$repo" = "mverb" ]; then
        plugin_name="Mverb2020"
    else
        plugin_name="$repo"
    fi

    tag=$(get_latest_tag "$repo")
    if [ -n "$tag" ]; then
        download_release "$repo" "$plugin_name" "$tag"
    else
        echo "Warning: No releases found for $repo"
    fi
done

echo ""
echo "=== Downloading slPlugins ==="
for plugin in "${SLPLUGINS[@]}"; do
    tag=$(get_plugin_tag "slPlugins" "$plugin")
    if [ -n "$tag" ]; then
        download_release "slPlugins" "$plugin" "$tag"
    else
        echo "Warning: No release found for $plugin"
    fi
done

echo ""
echo "=== Creating Combined Archives ==="

cd "$OUTPUT_DIR"

# Create All_Mac.zip
echo "Creating All_Mac.zip..."
cd mac
zip -r "$OUTPUT_DIR/All_Mac.zip" . -x "*.DS_Store"
cd ..

# Create All_Win.zip
echo "Creating All_Win.zip..."
cd win
zip -r "$OUTPUT_DIR/All_Win.zip" . -x "*.DS_Store"
cd ..

# Create All_Linux.zip
echo "Creating All_Linux.zip..."
cd linux
zip -r "$OUTPUT_DIR/All_Linux.zip" . -x "*.DS_Store"
cd ..

echo ""
echo "=== Uploading to SocaLabs ==="

if [ -z "$APIKEY" ]; then
    echo "Error: APIKEY environment variable not set"
    echo "Usage: APIKEY=your_api_key ./updateall.sh"
    exit 1
fi

for archive in All_Mac.zip All_Win.zip All_Linux.zip; do
    if [ -f "$OUTPUT_DIR/$archive" ]; then
        echo "Uploading $archive..."
        curl -F "files=@$OUTPUT_DIR/$archive" "https://socalabs.com/files/set.php?key=$APIKEY"
        echo ""
    fi
done

echo ""
echo "=== Done ==="
echo "Archives created:"
ls -lh "$OUTPUT_DIR"/*.zip
