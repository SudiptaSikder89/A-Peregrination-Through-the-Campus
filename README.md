# A-Peregrination-Through-the-Campus
 
Group Name: RASTERIA
Project Name: A Peregrination Through the Campus
GROUP MEMBERS:            Name               Student ID
                      Sudipta Sikder   _   ID - 233009212
                      Satyam Dhar      _   ID - 233016112
                      Rohan Sharma     _   ID - 233016012

A 2D animated campus environment developed using **C++ with OpenGL and GLUT**. The project combines buildings, roads, a river, vehicles, aircraft, a cruise ship, trees, clouds, a rainbow, characters, lighting effects, and an interactive Day/Night system into one animated scene.

## Project Overview

**A Peregrination Through the campus ** demonstrates fundamental computer graphics concepts through a continuously animated 2D environment.

The scene contains:

- Campus and architectural buildings
- Roads, sidewalks, and street lamps
- An animated river with water waves
- Moving school and campus buses
- A moving aeroplane and helicopter
- A rotating helicopter rotor
- A moving cruise ship with smoke
- Trees, bushes, clouds, and a rainbow
- Sun and moon
- Twinkling stars
- A boy flying a kite
- Boys playing football
- A nighttime person walking a dog
- Day Mode, Night Mode, and Automatic Day/Night Cycle

## Key Features

### Day & Night Mode

The project supports three time modes:

- **Day Mode** — bright sky, sun, colorful surroundings, and daytime activities
- **Night Mode** — dark sky, moon, stars, illuminated windows, and nighttime lighting
- **Automatic Cycle** — smoothly transitions between day and night

The transition is controlled using `nightFactor` and interpolation.

### Animation

The `timer()` function continuously updates the scene at approximately 60 FPS.

Animated elements include:

- School bus: right to left
- Campus buses: continuous road movement
- Aeroplane: left to right
- Helicopter: continuous movement with rotating rotor
- Cruise ship: continuous river movement
- Clouds: horizontal movement
- River waves: animated
- Ship smoke: animated
- Stars: twinkling
- Football: animated
- Kite: animated
- Night dog walker: left to right

## Interactive Controls

| Input | Action |
|---|---|
| `D` / `d` | Switch to Day Mode |
| `N` / `n` | Switch to Night Mode |
| `T` / `t` | Toggle Automatic Day/Night Cycle |
| `Space` | Toggle between Day and Night |
| Left Mouse Click | Toggle between Day and Night |
| `ESC` | Exit the application |

## Scene Components

### Architecture

The scene includes several stylized buildings such as:

- EDU NOVAS
- EDIS
- FOOD COURT
- EDU campus building
- Additional campus structures and decorative elements

### Transportation

The road system includes:

- Multiple traffic lanes
- Lane markings
- Sidewalk
- Street lamps
- Yellow school bus
- EDU campus buses

### River

The river contains:

- Gradient-style water
- Animated horizontal waves
- Nighttime reflections
- Moving cruise ship

### Sky

The sky contains:

- Sun in Day Mode
- Moon in Night Mode
- Twinkling stars
- Moving clouds
- Rainbow
- Aeroplane
- Helicopter
- Nighttime helicopter spotlight

### Characters and Activities

The project includes:

- Boy flying a kite
- Boys playing football
- Person walking a dog at night

## Technologies Used

- **C++**
- **OpenGL**
- **GLUT / FreeGLUT**
- **GLU**
- **Windows OpenGL environment**

## OpenGL Concepts Demonstrated

### 2D Orthographic Projection

The project uses:

```cpp
gluOrtho2D(0.0, 1200.0, 0.0, 600.0);
```

This creates a 2D coordinate system of 1200 × 600 units.

### Geometric Primitives

The scene is constructed using OpenGL primitives including:

- `GL_POLYGON`
- `GL_QUADS`
- `GL_LINES`
- `GL_TRIANGLES`
- `GL_LINE_LOOP`

### Transformations

Reusable objects are positioned using transformations such as:

```cpp
glPushMatrix();
glTranslatef(x, y, 0.0f);
...
glPopMatrix();
```

### Alpha Blending

Transparency is enabled using:

```cpp
glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
```

This is used for effects such as helicopter spotlights, reflections, and other transparent elements.

### Timer-Based Animation

Animation is controlled using:

```cpp
glutTimerFunc(16, timer, 0);
```

which corresponds to approximately 60 frames per second.

## Day/Night Implementation

The automatic cycle uses:

```cpp
targetNight = (sinf(cycleTimer) + 1.0f) * 0.5f;
```

The visual transition is smoothed with:

```cpp
nightFactor += (targetNight - nightFactor) * 0.06f;
```

This allows scene colors and lighting effects to transition gradually.

The Day/Night system affects:

- Sky
- Windows
- Water
- Sun/Moon
- Stars
- Street lighting
- Vehicle lighting
- Ship lighting and reflections
- Nighttime activities

## Main Program Structure

```text
main()
 ├── init()
 ├── display()
 │    ├── Sky
 │    ├── Stars
 │    ├── Sun / Moon
 │    ├── Rainbow
 │    ├── Aeroplane
 │    ├── Helicopter
 │    ├── Clouds
 │    ├── Buildings
 │    ├── Trees / Bushes
 │    ├── Characters
 │    ├── Road / River
 │    ├── Street Lamps
 │    ├── Cruise Ship
 │    └── Vehicles
 │
 ├── reshape()
 ├── keyboard()
 ├── mouse()
 └── timer()
      └── Updates animation variables
```

## Important Functions

| Function | Purpose |
|---|---|
| `display()` | Draws the complete scene |
| `timer()` | Updates animation variables |
| `reshape()` | Handles viewport and projection |
| `init()` | Initializes OpenGL |
| `keyboard()` | Handles keyboard input |
| `mouse()` | Handles mouse input |
| `drawRoadAndRiver()` | Draws road, sidewalk, and river |
| `drawSchoolBus()` | Draws the yellow school bus |
| `drawRealisticShip()` | Draws and animates the cruise ship |
| `drawStars()` | Draws stars |
| `drawSunAndMoon()` | Draws the sun and moon |
| `drawRainbow()` | Draws the rainbow |
| `drawCloud()` | Draws clouds |
| `drawStreetLamps()` | Draws street lamps |
| `drawBoysPlayingFootball()` | Draws football activity |
| `drawNightImage2DogWalker()` | Draws the nighttime dog walker |

The project also uses reusable helper functions for rectangles, circles, ellipses, borders, and other shapes.

## How to Run

### Requirements

- C++ compiler
- OpenGL
- GLUT or FreeGLUT
- Windows environment
- IDE such as Code::Blocks or Visual Studio

### Steps

1. Install/configure a C++ compiler.
2. Install OpenGL and GLUT/FreeGLUT.
3. Open the project in your IDE.
4. Add the project source file(s).
5. Link the required OpenGL and GLUT libraries.
6. Build the project.
7. Run the executable.

The application window is titled:

```text
Interactive 2D City Architecture (Day & Night Mode)
```

## Screenshots

### Day Mode

![Day Mode](screenshots/day-mode.png)

### Night Mode

![Night Mode](screenshots/night-mode.png)

> Put the two screenshots inside a folder named `screenshots` and use the exact filenames `day-mode.png` and `night-mode.png`.

## Recommended Repository Structure

```text
A-Peregrination-Through-the-Countryside/
│
├── README.md
├── main.cpp
│
├── screenshots/
│   ├── day-mode.png
│   └── night-mode.png
│
└── .gitignore
```

If your source code contains multiple `.cpp` or `.h` files, include them according to your actual project structure.

## Learning Objectives

This project demonstrates practical understanding of:

1. 2D computer graphics using OpenGL
2. Orthographic projection
3. Geometric primitives
4. Coordinate systems
5. Object transformations
6. Animation using timer callbacks
7. Color interpolation
8. Alpha blending
9. Keyboard and mouse interaction
10. Modular graphics programming
11. Creating an interactive Day/Night environment

## Future Improvements

Possible future extensions include:

- More detailed countryside elements
- Additional vehicles and pedestrians
- Traffic-light animation
- Weather effects such as rain
- More advanced lighting
- Interactive camera movement
- Sound effects
- More realistic water reflections
- Collision detection
- Additional environmental animations

## Author

| Name | Student ID |
|---|---|
| Sudipta Sikder | 233009212 |
| Satyam Dhar | 233016112 |
| Rohan Sharma | 233016012 |

**Department:** Computer Science and Engineering  
**Institution:** East Delta University  
**Course:** Computer Graphics  
**Project:** A Peregrination Through the Countryside

## License

This project is developed for **educational and academic purposes**.                      
