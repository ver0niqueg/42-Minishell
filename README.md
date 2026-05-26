# Minishell 🐚

Minishell is a simplified shell implementation that demonstrates fundamental concepts in shell programming, command parsing, process execution, and redirection handling. The goal is to create a functional shell that can parse and execute user commands with proper input/output redirection and builtin command support.

## Table of contents

- [Highlights](#highlights)
- [Builtins](#builtins)
- [Definitions](#definitions)
- [Installation](#installation)
- [Usage](#usage)
- [Testing](#testing)

## Highlights

- Parse and execute shell commands with arguments
- Implement standard shell builtins (cd, echo, export, env, exit, etc.)
- Handle input and output redirections (`<`, `>`, `>>`)
- Support pipe operations (`|`) for chaining commands
- Process environment variables and variable expansion
- Handle quoted strings (single and double quotes)
- Manage here-documents for multi-line input
- Error handling and signal management
- Support for background and foreground execution
- Proper exit status handling

## Builtins

### Core Builtins

**echo**: Display text on output (with `-n` flag support)

**cd**: Change the current working directory

**pwd**: Print working directory

**export**: Set or display environment variables

**unset**: Remove environment variables

**env**: Display all environment variables

**exit**: Terminate the shell with optional exit code

### Supporting Functions

**ft_cd**: Handles directory changes with error checking

**ft_echo**: Implements echo with flag parsing

**ft_pwd**: Retrieves and displays current directory

**ft_env**: Lists environment variables

**ft_export**: Manages variable export

**ft_exit**: Graceful shell termination

**builtins**: Main dispatcher for builtin execution

**builtins_utils**: Helper functions for builtin operations

## Definitions

| Term | Description |
|------|-------------|
| Shell | A command-line interpreter that reads and executes commands |
| Builtin | A command implemented directly in the shell (not external) |
| Pipe | A mechanism to connect stdout of one command to stdin of another |
| Redirection | Changing where a command reads input or writes output |
| Input Redirection | Redirecting stdin from a file (`<`) |
| Output Redirection | Redirecting stdout to a file (`>` or `>>`) |
| Here-document | Multi-line input terminated by a delimiter (`<<`) |
| Environment Variable | A key-value pair accessible by all processes |
| Variable Expansion | Replacing a variable name with its value |
| Token | A unit of parsing (command, argument, operator, etc.) |
| Syntax Error | Invalid command structure or grammar |
| Fork | Creating a child process to execute external commands |
| Exit Status | Return value of a command (0 for success, non-zero for failure) |

## Installation

Clone the repository:
```bash
git clone git@github.com:ver0niqueg/42-Minishell.git
cd 42-Minishell
```

Compile:
```bash
make
```

Clean object files:
```bash
make clean
```

Remove all generated files:
```bash
make fclean
```

Recompile:
```bash
make re
```

## Usage

Run the shell:
```bash
./minishell
```

### Basic Commands

```bash
$ echo Hello World
Hello World

$ cd /tmp
$ pwd
/tmp

$ export MY_VAR=value
$ echo $MY_VAR
value

$ ls -la | grep Makefile
-rw-r--r--  1 user group  2048 May 26 12:34 Makefile

$ cat < input.txt > output.txt

$ unset MY_VAR
```

### Examples

```bash
# Simple command execution
minishell> echo "Hello from minishell"

# Directory navigation
minishell> cd ..
minishell> pwd

# Environment variables
minishell> export PATH=/usr/bin:/bin
minishell> env | grep PATH

# Pipes
minishell> ls -la | grep ".c" | wc -l

# Redirections
minishell> echo "test" > file.txt
minishell> cat < file.txt

# Multiple commands
minishell> cd sources && ls -la
minishell> exit 0
```

## Testing

### What to look for

✅ **Correct parsing**: Commands are parsed without syntax errors  
✅ **Proper execution**: Commands execute with correct output  
✅ **Environment handling**: Variables are expanded correctly  
✅ **Redirections work**: Input/output redirections function properly  
✅ **Pipes function**: Piped commands execute in correct order  
✅ **Builtins respond**: All builtins execute as expected  
✅ **Error handling**: Invalid commands produce appropriate errors  
✅ **Clean exit**: Shell exits cleanly without memory leaks  

### Test scenarios

**Basic builtin commands**:
```bash
./minishell
minishell> pwd
minishell> echo test
minishell> cd /
minishell> pwd
```
Expected: Correct output for each command.

**Environment variables**:
```bash
minishell> export TEST_VAR=hello
minishell> echo $TEST_VAR
minishell> unset TEST_VAR
minishell> echo $TEST_VAR
```
Expected: Variable is set, displayed, then cleared.

**Piping**:
```bash
minishell> echo "one two three" | wc -w
minishell> ls -la | grep Makefile
```
Expected: Correct piped output.

**Redirections**:
```bash
minishell> echo "content" > test.txt
minishell> cat < test.txt
minishell> echo "more" >> test.txt
```
Expected: Files created and appended correctly.

**Error handling**:
```bash
minishell> cd /nonexistent
minishell> nonexistent_command
minishell> echo |
```
Expected: Appropriate error messages.

## Project Structure

```
42-Minishell/
├── Makefile                 # Main compilation
├── README.md               # This file
├── includes/
│   └── minishell.h        # Header definitions
├── libft/                 # Custom library
│   ├── Makefile
│   ├── includes/
│   └── sources/
└── sources/
    ├── main.c             # Entry point
    ├── builtins/          # Builtin commands
    ├── execution/         # Command execution
    ├── parsing/           # Input parsing
    └── utils/             # Utilities
```

## Shell Features

### Parsing
- Tokenization of input commands
- Quote handling (single and double)
- Variable expansion
- Syntax error detection
- Command and argument separation

### Execution
- External command execution via fork/exec
- Process management and cleanup
- Exit status handling
- Signal management

### Redirections
- Input from file (`<`)
- Output to file (`>`)
- Append to file (`>>`)
- Here-documents (`<<`)

### Pipe Support
- Multiple pipe chains
- Proper file descriptor management
- Process synchronization

---

vgalmich & vihane
