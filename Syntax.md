# Keywords
input, if, goto, def, label

# Variables
<variable_name> = <expression>;

## Example:
x = input;          # Assigns user input to x
i = 0;              # Assigns 0 to i

# Expressions
- Can include:
  - Arithmetic: +, -, *, /
  - Relational: ==, !=, >, <, >=, <=
  - Logical: &&, ||, !
  - Values: integers, floats, strings (enclosed in ""), or variables

## Example:
x = 5 + 3;          # x is assigned 8
y = x * 2;          # y is assigned 16
isValid = x > 10;   # isValid is assigned false

# Control Flow

1. **Conditional Statements**:
   - If-Else structure using blocks:
     if <condition> ->
         <statements>
     <-

## Example:
i = 0;
if i < 10 ->
    i = i + 1;
<-

2. **Goto Statements**:
   - Unconditionally jump to a label:
     if <condition> goto <label>;

## Example:
if i >= 5 goto label1;

3. **Labels**:
   - Define a label and its value or block:
     label <label_name> = <value>;

## Example:
label label1 = 10;  # Defines label1 with value 10

# Blocks
- Blocks are enclosed by:
  ->  (start of block)
  <-  (end of block)

## Example:
block label1 ->
    x = 10;
    y = x * 2;
<-

# Functions

1. **Defining a Function**:
   - Use the `def` keyword:
     def: <function_name>(<parameters>) ->
         <statements>
     <-

## Example:
def: add(a, b) ->
    result = a + b;
    return result;
<-

2. **Calling a Function**:
   - Use the function name with arguments:
     <function_name>(<arguments>);

## Example:
sum = add(5, 3);    # Calls the add function with arguments 5 and 3

# Full Program Example

# User input
x = input;
i = 0;

# Conditional and Goto
if i < 10 goto label1;

# Label Block
block label1 ->
    x = 10;
    y = x * 2;
    print y;
<-

# Function Example
def: multiply(a, b) ->
    product = a * b;
    return product;
<-

result = multiply(4, 5);
print result;
