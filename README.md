# Computor V2
A command-line calculator in C++

## How to build the project

### Requirements
* Boost version 1.71.0
    * MacOS - install with brew
    ```
    brew install boost
    ```
    * Ubuntu - install with apt
    ```
    sudo apt update
    sudo apt install libboost-all-dev
    ```
* CMake version 3.10+

### Command
```
mkdir build
cd build
cmake ..
cmake --build .
cd ..
```

## How to run the project
```
./bin/computorv2
```

## Inputs
* Commands
    * list_variables - list all the assigned variables
    * list_functions - list all the assigned variables
    * exit - exit the program
* Expression evaluation
    ```
    expression = ?
    ```
* Polynomial solver
    ```
    expression = expression ?
    ```
* Variable assignation
    Grammar
    ```
    variable = expression
    ```
    Examples
    ```
    x = 42
    y = x^2 + 2x + 1
    ```
    Warning:
    * If the variable is already assigned, this command will reassign it with the new value;
* Function assignation
    ```
    function_name(parameter) = expression
    ```


## Bonuses
* list_variables and list_functions
* history
* function composition
* parenthesis multiplication
* matrix can contain expression