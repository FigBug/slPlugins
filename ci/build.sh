#!/bin/bash -e

# linux specific stiff
if [ $OS = "linux" ]; then
  sudo apt-get update
  sudo apt-get install clang git ladspa-sdk freeglut3-dev g++ libasound2-dev libcurl4-openssl-dev libfreetype6-dev libjack-jackd2-dev libx11-dev libxcomposite-dev libxcursor-dev libxinerama-dev libxrandr-dev mesa-common-dev webkit2gtk-4.0 juce-tools xvfb
fi

# mac specific stuff
if [ $OS = "mac" ]; then
  # Create a temp keychain
  if [ -n "$GITHUB_ACTIONS" ]; then
    echo "Create a keychain"
    security create-keychain -p nr4aGPyz Keys.keychain

    echo $APPLICATION | base64 -D -o /tmp/Application.p12
    echo $INSTALLER | base64 -D -o /tmp/Installer.p12

    security import /tmp/Application.p12 -t agg -k Keys.keychain -P aym9PKWB -A -T /usr/bin/codesign
    security import /tmp/Installer.p12 -t agg -k Keys.keychain -P aym9PKWB -A -T /usr/bin/codesign

    security list-keychains -s Keys.keychain
    security default-keychain -s Keys.keychain
    security unlock-keychain -p nr4aGPyz Keys.keychain
    security set-keychain-settings -l -u -t 13600 Keys.keychain
    security set-key-partition-list -S apple-tool:,apple: -s -k nr4aGPyz Keys.keychain
  fi
  DEV_APP_ID="Developer ID Application: Roland Rabien (3FS7DJDG38)"
  DEV_INST_ID="Developer ID Installer: Roland Rabien (3FS7DJDG38)"
fi

ROOT=$(cd "$(dirname "$0")/.."; pwd)
cd "$ROOT"
echo "$ROOT"

BRANCH=${GITHUB_REF##*/}
echo "$BRANCH"

cd "$ROOT/ci"
rm -Rf bin
mkdir bin

# Get the hash
cd "$ROOT/modules/juce"
HASH=`git rev-parse HEAD`
echo "Hash: $HASH"

# Get the Projucer
cd "$ROOT/ci/bin"
while true
do
  PROJUCER_URL=$(curl -s -S "https://projucer.rabien.com/get_projucer.php?hash=$HASH&os=$OS&key=$APIKEY")
  echo "Response: $PROJUCER_URL"
  if [[ $PROJUCER_URL == http* ]]; then
    curl -s -S $PROJUCER_URL -o "$ROOT/ci/bin/Projucer.zip"
    unzip Projucer.zip
    break
  fi
  sleep 15
done

if [ $OS = "mac" ]; then
  # Build notarize tool
  cd "$ROOT/modules/gin/tools/notarize"
  "$ROOT/ci/bin/Projucer.app/Contents/MacOS/Projucer" --set-global-search-path osx defaultJuceModulePath "$ROOT/modules/juce/modules" 
  "$ROOT/ci/bin/Projucer.app/Contents/MacOS/Projucer" --resave "notarize.jucer"
  cd Builds/MacOSX
  xcodebuild -configuration Release || exit 1
  cd build/Release
  cp notarize "$ROOT/ci/bin"
fi

cd "$ROOT/ci"
cat pluginlist.txt | while read PLUGIN; do
  PLUGIN=$(echo $PLUGIN|tr -d '\n\r ')

  # Resave jucer file
  if [ "$OS" = "mac" ]; then
    "$ROOT/ci/bin/Projucer.app/Contents/MacOS/Projucer" --resave "$ROOT/plugins/$PLUGIN/$PLUGIN.jucer"
  elif [ "$OS" = "linux" ]; then
    "$ROOT/ci/bin/Projucer" --resave "$ROOT/plugins/$PLUGIN/$PLUGIN.jucer"
  else
    "$ROOT/ci/bin/Projucer.exe" --resave "$ROOT/plugins/$PLUGIN/$PLUGIN.jucer"
  fi

  # Build mac version
  if [ "$OS" = "mac" ]; then
    cd "$ROOT/plugins/$PLUGIN/Builds/MacOSX"
    xcodebuild -configuration Release || exit 1

    cp -R ~/Library/Audio/Plug-Ins/VST/$PLUGIN.vst "$ROOT/ci/bin"
    cp -R ~/Library/Audio/Plug-Ins/VST3/$PLUGIN.vst3 "$ROOT/ci/bin"
    cp -R ~/Library/Audio/Plug-Ins/Components/$PLUGIN.component "$ROOT/ci/bin"

    cd "$ROOT/ci/bin"
    codesign -s "$DEV_APP_ID" -v "$PLUGIN.vst" --options=runtime --timestamp --force
    codesign -s "$DEV_APP_ID" -v "$PLUGIN.vst3" --options=runtime --timestamp --force
    codesign -s "$DEV_APP_ID" -v "$PLUGIN.component" --options=runtime --timestamp --force

    # Notarize
    cd "$ROOT/ci/bin"
    zip -r ${PLUGIN}_Mac.zip $PLUGIN.vst  $PLUGIN.vst3 $PLUGIN.component

    "$ROOT/ci/bin/notarize" -ns ${PLUGIN}_Mac.zip $APPLE_USER $APPLE_PASS com.figbug.$PLUGIN.vst

    rm ${PLUGIN}_Mac.zip
    xcrun stapler staple $PLUGIN.vst
    xcrun stapler staple $PLUGIN.vst3
    xcrun stapler staple $PLUGIN.component
    zip -r ${PLUGIN}_Mac.zip $PLUGIN.vst $PLUGIN.component

    if [ "$BRANCH" = "release" ]; then
      curl -F "files=@${PLUGIN}_Mac.zip" "https://socalabs.com/files/set.php?key=$APIKEY"
    fi
  fi

  # Build linux version
  if [ "$OS" = "linux" ]; then
    cd "$ROOT/plugins/$PLUGIN/Builds/LinuxMakefile"
    make CONFIG=Release

    cp ./build/$PLUGIN.so "$ROOT/ci/bin"
    cp -r ./build/$PLUGIN.vst3 "$ROOT/ci/bin"

    cd "$ROOT/ci/bin"

    # Upload
    cd "$ROOT/ci/bin"
    zip -r ${PLUGIN}_Linux.zip $PLUGIN.so $PLUGIN.vst3

    if [ "$BRANCH" = "release" ]; then
      curl -F "files=@${PLUGIN}_Linux.zip" "https://socalabs.com/files/set.php?key=$APIKEY"
    fi
  fi

  # Build Win version
  if [ "$OS" = "win" ]; then
    VS_WHERE="C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe"
    
    MSBUILD_EXE=$("$VS_WHERE" -latest -requires Microsoft.Component.MSBuild -find "MSBuild\**\Bin\MSBuild.exe")
    echo $MSBUILD_EXE

    cd "$ROOT/plugins/$PLUGIN/Builds/VisualStudio2022"
    "$MSBUILD_EXE" "$PLUGIN.sln" "//p:VisualStudioVersion=16.0" "//m" "//t:Build" "//p:Configuration=Release64" "//p:Platform=x64" "//p:PreferredToolArchitecture=x64"
    "$MSBUILD_EXE" "$PLUGIN.sln" "//p:VisualStudioVersion=16.0" "//m" "//t:Build" "//p:Configuration=Release" "//p:PlatformTarget=x86" "//p:PreferredToolArchitecture=x64"

    cd "$ROOT/ci/bin"
    mkdir -p VST
    mkdir -p VST_32
    mkdir -p VST3
    mkdir -p VST3_32

    cp "$ROOT/plugins/$PLUGIN/Builds/VisualStudio2022/x64/Release64/VST/${PLUGIN}.dll" VST
    cp "$ROOT/plugins/$PLUGIN/Builds/VisualStudio2022/x64/Release64/VST3/${PLUGIN}.vst3" VST3
    cp "$ROOT/plugins/$PLUGIN/Builds/VisualStudio2022/Win32/Release/VST/${PLUGIN}.dll" VST_32
    cp "$ROOT/plugins/$PLUGIN/Builds/VisualStudio2022/Win32/Release/VST3/${PLUGIN}.vst3" VST3_32

    pwd
    7z a ${PLUGIN}_Win.zip VST VST_32 VST3 VST3_32
    if [ "$BRANCH" = "release" ]; then
      curl -F "files=@${PLUGIN}_Win.zip" "https://socalabs.com/files/set.php?key=$APIKEY"
    fi
  fi
done

# Make All.zip
if [ $OS = "mac" ]; then
  cd "$ROOT/ci/bin"
  curl -s -S "https://socalabs.com/files/get.php?id=SID_Mac.zip" -o "$ROOT/ci/bin/SID_Mac.zip"  
  curl -s -S "https://socalabs.com/files/get.php?id=PAPU_Mac.zip" -o "$ROOT/ci/bin/PAPU_Mac.zip"  
  curl -s -S "https://socalabs.com/files/get.php?id=Voc_Mac.zip" -o "$ROOT/ci/bin/Voc_Mac.zip"  
  curl -s -S "https://socalabs.com/files/get.php?id=SN76489_Mac.zip" -o "$ROOT/ci/bin/SN76489_Mac.zip"  
  curl -s -S "https://socalabs.com/files/get.php?id=RP2A03_Mac.zip" -o "$ROOT/ci/bin/RP2A03_Mac.zip"  
  curl -s -S "https://socalabs.com/files/get.php?id=Mverb2020_Mac.zip" -o "$ROOT/ci/bin/Mverb2020_Mac.zip"  

  unzip SID_Mac.zip
  unzip PAPU_Mac.zip
  unzip Voc_Mac.zip
  unzip SN76489_Mac.zip
  unzip RP2A03_Mac.zip
  unzip Mverb2020_Mac.zip

  mkdir VST
  mkdir VST3
  mkdir AU
  mv *.vst VST
  mv *.vst3 VST3
  mv *.component AU
  zip -r All_Mac.zip AU VST VST3

  if [ "$BRANCH" = "release" ]; then
    curl -F "files=@All_Mac.zip" "https://socalabs.com/files/set.php?key=$APIKEY"
  fi
elif [ $OS = "linux" ]; then
  cd "$ROOT/ci/bin"
  curl -s -S "https://socalabs.com/files/get.php?id=SID_Linux.zip" -o "$ROOT/ci/bin/SID_Linux.zip"  
  curl -s -S "https://socalabs.com/files/get.php?id=PAPU_Linux.zip" -o "$ROOT/ci/bin/PAPU_Linux.zip"  
  curl -s -S "https://socalabs.com/files/get.php?id=Voc_Linux.zip" -o "$ROOT/ci/bin/Voc_Linux.zip"  
  curl -s -S "https://socalabs.com/files/get.php?id=SN76489_Linux.zip" -o "$ROOT/ci/bin/SN76489_Linux.zip"  
  curl -s -S "https://socalabs.com/files/get.php?id=RP2A03_Linux.zip" -o "$ROOT/ci/bin/RP2A03_Linux.zip"  
  curl -s -S "https://socalabs.com/files/get.php?id=Mverb2020_Linux.zip" -o "$ROOT/ci/bin/Mverb2020_Linux.zip"  

  unzip SID_Linux.zip
  unzip PAPU_Linux.zip
  unzip Voc_Linux.zip
  unzip SN76489_Linux.zip
  unzip RP2A03_Linux.zip
  unzip Mverb2020_Linux.zip

  mkdir VST
  mkdir VST3
  mv *.so VST
  mv *.vst3 VS_WHERE

  zip -r All_Linux.zip VST VST3

  if [ "$BRANCH" = "release" ]; then
    curl -F "files=@All_Linux.zip" "https://socalabs.com/files/set.php?key=$APIKEY"  
  fi
else
  cd "$ROOT/ci/bin"
  curl -s -S "https://socalabs.com/files/get.php?id=SID_Win.zip" -o "$ROOT/ci/bin/SID_Win.zip"  
  curl -s -S "https://socalabs.com/files/get.php?id=PAPU_Win.zip" -o "$ROOT/ci/bin/PAPU_Win.zip"  
  curl -s -S "https://socalabs.com/files/get.php?id=Voc_Win.zip" -o "$ROOT/ci/bin/Voc_Win.zip"  
  curl -s -S "https://socalabs.com/files/get.php?id=SN76489_Win.zip" -o "$ROOT/ci/bin/SN76489_Win.zip"  
  curl -s -S "https://socalabs.com/files/get.php?id=RP2A03_Win.zip" -o "$ROOT/ci/bin/RP2A03_Win.zip"  
  curl -s -S "https://socalabs.com/files/get.php?id=Mverb2020_Win.zip" -o "$ROOT/ci/bin/Mverb2020_Win.zip"  

  unzip SID_Win.zip
  unzip PAPU_Win.zip
  unzip Voc_Win.zip
  unzip SN76489_Win.zip
  unzip RP2A03_Win.zip
  unzip Mverb2020_Win.zip

  7z a All_Win.zip VST VST_32 VST3 VST3_32

  if [ "$BRANCH" = "release" ]; then
    curl -F "files=@All_Win.zip" "https://socalabs.com/files/set.php?key=$APIKEY"
  fi
fi
