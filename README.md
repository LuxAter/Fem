# Arta #

Arta is an implementation of finite element analysis. It has been written in
C++ and has a user interface written in Lua.

## Install ##

First clone this repository, by
```fish
git clone https://github.com/LuxAtrumStudio/Fem --recurse-submodules
```

### Requirements ###

To compile, one needs a modern C++ compiler, libpng, and liblua installed.
As well as GNU make, and Cmake.

#### Mac OS ####

```fish
brew install libpng lua cmake
```

#### Ubuntu ####

```fish
sudo apt install libpng lua cmake
```

### Compile ###

To compile inter the floowing instructions

```fish
mkdir build && cd build
cmake ..
make
```

## Usage ##

To use the program, it only requires a script file, that is made by the user.
Some examples of script files are provided in ``examples/``. We provide a
minimal script file below.

```lua
mesh = "../pslg/rect.poly"
mesh_angle = 20
mesh_area = 0.01

A = {{1, 0}, {0, 1}}
B = {0, 0}
C = 0

bndry = {}

function force(x, y)
  return 0.0
end
```

To run the script with a script file ``test.lua``, simply execute
```fish
./arta -s test.lua
```

Any other command line options, can be found by using the built in help
documentation with the program.

### PSLG ###

The scripts require the definition of what source file to use for the
mesh. This is in the form of a PSLG. To generate a PSLG, one can
use the provided PSLG script, which requires a python3 interpreter.
