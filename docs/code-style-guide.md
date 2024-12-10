
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

- Use **UPPERCASE_UNDERSCORE** for constants, constant expressions, and macros.
- Use **lowercase_underscore** for functions and variables (also
  &ldquo;constant variables&rdquo;).
- Use **PascalCase** for the names of `class`, `struct`, and `enum` types, as
  well as for `enum` entries.
- Depending on their purpose, `enum` entries may follow the conventions for
  constants (UPPERCASE_UNDERSCORE) or &ldquo;constant variables&rdquo;
  (lowercase_underscore).
- In rare cases, the names of `class` or `struct` types may also follow the
  lowercase_underscore convention (e.g., `byte_view`).
- Use **lowercase letters (without underscores)** for namespaces.
- Prefix **protected** and **private** functions and variables with a single underscore
  (`_`).
