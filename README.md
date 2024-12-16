# Sudoku Validator with IPC

This repository contains a C program for validating Sudoku puzzles using parallel processing and inter-process communication (IPC) with named pipes (FIFOs). It ensures the correctness of rows, columns, and sub-grids in a Sudoku grid.

## Features

- **Parallel Processing**: Four processes handle decoding and validating rows, columns, and sub-grids concurrently.
- **Inter-Process Communication**: Named pipes (FIFOs) share data between parent and child processes.
- **Custom Input**: Reads encrypted Sudoku puzzles from `test.txt` and decodes them using Caesar cipher.

## How It Works

1. The parent process reads and cleans the encrypted puzzle from `test.txt`.
2. A decoder child process decrypts the puzzle and sends it to validation processes.
3. Validation processes check:
   - **Rows**: Unique characters in each row.
   - **Columns**: Unique characters in each column.
   - **Sub-Grids**: Unique characters in each block.
4. Results from all processes are aggregated to determine puzzle validity.

## Input Format

- A single string with grid dimensions (`X*Y*Z`) followed by the encrypted puzzle.
- Example: `9*3*3*ABCD...`.

## Example Output

- **Valid Sudoku**: `Sudoku Puzzle constraints satisfied!!!`
- **Invalid Sudoku**: `Sudoku Puzzle is Wrong!!`

## How to Run

1. Compile:
   ```bash
   gcc -o sudoku_validator sudoku_validator.c

