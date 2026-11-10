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

The repository's integration inputs use the `.syntax-test` extension. These files contain Makefile syntax and can be passed directly to `-f`.

```sh
# Build the default target from an existing test input
./minimake -f tests/test_integration/input/one_dep.syntax-test

# Build a specific target
./minimake -f tests/test_integration/input/one_dep.syntax-test child

# Build several targets in the given order
./minimake -f tests/test_integration/input/few_dep.syntax-test first second

# Display parsed variables and rules without executing recipes
./minimake -p -f tests/test_integration/input/complete.syntax-test

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

The existing file `tests/test_integration/input/one_dep.syntax-test` contains:

```makefile
all: child
	@echo all

child:
	@echo child
```

Recipe lines begin with a real tab character. Run:

```sh
./minimake -f tests/test_integration/input/one_dep.syntax-test all
```

Output:

```text
child
all
```

Minimake builds `child` before `all`. The leading `@` suppresses command logging.

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

| Files | Purpose |
| --- | --- |
| `tests/test_unit/*.c` | Criterion unit tests. |
| `tests/test_integration/test_*.sh` | Shell scripts that run Minimake and check its results. |
| `tests/test_integration/input/*.syntax-test` | Makefile inputs used by integration tests. |
| `tests/test_integration/expected/*.expected` | Expected output compared with the actual results. |
| `tests/test_date/Makefile` | Separate timestamp-based build example. |

Run an individual integration test from the project directory:

```sh
./tests/test_integration/test_execute_one_dep.sh
```

The `complete.syntax-test` input is suitable for pretty-printing; executing its recipes also requires `app.c` and `helper.c`, which are not included in the repository.

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
