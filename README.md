![title](https://raw.githubusercontent.com/matdombrock/EsoBinaria/refs/heads/master/asset_meta/itch-banner.png)

EsoBinaria is a tile-based visual programming game where players solve boolean logic puzzles. 

## [Check it out on Itch.io](https://replicat.itch.io/esobinaria)

![screenshot](https://raw.githubusercontent.com/matdombrock/EsoBinaria/refs/heads/master/asset_meta/screenshot/firstday.png)

![screenshot](https://raw.githubusercontent.com/matdombrock/EsoBinaria/refs/heads/master/asset_meta/screenshot/puzzle.png)

## Technical Features
- Custom minimalist game engine
    - C++ 17
    - SDL2
- Runs natively or in a browser
    - Build for WASM
    - Build for Linux
- Embedded Scheme interpreter
    - Used for verifying puzzle solutions 
- Containerized builds
- Hundreds of unique, algorithmically generated puzzles
    - In fact, this contains every possible puzzle variation

## Tiles

### AND

![and tile](https://raw.githubusercontent.com/matdombrock/EsoBinaria/refs/heads/master/asset_meta/tiles/tile-and.png)

The AND tile resolves true if all it's inputs are true. It can have any amount of inputs but ALL of them must be true for it to resolve true. 

Examples:
```c
A = 1
B = 0
C = 1
(AND A) => 1
(AND A B) => 0
(AND A B C) => 0
```

### OR

![or tile](https://raw.githubusercontent.com/matdombrock/EsoBinaria/refs/heads/master/asset_meta/tiles/tile-or.png)

The OR tile resolves true if any of it's inputs are true. It can have any amount of inputs.

Examples:
```c
A = 1
B = 0
C = 1
(OR A) => 1
(OR A B) => 1
(OR B) => 0
(OR A B C) => 1
```

### NOT

![not tile](https://raw.githubusercontent.com/matdombrock/EsoBinaria/refs/heads/master/asset_meta/tiles/tile-not.png)

This NOT tile inverts the value of its input. It will resolve true when its input is false and it will resolve false when its input is true. This tile can only operate on one input. It is an error to supply it with more than one input. 

Examples:
```c
A = 1
B = 0
C = 1
(NOT A) => 0
(NOT B) => 1
(NOT A C) => ERROR
```

Note: Even though the NOT tile only takes one input it must be wrapped in parenthesis to work. If you neglect the parenthesis the value will not be inverted.

### XOR

![xor tile](https://raw.githubusercontent.com/matdombrock/EsoBinaria/refs/heads/master/asset_meta/tiles/tile-xor.png)

The XOR tile is a logic operator that resolves to true if an odd number of its inputs are true. In other words, it outputs true only when the number of true inputs is odd. If all inputs are false or all are true, it resolves to false. This tile can have two or more inputs.

Examples:
```c
A = 1
B = 0
C = 1
(XOR A B) => 1
(XOR A B C) => 0
(XOR B C) => 1
(XOR A C) => 0
(XOR A B C A) => 1
```

### NAND

![nand tile](https://raw.githubusercontent.com/matdombrock/EsoBinaria/refs/heads/master/asset_meta/tiles/tile-nand.png)

The NAND tile outputs false only if all its inputs are true; otherwise, it outputs true. It is the inverse of the AND tile. It can have two or more inputs.

Examples:
```c
A = 1
B = 1
C = 0
(NAND A B) => 0
(NAND A B C) => 1
(NAND B C) => 1
```

### NOR

![nor tile](https://raw.githubusercontent.com/matdombrock/EsoBinaria/refs/heads/master/asset_meta/tiles/tile-nor.png)

The NOR tile outputs true only if all its inputs are false; otherwise, it outputs false. It is the inverse of the OR tile. It can have two or more inputs.

Examples:
```c
A = 0
B = 0
C = 1
(NOR A B) => 1
(NOR A B C) => 0
(NOR C) => 0
```

### XNOR

![xnor tile](https://raw.githubusercontent.com/matdombrock/EsoBinaria/refs/heads/master/asset_meta/tiles/tile-xnor.png)

The XNOR tile outputs true if an even number of its inputs are true (including zero); otherwise, it outputs false. It is the inverse of the XOR tile. It can have two or more inputs.

Examples:
```c
A = 1
B = 0
C = 1
(XNOR A B) => 0
(XNOR A B C) => 1
(XNOR B C) => 0
(XNOR A C) => 1
```

### CLEAR

![clear tile](https://raw.githubusercontent.com/matdombrock/EsoBinaria/refs/heads/master/asset_meta/tiles/tile-clear.png)

This meta tile removes an existing tile. It can not exist on the board and can not be placed by the user. 

### BLANK

![blank tile](https://raw.githubusercontent.com/matdombrock/EsoBinaria/refs/heads/master/asset_meta/tiles/tile-blank.png)

This is meta tile which acts as an empty space. Its only purpose is to serve as a way to place additional parenthesis as needed. 

### IN_A

![in_a tile](https://raw.githubusercontent.com/matdombrock/EsoBinaria/refs/heads/master/asset_meta/tiles/tile-ina.png)

Represents input A. Its value is set by the puzzle or user.

### IN_B

![in_b tile](https://raw.githubusercontent.com/matdombrock/EsoBinaria/refs/heads/master/asset_meta/tiles/tile-inb.png)

Represents input B. Its value is set by the puzzle or user.

### IN_C

![in_c tile](https://raw.githubusercontent.com/matdombrock/EsoBinaria/refs/heads/master/asset_meta/tiles/tile-inc.png)

Represents input C. Its value is set by the puzzle or user.

### VOID
This meta tile represents an empty cell that does nothing. This tile is not user-selectable and can not be placed.

# Build Instructions

## Build Container
It is recommended to build in a container. The scripts use `podmad`.

```sh
./tools/container.build.sh
```

## Build in Container

Native:
```sh
./tools/container.run.sh
```

WASM:
```sh
./tools/container.run.wasm.sh
```

## Run

Run with bundled libraries
```sh
./tools/run.bundle.sh
```

Run with local libraries
```sh
./tools/run.sh
```

Run WASM (host server)
```sh
./tools/run.wasm.sh
```
