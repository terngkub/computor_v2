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
    apt update
    sudo apt install boost
    ```

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

## Features
* Commands
    * list variables - list all the assigned variables
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
    function = expression
    ```