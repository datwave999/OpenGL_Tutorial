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

*   `main.cpp`: Entry point, main game loop, and render passes.
*   `Shader.h/cpp`: Shader compilation, linking, and uniform management.
*   `Window.h/cpp`: GLFW window setup and input callbacks.
*   `Camera.h/cpp`: View matrix calculation and movement logic.
*   `Mesh.h/cpp` / `Model.h/cpp`: 3D geometry generation and Assimp model loading.
*   `Texture.h/cpp`: Loading and binding 2D image textures.
*   `*Light.h/cpp`: Implementations for Directional, Point, and Spot lights.
*   `*ShadowMap.h/cpp`: Framebuffer and depth texture management for shadows.
*   `Skybox.h/cpp`: Cube map loading and rendering.

## Controls

*   **W, A, S, D**: Move Camera (Forward, Left, Backward, Right)
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