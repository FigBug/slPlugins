#!/bin/bash -e

ROOT=$(cd "$(dirname "$0")/.."; pwd)
cd "$ROOT"

export PATH=$PATH:"/c/Program Files/CMake/bin"

# Determine generator based on platform
if [ "$(uname)" == "Darwin" ]; then
  GENERATOR="Xcode"
elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
  GENERATOR="Ninja"
  export CMAKE_C_COMPILER=clang
  export CMAKE_CXX_COMPILER=clang++
elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW64_NT" ]; then
  GENERATOR="Visual Studio 17 2022"
fi

# Configure each plugin individually
cd "$ROOT/ci"
cat pluginlist.txt | while read PLUGIN; do
  PLUGIN=$(echo $PLUGIN|tr -d '\n\r ')

  echo "Configuring $PLUGIN..."
  cd "$ROOT/plugins/$PLUGIN"

  cmake -B build -G "$GENERATOR" -D JUCE_COPY_PLUGIN_AFTER_BUILD=ON

  echo "Configured $PLUGIN"
done

echo ""
echo "All plugins configured!"
