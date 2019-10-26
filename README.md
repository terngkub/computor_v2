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
* Expression evaluation
    ```
    expression = ?
    ```
* Polynomial solver
    ```
    expression = expression ?
    ```
* Variable assignation
    ```
    variable = expression
    ```
* Function assignation
    ```
    function = expression
    ```