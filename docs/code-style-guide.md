
# Code Style Guide

This document outlines the coding conventions for the project.
Following these guidelines ensures consistent and readable code throughout the project.

Note on exceptions to the rules and special cases:
While consistency is key to readability and maintainability, there are cases where deviating from the rule improves clarity or expressiveness.
Such exceptions should be rare, deliberate, and justifiable.
Some exceptions are mentioned below specifically or at least by way of example, but generally speaking:
The rules can (and should) be explicitly deviated from if the individual case &ldquo;requires&rdquo; this in terms of readability, comprehensibility, and/or maintainability.

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
    - Copy assignment operator
    - Move constructor (`noexcept`)
    - Move assignment operator (`noexcept`)
    - Destructor (`noexcept`)
    - Swap operation (`noexcept`)
  - Prototypes and implementations
    - (Pure) virtual functions
    - Overridden (virtual) functions
    - Final (virtual) functions
  - &ldquo;Regular&rdquo; (non-virtual) functions
    - Also &ldquo;properties&rdquo;, accessors and mutators (getters and setters), etc.
  - Operator overloads
    - Unary increment/decrement operators (`++`, `--`)
    - Comparison operators (`==`, `!=`, `<`, `>`, `<=`, `>=`, and/or `<=>`)
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
  - Friend declarations (or optionally at the very beginning of the class)

The same order tends to apply to a `struct` as well, but with one crucial difference:
variables are all public and come immediately after constants and type definitions; if there are any, otherwise variables are right at the beginning of the `struct`, in any case before possible constructors etc.

A concrete example of (possible) exceptions to the above rules:
Static factory methods should be placed immediately after the default and/or parameterized constructors, rather than grouped later with other &ldquo;regular&rdquo; static functions.
This order may be semantically more appropriate and improve code readability.

And another one:
Even though friend declarations usually go at the end (or optionally at the top) of a class, it's fine to place a friend operator with the other operator overloads if it replaces what would otherwise be a regular member operator.
`friend bool operator==(Foo a, Foo b) noexcept` instead of `bool operator==(Foo other) const noexcept` would be such a case, for example.
This can make the class interface easier to read and understand.

## `class` vs. `struct`

- If **any member variable** is **non-public** then there is an **invariant** and you should use `class`.
- Conversely, if **all member variables** are **public** and therefore can vary **independently**, you could/should use `struct`.
- However, in (rather rare) cases of need, a `struct` _may_ also contain private auxiliary methods, etc.
- When in doubt, use `class` rather than `struct`.

## Constructors, Destructors, etc.

- If you can avoid defining any default operation, do so (**rule of zero**).
- If you **define** or **delete** any **copy**, **move**, or **destructor** operation, define or delete them all (**rule of five**).
- Exception to the rule of five: classes that act as an **interface** and have a (public) **virtual default destructor** for this purpose may (and should) skip the other four.
- A base class destructor should be either **public and virtual**, or **protected and non-virtual**.
- Define and initialize data members in the order of member declaration.
- By default, declare single-argument constructors as `explicit`.
- Make move, destructor, and `swap` operations `noexcept`.
- The default constructor, parameterized constructors, and copy operations also preferably &ldquo;want&rdquo; to be `noexcept` if possible.
- Make copy/move assignments safe for self-assignment.
- Use factory functions if you need &ldquo;virtual behavior&rdquo; during construction/destruction.

## Other Default Operations

- Make `==` etc. symmetric with respect of operand types and `noexcept`.
- Beware of `==` etc. on base classes.
- Make `hash` etc. `noexcept`.

---

## Spacing and Indentation

- Use **4 spaces** per indentation level.
  Do not use tabs (the tab key is okay, but not the tab character `\t`).
- Use **4 spaces** indent width for line continuation as well.

## Line Breaks

- Place **one to three blank lines** between logical sections of code for readability.
- Avoid excessive vertical spacing that would go beyond this.
- Avoid trailing whitespace at the end of lines.
- Insert an empty line at the end of a file.

## Maximum Line Length

- Limit lines to **80-120** characters.
- However, this is a soft limit that should not be adhered to if there are good reasons.
- Comments should not exceed 84 characters.
  Why 84? It has something to do with my favorite number, 7.
  Now please don't ask any more questions!
- For multi-line comments, 84 characters is both the limit and the target, but still only a soft one:
  don't wrap a line just to dodge a trailing comma past it (especially after an already-long word), and don't split an inseparable term like &ldquo;horizontal/vertical&rdquo; just to stay under it.

---

## Curly Bracket Placement

- (Nearly) always break before braces.
- Exception: namespaces. Place the opening curly bracket on the same line here.
- Also use the same-line style for initializer lists and lambda function definitions.
- Don't indent braces themselves.

---

## Naming Conventions

- Use **UPPERCASE_UNDERSCORE** for constants, constant expressions, and macros.
- Use **lowercase_underscore** for functions and variables (also &ldquo;constant variables&rdquo;).
- Use **PascalCase** for the names of `class`, `struct`, and `enum` types, as well as for `enum` entries.
- Depending on their purpose, `enum` entries may follow the conventions for constants (UPPERCASE_UNDERSCORE) or &ldquo;constant variables&rdquo; (lowercase_underscore).
- In rare cases, the names of `class` or `struct` types may also follow the lowercase_underscore convention (e.g., `byte_view`).
- Use **lowercase letters (without underscores)** for namespaces.
- Prefix **protected** and **private** functions and variables with a single underscore (`_`).

---

## Comments

- Write comments in **American English**, regardless of the language used in commit messages or discussions.
- Keep comments short and to the point; avoid verbose, exhaustive explanations.
- Avoid non-ASCII characters (typographic dashes, arrows, decorative punctuation, etc.) in comments.
  Exceptions may occur (in the case of German umlauts, for example), but such instances should be rare.
- Avoid dashes, including the ASCII `--`, as a substitute for parentheses, commas, colons, or semicolons.
  Use whichever of those actually fits instead. Semicolons themselves should also be used only rarely.
- Avoid code syntax in comment prose; describe things in plain words instead (e.g., &ldquo;the size of the face&rdquo; rather than &ldquo;the `get_size()` of the `Face`&rdquo;), so comments don't need to change whenever an unrelated implementation detail does.
  If a code reference is unavoidable, use backticks around it.
- This extends to type names and keywords too: say &ldquo;a collection&rdquo; rather than &ldquo;a `Collection`&rdquo;, or &ldquo;null&rdquo;/&ldquo;NULL&rdquo; rather than &ldquo;`nullptr`&rdquo;, whenever the exact identifier isn't actually what the reader would need to go look up.
  The same goes for a parameter name that's already an ordinary word (style, key, state, overlay, ...): just use it as prose.
  Reserve backticks for an actual pointer to a specific symbol (e.g., &ldquo;see `_is_focusable()`&rdquo;) or for telling apart two different senses of the same word appearing close together.
  Backticks are great at marking a technical identifier as such, but they also break up a human reader's flow, so use them only where that's worth it.
- This isn't about whether the word happens to look ordinary, though: naming a specific value to state a precise, checkable fact (e.g., contrasting two exact enum values, `Baseline` vs. `Middle`) still calls for backticks, even though both read as plain English words, because the exact spelling is the point, not just the general idea.
  Lowercasing into plain prose (baseline, middle) is a fine alternative there instead, but don't leave a PascalCase name bare without either.

### Doc Comments (`/** ... */`)

- A function's doc comment is its **public contract**: describe *what* it does/returns and, briefly, *why*, at the level a caller needs, not *how* it's implemented.
  One or two sentences is usually enough. Reserve more for something genuinely non-obvious.
- If describing the &ldquo;how&rdquo; would take several clauses (exact formulas, intermediate values, every related method &ldquo;for completeness&rdquo;), that detail doesn't belong in the doc comment at all (see inline comments below).
- When a function has both a header declaration and an out-of-line `.cpp` definition, its doc comment is **one piece of text duplicated in two places**, not two independently maintained comments:
  - The wording must match **verbatim** between declaration and definition.
  - The formatting (single-line vs. multi-line block) must match as well (preferably multi-line).
  - If you change one side, change the other, even for a pure reformatting.
- Single-line `/** ... */` is mainly for members/attributes and other declarations without a separate out-of-line definition.
  For a function declared in a header and defined in a `.cpp`, decide on single-line or multi-line once and use it consistently in both places. When in doubt, prefer multi-line.

### Inline Comments (`//`)

- Use inline comments sparingly, and only where the *why* isn't obvious from the code itself (a hidden constraint, a workaround, a non-obvious invariant).
- Don't restate *what* the code does: well-named identifiers should already make that clear.
- Implementation-specific detail that doesn't belong in a doc comment (see above) belongs here instead, right next to the line it explains.
- Avoid narrating the current task/fix/caller (e.g., &ldquo;added for the X flow&rdquo;, &ldquo;fixes issue #123&rdquo;): that belongs in the commit message, not in code that outlives the context of the change.
- Being more implementation-focused than doc comments, and not formatted for tooling like Doxygen, inline comments may get more technical, and don't strictly need backticks around a code reference the way doc comments do.
  Plain prose is still preferable regardless, which usually makes backticks unnecessary anyway.

---

## Other Stuff

- Prefer `enum struct` over `enum class` for defining scoped enumerations.
  While both are functionally identical, `struct` has `public` as the default access specifier, whereas `class` defaults to `private`.
  Using `enum struct` ensures consistency with this expected behavior.
  It also aligns with the convention that `struct` is the default choice for POD-like types.
- Prefer `static constexpr` over `constexpr`, especially for non-trivial data types, and especially if you want to reference the data via pointers, etc.
  `constexpr` values are stack values (unless they are `static`) and may be copied there at runtime.
  `static` forces initialization at compile-time through the means of &ldquo;constant initialization&rdquo;.
