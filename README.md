# Air Conditioner – 3D Graphics Simulation

An interactive 3D air conditioner simulation developed using **C++**, **OpenGL**, **GLFW**, **GLEW**, and **GLM**.

This application was developed as an individual project for the Computer Graphics course at the Faculty of Technical Sciences, University of Novi Sad.

The project demonstrates real-time 3D rendering, camera controls, object interaction, animation, and state management through a simulated air conditioning system in a 3D environment.

## Preview

![Air Conditioner 3D Simulation](screenshots/air-conditioner-3d.png)

## Features

- **3D Environment** – Render an air conditioner, room, water container, and surrounding objects in a three-dimensional scene.
- **Interactive Camera** – Control the viewing direction using mouse movement and adjust the field of view using the scroll wheel.
- **Gaze-Based Interaction** – Turn the air conditioner on or off by looking directly at its power indicator.
- **Temperature Simulation** – Adjust the desired temperature between -10°C and 40°C and simulate changes in the measured temperature.
- **Animated Air Vent** – Smoothly open and close the air vent depending on the system state.
- **Water Simulation** – Animate falling water droplets and gradually fill a container.
- **Object Interaction** – Pick up a full water container, empty it while facing away from the air conditioner, and return it to its position.
- **Automatic Shutdown** – Turn off the air conditioner when the water container reaches capacity.
- **Rendering Controls** – Toggle depth testing and face culling during runtime.

## Technologies

| Component | Technology |
|---|---|
| Programming Language | C++ |
| Graphics API | OpenGL 3.3 |
| Windowing and Input | GLFW |
| OpenGL Extension Loading | GLEW |
| Mathematics | GLM |
| Image Loading | stb_image |
| Development Environment | Visual Studio |

## Controls

| Input | Action |
|---|---|
| Mouse movement | Look around the 3D scene |
| Mouse scroll | Adjust field of view |
| Look at the power indicator | Toggle the air conditioner |
| Up / Down Arrow | Adjust desired temperature |
| Left mouse click | Pick up the full water container |
| Space | Empty or return the water container, depending on the current state and viewing direction |
| 1 | Toggle depth testing |
| 2 | Toggle face culling |
| Escape | Exit the application |

## Getting Started

### Prerequisites

- Visual Studio with C++ desktop development support
- OpenGL-compatible graphics hardware and drivers
- GLFW, GLEW, and GLM dependencies

### Running the Application

1. Clone the repository.
2. Open `Sablon.sln` in Visual Studio.
3. Restore the required dependencies and configure library paths if necessary.
4. Build and run the project.

**Note:** Additional dependency configuration may be required depending on the local development environment.
