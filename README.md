# GPS Navigation App

A high-performance C++ navigation engine that computes the shortest path between two points on a real map using **Dijkstra’s algorithm** and a custom **KD-Tree** for spatial indexing.

![Navigation Demo](plot.png)

---

## Overview

This project implements a **from-scratch navigation system** — from map parsing to path visualization — optimized for both performance and scalability.

The system parses **OpenStreetMap (OSM)** data via [libosmium](https://osmcode.org/libosmium/), constructs an efficient **graph representation** of the road network, and computes routes using a fine-tuned version of **Dijkstra’s algorithm** combined with a **KD-Tree** for fast nearest-node lookups.

---

## Core Components

### **1. OSM Parsing**
- Uses *libosmium* to extract all relevant nodes and ways (streets) from raw OSM XML files.
- Generates lightweight text-based data files for faster subsequent loads (`Nodes.txt`, `Ways.txt`).
- Multi-threaded extraction of nodes and ways for improved preprocessing speed.

### **2. Graph Construction**
- Each map node is represented as a `Node` object with geographic coordinates.
- Edges (`Edge`) connect nodes with real-world distances computed using the **Haversine formula**.
- Supports **one-way** and **bidirectional** street definitions.
- Memory-optimized registry avoids redundant node creation and removes orphaned nodes.

### **3. KD-Tree for Spatial Queries**
- Custom 2D KD-Tree implementation for fast nearest-neighbor search in O(log n) time.
- Alternates between latitude and longitude axes at each level.
- Used to map any GPS coordinate to the nearest valid graph node before routing.

### **4. Dijkstra’s Algorithm**
- Optimized for sparse real-world graphs using:
  - Early exit once destination is reached.
  - Minimized priority-queue operations.
  - Edge relaxation limited by transportation mode.
- Achieves **millisecond-level performance** on city-scale maps.

### **5. Visualization**
- Uses **matplotlib-cpp** to render the map and highlight computed routes.
- Displays all nodes as scatter points and overlays the shortest path in red for easy verification.

---

##  Optimizations

| Category | Optimization |
|-----------|---------------|
| **Spatial Search** | Custom KD-Tree reduces nearest-node lookup time from O(n) to O(log n). |
| **Graph Handling** | Direct memory access and lightweight structures minimize overhead. |
| **Dijkstra Performance** | Early pruning and hash-based visitation tracking. |
| **Preprocessing** | OSM data extracted once, then reloaded from compact files for instant map initialization. |

---
