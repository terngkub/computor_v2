# Computor V2
A command-line calculator in C++

## How to build the project
* copy boost folder to thirdparty/boost
* build the program (TODO using cmake instead)
    ```
    g++ -I thirdpart/boost apps/computorv2/*.cpp -o computorv2
    ```

## How to run the project
```
./computorv2
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