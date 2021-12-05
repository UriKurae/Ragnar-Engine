# Ragnar Engine

Ragnar Engine is a Video Game Engine created by [Lucas Pérez](https://github.com/LucasPG14). and [Oriol Bernal](https://github.com/UriKurae)., two students from the university Universitat Politècnica de Catalunya (CITM). 

## Planning

The purpose of this project is to create a friendly-user, well optimized Game Engine for the subject "VideoGame Engines" from the 3rd year. The short term plan is to implement the basic features of a Game Engine, such as loading models, working in a 3D environment, etc...

For the long term, we would love to create a game with this Video Game Engine

## Installation

The installation is pretty straight-forward and easy, just go into the [releases tab](https://github.com/UriKurae/Ragnar-Engine/releases), and download the latest version.
The download files include an exe and you just have to execute it to test our engine.

**Carefull**: You can edit the config file to modify some properties like the window and everything, but they come with an optimal configuration already.

## Features

v0.8

- Stencil border for the selected GameObject.
- Raycasting objects to select them (MousePicking).
- MousePicking detects the closest target that is hit.
- Resource manager that manages all kinds of resources, with a cool design.
  - All files are shown in the resource manager.
  - You can import any asset you need in it.
  - For better user experience, the resource has icons for models, folders, textures...
- Play/Stop/Pause and Advance next frame for game mode.
- Own file formats, hidden in the Engine's background.
  - They are saved as .rgtexture, .rgmesh and .rgmodel.
  - The scenes are saved as .ragnar.
  - The user will see the file formats as the ones he/she imported (png, jpg...).
- Selected GameObjects have Guizmos for better user experience.
- Imported texture and model options and filters (Alienify, flip UVs, Gaussian Blur...)
- Game clock and Engine clock.
- Quadtree implementation for better optimization.
- Camera system (Camera component, FOV, planes, etc...).
- Mini viewer to see what the game camera is looking at in the editor view.
  - Must have the camera selected, and it will be show in the bottom right corner of the editor viewport. 
- Different viewports (Editor View and Game View).
- Correct serialization and load methods for the engine.
- Frustum Culling for the cameras.
- AABB system for all the GameObjects.
- Shortcuts and explorer viewer for save, load and open files for better user experience.

v0.5

- Camera movement (Rotate, Move, Orbit, Focus).
- Drag and drop textures and models.
- Load and save the configuration.
- GameObject and components system (Transform component, Mesh Renderer and Material Component).
- GameObject options (Rename, activate, move in the hierarchy, delete).
- Inspector information (Transform, mesh renderer, material).
- Own basic shapes (Pyramid, Sphere, Cube and Cylinder).
- Saving images to DDS for lower size.
- Beautiful and usable UX (provided by ImGui).
- Debug options for GameObjects (Normals, Normal length, Normal color, etc...).
- System information (Console, Hardware, Software, etc...).
- Opengl options (Wireframe Mode, Texture2D, Vsync, Stencil, etc...).
- Ability to close and reopen menus from the editor.

## Controls

- Right click + WASD: Move the camera in it's own axis.
- Right click + Q/E: Move the camera up or down in it's own axis.
- Mouse scroll wheel: Move the camera forward and backwards in it's own axis.
- While selecting a GameObject and holding Alt + left click: Orbit the camera around the GameObject.
- While selecting a GameObject and pressing F: Focus the GameObject.
- Key T: Reset the camera (World axis reset).
- Right click GameObject in the hierarchy: Open GameObject options.
- Supr while a GameObject is selected: Delete the GameObject.
- Supr while a resource is selected: Delete the resource.
- Hold left click in the hierarchy menu: Move GameObjects in the hierarchy (Reparenting).
- W,E,R buttons: show Move, Scale, Rotate guizmos. 
- Ctrl + N: Generate a new scene (You will be warned to save your current project).
- Ctrl + O: Opens the explorer window to search for a file and open it.
- Ctrl + S: Saves the current Scene.
- Ctrl + Shift + S: Opens the explorer window to save the scene in a desired location.
- Ctrl + D: Duplicates the selected game object.

- Play/Stop/Pause and Advance frame: To start a game preview, hit the play button, when you hit the play button, the preview will start, and the play button will be swapped by
the stop button. Hitting the stop button will stop the game preview.
Pause button will pause the game preview until you hit it again. When having the game in pause, you can advance one frame if you want. Also, the buttons and the whole engine will have different UI colors to let the user know that he is in game preview mode.

## User Interface

The menus are also very straight-forward, you can navigate through them and have multiple options for the engine itself, there are many options, so it's highly encouraged to test everything to understand the interface, **but keep in mind the following**:
- You can close any tab from the user interface, but don't worry, as you can re-open any window at any time.
- You can click and hold any tab to reallocate it anywhere you want.


## Game Engine Developers

- [Lucas Pérez García](https://github.com/LucasPG14).
- [Oriol Bernal Martínez](https://github.com/UriKurae).

## License

**Copyright (c) Lucas Pérez and Oriol Bernal. License Under the MIT License.**

- [MIT License link to check more details about it.](https://opensource.org/licenses/MIT)
