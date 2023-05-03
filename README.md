# 3D Room Simulator

This program is a simple 3D room simulator, which demonstrates the use of OpenGL and GLUT libraries to render a 3D scene. The room contains a table, chair, lamp, and clock. The user can navigate the room using keyboard controls, and adjust the lighting by toggling two light sources.
Dependencies

To run this program, you'll need the following libraries:

    OpenGL
    GLUT
    GLU

On macOS, you can install the required libraries using Homebrew:

```bash
brew install freeglut
```

For Linux, you can use the package manager for your distribution. For example, on Ubuntu:

```bash
sudo apt-get install freeglut3-dev
```

Compilation

To compile the program, use the following command:

```bash
g++ main.cpp -o room_simulator -lGL -lGLU -lglut
```

Replace main.cpp with the name of your source file if different.
Usage

To run the program, simply execute the compiled binary:

```bash
./room_simulator
```

Controls

    W: Move up
    S: Move down
    A: Move left
    D: Move right
    Q: Move forward
    E: Move backward
    Arrow keys: Rotate the camera
    G: Toggle light source 0
    F: Toggle light source 1
    ESC: Quit the program

The mouse can also be used to control the camera's vertical and horizontal look direction.
Customization

You can customize the room by modifying the source code. For example, you can change the size, position, and color of the objects within the room, or add new objects to the scene.
