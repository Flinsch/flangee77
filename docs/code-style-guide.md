
# Code Style Guide

This document outlines the coding conventions for the project. Following these
guidelines ensures consistent and readable code throughout the project.

---

## Class Structure and Member Order

- **Access specifiers**: `public` &rarr; `protected` &rarr; `private`
- **Member order** (within each access specifier section):
  - Constants and type definitions
    - (Static) constants and constant expressions
    - Type aliases
    - Nested classes
  - Constructors, destructor, etc.
    - Default constructor
    - Parameterized constructors
    - Copy constructor
    - Move constructor (`noexcept`)
    - Destructor (`noexcept`)
    - Copy assignment operator
    - Move assignment operator (`noexcept`)
    - Swap operation (`noexcept`)
  - Prototypes and implementations
    - (Pure) virtual functions
    - Overridden (virtual) functions
    - Final (virtual) functions
  - &ldquo;Regular&rdquo; (non-virtual) functions
  - Accessors and mutators (getters and setters)
  - Operator overloads
    - Unary increment/decrement operators (`++`, `--`)
    - Comparison operators (`==`, `!=`, `<`, `>`, `<=`, `>=`)
    - Arithmetic operators (unary `+`/`-`, `+`, `-`, `*`, `/`, `%`, `+=`, `-=`, `*=`, `/=`, `%=`)
    - Bitwise operators (`~`, `&`, `|`, `^`, `&=`, `|=`, `^=`)
    - Logical operators (`!`, `&&`, `||`)
    - Stream operators (`<<`, `>>`, `<<=`, `>>=`)
    - Member access operators (`[]`, `*`, `&`, `->`, `->*`)
    - Conversion operators
  - Static functions
  - Variables
    - Static class variables
    - Instance variables
  - Friend declarations

The same order tends to apply to a `struct` as well, but with one crucial
difference: variables are all public and come immediately after constants and
type definitions&mdash;if there are any, otherwise variables are right at the
beginning of the `struct`, in any case before possible constructors etc.

---

## Spacing and Indentation

- Use **4 spaces** per indentation level. Do not use tabs (the tab key is okay,
  but not the tab character `\t`).
- Use **4 spaces** indent width for line continuation as well.

## Line Breaks

- Place **one to three blank lines** between logical sections of code for
  readability.
- Avoid excessive vertical spacing that would go beyond this.
- Avoid trailing whitespace at the end of lines.
- Insert an empty line at the end of a file.

## Maximum Line Length

- Limit lines to **80-100** characters.
- However, this is a soft limit that should not be adhered to if there are good
  reasons.
- Comments should not exceed 84 characters.
  Why 84? It has something to do with my favorite number, 7.
  Now please don't ask any more questions!

---

## Curly Bracket Placement

- (Nearly) always break before braces.
- Exception: namespaces. Place the opening curly bracket on the same line here.
- Also use the same-line style for lambda function definitions.
- Don't indent braces themselves.

---

## Naming Conventions

- Use uppercase underscore for constants, constant expressions, and macros.
- Use lowercase underscore for functions and variables.
- Protected and private functions and variables should be prefixed by a single
  underscore character.
- Use PascalCase for `class`, `struct`, and `enum` names as well as `enum`
  entries.
- Depending on the purpose, `enum` entries can also be understood more like
  constants or like &ldquo;constant variables&rdquo; and can therefore be
  written entirely as uppercase/lowercase underscore.
- In rare cases, `class`/`struct` names can also be lowercase underscore
  (`byte_view` for example, to name just one).
- Use lowercase letters (no underscores) for namespaces.
