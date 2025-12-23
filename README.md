# slPlugins
Various VST/AU Plugins from SocaLabs.com

Here are some plugins I wrote. Mostly effects. The synths like PAPU, SID, etc. have their own repos.

## Prerequisites

- CMake 3.24.0 or higher
- C++20 compatible compiler
- Git (for cloning submodules)

## Initial Setup

After cloning the repository, initialize and update the submodules:

```bash
git submodule update --init --recursive
```

This step is required before building, as the plugins depend on JUCE and other third-party modules.

## Build Status

| Plugin | macOS | Linux | Windows |
|--------|-------|-------|---------|
| ABTester | ![macOS](https://github.com/FigBug/slPlugins/workflows/Build%20ABTester%20-%20macOS/badge.svg) | ![Linux](https://github.com/FigBug/slPlugins/workflows/Build%20ABTester%20-%20Linux/badge.svg) | ![Windows](https://github.com/FigBug/slPlugins/workflows/Build%20ABTester%20-%20Windows/badge.svg) |
| AddInvert | ![macOS](https://github.com/FigBug/slPlugins/workflows/Build%20AddInvert%20-%20macOS/badge.svg) | ![Linux](https://github.com/FigBug/slPlugins/workflows/Build%20AddInvert%20-%20Linux/badge.svg) | ![Windows](https://github.com/FigBug/slPlugins/workflows/Build%20AddInvert%20-%20Windows/badge.svg) |
| ChannelMute | ![macOS](https://github.com/FigBug/slPlugins/workflows/Build%20ChannelMute%20-%20macOS/badge.svg) | ![Linux](https://github.com/FigBug/slPlugins/workflows/Build%20ChannelMute%20-%20Linux/badge.svg) | ![Windows](https://github.com/FigBug/slPlugins/workflows/Build%20ChannelMute%20-%20Windows/badge.svg) |
| CompensatedDelay | ![macOS](https://github.com/FigBug/slPlugins/workflows/Build%20CompensatedDelay%20-%20macOS/badge.svg) | ![Linux](https://github.com/FigBug/slPlugins/workflows/Build%20CompensatedDelay%20-%20Linux/badge.svg) | ![Windows](https://github.com/FigBug/slPlugins/workflows/Build%20CompensatedDelay%20-%20Windows/badge.svg) |
| Compressor | ![macOS](https://github.com/FigBug/slPlugins/workflows/Build%20Compressor%20-%20macOS/badge.svg) | ![Linux](https://github.com/FigBug/slPlugins/workflows/Build%20Compressor%20-%20Linux/badge.svg) | ![Windows](https://github.com/FigBug/slPlugins/workflows/Build%20Compressor%20-%20Windows/badge.svg) |
| Crossfeed | ![macOS](https://github.com/FigBug/slPlugins/workflows/Build%20Crossfeed%20-%20macOS/badge.svg) | ![Linux](https://github.com/FigBug/slPlugins/workflows/Build%20Crossfeed%20-%20Linux/badge.svg) | ![Windows](https://github.com/FigBug/slPlugins/workflows/Build%20Crossfeed%20-%20Windows/badge.svg) |
| Delay | ![macOS](https://github.com/FigBug/slPlugins/workflows/Build%20Delay%20-%20macOS/badge.svg) | ![Linux](https://github.com/FigBug/slPlugins/workflows/Build%20Delay%20-%20Linux/badge.svg) | ![Windows](https://github.com/FigBug/slPlugins/workflows/Build%20Delay%20-%20Windows/badge.svg) |
| EQ3 | ![macOS](https://github.com/FigBug/slPlugins/workflows/Build%20EQ3%20-%20macOS/badge.svg) | ![Linux](https://github.com/FigBug/slPlugins/workflows/Build%20EQ3%20-%20Linux/badge.svg) | ![Windows](https://github.com/FigBug/slPlugins/workflows/Build%20EQ3%20-%20Windows/badge.svg) |
| Expander | ![macOS](https://github.com/FigBug/slPlugins/workflows/Build%20Expander%20-%20macOS/badge.svg) | ![Linux](https://github.com/FigBug/slPlugins/workflows/Build%20Expander%20-%20Linux/badge.svg) | ![Windows](https://github.com/FigBug/slPlugins/workflows/Build%20Expander%20-%20Windows/badge.svg) |
| Gate | ![macOS](https://github.com/FigBug/slPlugins/workflows/Build%20Gate%20-%20macOS/badge.svg) | ![Linux](https://github.com/FigBug/slPlugins/workflows/Build%20Gate%20-%20Linux/badge.svg) | ![Windows](https://github.com/FigBug/slPlugins/workflows/Build%20Gate%20-%20Windows/badge.svg) |
| HugeGain | ![macOS](https://github.com/FigBug/slPlugins/workflows/Build%20HugeGain%20-%20macOS/badge.svg) | ![Linux](https://github.com/FigBug/slPlugins/workflows/Build%20HugeGain%20-%20Linux/badge.svg) | ![Windows](https://github.com/FigBug/slPlugins/workflows/Build%20HugeGain%20-%20Windows/badge.svg) |
| Limiter | ![macOS](https://github.com/FigBug/slPlugins/workflows/Build%20Limiter%20-%20macOS/badge.svg) | ![Linux](https://github.com/FigBug/slPlugins/workflows/Build%20Limiter%20-%20Linux/badge.svg) | ![Windows](https://github.com/FigBug/slPlugins/workflows/Build%20Limiter%20-%20Windows/badge.svg) |
| Maths | ![macOS](https://github.com/FigBug/slPlugins/workflows/Build%20Maths%20-%20macOS/badge.svg) | ![Linux](https://github.com/FigBug/slPlugins/workflows/Build%20Maths%20-%20Linux/badge.svg) | ![Windows](https://github.com/FigBug/slPlugins/workflows/Build%20Maths%20-%20Windows/badge.svg) |
| MidiLooper | ![macOS](https://github.com/FigBug/slPlugins/workflows/Build%20MidiLooper%20-%20macOS/badge.svg) | ![Linux](https://github.com/FigBug/slPlugins/workflows/Build%20MidiLooper%20-%20Linux/badge.svg) | ![Windows](https://github.com/FigBug/slPlugins/workflows/Build%20MidiLooper%20-%20Windows/badge.svg) |
| Oscilloscope | ![macOS](https://github.com/FigBug/slPlugins/workflows/Build%20Oscilloscope%20-%20macOS/badge.svg) | ![Linux](https://github.com/FigBug/slPlugins/workflows/Build%20Oscilloscope%20-%20Linux/badge.svg) | ![Windows](https://github.com/FigBug/slPlugins/workflows/Build%20Oscilloscope%20-%20Windows/badge.svg) |
| PitchTrack | ![macOS](https://github.com/FigBug/slPlugins/workflows/Build%20PitchTrack%20-%20macOS/badge.svg) | ![Linux](https://github.com/FigBug/slPlugins/workflows/Build%20PitchTrack%20-%20Linux/badge.svg) | ![Windows](https://github.com/FigBug/slPlugins/workflows/Build%20PitchTrack%20-%20Windows/badge.svg) |
| SFX8 | ![macOS](https://github.com/FigBug/slPlugins/workflows/Build%20SFX8%20-%20macOS/badge.svg) | ![Linux](https://github.com/FigBug/slPlugins/workflows/Build%20SFX8%20-%20Linux/badge.svg) | ![Windows](https://github.com/FigBug/slPlugins/workflows/Build%20SFX8%20-%20Windows/badge.svg) |
| SampleDelay | ![macOS](https://github.com/FigBug/slPlugins/workflows/Build%20SampleDelay%20-%20macOS/badge.svg) | ![Linux](https://github.com/FigBug/slPlugins/workflows/Build%20SampleDelay%20-%20Linux/badge.svg) | ![Windows](https://github.com/FigBug/slPlugins/workflows/Build%20SampleDelay%20-%20Windows/badge.svg) |
| SimpleVerb | ![macOS](https://github.com/FigBug/slPlugins/workflows/Build%20SimpleVerb%20-%20macOS/badge.svg) | ![Linux](https://github.com/FigBug/slPlugins/workflows/Build%20SimpleVerb%20-%20Linux/badge.svg) | ![Windows](https://github.com/FigBug/slPlugins/workflows/Build%20SimpleVerb%20-%20Windows/badge.svg) |
| SpectrumAnalyzer | ![macOS](https://github.com/FigBug/slPlugins/workflows/Build%20SpectrumAnalyzer%20-%20macOS/badge.svg) | ![Linux](https://github.com/FigBug/slPlugins/workflows/Build%20SpectrumAnalyzer%20-%20Linux/badge.svg) | ![Windows](https://github.com/FigBug/slPlugins/workflows/Build%20SpectrumAnalyzer%20-%20Windows/badge.svg) |
| StereoEnhancer | ![macOS](https://github.com/FigBug/slPlugins/workflows/Build%20StereoEnhancer%20-%20macOS/badge.svg) | ![Linux](https://github.com/FigBug/slPlugins/workflows/Build%20StereoEnhancer%20-%20Linux/badge.svg) | ![Windows](https://github.com/FigBug/slPlugins/workflows/Build%20StereoEnhancer%20-%20Windows/badge.svg) |
| StereoProcessor | ![macOS](https://github.com/FigBug/slPlugins/workflows/Build%20StereoProcessor%20-%20macOS/badge.svg) | ![Linux](https://github.com/FigBug/slPlugins/workflows/Build%20StereoProcessor%20-%20Linux/badge.svg) | ![Windows](https://github.com/FigBug/slPlugins/workflows/Build%20StereoProcessor%20-%20Windows/badge.svg) |
| ToneGenerator | ![macOS](https://github.com/FigBug/slPlugins/workflows/Build%20ToneGenerator%20-%20macOS/badge.svg) | ![Linux](https://github.com/FigBug/slPlugins/workflows/Build%20ToneGenerator%20-%20Linux/badge.svg) | ![Windows](https://github.com/FigBug/slPlugins/workflows/Build%20ToneGenerator%20-%20Windows/badge.svg) |
| WaveLooper | ![macOS](https://github.com/FigBug/slPlugins/workflows/Build%20WaveLooper%20-%20macOS/badge.svg) | ![Linux](https://github.com/FigBug/slPlugins/workflows/Build%20WaveLooper%20-%20Linux/badge.svg) | ![Windows](https://github.com/FigBug/slPlugins/workflows/Build%20WaveLooper%20-%20Windows/badge.svg) |
| XYScope | ![macOS](https://github.com/FigBug/slPlugins/workflows/Build%20XYScope%20-%20macOS/badge.svg) | ![Linux](https://github.com/FigBug/slPlugins/workflows/Build%20XYScope%20-%20Linux/badge.svg) | ![Windows](https://github.com/FigBug/slPlugins/workflows/Build%20XYScope%20-%20Windows/badge.svg) |

## Building

### Build All Plugins

To build all plugins at once from the root directory:

```bash
cmake -B build
cmake --build build --config Release
```

### Build Individual Plugin

Each plugin can also be built independently from its own directory:

```bash
cd plugins/<plugin_name>
cmake -B build
cmake --build build --config Release
```

Built plugins will be located in the `build` directory, with VST/VST3/AU/LV2 formats depending on your platform.
