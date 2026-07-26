# DO THE THING

**Do The Thing** is a simple ADHD inspired simulation game where the player tries to complete a laundry task while managing focus, distractions, and overwhelm.

## About

This game gives a simple, interactive impression of how ADHD can make everyday tasks feel more difficult. It also aims to reflect common coping strategies in a light and accessible way.

The player runs through a laundry room, jumps over puddles, and collects laundry before time runs out.

During the game, distractions appear and interrupt the flow. When they become too overwhelming, the player can use a short **Grounding** exercise to calm down and continue. The game also includes **Hyperfocus**, a temporary state that makes the player faster and helps collect more laundry.

## Goal

The goal is to collect enough laundry before the laundry room closes.

The player must manage distractions, avoid obstacles, and use focus-related mechanics to keep going.

## How to Play

* Hold **Right Arrow** to run.
* Press **Up Arrow** to jump.
* Collect laundry to gain points.
* Avoid puddles.
* Fill the **Hyperfocus** bar to move faster and collect more.
* Use **Grounding** when distractions become overwhelming.

## Features

* 2D running and jumping gameplay
* Laundry collection and scoring system
* Puddle obstacles
* Distraction popups
* Hyperfocus power up
* Grounding breathing break
* Countdown timer
* Lives system
* Win and lose screens
* Simple story/introduction screen


## Technologies

* C++
* SFML
* CMake

## Project Structure



src/

│

├── audio/ # SoundPool and sound-related helper classes

├── core/ # Game loop, constants, resource manager, game state interface

├── entities/ # Player class

├── mechanics/ # Hyperfocus, Distraction, Grounding

├── states/ # Start menu, story, playing, grounding, popup states

├── ui/ # Buttons, progress bars, text helpers, timers, effects

└── world/ # Background layers, object spawning, parallax scrolling



assets/         # Images, fonts, and sounds

dll/            # Required runtime DLL files

## How to Build and Run

This project uses **CMake** and **SFML 2.6.2**.

### Requirements

* Git
* CMake
* Visual Studio / MSVC compiler
* SFML 2.6.2

The project expects SFML to be installed here:



C:/Libraries/SFML-2.6.2/

### Download



git clone git@github.com:seraytufan/DoTheThing.git

cd DoTheThing

### Build



cmake -S . -B build

cmake --build build

During the build, CMake automatically copies the required assets folder and the DLL files from the dll folder next to the executable.

### Run



.\\build\\Debug\\DoTheThing.exe

