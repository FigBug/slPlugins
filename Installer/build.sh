#!/bin/bash -e
set -x

#
# slPlugins installer driver. With no argument, builds every plugin in
# ci/pluginlist.txt. With a single PLUGIN argument, builds only that plugin —
# the per-plugin GitHub workflows pass this so each plugin runs as its own
# parallel job.
#
# Each plugin is built from its own plugins/<P>/CMakeLists.txt as an
# independent cmake project (`cmake -S plugins/<P> -B plugins/<P>/build`).
# This sidesteps the cross-plugin LTO interference we hit when invoking the
# top-level CMakeLists with --target X_LV2.
#

SINGLE_PLUGIN="${1:-}"

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

rm -Rf "$PROJECT_ROOT/Installer/$PLATFORM/bin"
mkdir -p "$PROJECT_ROOT/Installer/$PLATFORM/bin"
rm -Rf "$PROJECT_ROOT/bin"
mkdir -p "$PROJECT_ROOT/bin"

if [ -n "$SINGLE_PLUGIN" ]; then
  PLUGINS="$SINGLE_PLUGIN"
else
  PLUGINS=$(cat "$PROJECT_ROOT/ci/pluginlist.txt" | tr -d '\r' | grep -v '^$')
fi

# Configure + build a single plugin from its own CMakeLists.txt. Echoes the
# build directory so callers can locate the artefacts.
build_one_plugin() {
  local P=$1
  local PDIR="$PROJECT_ROOT/plugins/$P"

  if [ "$PLATFORM" = "macOS" ]; then
    local BDIR="$PDIR/build-xcode"
    cmake -S "$PDIR" -B "$BDIR" -GXcode \
          -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64" \
          -DCMAKE_OSX_DEPLOYMENT_TARGET=10.13 1>&2
    cmake --build "$BDIR" --config Release 1>&2
    echo "$BDIR"
  elif [ "$PLATFORM" = "linux" ]; then
    local BDIR="$PDIR/build-ninja"
    cmake -S "$PDIR" -B "$BDIR" -GNinja \
          -DCMAKE_BUILD_TYPE=Release \
          -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ 1>&2
    cmake --build "$BDIR" --config Release 1>&2
    echo "$BDIR"
  else
    local BDIR="$PDIR/build-vs"
    cmake -S "$PDIR" -B "$BDIR" -G"Visual Studio 17 2022" -A x64 1>&2
    cmake --build "$BDIR" --config Release 1>&2
    echo "$BDIR"
  fi
}

############################################################
# macOS — pkgbuild + productbuild per plugin
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

  STAGE_BASE="$PROJECT_ROOT/Installer/macOS/bin"

  for PLUGIN in $PLUGINS; do
    BDIR=$(build_one_plugin "$PLUGIN")
    PLOWER=$(echo "$PLUGIN" | tr '[:upper:]' '[:lower:]')
    BUNDLE_BASE="com.socalabs.${PLOWER}"
    VERSION=$(cat "$PROJECT_ROOT/plugins/$PLUGIN/VERSION" | tr -d '[:space:]')
    ART_DIR="$BDIR/${PLUGIN}_artefacts/Release"

    STAGE="$STAGE_BASE/$PLUGIN/stage"
    PKG_DIR="$STAGE_BASE/$PLUGIN/pkgs"
    rm -Rf "$STAGE" "$PKG_DIR"
    mkdir -p "$STAGE/vst" "$STAGE/vst3" "$STAGE/au" "$STAGE/clap" "$PKG_DIR"

    cp -RL "$ART_DIR/VST/$PLUGIN.vst"      "$STAGE/vst/"
    cp -RL "$ART_DIR/VST3/$PLUGIN.vst3"    "$STAGE/vst3/"
    cp -RL "$ART_DIR/AU/$PLUGIN.component" "$STAGE/au/"
    cp -RL "$ART_DIR/CLAP/$PLUGIN.clap"    "$STAGE/clap/"

    HAS_PRESETS=no
    if [ -d "$PROJECT_ROOT/plugins/$PLUGIN/Resources" ] && \
       compgen -G "$PROJECT_ROOT/plugins/$PLUGIN/Resources/*.xml" > /dev/null; then
      HAS_PRESETS=yes
      mkdir -p "$STAGE/resources/Library/Audio/Presets/$VENDOR/$PLUGIN/Presets"
      cp "$PROJECT_ROOT/plugins/$PLUGIN/Resources/"*.xml \
         "$STAGE/resources/Library/Audio/Presets/$VENDOR/$PLUGIN/Presets/"
    fi
    find "$STAGE" -name ".DS_Store" -delete

    if [ -n "${APPLICATION:-}" ]; then
      codesign -s "$DEV_APP_ID" --options=runtime --timestamp --force -v "$STAGE/vst/$PLUGIN.vst"
      codesign -s "$DEV_APP_ID" --options=runtime --timestamp --force -v "$STAGE/vst3/$PLUGIN.vst3"
      codesign -s "$DEV_APP_ID" --options=runtime --timestamp --force -v "$STAGE/au/$PLUGIN.component"
      codesign -s "$DEV_APP_ID" --options=runtime --timestamp --force -v "$STAGE/clap/$PLUGIN.clap"
    fi

    pkgbuild --root "$STAGE/vst"  --install-location "/Library/Audio/Plug-Ins/VST"        --identifier "${BUNDLE_BASE}.vst.pkg"  --version "$VERSION" "$PKG_DIR/vst.pkg"
    pkgbuild --root "$STAGE/vst3" --install-location "/Library/Audio/Plug-Ins/VST3"       --identifier "${BUNDLE_BASE}.vst3.pkg" --version "$VERSION" "$PKG_DIR/vst3.pkg"
    pkgbuild --root "$STAGE/au"   --install-location "/Library/Audio/Plug-Ins/Components" --identifier "${BUNDLE_BASE}.au.pkg"   --version "$VERSION" "$PKG_DIR/au.pkg"
    pkgbuild --root "$STAGE/clap" --install-location "/Library/Audio/Plug-Ins/CLAP"       --identifier "${BUNDLE_BASE}.clap.pkg" --version "$VERSION" "$PKG_DIR/clap.pkg"

    SCRIPTS="$STAGE/scripts"
    mkdir -p "$SCRIPTS"
    cp "$PROJECT_ROOT/Installer/macOS/scripts/preinstall_${PLUGIN}" "$SCRIPTS/preinstall"
    cp "$PROJECT_ROOT/Installer/macOS/scripts/postinstall"          "$SCRIPTS/postinstall"
    chmod +x "$SCRIPTS/preinstall" "$SCRIPTS/postinstall"

    if [ "$HAS_PRESETS" = "yes" ]; then
      pkgbuild --root "$STAGE/resources" --install-location "/" --identifier "${BUNDLE_BASE}.resources.pkg" --version "$VERSION" --scripts "$SCRIPTS" "$PKG_DIR/resources.pkg"
    else
      pkgbuild --root "$STAGE/clap" --install-location "/Library/Audio/Plug-Ins/CLAP" --identifier "${BUNDLE_BASE}.clap.pkg" --version "$VERSION" --scripts "$SCRIPTS" "$PKG_DIR/clap.pkg"
    fi

    cp "$PROJECT_ROOT/Installer/EULA.rtf"          "$PKG_DIR/EULA.rtf"
    cp "$PROJECT_ROOT/Installer/macOS/welcome.txt" "$PKG_DIR/welcome.txt"

    PKG_OUT="$STAGE_BASE/${PLUGIN}.pkg"
    productbuild --distribution "$PROJECT_ROOT/Installer/macOS/distribution_${PLUGIN}.xml" \
                 --package-path "$PKG_DIR" \
                 --resources "$PKG_DIR" \
                 --version "$VERSION" \
                 "$PKG_OUT.unsigned"

    if [ -n "${INSTALLER:-}" ]; then
      productsign --sign "$DEV_INST_ID" "$PKG_OUT.unsigned" "$PKG_OUT"
      rm "$PKG_OUT.unsigned"
    else
      mv "$PKG_OUT.unsigned" "$PKG_OUT"
    fi

    if [ -n "${APPLE_USER:-}" ] && [ -n "${APPLE_PASS:-}" ]; then
      SUBMISSION_OUTPUT=$(xcrun notarytool submit --verbose --apple-id "$APPLE_USER" --password "$APPLE_PASS" --team-id "$TEAM_ID" --wait --timeout 30m "$PKG_OUT" 2>&1) || NOTARY_FAILED=1
      echo "$SUBMISSION_OUTPUT"
      SUBMISSION_ID=$(echo "$SUBMISSION_OUTPUT" | awk "/^  id:/ { print \$2; exit }")
      if [ "${NOTARY_FAILED:-0}" = "1" ] && [ -n "$SUBMISSION_ID" ]; then
        echo "Notarization failed for $PLUGIN — fetching log $SUBMISSION_ID"
        xcrun notarytool log "$SUBMISSION_ID" --apple-id "$APPLE_USER" --password "$APPLE_PASS" --team-id "$TEAM_ID" || true
        exit 1
      fi
      xcrun stapler staple "$PKG_OUT"
    fi

    cp "$PKG_OUT" "$PROJECT_ROOT/bin/"
  done

############################################################
# Linux — per-plugin .deb via dpkg-deb (no cmake --install / CPack)
############################################################
elif [ "$PLATFORM" = "linux" ]; then
  for PLUGIN in $PLUGINS; do
    BDIR=$(build_one_plugin "$PLUGIN")
    PLOWER=$(echo "$PLUGIN" | tr '[:upper:]' '[:lower:]')
    VERSION=$(cat "$PROJECT_ROOT/plugins/$PLUGIN/VERSION" | tr -d '[:space:]')
    DEB_ROOT="$PROJECT_ROOT/Installer/linux/bin/$PLUGIN"
    DEB_USR="$DEB_ROOT/usr"
    rm -Rf "$DEB_ROOT"
    mkdir -p "$DEB_ROOT/DEBIAN" "$DEB_USR/lib/vst" "$DEB_USR/lib/vst3" "$DEB_USR/lib/lv2" "$DEB_USR/lib/clap"

    ART="$BDIR/${PLUGIN}_artefacts/Release"
    cp    "$ART/VST/lib${PLUGIN}.so"    "$DEB_USR/lib/vst/${PLUGIN}.so"
    cp -R "$ART/VST3/${PLUGIN}.vst3"    "$DEB_USR/lib/vst3/"
    cp -R "$ART/LV2/${PLUGIN}.lv2"      "$DEB_USR/lib/lv2/"
    cp    "$ART/CLAP/${PLUGIN}.clap"    "$DEB_USR/lib/clap/${PLUGIN}.clap"

    if [ -d "$PROJECT_ROOT/plugins/${PLUGIN}/Resources" ] && \
       compgen -G "$PROJECT_ROOT/plugins/${PLUGIN}/Resources/*.xml" > /dev/null; then
      mkdir -p "$DEB_USR/share/SocaLabs/${PLUGIN}/Presets"
      cp "$PROJECT_ROOT/plugins/${PLUGIN}/Resources/"*.xml \
         "$DEB_USR/share/SocaLabs/${PLUGIN}/Presets/"
    fi

    SIZE_KB=$(du -sk "$DEB_USR" | awk '{print $1}')
    cat > "$DEB_ROOT/DEBIAN/control" <<DEBEOF
Package: ${PLOWER}
Version: ${VERSION}
Section: sound
Priority: optional
Architecture: amd64
Maintainer: Roland Rabien <support@socalabs.com>
Installed-Size: ${SIZE_KB}
Homepage: https://socalabs.com/
Description: SocaLabs ${PLUGIN} audio plug-in
DEBEOF

    fakeroot dpkg-deb --build "$DEB_ROOT" "$PROJECT_ROOT/bin/${PLUGIN}.deb"
  done

############################################################
# Windows — Inno Setup + Azure Trusted Signing per plugin
############################################################
else
  uuid_re='^[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{12}$'
  WIN_SIGN=0
  if [ -n "${AZURE_TENANT_ID:-}" ] || [ -n "${AZURE_CLIENT_ID:-}" ] || [ -n "${AZURE_CLIENT_SECRET:-}" ]; then
    if   [[ ! "${AZURE_TENANT_ID:-}" =~ $uuid_re ]]; then echo "ERROR: AZURE_TENANT_ID is not a valid GUID"; exit 1
    elif [[ ! "${AZURE_CLIENT_ID:-}" =~ $uuid_re ]]; then echo "ERROR: AZURE_CLIENT_ID is not a valid GUID"; exit 1
    elif [ -z "${AZURE_CLIENT_SECRET:-}" ]; then          echo "ERROR: AZURE_CLIENT_SECRET is empty"; exit 1
    else WIN_SIGN=1
    fi
  fi

  if [ "$WIN_SIGN" = "1" ]; then
    SIGNTOOL=$(ls -1 "/c/Program Files (x86)/Windows Kits/10/bin/"*/x64/signtool.exe 2>/dev/null | sort | tail -1)
    [ -z "$SIGNTOOL" ] && { echo "signtool.exe not found"; exit 1; }

    TOOLS_DIR="$PROJECT_ROOT/Installer/win/_signingtools"
    mkdir -p "$TOOLS_DIR"
    nuget install Microsoft.Trusted.Signing.Client -Version 1.0.86 -OutputDirectory "$TOOLS_DIR" -ExcludeVersion -NonInteractive
    DLIB="$TOOLS_DIR/Microsoft.Trusted.Signing.Client/bin/x64/Azure.CodeSigning.Dlib.dll"
    METADATA="$PROJECT_ROOT/Installer/win/metadata.json"

    sign_file () { "$SIGNTOOL" sign -v -fd SHA256 -tr "http://timestamp.acs.microsoft.com" -td SHA256 -dlib "$DLIB" -dmdf "$METADATA" "$1"; }
  fi

  ISCC="/c/Program Files (x86)/Inno Setup 6/ISCC.exe"
  [ -f "$ISCC" ] || ISCC="/c/Program Files/Inno Setup 6/ISCC.exe"

  WIN_BIN="$PROJECT_ROOT/Installer/win/bin"
  for PLUGIN in $PLUGINS; do
    BDIR=$(build_one_plugin "$PLUGIN")
    ART_DIR="$BDIR/${PLUGIN}_artefacts/Release"

    rm -Rf "$WIN_BIN/VST" "$WIN_BIN/VST3" "$WIN_BIN/CLAP"
    mkdir -p "$WIN_BIN/VST" "$WIN_BIN/VST3" "$WIN_BIN/CLAP"
    cp -R "$ART_DIR/VST/$PLUGIN.dll"   "$WIN_BIN/VST/"
    cp -R "$ART_DIR/VST3/$PLUGIN.vst3" "$WIN_BIN/VST3/"
    cp -R "$ART_DIR/CLAP/$PLUGIN.clap" "$WIN_BIN/CLAP/"

    if [ "$WIN_SIGN" = "1" ]; then
      sign_file "$WIN_BIN/VST/$PLUGIN.dll"
      sign_file "$WIN_BIN/VST3/$PLUGIN.vst3/Contents/x86_64-win/$PLUGIN.vst3"
      sign_file "$WIN_BIN/CLAP/$PLUGIN.clap"
    fi

    (cd "$PROJECT_ROOT/Installer/win" && "$ISCC" "${PLUGIN}.iss")

    EXE_OUT="$WIN_BIN/${PLUGIN}.exe"
    [ "$WIN_SIGN" = "1" ] && sign_file "$EXE_OUT"
    cp "$EXE_OUT" "$PROJECT_ROOT/bin/"
  done
fi
