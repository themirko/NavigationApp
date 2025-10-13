# GPS Navigation App
A simple GPS navigation system that calculates the shortest path between two points in your city using Dijkstra’s algorithm. It parses map data, builds a custom graph, and visualizes the result.

![Navigation Demo](plot.png)

---

## How It's Made:

**Tech used:** C++, Osmium, Custom KD-Tree, Matplotlib-cpp

This navigation app is built from scratch in C++. I used [libosmium](https://osmcode.org/libosmium/) to parse OpenStreetMap (OSM) data and extract all the nodes and streets relevant to the city map. After extraction, I generated a graph structure where each node represents a point on the map and edges represent streets connecting them.

To allow fast lookup of the closest node to any GPS coordinate, I implemented a custom KD-Tree structure. The pathfinding is done using Dijkstra’s algorithm, adapted to work with real-world coordinates using the Haversine formula.

Since there's no user interface yet, a plotting function is included to visualize the computed path on a static map for verification and debugging.

---

## Optimizations
- Implemented a KD-Tree from scratch for fast spatial queries (node lookups).
- Efficient graph construction from OSM data via direct memory access and minimal object overhead.
- Early pruning in Dijkstra’s implementation to skip unnecessary nodes.

---

## Future Plans
- Create a full-featured UI.
- Currently optimized for either pedestrian or vehicle mode - future improvements will allow mode-specific pathfinding (e.g., avoid highways for pedestrians).
- Integrate a GPS location API to automatically extract the user's current location as the starting point for navigation.