Minesweeper
===========

My hobby implementation of Minesweeper using Qt.

## Description

This is a simple implementation of Minesweeper.
Currently, I test only on Arch Linux, i3-WM, clang and libc++.

## Requirement

- Qt 5
- clang (maybe work on others)
- libc++ (maybe work on others) 

## Build

```sh
$ cd /path/to/repository
$ mkdir build
$ cd build
$ qmake .. -spec linux-clang
$ make
```

## Usage

Run `Minesweeper`, choose difficulty, and then enjoy!
Left click is open. Right click is Flag. Middle click or Double click is automatically open if enough cells are flagged.
