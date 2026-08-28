# A-Peregrination-Through-the-Campus
 
Group Name: RASTERIA
Project Name: A Peregrination Through the Campus
GROUP MEMBERS:            Name               Student ID
                      Sudipta Sikder   _   ID - 233009212
                      Satyam Dhar      _   ID - 233016112
                      Rohan Sharma     _   ID - 233016012

## Overview
A peregrination through the campus is a C++ OpenGL project that creates an animated 2D city and campus environment. The scene contains buildings, roads, a river, trees, vehicles, aircraft, people, and other objects with both Day and Night modes.
The main purpose of this project is to demonstrate basic Computer Graphics concepts such as 2D drawing, geometric shapes, animation, transformations, color blending, keyboard and mouse interaction, and OpenGL rendering.
________________________________________
## Project Features
•	Day and Night mode
•	Automatic Day/Night cycle
•	Smooth Day/Night transition
•	Moving clouds
•	Moving buses
•	Moving school bus
•	Moving aeroplane
•	Moving helicopter
•	Rotating helicopter rotor
•	Moving cruise ship
•	Animated water
•	Animated ship smoke
•	Animated kite
•	Football passing animation
•	Night-time dog walker animation
•	Stars with twinkling effect
•	Street lamps and night lighting
•	Campus and city buildings
•	Trees and bushes
•	Road, sidewalk, and river
•	Keyboard controls
•	Mouse control
The main display() function draws the scene in separate stages, including the sky, aircraft, clouds, buildings, landscape, characters, road, river, ship, and buses.
________________________________________
## Technologies Used
•	C++
•	OpenGL
•	GLUT / FreeGLUT
•	GLU
•	MinGW / G++
Header Files
#include <windows.h>
#include <GL/glut.h>
#include <cmath>
#include <string>
#include <vector>
The program uses OpenGL and GLUT for graphics and window management, while cmath is used for mathematical calculations such as sine and cosine.
________________________________________
## Installation and Setup
There are two ways to set up and run this project.
## Option 1: MinGW / G++ Command Line
This is the simplest method if MinGW and FreeGLUT are already configured.
Step 1: Install MinGW
Install a C++ compiler that provides g++.
Check whether G++ is installed:
g++ --version
If the command works, G++ is available.
Step 2: Install FreeGLUT
Make sure FreeGLUT is installed and its files are available to your compiler.
You normally need:
include/
    GL/
        freeglut.h
        glut.h

lib/
    libfreeglut.a
    libopengl32.a
    libglu32.a
The exact folder structure can be different depending on how FreeGLUT was installed.
Step 3: Open the Project Folder
Open a terminal inside the folder containing:
main.cpp
Step 4: Compile
g++ main.cpp -o city_scene.exe -lfreeglut -lopengl32 -lglu32
Step 5: Run
city_scene.exe
________________________________________
## Option 2: 

Step 1: 
Openglportable-64bit Zip file Extract 
Step 2: 
Openglportable-64bit > Openglportable 
Step 3: 
Openglportable.cbp file Drag and drop in Codeblocks.

Build and Run
Compilation
Using G++:
g++ main.cpp -o city_scene.exe -lfreeglut -lopengl32 -lglu32
Execution
city_scene.exe
The program creates an OpenGL window with the title:
Interactive 2D City Architecture (Day & Night Mode)
The window is initialized with a width of 1200 and a height of 600.
________________________________________
## Controls
Key / Input	Action
D or d	Switch to Day
N or n	Switch to Night
Space	Toggle Day/Night
T or t	Toggle automatic Day/Night cycle
Left Mouse Button	Toggle Day/Night
ESC	Exit the program
The keyboard input is handled inside the keyboard() function. The mouse input is handled inside the mouse() function.
________________________________________
## Main Program Structure
The program is written mainly inside main.cpp. The code is divided into functions so that different parts of the scene can be drawn and controlled separately.
main.cpp

Basic Drawing Functions
    drawRect()
    drawRectBorder()
    drawCircle()
    drawEllipse()
    drawTriangle()
    drawText()

Initialization
    initStars()
    init()

Environment
    drawSky()
    drawStars()
    drawSunAndMoon()
    drawRainbow()
    drawCloud()
    drawStreetLamps()
    drawRoadAndRiver()

Buildings
    drawLeftCampusBuildings()
    drawMidApartmentBuilding()
    drawFoodCourtBuilding()
    drawAccurateEDUCampusBuilding()

Trees and Plants
    drawLargeTree()
    drawPineTree()
    drawGoldPineTree()
    drawFloweringBush()
    drawRoundBerryBush()

Characters
    drawBoyFlyingKite()
    drawBoysPlayingFootball()
    drawNightImage2DogWalker()

Vehicles
    drawBus()
    drawSchoolBus()
    drawRealisticShip()
    drawExactPictureAeroplane()
    drawHelicopter()

Animation
    timer()

User Input
    keyboard()
    mouse()

OpenGL Window
    reshape()
    display()
    main()
The function prototypes in the source code show this organization clearly.
________________________________________
## Important Functions
## 1. main()
main() is the starting point of the program.
It performs the basic OpenGL and GLUT setup:
•	Initializes GLUT
•	Sets the display mode
•	Creates the window
•	Sets the window size
•	Calls init()
•	Registers callback functions
•	Starts the GLUT main loop
glutDisplayFunc(display);
glutReshapeFunc(reshape);
glutKeyboardFunc(keyboard);
glutMouseFunc(mouse);
glutTimerFunc(0, timer, 0);

glutMainLoop();
This means GLUT continuously manages drawing, input, resizing, and animation after the program starts.
________________________________________
## 2. init()
The init() function prepares the OpenGL environment.
It:
•	Sets the background color
•	Sets the 2D projection
•	Enables blending
•	Sets the blending mode
•	Initializes the stars
glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
Blending is important for transparent objects and effects such as night lighting and stars.
________________________________________
## 3. reshape()
reshape() is called when the OpenGL window is resized.
It changes the viewport and resets the projection:
glViewport(0, 0, w, h);
The project uses:
gluOrtho2D(0.0, 1200.0, 0.0, 600.0);
This creates a 2D coordinate system where:
X = 0 to 1200
Y = 0 to 600
This makes it easier to place objects using normal 2D coordinates.
________________________________________
## 4. display()
display() is the main drawing function.
It clears the previous frame and then draws the complete scene.
The drawing order is important because objects drawn later can appear in front of objects drawn earlier.
The function draws:
•	Sky
•	Stars
•	Sun/Moon
•	Rainbow
•	Aeroplane
•	Helicopter
•	Clouds
•	Buildings
•	Trees
•	Characters
•	Road
•	River
•	Street lamps
•	Cruise ship
•	Buses
Finally, it calls:
glutSwapBuffers();
This is used for double buffering so the animation can appear smoother.
________________________________________
## Basic Drawing Functions
These functions are used as building blocks for almost every object in the scene.
drawRect()
Draws a rectangle using four vertices.
drawRect(x1, y1, x2, y2, r, g, b, a);
It uses:
GL_QUADS
This function is useful for:
•	Buildings
•	Windows
•	Roads
•	Bus bodies
•	Doors
•	Walls
•	Other rectangular objects
The function sets the color and creates four vertices to form the rectangle.
________________________________________
drawRectBorder()
Draws a filled rectangle and then adds an outline around it.
It first calls drawRect() and then uses:
GL_LINE_LOOP
This is useful for objects such as windows, doors, signs, and other shapes that need borders.
________________________________________
drawCircle()
Draws a circle using multiple points around its circumference.
The position of each point is calculated using:
cos()
sin()
The more segments used, the smoother the circle appears.
x = cx + radius * cos(theta);
y = cy + radius * sin(theta);
This function is used for:
•	Sun
•	Moon
•	Wheels
•	Stars
•	Heads
•	Small details
________________________________________
drawEllipse()
Works similarly to drawCircle(), but allows different horizontal and vertical sizes.
drawEllipse(cx, cy, rx, ry, ...);
Here:
•	cx, cy are the center
•	rx is the horizontal radius
•	ry is the vertical radius
It is useful for:
•	Clouds
•	Wheels
•	Water effects
•	Body parts
•	Smoke
•	Other rounded shapes
________________________________________
drawTriangle()
Creates a triangle using three points.
Triangles are useful for creating:
•	Roofs
•	Wings
•	Tail sections
•	Small decorative shapes
•	Character details
The function uses:
GL_TRIANGLES
________________________________________
drawText()
Displays text inside the OpenGL window.
It sets the raster position and then displays each character using:
glutBitmapCharacter()
________________________________________
Environment Functions
drawSky()
Creates the background sky.
The color changes depending on nightFactor.
The function uses interpolation between daytime and nighttime colors:
lerp(dayColor, nightColor, nightFactor)
This allows the sky to gradually change instead of changing instantly.
________________________________________
drawStars()
Draws the stars when the scene becomes dark.
The function uses the starTwinkle value to change the transparency of the stars over time.
This creates a simple twinkling effect.
________________________________________
initStars()
Creates the initial information for the stars.
There are:
const int NUM_STARS = 60;
Each star stores:
•	X position
•	Y position
•	Size
•	Speed
•	Phase
________________________________________
drawRoadAndRiver()
Draws the road, sidewalk, and river.
The river also contains moving wave lines. The wave positions are calculated using the waterWave animation value.
________________________________________
## Building Functions
The project contains separate functions for different buildings.
drawLeftCampusBuildings()
drawMidApartmentBuilding()
drawFoodCourtBuilding()
drawAccurateEDUCampusBuilding()
Keeping buildings in separate functions makes the code easier to understand and modify.
For example, drawLeftCampusBuildings() creates the main building using rectangles and windows. The building colors also change according to the Day/Night state.
________________________________________
## Character Functions
drawBoyFlyingKite()
Creates the boy and kite scene used during the daytime.
The character is constructed from simple shapes such as rectangles, circles, lines, and triangles.
The kite also contains animation values so that it can move slightly during the animation.
________________________________________
drawBoysPlayingFootball()
Creates two boys and a football.
The football position is calculated using a sine function:
float passProgress = (sinf(footballAnim) + 1.0f) * 0.5f;
This creates a smooth movement between the two players.
________________________________________
drawNightImage2DogWalker()
Creates the person walking a dog.
The function only becomes visible when the scene is sufficiently dark:
if (nightFactor < 0.15f) return;
The walking animation uses walkAnim to move the legs and the dog's legs.
________________________________________
Vehicle Functions
drawBus()
Creates the EDU buses.
Different color values can be passed to the function, allowing the same drawing function to create buses with different colors.
________________________________________
drawSchoolBus()
Creates the yellow school bus.
Its position is controlled by yellowBusX.
________________________________________
drawRealisticShip()
Creates the cruise ship and includes additional details such as smoke and night lighting.
The ship moves horizontally using the shipX variable.
________________________________________
drawExactPictureAeroplane()
Creates the aeroplane using multiple OpenGL shapes.
The aeroplane contains several parts such as:
•	Fuselage
•	Wings
•	Tail
•	Cockpit
•	Details and lights
The aircraft position is controlled by planeX.
________________________________________
drawHelicopter()
Creates the helicopter.
The helicopter moves horizontally, while heliRotor is continuously updated to create the rotor animation.
________________________________________
## Animation Function
timer()
timer() is one of the most important functions in the project.
It updates the values that control movement and animation.
For example:
busBaseX
yellowBusX
planeX
heliX
shipX
footballAnim
dogWalkerX
waterWave
smokeAnim
kiteAnim
cloudOffset
The timer runs approximately every 16 milliseconds.
glutPostRedisplay();
glutTimerFunc(16, timer, 0);
This tells OpenGL to redraw the scene and then run the timer again.
________________________________________
## Day and Night System
The Day/Night system uses three important variables:
float nightFactor;
float targetNight;
bool autoCycle;
nightFactor represents the current amount of nighttime:
0.0 = Day
1.0 = Night
targetNight represents the desired Day/Night state.
When automatic cycling is enabled, the program calculates:
targetNight = (sinf(cycleTimer) + 1.0f) * 0.5f;
Then the current value moves gradually toward the target:
nightFactor += (targetNight - nightFactor) * 0.06f;
This creates a smooth transition between Day and Night.
________________________________________
OpenGL Concepts Demonstrated
## 1. 2D Orthographic Projection
The project uses:
gluOrtho2D(0.0, 1200.0, 0.0, 600.0);
This creates a 2D coordinate system.
Unlike perspective projection, objects do not become smaller simply because they are placed farther away.
This is useful for creating a 2D scene where objects can be positioned directly using X and Y coordinates.
________________________________________
## 2. Geometric Shapes
The scene is created from basic geometric shapes.
The main shapes include:
Rectangle
Circle
Ellipse
Triangle
Line
Polygon
Complex objects such as buses, buildings, people, and aircraft are created by combining these simple shapes.
________________________________________
## 3. Color
OpenGL color is set using functions such as:
glColor3f()
glColor4f()
The values normally range from:
0.0 to 1.0
For example:
glColor3f(1.0f, 0.0f, 0.0f);
represents red.
The fourth value in glColor4f() is the alpha value, which controls transparency.
________________________________________
## 4. Alpha Blending
The program enables blending using:
glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
This allows transparent effects to be created.
The project uses this for things such as:
•	Stars
•	Night lights
•	Aircraft lights
•	Water reflections
•	Other transparent effects
________________________________________
## 5. Animation
Animation is created by changing object position or animation variables over time.
For example:
planeX += 2.6f;
moves the aeroplane horizontally.
When it reaches the end of the screen, its position is reset:
if (planeX > 1320.0f)
    planeX = -160.0f;
This makes the aeroplane continuously travel across the screen.
________________________________________
## 6. Sine and Cosine Functions
The project uses mathematical functions such as:
sinf()
cosf()
These are useful for creating smooth or circular movement.
Examples include:
•	Football movement
•	Water waves
•	Star twinkling
•	Kite movement
•	Walking animation
•	Smoke movement
________________________________________
## 7. Translation
The project uses:
glTranslatef()
to move an object's local drawing position.
For example, a character can be created around (0, 0) and then moved to another location using translation.
The dog walker uses translation before drawing the character and dog.
________________________________________
## 8. Matrix Stack
The project uses:
glPushMatrix();
glPopMatrix();
These functions save and restore the current transformation.
This is useful when an object contains several smaller parts.
For example, a character can be translated without affecting the position of the next object.
________________________________________
## 9. Double Buffering
The window is created using:
GLUT_DOUBLE | GLUT_RGB
The program then uses:
glutSwapBuffers();
Double buffering helps reduce visible flickering during animation.
10. GLUT Callback Functions
The program uses GLUT callback functions to control different parts of the application.
display()
    Draws the scene

reshape()
    Handles window resizing

keyboard()
    Handles keyboard input

mouse()
    Handles mouse input

timer()
    Updates animation
These functions are registered in main().
________________________________________
## How the Complete Program Works
The program starts from main() and initializes GLUT and OpenGL.
After initialization, the program enters glutMainLoop().
The display callback calls display(), which draws the complete scene.
At the same time, the timer continuously changes animation variables such as vehicle positions, cloud positions, water movement, and character animations.
Whenever the timer updates the values, glutPostRedisplay() tells OpenGL to draw the scene again.
Keyboard and mouse callbacks allow the user to change the Day/Night state while the animation is running.
The overall process can be understood as:
Initialize OpenGL
Create Window
Initialize Scene
Register Callbacks
Run Main Loop
Draw Scene
Update Animation
Handle User Input
Draw Scene Again
________________________________________
## Project Structure
Interactive-2D-City/
│
├── main.cpp
├── README.md
│
└── screenshot.png
Currently, the main source code is contained in main.cpp.


## Screenshots

### Day Mode

![Day Mode](screenshots/day-mode.png)<img width="959" height="503" alt="Screenshot day-mode" src="https://github.com/user-attachments/assets/f078d065-5445-4c00-b38d-01eb294067fd" />


### Night Mode

![Night Mode](screenshots/night-mode.png)<img width="959" height="505" alt="Screenshot night-mode" src="https://github.com/user-attachments/assets/9f4e8152-e7d5-4756-bf91-465691136cf5" />


## Learning Objectives
Through this project, I learned and practiced:
•	Basic C++ programming
•	OpenGL initialization
•	GLUT window management
•	2D coordinate systems
•	Geometric shape drawing
•	Color and transparency
•	Orthographic projection
•	Object positioning
•	Translation
•	Matrix operations
•	Animation using timers
•	Keyboard input
•	Mouse input
•	Basic mathematical animation
•	Sine and cosine based movement
•	Double buffering
•	Organizing a graphics project using functions
________________________________________
## Troubleshooting
OpenGL Header Not Found
If you get:
GL/glut.h: No such file or directory
check that FreeGLUT is installed and that its include directory has been added to the compiler configuration.
FreeGLUT Library Error
If the compiler reports an error related to FreeGLUT, check that the library is installed and linked during compilation.
## Example:
g++ main.cpp -o city_scene.exe -lfreeglut -lopengl32 -lglu32
Program Does Not Start
Check that the required FreeGLUT DLL is available with the executable if your Windows setup requires it.
Also make sure the program was compiled successfully before running:
city_scene.exe
________________________________________
## Future Improvements
Some possible improvements for the project are:
•	Add more buildings
•	Add traffic lights
•	Add more vehicles
•	Add more characters
•	Add rain and weather effects
•	Add sound effects
•	Add camera movement
•	Add more user controls
•	Improve object animations
•	Separate the large main.cpp into multiple source and header files
•	Add a proper project build system
________________________________________


## Author

| Name | Student ID |
|---|---|
| Sudipta Sikder | 233009212 |
| Satyam Dhar | 233016112 |
| Rohan Sharma | 233016012 |

**Department:** Computer Science and Engineering  
**Institution:** East Delta University  
**Course:** Computer Graphics  
**Project:** A Peregrination Through the Campus

## License
This project was created for educational purposes as part of a Computer Graphics project.
                
