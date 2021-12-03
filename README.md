# GhostHouse

## Description

This is a group project done by Haoyu Tan and Ruichen He.

This project extended the previous OpenGL Maze Game. It is a 3D first person shooter adventure game. When the game starts, the player is in an old castle with a gun and a torch in hand. There are also some ghosts in the castle that are waiting to attack the player. The player needs to avoid being attacked by ghosts in the house and find the final treasure. The gun can freeze the ghost for a while. At the same time, if the torch is turned off, the ghost will only detect player in a longer distance comparing with that of when the torch is on. The user control and presentation video attached below will provide more details about this project.

**This is a course project (Spring 2021 Semester), and the original submission page is on: https://sites.google.com/umn.edu/haoyutan/home/final-project-report. However, the submission webpage is for reference only so please still use the code from GitHub. Thanks a lot!**

## Presentation Video

Link: https://www.youtube.com/watch?v=rUeTahx_hps

[![321b61b487700d2a87ac9d09339ded7](https://user-images.githubusercontent.com/35856355/143881077-36ed0669-e705-42a6-9f7b-3f74148b496b.png)
](https://www.youtube.com/watch?v=rUeTahx_hps)

## How to Run the Program

This program is written in C++ with OpenGL and SDL2 so please make sure your computer has SDL library downloaded. There are two ways to run the program

1. There is a **Makefile** in the ./GhostHouse folder. Run the command **make** on terminal to compile. The output executable is called **GhostHouse**. Use the following command on terminal to run the program (map4.txt has the best quality):

        ./GhostHouse ./TestFile/map4.txt 

3. If there make is not available, use the **FinalGame.sln** in the ./GhostHouse folder with some settings:

    (1) Open the .sln file and click on **Properties**

    ![64cdc56594833c4473bb06b01abdec4](https://user-images.githubusercontent.com/35856355/144598519-00025335-5477-41fa-849e-84e3d195f21c.png)

    (2) Click on **VC++Directories** and change the paths of **Include Directories** and **Library Directories** to the SDL2 include and SDL2 lib on your computer. Also change the **Configuration** on the top to **Active(Debug)** and **Platform** to **x64**

    ![a153c444cdc12e074964feeb58da9b8](https://user-images.githubusercontent.com/35856355/144598775-54870698-4c25-43d0-b6bd-4dc75b0e35ec.png)

    (3) Click on **Linker** and then **Input**, added the following dependencies on **Additional Dependencies**

    ![a2acfb2b0d3c04ef5846aa0c70493c4](https://user-images.githubusercontent.com/35856355/144599617-010016b4-6d4a-419d-8088-2452e32a191b.png)

    (4) Click on **System** select **Console** as subsystem
    ![2705d92450b5f1b20c413f2ddf00dc2](https://user-images.githubusercontent.com/35856355/144599862-7bad510a-d64d-4865-ab34-b6068793dc6d.png)

    (5) If the **Command Arguments** under the **Debugging** is empty, add a file inside the ./TestFile folder(map4.txt has the best quality).
    ![18274f1dca887476ab75d5c3c7e231b](https://user-images.githubusercontent.com/35856355/144600159-da6c2c07-81b4-423b-8b6e-b342d0775e63.png)

    (6) Hit apply. Then go back and click on "Local Windows Debugger" to run the program
    ![00741734897ff5b19c63f19d8ba903f](https://user-images.githubusercontent.com/35856355/144600835-c5136168-e3c1-410c-a267-8f597b2933e7.png)

## User Control

-- press '1' on keyboard: show/hide the gun

-- left click on mouse: shooting (required to show the gun on screen by pressing '1' first)

-- right click on mouse: turn the torch on/off

-- 'w', 'a', 's', 'd' on keyboard: move in the scene

-- 'up', 'down', 'left', 'right' on keyboard or mouse movement: rotate the camera

Have fun :)


## Key Features & Images

(1) Animation of NPC white ghost: 

![08b73a97225120c76ae58830095bc49](https://user-images.githubusercontent.com/35856355/139039169-a4d60934-9340-41e2-b376-632b91c8c7ea.png)

(2) Animation of NPC blue ghost: 

![02542525cd33870094fdd132d840cc8](https://user-images.githubusercontent.com/35856355/139039192-5f22f5fe-9bd6-40db-a910-5434142b2e01.png)

(3) UI-blood level and inventory system for bullets and keys

![fp1](https://user-images.githubusercontent.com/35856355/139039224-feb58f42-f373-4426-96a8-23916ea6da04.png)

(4) Key and Immediately shuted door

![8cc5bd00795a48b1f89590d5fdf8fdb](https://user-images.githubusercontent.com/35856355/139039274-647ddbbb-1b49-45a9-b8be-63e3249e76e0.png)

(5) If the player is caught by the white ghost, the player will be noticed by the red light and the player is followed by the ghost.

![01f072dc91ece04a92a6ae2185231ee](https://user-images.githubusercontent.com/35856355/139039336-d674bf97-5ed1-43dd-bb32-257842ff7811.png)

(6) Final treasure

![ad0c1743db1b0b1197d9f0edba49ca5](https://user-images.githubusercontent.com/35856355/139039526-ea6bffa0-b236-4a64-86d5-d034ea726949.png)

## References:

http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/

https://www.patreon.com/quaternius

https://www.youtube.com/watch?v=MKa06EwFuJk
