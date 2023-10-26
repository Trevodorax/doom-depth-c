# Doom depth C

## Introduction
This is a school project in which we aim to create a small version of the game Doom.

Our main concerns are to make it both :
- cross-platform
- extermely clean and well documented
- easy to use and understand

Hope you will enjoy !

## Project setup
### Requirements
- CMake
- vcpkg

### Installation
- Clone the repository
- Use vcpkg to install SDL2 and sqlite3
- Use CMake with vcpkg to build the project

### IDE
This project is structured to be opened in CLion, but you can use any IDE you want, as long as you manage to use vcpgk and CMake.

## Authors
This project is built by a wonderful team of 4 students from ESGI Paris:
- Tom Bourlard
- Noam De Masure
- Anaelle Ea
- Paul Gaudeaux

## Informations about the game
### Monsters
To add monsters, you must add them to the database init script, and add the corresponding image in .png format in the assets folder.

The current monsters list is:
- bat (lvl 1)
- goblin (lvl 2)
- zombie (lvl 3)
- skeleton (lvl 4)
- troll (lvl 5)
- ghost (lvl 6)
- ghoul (lvl 7)
- demon (lvl 8)
- flowey (lvl 9)

The bosses (they are monsters as well, trust us) are:
- M. Sananes
- M. Delon
- M. Demacon
- M. Machavoine

### Maps
Maps are stored in json and are editable this way.

You can create maps by adding them to the maps.json file in the assets folder (which is pretty much self documented).
