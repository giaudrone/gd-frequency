# gd-frequency

A [GoldenDict](https://github.com/xiaoyifang/goldendict-ng) plugin to easily lookup the frequency of Japanese words

![image](https://github.com/user-attachments/assets/e1c1a06a-9e96-49d3-a176-77cbf8b44579)


## Installation
Clone this repository into a directory
```
git clone 'https://github.com/toytoi/gd-frequency.git'
```

Run the Makefile using
```
make
```
Then, add the `gd-frequency` executable into your $PATH

## Setup

In GoldenDict, go to "Edit" > "Dictionaries" > "Programs" and add a new entry with the type set to `html` .
In Command Line put: `gd-frequency --word %GDWORD% --dict-path <path> --bin-path <path>`
This program will then be treated as a dictionary.

## Usage

```
gd-frequency --word %GDWORD% --dict-path <PATH/TO/DICT/FILE> --bin-path <PATH/TO/BIN/FILE>
```

The first run of the script will take extra time as the frequency dictionary must be parsed and saved into a bin file.

The repository already includes a [JPDB frequency list](https://github.com/Kuuuube/yomitan-dictionaries?tab=readme-ov-file#jpdb-v21-frequency) in the data directory, so it is reccomended to point the `--dict-path` to it. The frequency list json must be in yomichan format.

Additionally, it is reccomended to point the `--bin-path` to the data directory as well.




