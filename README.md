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

- Sorting demo:
<img width="1392" height="912" alt="sorting-demo" src="https://github.com/user-attachments/assets/c805a2c1-7b77-4184-9eef-d20cb72ebd60" />

- Graph demo:
<img width="1392" height="912" alt="graph-demo" src="https://github.com/user-attachments/assets/abd31ace-510f-460b-97ec-6e424aacb66d" />

- Binary search demo:
<img width="1300" height="820" alt="Screen Recording 2026-04-22 at 09 19 14" src="https://github.com/user-attachments/assets/837d9b0d-693d-4009-92ce-e7144d2ff00d" />

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
