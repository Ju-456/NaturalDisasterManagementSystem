# NaturalDisasterManagementSystem

This project simulates earthquakes and visualizes their impact on a city's infrastructure through a graph-based system. It combines real-time graphical rendering with powerful graph algorithms and modular code architecture.

---

## Overview

The system models a city using vertices (e.g., cities, hospitals) and edges (roads), supports earthquake simulation, and provides visual and terminal feedback through a user-friendly graphical interface.

It is designed to be **highly modularized**, with functions written to be **flexible and above all easily reusable**.

---

## 📁 Project Structure

```
project_root/
│
├── algorithmic_side/                     # Initial development with focus on core logic
│   ├── main.c
│   ├── general.c
│   ├── parson.c and .h                   # JSON parsing and handling
│   ├── vertex.c and .h
│   ├── road.c and .h
│   ├── earthquake.c and .h
│   ├── window.c and .h
│   ├── shortestPath.c and .h
│   └── travel.c and .h                   # Manages algorithmic travel logic
│
├── interface_side/                       # Early integration of graphical interface
│   ├── main.c
│   ├── general.c
│   ├── parson.c and .h
│   ├── vertex.c and .h
│   └── window.c and .h
│
├── dynamic_interface/                    # Most updated and complete version
│   ├── main.c
│   ├── general.c
│   ├── parson.c and .h
│   ├── vertex.c and .h
│   ├── road.c and .h
│   ├── earthquake.c and .h
│   ├── window_draw_part.c and .h
│   ├── window_general_part.c and .h
│   ├── shortestPath.c and .h
│   ├── travel.c and .h
│   ├── vertexGroup.c and .h
│   └── minimalSpanningTree.c and .h
│   └── Assets/                           # Graphical resources
│       ├── CCity_mockup.png              # Buildings
│       ├── City_Tilemap.asprite          # Tileset source
│       ├── City_Transparent.png          # Transparent version of mockup
│       ├── grass_retro.png               # Graph background
│       ├── grass_retro1.png              # Darker version of graph background
│       └── transition_texture.png        # Blurred background for transition text
│
├── Graph_json/                           # JSON graphs used for GUI
│   ├── graphe3.json                      # Small 6-vertex to test graph "vertexGroup"
│   ├── grapheS.json
│   ├── grapheM.json
│   ├── grapheL1.json                     # Main graph (52 vertices)
│   ├── grapheL2.json
│   └── grapheXL.json                     # Largest graph (77 vertices)
│
├── MatAdj_txt/                           # Adjacency matrices for algorithmic processing
│   ├── graphe3.txt
│   ├── grapheS.txt
│   ├── grapheM.txt
│   ├── grapheL1.txt
│   ├── grapheL2.txt
│   └── grapheXL.txt
```

> 📌 **Note:** JSON and TXT data aren't directly linked but are **correlated** to ensure full system functionality.

---

### 🖼️ Function Naming Conventions

- `draw*` → Graphical rendering functions (also handle light computation)
- `display*` → Terminal-based visualization for debugging and inspection

---

## 🖱️ User Interaction & Display Logic

Raylib only allows **one main window**, so multiple modes (pseudo-windows) are handled using:

```c
typedef enum {
    MODE_GRAPH,
    MODE_VERTEX_DETAILS,
    MODE_ROAD_DETAILS
} AppMode;
```

- **Right-click on a vertex** → View road info
- **Left-click on a road** → View vertex info

![image](https://github.com/user-attachments/assets/3e56e6a3-d1f2-4b2e-bd1f-19c16f75a395) ![image](https://github.com/user-attachments/assets/6c76d8a9-efa9-4140-9630-c3dbdb01a84e)
> If the state **"State: (4 → 2)" appears in orange**, it indicates that the road's state changed from **4 to 2** as a result of the earthquake.

---

### 🔧 Customizing the Graph Input

Inside **`graph.h`**, which serves as the main header file (used by `general.c` and others), you can easily switch the graph data source by modifying the following lines (typically around lines 5–6):

```c
#define FILENAME_TXT "graphL1.txt"
#define FILENAME_JSON "graphL1.json"
```

Change these filenames to use a different graph file from the **Graph\_json/** or **MatAdj\_txt/** directories.

> 🛠️ Want to create your own graphs?
> Check out my **AnalogToDigitalConverter** project, which allows you to generate compatible .json and .txt graph files from analog signals.
(enable in my git :https://github.com/Ju-456/AnalogToDigitalConverter)
---

### 🔘 UI Buttons (Top-left corner, 3 dots)
![image](https://github.com/user-attachments/assets/6f34b819-97e0-4b6a-b814-4d63fc1a486c) ![image](https://github.com/user-attachments/assets/0733d84d-bbe5-4f68-8caf-d9dc7112cd50)

If the **H icon is green**, it means the corresponding option is currently **enabled and visible** in the interface.


| Feature | Description |
|--------|-------------|
| **Earthquake** | Randomly affects a city quadrant (NE, NW, SE, SW). Affects road status (orange = damaged). Can be triggered multiple times. |
| **State's Roads** | Toggle ON/OFF the visibility of damaged roads. |
| **Intervention** | Sends help from the closest hospital/warehouse to affected nodes. |
| **Group Accessible Areas** | Highlights strongly connected areas in the graph. |
| **Minimal Spanning Tree** | Displays the MST of the graph. |

Certainly! Here's a revised and polished version of that section in clear, correct English, ready to be added to the README:

---

### 🖼️ A Few Examples (with Graph Visualizations)

Below are a few visual examples using **`graphe3.json`**, the small 6-vertex test graph:

---

![image](https://github.com/user-attachments/assets/e1020043-8a22-4226-bffa-ac410f4e4280)

**State's Roads "ON" after two earthquakes**
You can see the affected roads highlighted in **orange**, indicating damaged connections following the simulated earthquakes.

---

![image](https://github.com/user-attachments/assets/520c3ad2-684f-433d-a605-b8cdf6539d9a)

**Group Accessible Areas "ON"**
This mode highlights **strongly connected components** of the graph, helping to visualize clusters that remain internally accessible.

---

![image](https://github.com/user-attachments/assets/d0140458-c467-473f-a56a-c84a34d8d8e3)

**Minimal Spanning Tree "ON"**
This view displays the **Minimal Spanning Tree (MST)** of the current graph, showing a subset of edges that connects all vertices with the minimum total weight.

---

## ⚠️ Earthquake Simulation Tips

- Multiple earthquakes can be triggered to **increase intervention opportunities**.
- If **no intervention occurs**, it is **not necessarily a bug**—the damaged node might be **unreachable** via Dijkstra's algorithm.
- Most nodes should remain accessible.

---

## 🧰  Building & Running the Project

### Prerequisites

- `gcc`
- `raylib` (you may need to **update the include path** in your `CFLAGS` accordingly. Normally it's adaptive but in case.)

### Compilation (example)

From `dynamic_interface/`, run:

```bash
make
```

```bash
./graph
```

To clean and rebuild:

```bash
make clean
make
```
