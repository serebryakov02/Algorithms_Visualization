# Algorithms Visualization

A Qt desktop application for visualizing classic sorting, graph, and search algorithms in a simple interactive UI.

## Overview

The application is split into three tabs:

- `Sorting Algorithms`
- `Graph Visualizer`
- `Search Algorithms`

It is built with `Qt 6`, `C++17`, and `CMake`.

## Features

### Sorting Algorithms

- Bubble Sort
- Selection Sort
- Insertion Sort
- Gnome Sort
- Cocktail Sort
- Odd-Even Sort
- Adjustable number of bars
- Custom bar color and background color

### Graph Visualizer

- Random graph generation
- Adjustable number of vertices
- Weighted and unweighted graph modes
- Source and target selection
- Dijkstra visualization
- BFS visualization
- DFS visualization
- Optional on-canvas status overlay
- Custom node color and background color

### Search Algorithms

- Binary search visualization on a sorted array
- Adjustable number of elements
- Slider and spin box for element count
- Target value selection
- Quick option to pick a value that already exists in the array
- Custom bar color and background color

## Demo

- Sorting demo: `[TODO: put video here]`
- Graph demo: `[TODO: put video here]`
- Binary search demo: `[TODO: put video here]`

## Build

### Requirements

- CMake 3.21 or newer
- Qt 6 with the `Widgets` module
- A C++17 compiler

### Configure and build

```bash
cmake -S . -B build
cmake --build build
```

If CMake cannot find Qt, set `CMAKE_PREFIX_PATH` to your Qt installation:

```bash
cmake -S . -B build -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x.x/lib/cmake
cmake --build build
```