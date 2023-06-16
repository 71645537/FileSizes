# FileSizes
This is a The Directory Size Calculator. It Iterate through the files under a specified folder to list all file sizes. This is a command-line program that calculates the sizes of files and directories within a specified path. It provides a summary of the sizes in a sorted order, along with any encountered errors during the calculation process.

## Usage

```
./FileSizes [OPTIONS] [PATH]
```

Replace `./FileSizes` with the appropriate command to run the program on your system.

### Options

- `-r`: Recursive mode. Calculate the sizes of all files and directories recursively within the specified path.
- `-g`: Specify the path directly, without recursive calculation.

### Path

The `PATH` argument represents the directory path for which you want to calculate the sizes. If no path is provided, the program will use the current working directory as the default path.

## Features

- Calculates the sizes of files and directories within the specified path.
- Supports recursive calculation using the `-r` option.
- Displays the results in descending order based on size.
- Handles errors encountered during the calculation process and reports them separately.

## Output

The program will output the calculated sizes in the following format:

```
"PATH" ..................... SIZE
```

- `"PATH"`: The file or directory path, surrounded by double quotes.
- `SIZE`: The calculated size of the file or directory.

For example:

```
"C:\Users\admin\Documents\Dry Cactus" ..................... 1 MB
"C:\Users\admin\Documents\Green Turtle" ................. 553 KB
```

If any errors occur during the calculation, they will be displayed at the end of the output, following a horizontal line (`------`). The error messages will indicate the path that failed and the specific error encountered.

## Examples

Calculate the sizes of files and directories in the current working directory:

```
./FileSizes
```

Calculate the sizes of files and directories recursively within a specific path:

```
./FileSizes -r /relative/path/to/directory
```

Calculate the sizes of files and directories directly without recursion:

```
./FileSizes -g /global/path/to/directory
```

## Dependencies

The program utilizes the following libraries:

- `iostream`: Standard Input/Output streams.
- `filesystem`: Provides access to the filesystem and file-related operations.
- `map`: Implements a key-value map container for sorting purposes.
- `string`: String manipulation and handling.
- `csignal`: Signal handling for interrupt signals.
- `thread`: Concurrency support for running the animation and main logic concurrently.
- `atomic`: Provides atomic operations for synchronization.
- `iomanip`: Input/output manipulators for formatting the output.

## TODO

There are some features planned ahead
- Print into tree

## License

This program is released under the [MIT License](LICENSE).