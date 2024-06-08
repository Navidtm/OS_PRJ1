# OS Project (simple rdfind)

the project is a multi-thread application similar to the rdfind program in Linux, which identifies duplicate files and deletes them, and displays the list of all checked files, types of checked and deleted files as an output.

## Features

- **dentifies duplicate files and deletes them**
- **Thread-Safe Updates**

## Functions

- `calculateFileHash`: Function to calculate SHA-1 hash of a file.
- `checkAndDeleteDuplicates`: Function to check for duplicate files and delete them.
- `createLogFile`: Function to create a log file for each folders.
- `processFiles`: Function to check file in directory and and checks it
- `displayMonitoringInfo`: Display information about each type of file format.

## Graphical Output

In the next updates

## How to Use

1. Clone the repository to your local machine.
2. Compile and run the code.
3. Choose the directory you want to analyze.

## Dependencies

- <openssl/sha.h>: SHA-1 (Secure Hash Algorithm) is a cryptographic hash function. you should installed on your system.