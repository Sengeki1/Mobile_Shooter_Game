# Shooter Game (Android Studio + AGDK)


<p align="center">
  <img src="https://github.com/user-attachments/assets/f645ae9f-9724-4abb-994e-b284e4064e9d" alt="Game Screenshot">
</p>


## Overview
This is a first-person shooter game developed in **Android Studio** using **Android Game Development Kit (AGDK)**. The project utilizes several key libraries, including:

- **GLAD/EGL** – for window and OpenGL API calls
- **glm** – for mathematical computations
- **Assimp** – for importing 3D models

## Core Features
- **Shooting Mechanic** – Fire projectiles at enemies
- **Player Movement** – Navigate the game world freely
- **Look Around** – Adjust camera orientation
- **3D Model Importing** – Load assets using Assimp
- **Billboarding for AI Enemies** – Ensure enemies always face the player
- **Cube Mapping for Skybox** – Implement realistic sky rendering

## Missing Features / Future Improvements
- **Performance Optimization**
  - Implement **Frustum Culling** to optimize rendering
  - Introduce **Level of Detail (LOD)** for better performance
- **Animations** – Add skeletal and keyframe animations
- **Better Collision Detection** – Improve hit detection for shooting mechanics

## Setup & Requirements
### Prerequisites
- **Android Studio** (Latest version)
- **NDK & AGDK** installed
- **C++ Support enabled**

### Building the Project
1. Clone the repository:
   ```sh
   git clone https://github.com/Sengeki1/Mobile_Shooter_Game.git
   ```
2. Open the project in **Android Studio**
3. Configure **NDK and CMake** in the project settings
4. Build and run the application on an **Android device or emulator**

## Contributions
Contributions are welcome! If you have suggestions or want to improve features, feel free to submit a pull request.

