#!/bin/bash -e
set -x

# Tag format: <plugin_lower>_v<version>  (existing convention)
TAG="$GITHUB_REF_NAME"
PLUGIN_LOWER="${TAG%_v*}"

# Map back to the plugin's CamelCase by scanning plugins/.
PLUGIN=""
for p in plugins/*; do
  [ -d "$p" ] || continue
  name=$(basename "$p")
  if [ "$(echo "$name" | tr '[:upper:]' '[:lower:]')" = "$PLUGIN_LOWER" ]; then
    PLUGIN="$name"
    break
  fi
done

if [ -z "$PLUGIN" ]; then
  echo "Could not resolve plugin name from tag $TAG"
  exit 1
fi

NOTES=""
if [ -f "plugins/$PLUGIN/Changelist.txt" ]; then
  VER="${TAG#${PLUGIN_LOWER}_v}"
  NOTES=$(awk -v ver="$VER" '
      BEGIN { found=0; printing=0; pattern="^"ver":?$" }
      $0 ~ pattern { found=1; printing=1; next }
      printing && /^[0-9]+\.[0-9]+\.[0-9]+:?$/ { printing=0 }
      printing { print }
  ' "plugins/$PLUGIN/Changelist.txt")
fi
[ -z "$NOTES" ] && NOTES="Release $TAG"
echo "$NOTES" > /tmp/release_notes.txt

ASSETS=()
[ -f "./Binaries Linux/${PLUGIN}.deb" ]   && ASSETS+=("./Binaries Linux/${PLUGIN}.deb")
[ -f "./Binaries Windows/${PLUGIN}.exe" ] && ASSETS+=("./Binaries Windows/${PLUGIN}.exe")
[ -f "./Binaries macOS/${PLUGIN}.pkg" ]   && ASSETS+=("./Binaries macOS/${PLUGIN}.pkg")

gh release create "$TAG" --title "$TAG" -F /tmp/release_notes.txt "${ASSETS[@]}"

for f in "${ASSETS[@]}"; do
  curl -F "files=@${f}" "https://socalabs.com/files/set.php?key=$APIKEY"
done
