# Quadris

Like Tetris, but turn-based and with extra features!

The game features:

* A text-based interface with no time control
* A help message that tells you what you can do in the game
* Exit keyword to exit gracefully
* 4 levels, numbered 0 through 4, with increasing difficulty
* The ability to define your own commands (macros)
* The ability to switch between pseudorandom block selection and using a predefined sequence
* The ability to change levels at any time
* Command shortcuts and multipliers
* Hints to suggest to you a possibly good drop location
* The ability to run a file as a series of commands, like a script
* The ability to cheat by changing the current block to another type

Modders will enjoy additional design features:

* Easy mechanism for adding new levels: just define a new `Level` subclass, add an edge in the level graph, and implement the appropriate pass-offs
* Almost everything is contained in a `Session` object, so you can run multiple sessions if you like
* The ability to attach your own `DisplayManager`, if you have an alternate one you want to use

Our design document can be found in the `design` folder.

## How to install

Go into the game directory from your terminal, and run `make`. This should produce an executable named `quadris`, which you can run.

We primarily support Linux, but it should work on any platform if you have the right tools to compile it.
