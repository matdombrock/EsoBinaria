A `compile_commands.json` file is required for many LSP functions like LSP rename

The compile_commands.json file is generated automatically when you do a full build. For instance:
```
./tools/build.sh
```

This file can also be generated manually by running:
```
./tools/compile_commands.sh
```

This file is not tracked by git and must be generated for each new work environment. 

