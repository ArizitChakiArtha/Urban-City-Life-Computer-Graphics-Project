# Urban City Life — Computer Graphics Project

A 2D animated city-life simulation developed in **C++** using **OpenGL** and **GLUT/freeglut**. The project presents five connected urban environments with moving vehicles, environmental effects, day–night transitions, and interactive keyboard controls.

The application automatically cycles through the scenes while also allowing users to switch scenes manually and control selected animations.

---

## Project Overview

**Urban City Life** visualizes different moments and environments in a modern city through five animated scenes:

| Scene | Environment | Main Elements |
|---|---|---|
| `1` | Riverside Metro Area | Metro train, pond, moving boats, buildings, trees, kite, person, and day/night mode |
| `2` | Urban Road and Recreation Area | Cars, ambulance, truck, airplane, traffic signal, playground, university building, football, and Ferris wheel |
| `3` | Sunset-to-Night City | Animated sunset, rising moon, stars, skyline lights, moving cars, water reflections, birds, clouds, and a silhouette boat |
| `4` | Rainy City Street | Rainfall, lightning, wet roads, puddles, streetlights, umbrella pedestrians, cars, dark clouds, distant spire, and a dog |
| `5` | Night Park | Park lake, fountain, lamps, fireflies, swing, gazebo, walkers, dog walker, owl, shooting star, picnic table, and trees |

Each scene is rendered using OpenGL primitives such as polygons, quads, triangles, lines, and circles.

---

## Key Features

- Five animated urban-life scenes in a single application
- Automatic scene cycling approximately every **12 seconds**
- Manual scene navigation using number keys
- Day and night mode switching
- Animated vehicles, metro train, boats, airplane, birds, clouds, and pedestrians
- Traffic light and motion pause/resume interaction
- Weather animation with rain, puddle ripples, and lightning
- Sunset-to-night transition with city-light effects and reflections
- Night-park ambience with fireflies, fountain, lamps, swing, owl, and shooting star
- Kite movement using arrow keys
- Double-buffered rendering for smoother animation

---

## Controls

| Key | Action |
|---|---|
| `1` | Open Scene 1: Riverside Metro Area |
| `2` | Open Scene 2: Urban Road and Recreation Area |
| `3` | Open Scene 3: Sunset-to-Night City and restart the transition |
| `4` | Open Scene 4: Rainy City Street |
| `5` | Open Scene 5: Night Park |
| `S` | Stop or resume the metro and road vehicles; switch the traffic signal between red and green |
| `N` | Toggle day/night mode |
| `P` | Start or stop the Ferris wheel |
| `B` | Move the football between the players |
| `F` | Turn the park fountain on or off |
| `L` | Turn the park lamps on or off |
| `←` `→` `↑` `↓` | Move the kite in Scene 1 |

> Keyboard letters are handled as lowercase characters in the source code. Use lowercase keys when running the application.

---

## Technologies Used

- **Language:** C++
- **Graphics API:** OpenGL
- **Windowing and input:** GLUT or freeglut
- **Rendering style:** Immediate-mode 2D graphics
- **Projection:** Orthographic projection using `gluOrtho2D(0, 1000, 0, 600)`
- **Animation:** Timer-based updates at approximately 16 ms intervals

---

## Requirements

Install the following before building the project:

- A C++ compiler with C++11 support or newer
- OpenGL development libraries
- GLU
- GLUT or freeglut

The supplied source includes:

```cpp
#include <windows.h>
```

This header is intended for Windows. Remove or comment it out when compiling on Linux or macOS.

---

## Getting Started

### 1. Prepare the Source File

The uploaded source is provided as a text file. Rename it to `main.cpp` before compiling:

```bash
mv Urban-City-Life-Computer-Graphics-Project.txt main.cpp
```

On Windows, rename the file manually in File Explorer or from Command Prompt:

```bat
ren Urban-City-Life-Computer-Graphics-Project.txt main.cpp
```

### 2. Compile and Run

#### Windows with MinGW and freeglut

Ensure that freeglut is installed and configured in your compiler environment. Then run:

```bash
g++ main.cpp -o urban_city_life.exe -lfreeglut -lopengl32 -lglu32
./urban_city_life.exe
```

#### Linux (Ubuntu or Debian)

Install the required packages:

```bash
sudo apt update
sudo apt install build-essential freeglut3-dev
```

Remove or comment out `#include <windows.h>` from `main.cpp`, then compile and run:

```bash
g++ main.cpp -o urban_city_life -lglut -lGLU -lGL
./urban_city_life
```

#### macOS

Remove or comment out `#include <windows.h>`, then compile using the system frameworks:

```bash
clang++ main.cpp -o urban_city_life -framework OpenGL -framework GLUT
./urban_city_life
```

> OpenGL and GLUT support on newer macOS versions may be deprecated. A freeglut-based environment can be used when the system GLUT framework is unavailable.

---

## Running the Project in Code::Blocks

1. Create a new **Empty Project** in Code::Blocks.
2. Add the renamed `main.cpp` file to the project.
3. Open **Project → Build Options → Linker Settings**.
4. Add the required libraries:
   - `freeglut`
   - `opengl32`
   - `glu32`
5. Confirm that the freeglut include and library directories are configured.
6. Build and run the project.

---

## Suggested Project Structure

```text
Urban-City-Life-Computer-Graphics-Project/
├── main.cpp
├── README.md
├── screenshots/
│   ├── scene-1-riverside-metro.png
│   ├── scene-2-urban-road.png
│   ├── scene-3-sunset-city.png
│   ├── scene-4-rainy-street.png
│   └── scene-5-night-park.png
└── LICENSE
```

The `screenshots/` directory and `LICENSE` file are optional additions for a public repository.

---

## How the Animation Works

The program uses GLUT callbacks to manage rendering, keyboard input, resizing, and animation:

```cpp
glutKeyboardFunc(handleKeypress);
glutSpecialFunc(handleSpecialKeypress);
glutReshapeFunc(resize);
glutTimerFunc(0, update, 0);
glutDisplayFunc(display);
glutIdleFunc(idle);
```

The `update()` function runs approximately every 16 ms and updates moving objects, transitions, weather effects, and scene cycling. The `display()` function renders one of the five scenes based on the value of `currentScene`.

---

## Main Technical Components

- **Reusable drawing helpers:** circles, buildings, trees, boats, cars, streetlights, people, and park objects
- **Scene management:** a `switch` statement selects the active scene
- **Automatic progression:** `sceneTimer` changes the scene after a defined duration
- **Interactive input:** keyboard and arrow-key callbacks modify animation states
- **State-based visuals:** boolean variables control night mode, traffic state, Ferris-wheel rotation, lamps, and fountain effects
- **Procedural effects:** rain positions, firefly motion, blinking lights, reflections, and cloud movement are updated continuously

---

## Troubleshooting

### `GL/glut.h: No such file or directory`

Install GLUT or freeglut development libraries and confirm that your compiler can access their include path.

### Linker errors such as `undefined reference to glutInit`

Add the correct OpenGL, GLU, and GLUT/freeglut libraries to the linker command or IDE configuration.

### `windows.h: No such file or directory`

Remove or comment out this line when compiling on Linux or macOS:

```cpp
#include <windows.h>
```

### The window opens but some controls do not respond

Use lowercase keyboard letters: `s`, `n`, `p`, `b`, `f`, and `l`.

---

## Future Improvements

Possible extensions include:

- Adding on-screen instructions and scene labels
- Including sound effects for traffic, rain, and park ambience
- Splitting the large source file into reusable header and implementation files
- Adding mouse-based scene navigation
- Providing a menu to control animation speed
- Recording a short demonstration video or adding screenshots to the repository

---

## License

This project is intended for educational and academic use. Add a suitable license file before publishing or redistributing the code.

---

## Acknowledgements

This project demonstrates fundamental computer-graphics concepts through a multi-scene urban simulation built with C++, OpenGL, and GLUT/freeglut.
