# Dungeon Tavern

## Project Overview

**Dungeon Tavern** is an interactive 3D computer graphics project developed for the course **090958 Computer Graphics** at **Politecnico di Milano**.

The project presents a real-time rendered dungeon tavern environment built with **Vulkan**. The scene includes a tavern room with textured walls, floor, ceiling, furniture, torches, decorative chains, NPCs, background ambience, collision detection, lighting, and interactive dialogue elements.

The goal of the project is to combine several core computer graphics concepts into one coherent interactive scene, including model loading, texture mapping, descriptor sets, uniform buffers, shaders, camera movement, object transformations, lighting, collision handling, and simple game-style interactions.

---

## Authors

This project was developed by:

- **Carl Eskång**
- **Fanny Nyberg**
- **Felicia Murkes**

Course: **090958 Computer Graphics**  
University: **Politecnico di Milano**

---

## Main Features

The project includes:

- A fully explorable 3D tavern environment
- Imported 3D models for characters, furniture, torches, doors, chains, and other tavern objects
- Procedurally generated room geometry for floor, walls, and ceiling
- Textured models and room surfaces
- Player movement with collision detection
- Dynamic torch lighting
- Torch interaction system
- NPC interaction system
- Dialogue boxes for interactable NPCs
- Background tavern ambience audio
- Vulkan-based rendering pipeline

---

## Gameplay

The player can move around inside the tavern and interact with selected objects and NPCs.

### Movement Controls

| Key | Action |
|---|---|
| `W` | Move forward |
| `A` | Rotate/look left |
| `S` | Move backward |
| `D` | Rotate/look right |
| `Arrow Up` | Look up |
| `Arrow Down` | Look down |
| `F` | Toggle a nearby torch on/off |
| `E` | Interact with a nearby NPC / open dialogue |
| `Esc` | Close the application |

### Interactions

The game contains two main types of interactions:

#### Torch Interaction

When the player is close enough to a wall torch, the torch can be toggled on or off by pressing `F`. This affects the torch lighting in the scene, allowing the player to dynamically change the atmosphere of the tavern.

#### NPC Interaction

The player can interact with NPCs in the tavern by pressing `E` when close enough to an interactable character. Currently, the project includes dialogue interactions with:

- The **Innkeeper**
- The **Orc Bartender**

When the player is close enough to an interactable NPC and presses `E`, a dialogue box can be displayed.

---

## Repository Structure

The repository is structured into several main folders and source files.

```
.
├── assets/
│   ├── audio/
│   ├── models/
│   ├── textures/
│   └── ui/
│
├── include/
│   ├── CollisionSystem.hpp
│   ├── dialogBox.hpp
│   ├── NPCInteraction.hpp
│   ├── sceneObjects.hpp
│   └── torchInteraction.hpp
│
├── src/
│   ├── CollisionSystem.cpp
│   ├── dialogBox.cpp
│   ├── main.cpp
│   ├── NPCInteraction.cpp
│   ├── sceneObjects.cpp
│   └── torchInteraction.cpp
│
├── shaders/
│   ├── shader.vert
│   ├── shader.frag
│   ├── dialogbox.vert
│   └── dialogbox.frag
│
├── CMakeLists.txt
└── README.md
```
## Folder Description

### `assets/`

Contains all external resources used in the project.

#### `assets/models/`

Contains the 3D models used in the tavern scene.

#### `assets/textures/`

Contains texture files used for the models and room geometry.

#### `assets/ui/`

Contains UI-related textures, mainly dialogue box images for NPC interactions.

#### `assets/audio/`

Contains the background ambience used in the tavern scene.

### `include/`

Contains the header files for the main systems in the project.

Important files include:

- `sceneObjects.hpp`  
  Declares all models, textures, descriptor sets, room geometry, and functions for loading, drawing, updating, and cleaning up scene objects.

- `CollisionSystem.hpp`  
  Declares the collision system used to prevent the player from walking through walls, tables, bars, NPCs, and other solid objects.

- `NPCInteraction.hpp`  
  Declares the logic for interacting with NPCs and triggering dialogue boxes.

- `dialogBox.hpp`  
  Declares the UI system used to render dialogue boxes on screen.

- `torchInteraction.hpp`  
  Declares the logic for detecting nearby torches and toggling them on or off.

### `src/`

Contains the implementation files.

Important files include:

- `main.cpp`  
  Contains the main application class, Vulkan setup, camera movement, render loop, global lighting, audio initialization, and calls to the different scene systems.

- `sceneObjects.cpp`  
  Loads all models and textures, creates procedural room geometry, registers colliders, initializes descriptor sets, draws scene objects, updates uniform buffers, and cleans up resources.

- `CollisionSystem.cpp`  
  Implements player collision handling.

- `NPCInteraction.cpp`  
  Implements NPC interaction detection and dialogue triggering.

- `dialogBox.cpp`  
  Implements rendering of dialogue boxes.

- `torchInteraction.cpp`  
  Implements torch proximity detection and toggle behavior.

  ### `shaders/`

Contains the GLSL shader files used for rendering.

The main shaders are responsible for rendering the 3D scene with lighting and textures, while the dialogue box shaders are used for rendering the 2D UI overlay.

## Technical Implementation

The project is implemented in **C++** using **Vulkan**.

The main technical components include:

- Vulkan graphics pipeline setup
- Vertex descriptors for 3D scene objects and UI elements
- Descriptor set layouts for global, object, and UI data
- Uniform buffers for object transformations and global lighting
- Texture loading and sampling
- Imported `.obj` models
- Procedural mesh creation for the room
- Camera movement and view/projection matrices
- Collision detection using simple circle and box colliders
- Dynamic point lights for torches
- UI rendering for dialogue boxes
- Audio playback using `miniaudio`

## How to Run the Project

The project is intended to be run using **CLion** together with **Vulkan**.

### Requirements

Before running the project, make sure you have:

- CLion installed
- A C++ compiler
- CMake
- Vulkan SDK installed
- A Vulkan-compatible GPU or Vulkan support through MoltenVK on macOS

### Running in CLion

1. Open the project folder in **CLion**.
2. Make sure the Vulkan SDK is installed and correctly configured.
3. Let CLion load the `CMakeLists.txt` file.
4. Build the project.
5. Run the main executable from CLion.

The application should open a window showing the dungeon tavern scene.

## Demo
The demo shows:

- Movement inside the tavern
- Torch interaction
- NPC dialogue interaction
- Lighting changes
- The overall tavern environment

Add demo here!!!
