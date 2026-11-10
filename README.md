# Minimake

A simplified **build automation tool** written in C, designed to build targets from dependencies and avoid rebuilding files that are already up to date.

Created by **Kilian Meddas**.

---

## Overview

This project was developed as part of the EPITA engineering curriculum.

Minimake reproduces a subset of the Unix `make` utility. It reads a Makefile, stores its rules and variables, and executes the commands required to build the requested targets. Dependencies are processed before their targets, while file modification times determine whether a rebuild is needed.

The project combines:

- Makefile parsing
- Variable expansion
- Dependency resolution
- Timestamp-based builds
- Shell command execution
- Automated testing

---

## Features

### Makefile Parsing

- Parse targets, dependencies, and tab-indented recipes
- Define and update variables using `NAME = value`
- Handle comments outside recipes and report invalid syntax
- Display parsed variables and rules using pretty-print mode

### Variable Expansion

- Support `$(NAME)`, `${NAME}`, and single-character references such as `$N`
- Expand variables in targets, dependencies, and recipes
- Convert `$$` into a literal `$`
- Replace undefined variables with an empty string

### Target Building

- Build one or several targets in the requested order
- Resolve dependencies recursively before building their targets
- Select the first non-pattern rule when no target is specified
- Use file modification times to skip up-to-date targets
- Avoid executing the same target twice during one invocation

### Command Execution

- Run each recipe command in a separate `/bin/sh -c` process
- Print commands before execution
- Support the `@` prefix to suppress command logging
- Stop at the first failed command and report errors on standard error

---

## Technologies

- **C99** — application implementation
- **POSIX APIs** — process creation, command execution, and file information
- **GNU Make** — build automation
- **Criterion** — unit testing
- **AddressSanitizer** — memory error and leak detection

---

## Requirements

- A POSIX environment, such as Linux.
- GCC and GNU Make.
- `/bin/sh` for recipe execution.
- Criterion for unit tests.
- AddressSanitizer support for memory checks.
- `clang-format` for the optional formatting target.

---

## Installation

### 1. Clone the repository

```sh
git clone git@github.com:kilianMeddas/minimake.git
cd minimake/minimake
```

The source project is stored in the repository’s `minimake/` directory. Run the following build, usage, and test commands from that directory.

### 2. Build the project

```sh
make
```

This produces the `minimake` executable. Source files are compiled with:

```text
-std=c99 -pedantic -Werror -Wall -Wextra -Wvla
```

Remove generated files with:

```sh
make clean
```

---

## Usage

```sh
./minimake -f <makefile> [target ...]
```

A Makefile must be selected explicitly with `-f`, `--file=`, or `--makefile=`. If no target is provided, Minimake builds the first non-pattern rule.

```sh
# Build the default target
./minimake -f Example.mk

# Build a specific target
./minimake -f Example.mk hello

# Build several targets in the given order
./minimake -f Example.mk hello goodbye

# Display parsed variables and rules without executing recipes
./minimake -p -f Example.mk

# Display help
./minimake --help
```

### Options

| Option | Description |
| --- | --- |
| `-f file` | Select the Makefile to read. |
| `--file=file`, `--makefile=file` | Alternative forms of `-f`. |
| `-h`, `--help` | Print help and exit. Takes priority over other options. |
| `-p`, `--print-data-base`, `--pretty-print` | Print parsed variables, rules, dependencies, and recipes. |

If several file options are provided, the last one is used.

### Example

Save the following as `Example.mk`. Recipe lines must begin with a real tab character.

```makefile
NAME = world

all: hello goodbye

hello:
	@echo "Hello, $(NAME)!"

goodbye:
	@echo "Goodbye, ${NAME}!"
```

Run:

```sh
./minimake -f Example.mk
```

Output:

```text
Hello, world!
Goodbye, world!
```

Remove the leading `@` from a recipe command to print the expanded command before its output.

### Exit status

| Status | Meaning |
| --- | --- |
| `0` | Successful execution or help display. |
| `2` | Argument, parsing, dependency, or command execution error. |

Errors are reported on standard error.

---

## Testing

Run the Criterion unit tests and shell integration tests:

```sh
make check
```

Run unit tests with AddressSanitizer and leak detection:

```sh
make check-asan
```

The test suite covers variable storage and expansion, rule and list management, pretty-printing, argument errors, and dependency execution.

Format the source files and tests:

```sh
make format
```

---

## Project Structure

Paths below are relative to the `minimake/` directory.

| Path | Purpose |
| --- | --- |
| `src/minimake.c` | Program entry point. |
| `src/argument.c`, `src/options.c` | Command-line parsing and target selection. |
| `src/parser.c`, `src/append.c` | Makefile parsing and data collection. |
| `src/helpers.c`, `src/resize.c` | Variable expansion and buffer utilities. |
| `src/chained_list.c`, `src/list.c` | Variable, rule, dependency, and command storage. |
| `src/execute.c` | Dependency traversal, timestamp checks, and recipe execution. |
| `src/pretty_print.c` | Parsed database output. |
| `src/include/` | Header files. |
| `tests/test_unit/` | Criterion unit tests. |
| `tests/test_integration/` | Shell integration tests and fixtures. |
| `tests/test_date/` | Timestamp-based build example. |
| `Makefile` | Build, test, formatting, and cleanup targets. |

---

## Limitations

This project implements a subset of `make`. The current version does not implement automatic Makefile discovery, recursive variable expansion, environment-variable fallback, automatic variables (`$@`, `$<`, `$^`), `.PHONY` semantics, pattern-rule resolution, or the `-n` and `-s` options.

Circular dependencies are not detected. Each recipe line runs in its own shell, so shell state does not persist between lines.

---

## Author

**Kilian Meddas**

[GitHub](https://github.com/kilianMeddas/)
