#!/bin/bash -e
set -x

#
# SocaLabs bundle installer driver.
#
#   ./Bundle/build.sh <BundleName> <bundle_id> <slugs>
#
# BundleName  display name + output filename (SocaLabs → SocaLabs.pkg/.exe/.deb)
# bundle_id   lowercase id used as upload.php's `plugin` field
# slugs       comma-separated plugins.json slugs to include (empty = all)
#
# Per platform: fetch plugins.json, download each plugin's installer for the
# current platform, extract its payload, stage all plug-in files into a single
# tree, repack as one big installer, sign + (mac) notarize, upload to
# socalabs.com via upload.php.
#

BUNDLE_NAME="$1"
BUNDLE_ID="$2"
SLUGS="$3"

cd "$(dirname "$0")"
cd ..
PROJECT_ROOT=$(pwd)
VENDOR=SocaLabs

if [ "$(uname)" = "Darwin" ]; then
  PLATFORM=macOS
elif [ "$(expr substr "$(uname -s)" 1 5)" = "Linux" ]; then
  PLATFORM=linux
else
  PLATFORM=win
fi

DOWNLOADS="$PROJECT_ROOT/Bundle/$PLATFORM/downloads"
STAGE="$PROJECT_ROOT/Bundle/$PLATFORM/stage"
rm -Rf "$DOWNLOADS" "$STAGE"
mkdir -p "$DOWNLOADS"
rm -Rf "$PROJECT_ROOT/bin"
mkdir -p "$PROJECT_ROOT/bin"

# Stable Inno Setup AppIDs per bundle so Windows treats reinstalls as upgrades.
case "$BUNDLE_ID" in
  socalabs)   APP_GUID="84308434-7E67-49DA-B500-F3EDC747DB6E" ;;
  8bittreats) APP_GUID="6DA3264A-9ACF-4D9E-93E4-32D3123C0FC4" ;;
  *)          APP_GUID=$(uuidgen) ;;
esac

BUNDLE_VERSION=$(date +%Y.%m.%d)

# ---- Fetch + filter plugins.json ----
PLUGINS_JSON=/tmp/plugins.json
curl -fsSL "https://socalabs.com/plugins.json" -o "$PLUGINS_JSON"

if [ -z "$SLUGS" ]; then
  FILTERED=$(jq '.plugins' "$PLUGINS_JSON")
else
  FILTERED=$(jq --arg s "$SLUGS" '
    [ .plugins[] | select(.slug as $slug | $s | split(",") | index($slug)) ]
  ' "$PLUGINS_JSON")
fi
echo "$FILTERED" > /tmp/bundle_plugins.json
echo "Bundle includes $(jq 'length' /tmp/bundle_plugins.json) plugins"

case "$PLATFORM" in
  macOS) FILE_KEY=mac;   EXT=pkg ;;
  win)   FILE_KEY=win;   EXT=exe ;;
  linux) FILE_KEY=linux; EXT=deb ;;
esac

# ---- Download installers ----
# tr -d '\r' guards against Git-Bash-on-Windows reading CRLF from jq output.
jq -r ".[] | \"\(.slug)|\(.downloads.$FILE_KEY)\"" /tmp/bundle_plugins.json \
  | tr -d '\r' \
  | while IFS='|' read -r SLUG URL; do
      [ -z "$SLUG" ] && continue
      echo "Downloading $SLUG: $URL"
      curl -fsSL "$URL" -o "$DOWNLOADS/$SLUG.$EXT"
    done

############################################################
# macOS — pkgutil expand → stage → pkgbuild + productbuild → sign + notarize
############################################################
if [ "$PLATFORM" = "macOS" ]; then
  TEAM_ID="${TEAM_ID:-3FS7DJDG38}"
  DEV_APP_ID="${DEV_APP_ID:-Developer ID Application: Roland Rabien (${TEAM_ID})}"
  DEV_INST_ID="${DEV_INST_ID:-Developer ID Installer: Roland Rabien (${TEAM_ID})}"

  if [ -n "${APPLICATION:-}" ] && [ -n "${INSTALLER:-}" ]; then
    KC_PASS="${KEYCHAIN_PASSWORD:-nr4aGPyz}"
    P12_PASS="${P12_PASSWORD:-aym9PKWB}"

    security create-keychain -p "$KC_PASS" Keys.keychain || true
    echo "$APPLICATION" | base64 -D -o /tmp/Application.p12
    echo "$INSTALLER"   | base64 -D -o /tmp/Installer.p12
    security import /tmp/Application.p12 -t agg -k Keys.keychain -P "$P12_PASS" -A -T /usr/bin/codesign
    security import /tmp/Installer.p12   -t agg -k Keys.keychain -P "$P12_PASS" -A -T /usr/bin/productsign
    security list-keychains -s Keys.keychain
    security default-keychain -s Keys.keychain
    security unlock-keychain -p "$KC_PASS" Keys.keychain
    security set-keychain-settings -l -u -t 13600 Keys.keychain
    security set-key-partition-list -S apple-tool:,apple: -s -k "$KC_PASS" Keys.keychain
  fi

  mkdir -p "$STAGE/vst" "$STAGE/vst3" "$STAGE/au" "$STAGE/clap" "$STAGE/resources"

  for pkg in "$DOWNLOADS"/*.pkg; do
    SLUG=$(basename "$pkg" .pkg)
    EX="/tmp/expand_$SLUG"
    rm -rf "$EX"
    pkgutil --expand "$pkg" "$EX"

    for COMPONENT in vst vst3 au clap resources; do
      PAYLOAD="$EX/$COMPONENT.pkg/Payload"
      [ -f "$PAYLOAD" ] || continue

      # Each component's Payload is a gzipped cpio archive rooted at the
      # install-location. resources.pkg is rooted at "/" so it carries
      # Library/Audio/Presets/...; the format-specific ones are rooted at the
      # plug-in folder so they carry the .vst/.vst3/.component/.clap directly.
      OUT="/tmp/payload_${SLUG}_${COMPONENT}"
      rm -rf "$OUT"
      mkdir -p "$OUT"
      ( cd "$OUT" && gzip -dc "$PAYLOAD" | cpio -i 2>/dev/null )

      if [ "$COMPONENT" = "resources" ]; then
        # Merge Library tree into the staged resources component.
        if [ -d "$OUT/Library" ]; then
          cp -RL "$OUT/Library" "$STAGE/resources/"
        fi
      else
        # Move the .vst / .vst3 / .component / .clap bundle(s) into the staged dir.
        cp -RL "$OUT"/* "$STAGE/$COMPONENT/" 2>/dev/null || true
      fi
      rm -rf "$OUT"
    done
  done
  find "$STAGE" -name ".DS_Store" -delete

  # Re-sign each plug-in bundle in the staged tree. The originals were already
  # signed but cpio extraction can drop the resource-fork sig on some bundles;
  # re-signing is cheap insurance.
  if [ -n "${APPLICATION:-}" ]; then
    find "$STAGE/vst"  -maxdepth 1 -name "*.vst"       -print0 | xargs -0 -I{} codesign -s "$DEV_APP_ID" --options=runtime --timestamp --force -v {}
    find "$STAGE/vst3" -maxdepth 1 -name "*.vst3"      -print0 | xargs -0 -I{} codesign -s "$DEV_APP_ID" --options=runtime --timestamp --force -v {}
    find "$STAGE/au"   -maxdepth 1 -name "*.component" -print0 | xargs -0 -I{} codesign -s "$DEV_APP_ID" --options=runtime --timestamp --force -v {}
    find "$STAGE/clap" -maxdepth 1 -name "*.clap"      -print0 | xargs -0 -I{} codesign -s "$DEV_APP_ID" --options=runtime --timestamp --force -v {}
  fi

  PKG_DIR="$STAGE/_pkgs"
  rm -rf "$PKG_DIR" && mkdir -p "$PKG_DIR"

  pkgbuild --root "$STAGE/vst"       --install-location "/Library/Audio/Plug-Ins/VST"        --identifier "com.socalabs.${BUNDLE_ID}.vst"       --version "$BUNDLE_VERSION" "$PKG_DIR/vst.pkg"
  pkgbuild --root "$STAGE/vst3"      --install-location "/Library/Audio/Plug-Ins/VST3"       --identifier "com.socalabs.${BUNDLE_ID}.vst3"      --version "$BUNDLE_VERSION" "$PKG_DIR/vst3.pkg"
  pkgbuild --root "$STAGE/au"        --install-location "/Library/Audio/Plug-Ins/Components" --identifier "com.socalabs.${BUNDLE_ID}.au"        --version "$BUNDLE_VERSION" "$PKG_DIR/au.pkg"
  pkgbuild --root "$STAGE/clap"      --install-location "/Library/Audio/Plug-Ins/CLAP"       --identifier "com.socalabs.${BUNDLE_ID}.clap"      --version "$BUNDLE_VERSION" "$PKG_DIR/clap.pkg"
  pkgbuild --root "$STAGE/resources" --install-location "/"                                  --identifier "com.socalabs.${BUNDLE_ID}.resources" --version "$BUNDLE_VERSION" --scripts "$PROJECT_ROOT/Bundle/macOS/scripts" "$PKG_DIR/resources.pkg"

  cat > "$PKG_DIR/distribution.xml" <<EOF
<?xml version="1.0" encoding="utf-8"?>
<installer-gui-script minSpecVersion="2">
    <title>${BUNDLE_NAME}</title>
    <organization>com.socalabs</organization>
    <domains enable_anywhere="false" enable_currentUserHome="false" enable_localSystem="true"/>
    <options customize="never" require-scripts="false" rootVolumeOnly="true"/>
    <license file="EULA.rtf"/>
    <welcome file="welcome.txt" mime-type="text/plain"/>

    <choices-outline>
        <line choice="vst"/>
        <line choice="vst3"/>
        <line choice="au"/>
        <line choice="clap"/>
        <line choice="resources"/>
    </choices-outline>

    <choice id="vst"       title="VST"             visible="false"><pkg-ref id="com.socalabs.${BUNDLE_ID}.vst"/></choice>
    <choice id="vst3"      title="VST3"            visible="false"><pkg-ref id="com.socalabs.${BUNDLE_ID}.vst3"/></choice>
    <choice id="au"        title="Audio Unit"      visible="false"><pkg-ref id="com.socalabs.${BUNDLE_ID}.au"/></choice>
    <choice id="clap"      title="CLAP"            visible="false"><pkg-ref id="com.socalabs.${BUNDLE_ID}.clap"/></choice>
    <choice id="resources" title="Factory Content" visible="false"><pkg-ref id="com.socalabs.${BUNDLE_ID}.resources"/></choice>

    <pkg-ref id="com.socalabs.${BUNDLE_ID}.vst"       version="0">vst.pkg</pkg-ref>
    <pkg-ref id="com.socalabs.${BUNDLE_ID}.vst3"      version="0">vst3.pkg</pkg-ref>
    <pkg-ref id="com.socalabs.${BUNDLE_ID}.au"        version="0">au.pkg</pkg-ref>
    <pkg-ref id="com.socalabs.${BUNDLE_ID}.clap"      version="0">clap.pkg</pkg-ref>
    <pkg-ref id="com.socalabs.${BUNDLE_ID}.resources" version="0">resources.pkg</pkg-ref>
</installer-gui-script>
EOF

  cp "$PROJECT_ROOT/Bundle/EULA.rtf"    "$PKG_DIR/EULA.rtf"
  cp "$PROJECT_ROOT/Bundle/welcome.txt" "$PKG_DIR/welcome.txt"

  PKG_OUT="$PROJECT_ROOT/bin/${BUNDLE_NAME}.pkg"
  productbuild --distribution "$PKG_DIR/distribution.xml" \
               --package-path "$PKG_DIR" \
               --resources "$PKG_DIR" \
               --version "$BUNDLE_VERSION" \
               "$PKG_OUT.unsigned"

  if [ -n "${INSTALLER:-}" ]; then
    productsign --sign "$DEV_INST_ID" "$PKG_OUT.unsigned" "$PKG_OUT"
    rm "$PKG_OUT.unsigned"
  else
    mv "$PKG_OUT.unsigned" "$PKG_OUT"
  fi

  if [ -n "${APPLE_USER:-}" ] && [ -n "${APPLE_PASS:-}" ]; then
    SUBMISSION_OUTPUT=$(xcrun notarytool submit --verbose --apple-id "$APPLE_USER" --password "$APPLE_PASS" --team-id "$TEAM_ID" --wait --timeout 60m "$PKG_OUT" 2>&1) || NOTARY_FAILED=1
    echo "$SUBMISSION_OUTPUT"
    SUBMISSION_ID=$(echo "$SUBMISSION_OUTPUT" | awk "/^  id:/ { print \$2; exit }")
    if [ "${NOTARY_FAILED:-0}" = "1" ] && [ -n "$SUBMISSION_ID" ]; then
      xcrun notarytool log "$SUBMISSION_ID" --apple-id "$APPLE_USER" --password "$APPLE_PASS" --team-id "$TEAM_ID" || true
      exit 1
    fi
    xcrun stapler staple "$PKG_OUT"
  fi

############################################################
# Linux — dpkg-deb -x → merge → dpkg-deb --build
############################################################
elif [ "$PLATFORM" = "linux" ]; then
  mkdir -p "$STAGE/DEBIAN" "$STAGE/usr"

  for deb in "$DOWNLOADS"/*.deb; do
    TMP="/tmp/deb_$(basename "$deb" .deb)"
    rm -rf "$TMP" && mkdir -p "$TMP"
    dpkg-deb -x "$deb" "$TMP"
    cp -RL "$TMP/usr/." "$STAGE/usr/"
  done

  PACKAGE_NAME=$(echo "$BUNDLE_ID" | tr '_' '-')
  SIZE_KB=$(du -sk "$STAGE/usr" | awk '{print $1}')
  cat > "$STAGE/DEBIAN/control" <<DEBEOF
Package: ${PACKAGE_NAME}
Version: ${BUNDLE_VERSION}
Section: sound
Priority: optional
Architecture: amd64
Maintainer: Roland Rabien <support@socalabs.com>
Installed-Size: ${SIZE_KB}
Homepage: https://socalabs.com/
Description: ${BUNDLE_NAME} bundle of SocaLabs audio plug-ins
DEBEOF

  fakeroot dpkg-deb --build "$STAGE" "$PROJECT_ROOT/bin/${BUNDLE_NAME}.deb"

############################################################
# Windows — innounp -x → merge → ISCC → Azure Trusted Signing
############################################################
else
  mkdir -p "$STAGE/VST" "$STAGE/VST3" "$STAGE/CLAP" "$STAGE/Resources"

  for exe in "$DOWNLOADS"/*.exe; do
    SLUG=$(basename "$exe" .exe)
    TMP="/tmp/innounp_$SLUG"
    rm -rf "$TMP" && mkdir -p "$TMP"
    # innounp -x extracts files; -d sets output dir; -y overwrites; -m extracts internal
    innounp -x -d"$TMP" -y "$exe" || true

    # Inno Setup writes paths with constants like {commoncf64} which innounp
    # leaves as literal directory names. Walk every plausible location.
    for ROOT in "$TMP/{commoncf64}" "$TMP/{cf64}" "$TMP/{commonpf64}" "$TMP/{app}" "$TMP"; do
      [ -d "$ROOT/VST"  ] && cp -R "$ROOT/VST/"*  "$STAGE/VST/"  2>/dev/null || true
      [ -d "$ROOT/VST3" ] && cp -R "$ROOT/VST3/"* "$STAGE/VST3/" 2>/dev/null || true
      [ -d "$ROOT/CLAP" ] && cp -R "$ROOT/CLAP/"* "$STAGE/CLAP/" 2>/dev/null || true
    done
    # Factory presets land under {commonappdata}\SocaLabs\<Plugin>\
    for ROOT in "$TMP/{commonappdata}" "$TMP/{commonappdata}/SocaLabs" "$TMP"; do
      if [ -d "$ROOT/SocaLabs" ]; then
        cp -R "$ROOT/SocaLabs" "$STAGE/Resources/" 2>/dev/null || true
      fi
    done
  done

  uuid_re='^[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{12}$'
  WIN_SIGN=0
  if [[ "${AZURE_TENANT_ID:-}" =~ $uuid_re && "${AZURE_CLIENT_ID:-}" =~ $uuid_re && -n "${AZURE_CLIENT_SECRET:-}" ]]; then
    WIN_SIGN=1
  fi

  if [ "$WIN_SIGN" = "1" ]; then
    SIGNTOOL=$(ls -1 "/c/Program Files (x86)/Windows Kits/10/bin/"*/x64/signtool.exe 2>/dev/null | sort | tail -1)
    [ -z "$SIGNTOOL" ] && { echo "signtool.exe not found"; exit 1; }
    TOOLS_DIR="$STAGE/_signingtools"
    mkdir -p "$TOOLS_DIR"
    nuget install Microsoft.Trusted.Signing.Client -Version 1.0.86 -OutputDirectory "$TOOLS_DIR" -ExcludeVersion -NonInteractive
    DLIB="$TOOLS_DIR/Microsoft.Trusted.Signing.Client/bin/x64/Azure.CodeSigning.Dlib.dll"
    METADATA="$PROJECT_ROOT/Installer/win/metadata.json"
    sign_file () { "$SIGNTOOL" sign -v -fd SHA256 -tr "http://timestamp.acs.microsoft.com" -td SHA256 -dlib "$DLIB" -dmdf "$METADATA" "$1"; }
  fi

  cat > "$STAGE/${BUNDLE_NAME}.iss" <<ISSEOF
; ${BUNDLE_NAME} bundle (Inno Setup)

#define MyAppName "${BUNDLE_NAME}"
#define MyAppCompany "SocaLabs"
#define MyAppPublisher "SocaLabs"
#define MyAppCopyright "2026 SocaLabs"
#define MyAppURL "https://socalabs.com/"
#define MyAppVersion "${BUNDLE_VERSION}"

[Setup]
AppID={{${APP_GUID}}
AppName={#MyAppCompany} {#MyAppName}
AppVerName={#MyAppCompany} {#MyAppName} {#MyAppVersion}
AppVersion={#MyAppVersion}
AppCopyright={#MyAppCopyright}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
DefaultDirName={commoncf64}\\VST3
OutputDir=.
OutputBaseFilename=${BUNDLE_NAME}
Compression=lzma/ultra
SolidCompression=true
LicenseFile=..\\EULA.rtf
DisableDirPage=yes
DisableProgramGroupPage=yes
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible
PrivilegesRequired=admin
Uninstallable=no
VersionInfoVersion={#MyAppVersion}
VersionInfoCompany={#MyAppPublisher}
VersionInfoCopyright={#MyAppCopyright}
VersionInfoProductName={#MyAppCompany} {#MyAppName} {#MyAppVersion}

[Languages]
Name: english; MessagesFile: compiler:Default.isl

[Files]
Source: "VST\\*";       DestDir: "{commoncf64}\\VST";                  Flags: ignoreversion overwritereadonly recursesubdirs createallsubdirs
Source: "VST3\\*";      DestDir: "{commoncf64}\\VST3";                 Flags: ignoreversion overwritereadonly recursesubdirs createallsubdirs
Source: "CLAP\\*";      DestDir: "{commoncf64}\\CLAP";                 Flags: ignoreversion overwritereadonly recursesubdirs createallsubdirs
Source: "Resources\\SocaLabs\\*"; DestDir: "{commonappdata}\\SocaLabs"; Flags: ignoreversion overwritereadonly recursesubdirs createallsubdirs skipifsourcedoesntexist
ISSEOF

  ISCC="/c/Program Files (x86)/Inno Setup 6/ISCC.exe"
  [ -f "$ISCC" ] || ISCC="/c/Program Files/Inno Setup 6/ISCC.exe"
  ( cd "$STAGE" && "$ISCC" "${BUNDLE_NAME}.iss" )

  EXE_OUT="$STAGE/${BUNDLE_NAME}.exe"
  [ "$WIN_SIGN" = "1" ] && sign_file "$EXE_OUT"
  cp "$EXE_OUT" "$PROJECT_ROOT/bin/${BUNDLE_NAME}.exe"
fi

############################################################
# Upload to socalabs.com
############################################################
if [ -n "${APIKEY:-}" ]; then
  OUT="$PROJECT_ROOT/bin/${BUNDLE_NAME}.${EXT}"
  curl -sS --fail-with-body -F "files=@${OUT}" \
          -F "plugin=${BUNDLE_ID}" \
          -F "version=${BUNDLE_VERSION}" \
          -F "changelog=${BUNDLE_NAME} bundle, ${BUNDLE_VERSION}" \
          "https://socalabs.com/files/upload.php?key=${APIKEY}"
fi
