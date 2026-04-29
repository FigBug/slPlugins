#!/bin/bash -e
set -x

#
# slPlugins installer driver. Builds all 24 plugins once via cmake, then loops
# through ci/pluginlist.txt to produce 24 separate .pkg / .exe / .deb installers.
#

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

PLUGINS=$(cat "$PROJECT_ROOT/ci/pluginlist.txt" | tr -d '\r' | grep -v '^$')

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

  cd "$PROJECT_ROOT"
  cmake --preset xcode
  cmake --build --preset xcode --config Release

  STAGE_BASE="$PROJECT_ROOT/Installer/macOS/bin"

  for PLUGIN in $PLUGINS; do
    PLOWER=$(echo "$PLUGIN" | tr '[:upper:]' '[:lower:]')
    BUNDLE_BASE="com.socalabs.${PLOWER}"
    VERSION=$(cat "$PROJECT_ROOT/plugins/$PLUGIN/VERSION" | tr -d '[:space:]')
    ART_DIR="$PROJECT_ROOT/Builds/xcode/plugins/$PLUGIN/${PLUGIN}_artefacts/Release"

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

    # Per-plugin scripts dir (needs only the matching preinstall + shared postinstall).
    SCRIPTS="$STAGE/scripts"
    mkdir -p "$SCRIPTS"
    cp "$PROJECT_ROOT/Installer/macOS/scripts/preinstall_${PLUGIN}" "$SCRIPTS/preinstall"
    cp "$PROJECT_ROOT/Installer/macOS/scripts/postinstall"          "$SCRIPTS/postinstall"
    chmod +x "$SCRIPTS/preinstall" "$SCRIPTS/postinstall"

    if [ "$HAS_PRESETS" = "yes" ]; then
      pkgbuild --root "$STAGE/resources" --install-location "/" --identifier "${BUNDLE_BASE}.resources.pkg" --version "$VERSION" --scripts "$SCRIPTS" "$PKG_DIR/resources.pkg"
    else
      # Use the scripts component on the CLAP package so preinstall/postinstall still run.
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
# Linux — per-plugin .deb via dpkg-deb + cmake --install per component
############################################################
elif [ "$PLATFORM" = "linux" ]; then
  cd "$PROJECT_ROOT"
  cmake --preset ninja-clang
  cmake --build --preset ninja-clang --config Release

  for PLUGIN in $PLUGINS; do
    PLOWER=$(echo "$PLUGIN" | tr '[:upper:]' '[:lower:]')
    VERSION=$(cat "$PROJECT_ROOT/plugins/$PLUGIN/VERSION" | tr -d '[:space:]')
    DEB_ROOT="$PROJECT_ROOT/Installer/linux/bin/$PLUGIN"
    rm -Rf "$DEB_ROOT"
    mkdir -p "$DEB_ROOT/DEBIAN" "$DEB_ROOT/usr"

    cmake --install "$PROJECT_ROOT/Builds/ninja-clang" --component "$PLUGIN" --prefix "$DEB_ROOT/usr"

    SIZE_KB=$(du -sk "$DEB_ROOT/usr" | awk '{print $1}')

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
  cd "$PROJECT_ROOT"
  cmake --preset vs
  cmake --build --preset vs --config Release

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

  for PLUGIN in $PLUGINS; do
    ART_DIR="$PROJECT_ROOT/Builds/vs/plugins/$PLUGIN/${PLUGIN}_artefacts/Release"
    STAGE="$PROJECT_ROOT/Installer/win/$PLUGIN"
    rm -Rf "$STAGE"
    mkdir -p "$STAGE/bin/VST" "$STAGE/bin/VST3" "$STAGE/bin/CLAP"

    cp -R "$ART_DIR/VST/$PLUGIN.dll"   "$STAGE/bin/VST/"
    cp -R "$ART_DIR/VST3/$PLUGIN.vst3" "$STAGE/bin/VST3/"
    cp -R "$ART_DIR/CLAP/$PLUGIN.clap" "$STAGE/bin/CLAP/"

    if [ "$WIN_SIGN" = "1" ]; then
      sign_file "$STAGE/bin/VST/$PLUGIN.dll"
      sign_file "$STAGE/bin/VST3/$PLUGIN.vst3/Contents/x86_64-win/$PLUGIN.vst3"
      sign_file "$STAGE/bin/CLAP/$PLUGIN.clap"
    fi

    # Run ISCC from STAGE so OutputDir=.\bin works as expected.
    cp "$PROJECT_ROOT/Installer/win/${PLUGIN}.iss" "$STAGE/${PLUGIN}.iss"
    cp -R "$PROJECT_ROOT/Installer/EULA.rtf" "$STAGE/../EULA.rtf"
    (cd "$STAGE" && "$ISCC" "${PLUGIN}.iss")

    EXE_OUT="$STAGE/bin/${PLUGIN}.exe"
    [ "$WIN_SIGN" = "1" ] && sign_file "$EXE_OUT"
    cp "$EXE_OUT" "$PROJECT_ROOT/bin/"
  done
fi
