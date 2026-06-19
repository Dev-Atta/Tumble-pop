# Tumble Pop

A 2D arcade-style game built in C++, featuring custom sprite-based characters, animation, and sound.

## About

Tumble Pop is a 2D game where the player navigates through obstacles, avoids or captures enemies such as ghosts and skeletons, and collects items along the way.

> Replace this section with an accurate description of your actual gameplay — the goal, win/loss conditions, and core mechanics.

## Features

- Custom sprite-based characters and objects
- Smooth 2D animation and movement
- Background music and sound effects
- Simple, intuitive controls
- Classic arcade-style gameplay

## Preview

(Add a screenshot or GIF of gameplay here once available)

```
![Gameplay Screenshot](Data/screenshot.png)
```

## Built With

- C++ — core game logic
- SFML (or other graphics/audio library) — rendering and sprite handling. Update with whichever library you are using.

## Project Structure

```
Tumble-Pop/
├── code.cpp          Main game source code
├── Data/              Game assets (sprites, audio)
│   ├── player.png
│   ├── player1.png
│   ├── ghostk.png
│   ├── skeletion.png
│   ├── vaccum.png
│   ├── block1.png
│   ├── bag.png
│   ├── bg.png
│   └── mus.ogg
└── pf_project/        Project files
```

## Getting Started

### Prerequisites

- A C++ compiler (e.g. g++, MSVC)
- SFML, or whichever graphics/audio library the project depends on

### Build and Run

```bash
g++ code.cpp -o TumblePop -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
./TumblePop
```

Update the build command above to match your actual compiler setup and linked libraries.

## Controls

| Key   | Action            |
|-------|-------------------|
| Left/Right Arrow | Move left / right |
| Space | Jump / Action     |
| Esc   | Pause / Quit      |

Update this table with the game's actual controls.
