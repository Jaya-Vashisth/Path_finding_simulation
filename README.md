# Dijkstra Visualizer

An interactive C++/SFML application for visualizing Dijkstra’s shortest path algorithm. This tool allows users to build, edit, and animate custom graphs, step through the algorithm, and observe the evolution of the priority queue in real time.

---

## Features

- **Interactive Graph Creation:**  
  Add and remove nodes and edges with mouse clicks. Assign custom weights to edges.

- **Dijkstra’s Algorithm Animation:**  
  Step forward and backward through the algorithm, watching nodes and edges as they are visited and updated.

- **Priority Queue Visualization:**  
  See the exact state of the priority queue at every step, including duplicate entries, as in a real C++ `std::priority_queue`.

- **Final Path Highlighting:**  
  After the algorithm completes, the shortest path is highlighted in green.

- **User-Friendly Controls:**  
  - Left click: Add node or select nodes to create an edge  
  - Right click: Delete node  
  - Press `S`: Set start node  
  - Press `E`: Set end node  
  - Press `R`: Reset graph  
  - Press `SPACE`: Start or repeat animation  
  - "Next"/"Prev" buttons: Step through the algorithm

---

## Demo
[![Dijkstra Visualizer Demo](https://img.youtube.com/vi/YOUR_VIDEO_ID/0.jpg)](https://www.youtube.com/watch?v=YOUR_VIDEO_ID)

---

## Installation

1. **Clone the repository:**

2. **Install dependencies:**
- [SFML](https://www.sfml-dev.org/download.php) (Simple and Fast Multimedia Library)
- A C++20 compatible compiler (e.g., MSVC, GCC, Clang)

3. **Build the project:**
- Open the solution in Visual Studio 2022 and build, **or**
- Use CMake or your preferred build system.

4. **Run the application:**
- Make sure `arial.ttf` is present in the executable directory.

---

## Usage

- **Add Nodes:** Left-click anywhere in the main area.
- **Create Edges:** Click two nodes in succession, then enter the edge weight.
- **Delete Node:** Right-click on a node.
- **Set Start/End:** Hover over a node and press `S` (start) or `E` (end).
- **Reset:** Press `R` to clear the graph.
- **Start Animation:** Press `SPACE` (after setting start and end nodes).
- **Step Animation:** Use the "Next" and "Prev" buttons to step through the algorithm.
- **Observe:** Watch the priority queue panel and graph update in sync.

---

## Project Structure

- `DijkstraSolver.*` — Core Dijkstra algorithm and animation logic
- `PriorityQueuePanel.*` — Visualization of the priority queue
- `Graph.*` — Graph data structure and rendering
- `Panel.*`, `UIButton.*`, `InputBox.*` — UI components

---

## License

This project is licensed under the MIT License.

---

## Acknowledgements

- [SFML](https://www.sfml-dev.org/) for graphics and windowing
- Dijkstra’s algorithm (Edsger W. Dijkstra, 1956)

---
