# MatrixLab

As part of import substitution, it is necessary to develop software that allows calculations with matrices.  
The elements of a matrix can be rational numbers, where the numerator and denominator are long integers.

To write commands in the developed language of matrix computations, Cambridge prefix notation is used (this is Polish notation with parentheses).  
In prefix notation, the operator is placed to the left of the operands, but because operators and functions may have a different number of operands and arguments, round parentheses are used in the notation to avoid ambiguity in interpretation.

For example, a function of three arguments `FUNC(x, y, z)` will be written as:
```
(FUNC x y z)
```

And the addition operator (addition of `A` and `B`) will be written as:
```
(+ A B)
```

The arguments of functions and operators can be variables, constants, and other functions. For example, the expression `FUNC(x, y, z) + FUNC(a, 1, 2)` will be written as:
```
(+ (FUNC x y z) (FUNC a 1 2))
```

For variable names, only letters of the English alphabet (uppercase and lowercase) and digits will be used (a digit cannot be the first character in a variable name).  
A variable name (identifier) may contain up to 31 characters.

## Supported operators and functions

- `(= var value)` — assignment of the value `value` to the variable `var`
- `(+ A B)` — addition *A + B*
- `(- A B)` — subtraction *A - B*
- `(.* A B)` — elementwise multiplication of matrices *A* and *B* of the same size (result is a matrix of the same size with elements a<sub>ij</sub> * b<sub>ij</sub>)
- `(./ A B)` — elementwise division of matrices *A* and *B* of the same size (result is a matrix of the same size with elements a<sub>ij</sub> / b<sub>ij</sub>)
- `(* A B)` — matrix multiplication *A \* B*
- `(/ A B)` — matrix division *A \* B*<sup>-1</sup>
- `(- A)` — unary minus
- `(zeros n m)` — matrix of zeros of size *n × m*
- `(ones n m)` — matrix of ones of size *n × m*
- `(eye n)` — identity matrix *n × n*
- `(linspace x0 x1 n)` — row vector with *n* elements from *x0* to *x1*
- `(vertcat A1 A2 ... An)` — vertical concatenation
- `(horzcat A1 A2 ... An)` — horizontal concatenation
- `(transpose A)` — matrix transpose
- `(det A)` — determinant of a matrix
- `(inv A)` — inverse matrix
- `(sum A)` — sum of all matrix elements
- `(prod A)` — product of all matrix elements
- `(min A B)` — elementwise minimum of matrices A and B
- `(max A B)` — elementwise maximum of matrices A and B
- `(disp A)` — output value of *A* to the output file

## Program arguments

- input file name  
- output file name  

The input file contains a program — a script in the matrix computation language.  
Each command is written on a separate line.

If an error occurs in the input file expressions, the output file must contain:
```
Error in line X
```
where `X` is the line number (starting from 1).

## Output format

### Scalar

If `A` is a scalar:
```
variable_name = value
```

- If the value is an integer → print as integer  
- If rational → print as `numerator/denominator`  
- Negative sign applies to the numerator  

---

### Matrix

If `A` is a matrix:

```
variable_name = [
<row1>
<row2>
...
]
```

- Print row by row  
- Align columns to the right  

---

### Special case

If `disp` receives an expression instead of a variable:

```
(disp (expression))
```

then the variable name must be:

```
ans
```
