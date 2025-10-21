# EsoBinaria

EsoBinaria is a tile-based visual programming game where players solve boolean logic puzzles. 

## Tiles

### AND
This operator tile resolves true if all it's inputs are true. It can have any amount of inputs but ALL of them must be true for it to resolve true. 

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
This operator tile resolves true if any of it's inputs are true. It can have any amount of inputs and will resolve true if any of its inputs are true. 

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
This operator tile inverts the value of its input. It will resolve true when its input is false and it will resolve false when its input is true. This tile can only operate on one input. It is an error to supply it with more than one input. 

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
The XOR tile is a logic operator that resolves true if an odd number of its inputs are true. In other words, it outputs true only when the inputs are different. If all inputs are false or all are true, it resolves false. This tile can have two or more inputs.

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
This operator tile outputs false only if all its inputs are true; otherwise, it outputs true. It is the inverse of the AND tile and can have two or more inputs.

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
This operator tile outputs true only if all its inputs are false; otherwise, it outputs false. It is the inverse of the OR tile and can have two or more inputs.

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
This operator tile outputs true if an even number of its inputs are true (including zero); otherwise, it outputs false. It is the inverse of the XOR tile and can have two or more inputs.

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
This meta tile removes an existing tile. It can not exist on the board and can not be placed by the user. 

### BLANK
This is meta tile which acts as an empty space. Its only purpose is to serve as a way to place additional parenthesis as needed. 

### INA
Represents input A. Its value is set by the puzzle or user.

### INB
Represents input B. Its value is set by the puzzle or user.

### INC
Represents input C. Its value is set by the puzzle or user.

### IND
Represents input D. Its value is set by the puzzle or user.

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
