# HW3 - Recursive-Descent Parser

## Compile
```
gcc -o main main.c
```

## Run
```
./main
```
Input is read from stdin. Type the expression and press Enter.

## Example
```
./main
(1+2+(3+4))+5
```

## Output Format
Each grammar rule applied is printed with 2-space indentation per depth level.
```
S -> E S'
  E -> ( S )
    S -> E S'
      1
    S' -> + S
      S -> E S'
        2
      ...
  S' -> + S
    S -> E S'
      5
    S' -> e
```

## Grammar
```
S  -> E S'
S' -> e
S' -> + S
E  -> num
E  -> ( S )
```
