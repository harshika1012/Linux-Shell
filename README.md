# README - Custom Shell Implementation

## Overview
This project implements a custom Unix shell with essential functionalities, including basic system calls, input handling, directory navigation, file listing, command logging, and background process management.

## Features Implemented

### 1. Display Requirement
- The shell prompt follows the format `<Username@SystemName:~>`.
- Username and system name are dynamically retrieved.
- The home directory of the shell is represented as `~`.
- Relative and absolute paths are displayed correctly based on the current working directory.
- Directory changes reflect immediately in the prompt.

### 2. Input Handling
- Supports `;` for sequential command execution.
- Supports `&` for background process execution, displaying the process ID.
- Handles random spaces and tabs in input.
- Prints an error message for invalid commands.

### 3. `hop` Command (Custom `cd` Implementation)
- Changes the current working directory and prints the absolute path.
- Supports:
  - `.` (current directory)
  - `..` (parent directory)
  - `~` (home directory)
  - `-` (last visited directory)
- Accepts multiple arguments and processes them sequentially.
- Supports both absolute and relative paths.
- Does not use `execvp` or similar functions.

### 4. `reveal` Command (Custom `ls` Implementation)
- Lists files and directories in lexicographic order.
- Supports flags:
  - `-a`: Includes hidden files.
  - `-l`: Displays detailed file information.
  - `-al`, `-la`, `-lala`, etc.: Handles multiple occurrences of `a` and `l`.
- Supports absolute and relative paths.
- Implements color coding:
  - Green for executables
  - White for files
  - Blue for directories
- Does not use `execvp` or similar functions.

### 5. `log` Command (Command History Management)
- Stores the last 15 unique commands, avoiding duplicates.
- Commands separated by `;` or `&` are stored as a single entry.
- Erroneous commands are stored optionally (mentioned in this README).
- The `log` command itself is **not stored**.
- `log purge` clears the command history.
- `log execute <index>` runs a command from the history (most recent command is index `1`).

### 6. Execution of System Commands
- Supports execution of system commands like `vim`, `gedit`, etc.
- Foreground execution:
  - If runtime exceeds 2 seconds, displays process name and runtime in the next prompt.
- Background execution:
  - Prints the PID of the spawned process.
  - Automatically notifies the user when a background process terminates, stating whether it exited normally or abnormally.

## Additional Notes
- Background processes are only supported for system commands, not for custom commands (`hop`, `reveal`, `log`).
- Proper error handling is implemented for invalid commands and incorrect usage of `hop` and `reveal`.
- The shell handles concurrent background processes and reports their termination asynchronously.

## Compilation & Execution
To compile and run the shell:
```sh
make
./a.out
```

Process Management

    proclore: Retrieve information about a specific process or the current shell process if no arguments are provided.
    activities: Lists all running or stopped processes spawned by the shell in lexicographic order.
    ping: Sends signals to processes based on PID and signal number (mod 32).
    fg & bg: Brings a stopped/running process to the foreground or resumes a stopped process in the background.

File & Directory Operations

    seek: Searches for files and directories recursively within a specified directory (or the current one by default). Supports flags:
        -d: Search only for directories.
        -f: Search only for files.
        -e: Automatically prints file content or changes to a found directory.

I/O Redirection & Piping

    Supports >, >>, < for redirecting input and output.
    Handles multiple pipes (|) in commands, ensuring smooth data flow between processes.
    Supports redirection and piping together, allowing complex command execution.

Custom Shell Configuration

    .myshrc: Custom shell configuration file for defining aliases and functions.
    Supports simple aliases (e.g., alias home = hop ~).
    Bonus functions:
        mk_hop(): Creates and switches into a directory.
        hop_seek(): Changes directory and searches for matching files.

Job Control & Signal Handling

    Ctrl+C: Interrupts the foreground process.
    Ctrl+D: Exits the shell, terminating all child processes.
    Ctrl+Z: Stops the foreground process and moves it to the background.

Bonus Features

    neonate: Periodically prints the PID of the most recently created process until 'x' is pressed.
    iMan: Fetches man pages from http://man.he.net/ using sockets.

