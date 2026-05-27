# HW3 - Recursive-Descent Parser

## Compile
```
gcc -o main main.c
```

## Run

**With an input file (recommended, same as HW1):**
```
.\main.exe input.txt
```

**With stdin (Linux/bash only):**
```bash
echo "(1+2+(3+4))+5" | ./main
```

> Note: Do **not** pass the expression as a command-line argument.
> PowerShell evaluates `(...)` as arithmetic before passing to the program.

## Example

`input.txt`:
```
(1+2+(3+4))+5
```

```
.\main.exe input.txt
```

Output:
```
S -> E S'
  E -> ( S )
    S -> E S'
      1
    S' -> + S
      S -> E S'
        2
      S' -> + S
        S -> E S'
          E -> ( S )
            S -> E S'
              3
            S' -> + S
              S -> E S'
                4
              S' -> e
          S' -> e
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
