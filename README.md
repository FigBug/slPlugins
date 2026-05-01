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

| Plugin | Build |
|--------|-------|
| ABTester | ![Build ABTester](https://github.com/FigBug/slPlugins/workflows/Build%20ABTester/badge.svg) |
| AddInvert | ![Build AddInvert](https://github.com/FigBug/slPlugins/workflows/Build%20AddInvert/badge.svg) |
| ChannelMute | ![Build ChannelMute](https://github.com/FigBug/slPlugins/workflows/Build%20ChannelMute/badge.svg) |
| CompensatedDelay | ![Build CompensatedDelay](https://github.com/FigBug/slPlugins/workflows/Build%20CompensatedDelay/badge.svg) |
| Compressor | ![Build Compressor](https://github.com/FigBug/slPlugins/workflows/Build%20Compressor/badge.svg) |
| Crossfeed | ![Build Crossfeed](https://github.com/FigBug/slPlugins/workflows/Build%20Crossfeed/badge.svg) |
| Delay | ![Build Delay](https://github.com/FigBug/slPlugins/workflows/Build%20Delay/badge.svg) |
| Expander | ![Build Expander](https://github.com/FigBug/slPlugins/workflows/Build%20Expander/badge.svg) |
| Gate | ![Build Gate](https://github.com/FigBug/slPlugins/workflows/Build%20Gate/badge.svg) |
| GraphicEQ | ![Build GraphicEQ](https://github.com/FigBug/slPlugins/workflows/Build%20GraphicEQ/badge.svg) |
| HugeGain | ![Build HugeGain](https://github.com/FigBug/slPlugins/workflows/Build%20HugeGain/badge.svg) |
| Limiter | ![Build Limiter](https://github.com/FigBug/slPlugins/workflows/Build%20Limiter/badge.svg) |
| Maths | ![Build Maths](https://github.com/FigBug/slPlugins/workflows/Build%20Maths/badge.svg) |
| MidiLooper | ![Build MidiLooper](https://github.com/FigBug/slPlugins/workflows/Build%20MidiLooper/badge.svg) |
| MidiMonitor | ![Build MidiMonitor](https://github.com/FigBug/slPlugins/workflows/Build%20MidiMonitor/badge.svg) |
| Oscilloscope | ![Build Oscilloscope](https://github.com/FigBug/slPlugins/workflows/Build%20Oscilloscope/badge.svg) |
| PitchTrack | ![Build PitchTrack](https://github.com/FigBug/slPlugins/workflows/Build%20PitchTrack/badge.svg) |
| SampleDelay | ![Build SampleDelay](https://github.com/FigBug/slPlugins/workflows/Build%20SampleDelay/badge.svg) |
| SFX8 | ![Build SFX8](https://github.com/FigBug/slPlugins/workflows/Build%20SFX8/badge.svg) |
| SimpleVerb | ![Build SimpleVerb](https://github.com/FigBug/slPlugins/workflows/Build%20SimpleVerb/badge.svg) |
| SpectrumAnalyzer | ![Build SpectrumAnalyzer](https://github.com/FigBug/slPlugins/workflows/Build%20SpectrumAnalyzer/badge.svg) |
| StereoEnhancer | ![Build StereoEnhancer](https://github.com/FigBug/slPlugins/workflows/Build%20StereoEnhancer/badge.svg) |
| StereoProcessor | ![Build StereoProcessor](https://github.com/FigBug/slPlugins/workflows/Build%20StereoProcessor/badge.svg) |
| ToneGenerator | ![Build ToneGenerator](https://github.com/FigBug/slPlugins/workflows/Build%20ToneGenerator/badge.svg) |
| WaveLooper | ![Build WaveLooper](https://github.com/FigBug/slPlugins/workflows/Build%20WaveLooper/badge.svg) |
| XYScope | ![Build XYScope](https://github.com/FigBug/slPlugins/workflows/Build%20XYScope/badge.svg) |

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
