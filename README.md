# Synchronized-Animations-OpenGL

## Run Project by Terminal

1. open terminal
2. `g++ -o program Assignment.cpp -lGL -lGLU -lglut`
3. `./program`

## OR Run Project by Geany
1) start up a editor, eg Geany
2) change the build command to `g++ -Wall -o "%e" "%f" -lm -lGL -lGLU -lglut`
3) Compile

## Overview of the Project
![image](https://github.com/MinfangYu/Synchronized-Animations-OpenGL/blob/master/scene.jpg)

This is the whole sense. It is built in the snow mountain(sky box) and has two synchronized animations.
The torus swings left and right, while the sphere will swing back and forth. When the torus reaches its lowest point and the sphere reaches its highest point, the sphere can pass through the middle of the torus. 
The sphere is bounced back and forth by two spring pillars in a circular motion, and the torus is pulled in a circular motion by an inelastic rope hung on the shelf. 
There is a base next to the shelf with a barrel built on it. It detects collisions and automatically launches a bullet every time the sphere hits the spring pillar in front. 
The bullet will make a parabolic motion after firing. The green sphere in the lower right corner of the photo is a bullet moving along the parabola.

The most important thing is that the torus and sphere will never collide. The
sphere can pass through the middle of the torus. They will reach the point shown in
previous two photos at the same time, which is the only point where their respective
circular motions intersect.

I also generate sweep surface besides sky box. It is a barrel and the green bullet will be launched from its muzzle.

## Keyboard Function:
* `Page Up`:  Increase camera height 1.0 in initial camera mode
* `Page Down`: Decrease camera height 1.0 in initial camera mode
* `Up Arrow Key`: Move camera forward
* `Down Arrow Key`: Move camera backward
* `Right Arrow Key`: Turn right some angle
* `Left Arrow Key`: Turn left some angle

## Extra Features:
1. A planar shadow of the pink sphere.
2. A spot light, it is obvious and it will move with the movement of the yellow torus.
3. A parabolic calculation, the bullet will make a parabolic motion after firing.
4. Two circular motion calculations of the sphere and torus.
5. A collision detection that the barrel will automatically launch a bullet every Time the sphere hits the spring pillar in front.
6. A sky box which is shown as snow mountains.
7. Contains CmakeLists.txt file.
