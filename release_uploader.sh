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

VER="${TAG#${PLUGIN_LOWER}_v}"
NOTES=""
if [ -f "plugins/$PLUGIN/Changelist.txt" ]; then
  NOTES=$(awk -v ver="$VER" '
      BEGIN { found=0; printing=0; pattern="^"ver":?$" }
      $0 ~ pattern { found=1; printing=1; next }
      printing && /^[0-9]+\.[0-9]+\.[0-9]+:?$/ { printing=0 }
      printing { print }
  ' "plugins/$PLUGIN/Changelist.txt")
fi
[ -z "$NOTES" ] && NOTES="Release $TAG"
echo "$NOTES" > /tmp/release_notes.txt

# --- Debug symbols -> crash server -------------------------------------------
# Uploaded here (not the build job) so a failed upload fails the release. Auth is
# the SocaLabs team key + a plugin selector (monorepo). The server stores symbols
# write-once per (plugin, platform, version): a re-tagged or rebuilt version whose
# symbols already exist returns 409 and fails the release loudly — delete the
# stale symbols in the crash site, then re-run.
CRASH_BASE="https://crashreports.rabiensoftware.com"
upload_symbols () { # $1 platform, $2 zip
  if [ ! -f "$2" ]; then echo "Error: expected symbols $2 not found"; exit 1; fi
  echo "Uploading $1 symbols for $PLUGIN $VER"
  curl -sS --fail-with-body -H "X-API-Key: $SYMBOL_API_KEY" \
    -F "platform=$1" -F "plugin=$PLUGIN" -F "version=$VER" -F "files[]=@$2" \
    "$CRASH_BASE/symbols/"
  echo
}
upload_symbols mac "./${PLUGIN} macOS/${PLUGIN}_Symbols_Mac.zip"
upload_symbols win "./${PLUGIN} Windows/${PLUGIN}_Symbols_Win.zip"

ASSETS=()
[ -f "./${PLUGIN} Linux/${PLUGIN}.deb" ]   && ASSETS+=("./${PLUGIN} Linux/${PLUGIN}.deb")
[ -f "./${PLUGIN} Windows/${PLUGIN}.exe" ] && ASSETS+=("./${PLUGIN} Windows/${PLUGIN}.exe")
[ -f "./${PLUGIN} macOS/${PLUGIN}.pkg" ]   && ASSETS+=("./${PLUGIN} macOS/${PLUGIN}.pkg")

gh release create "$TAG" --title "$TAG" -F /tmp/release_notes.txt "${ASSETS[@]}"

for f in "${ASSETS[@]}"; do
  curl -sS --fail-with-body -F "files=@${f}" \
          -F "plugin=${PLUGIN_LOWER}" \
          -F "version=${VER}" \
          -F "changelog=${NOTES}" \
          "https://socalabs.com/files/upload.php?key=$APIKEY"
done
