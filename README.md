# OpenGL Graphics Engine

A 3D graphics engine built from scratch using C++ and OpenGL 3.3. This project demonstrates core graphics programming concepts, including advanced lighting, shadow mapping, and 3D model loading.

## Features

*   **Modern OpenGL**: Uses OpenGL 3.3 Core Profile.
*   **Lighting System**: Supports multiple types of lights simulating real-world illumination:
    *   Directional Lights (e.g., Sun)
    *   Point Lights (e.g., Lightbulbs)
    *   Spot Lights (e.g., Flashlights)
*   **Shadow Mapping**: Dynamic shadows cast by objects in the scene.
    *   Standard 2D Shadow Mapping for Directional Lights.
    *   Omnidirectional Shadow Mapping (using Cube Maps and Geometry Shaders) for Point and Spot Lights.
*   **Model Loading**: Import complex 3D models (like `.obj` files) using the Assimp library.
*   **Skybox rendering**: Environment mapping using cube maps.
*   **Texture Mapping**: Supports standard 2D texture loading via `stb_image`.
*   **Camera System**: A free-look FPS-style camera with keyboard and mouse controls.
*   **Material System**: Adjustable specular intensity and shininess for different surfaces.

## Dependencies

This project relies on several third-party libraries:

*   **GLFW**: Window creation, context management, and input handling.
*   **GLEW**: OpenGL Extension Wrangler Library for loading OpenGL functions.
*   **GLM**: OpenGL Mathematics for vector and matrix operations.
*   **Assimp**: Open Asset Import Library for loading 3D models.
*   **stb_image**: Single-header image loading library.

## Project Structure

The project is organized into several classes, each responsible for a specific part of the graphics pipeline.

*   **Core Engine**
    *   `main.cpp`: The main application entry point. Handles initialization of all systems and runs the main render loop.
    *   `Window.h/cpp`: A wrapper for the GLFW window. Manages window creation, input callbacks, and buffer swapping.
    *   `Camera.h/cpp`: A first-person-style camera that handles view matrix calculations and processes keyboard/mouse input for movement and orientation.

*   **Rendering Components**
    *   `Shader.h/cpp`: A robust shader class for reading GLSL from files, compiling/linking them into a program, and setting uniforms.
    *   `Mesh.h/cpp`: Represents a single drawable object. Manages its own VAO, VBOs, and IBO, and contains the logic to render itself.
    *   `Model.h/cpp`: Represents a collection of meshes. Handles loading complex 3D models from files (e.g., `.obj`) using the Assimp library.
    *   `Texture.h/cpp`: Handles loading image data from files (using `stb_image`) and creating OpenGL texture objects.
    *   `Skybox.h/cpp`: Manages the loading of six textures into a cubemap and rendering the skybox.
    *   `Material.h/cpp`: Defines the surface properties of an object, such as specular intensity and shininess, which are passed to shaders.

*   **Lighting and Shadows**
    *   `Light.h/cpp`: (Base class) Defines common attributes for lights, such as color and intensity.
    *   `DirectionalLight.h/cpp`: A light source that simulates a distant source (like the sun) with parallel rays.
    *   `PointLight.h/cpp`: A light that emanates from a single point in all directions.
    *   `SpotLight.h/cpp`: A cone-shaped light, like a flashlight.
    *   `ShadowMap.h/cpp`: (Base class) Manages a Framebuffer Object (FBO) used to render a scene's depth information from a light's perspective.
    *   `DirectionalShadowMap.h/cpp`: A subclass for handling standard 2D shadow maps used by directional lights.
    *   `OmniShadowMap.h/cpp`: A subclass for handling omnidirectional (cubemap) shadow maps used by point and spot lights.

*   **Utilities**
    *   `CommonValues.h`: A header for storing globally used constants and enums.
    *   `Utils.h/cpp`: Contains miscellaneous helper functions, such as the one for calculating average normals.

## Controls

*   **W, A, S, D**: Move Camera (Forward, Left, Backward, Right)
*   **SPACE, CONTROL**: Move Camera (Up, Down)
*   **SHIFT**: Speed Up
*   **Mouse Movement**: Look around (Pitch and Yaw)
*   **Escape**: Close the application

## Setup and Compilation

1. Clone the repository.
2. Ensure you have a C++ compiler and an IDE (like Visual Studio) installed.
3. Make sure the dependency libraries (GLFW, GLEW, GLM, Assimp) are correctly linked in your project settings.
4. Ensure the working directory is set to the project root so the executable can locate the `Shaders/`, `Textures/`, and `Models/` directories.
5. Build and run.

## Acknowledgements

This project was built as part of an OpenGL learning path.
