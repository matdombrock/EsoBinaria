# EsoBinaria
EsoBinaria is a tile-based visual programming game where players solve puzzles by arranging and configuring various logic gates and components. The goal is to manipulate inputs and outputs to achieve specific conditions or results, using a grid-based interface. Players can interact with different game elements such as buttons, menus, and sprites to navigate through levels and challenges. The game features a main menu, setup screens, and various in-game entities to provide an engaging and educational experience in logical thinking and problem-solving.

## Tiles

### VOID
This meta tile represents an empty cell that does nothing. This tile is not user-selectable and can not be placed.

### CLEAR
This meta tile removes an existing tile. It can not exist on the board and can not be placed by the user. 

### BLANK
This is meta tile which acts as an empty space. Its only purpose is to serve as a way to place additional parenthesis as needed. 

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
This tile resolves true if the inputs are different. 

### NAND: Outputs false if both inputs are true.
### NOR: Outputs false if either input is true.
### XNOR: Outputs false if the inputs are different.
### INA: Represents input A.
### INB: Represents input B.
### INC: Represents input C.
### IND: Represents input D.
