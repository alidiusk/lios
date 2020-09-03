# lios

![lios demo](demo.gf)

This is a custom 32bit operating system made from scratch named `LIOS`.
Its implementation is inspired by the renowned guide [OS Tutorial](https://github.com/cfenollosa/os-tutorial) that walks individuals through making their own OS from scratch.

## Building
To build `lios`, you will need:
* clang
* llvm
* ld.lld
* nasm

Clone the repository and use `make build`.

## Running
To actually run `lios`, I recommend you use [QEMU](https://www.qemu.org/).

If you have `QEMU` installed, you can run `lios` with `make run`.

## Features
* Boots from scratch
* 32-bit
* Handles keyboard interrupts and thus can accept user input
* Can print output to screen
* Basic `libc` implementation
* A simplistic user shell
* Memory allocation

## Usage
The user shell currently only accepts two commands:
1. `ALLOC-PAGE`, which allocates an aligned page of memory and returns the address.
2. `END`, which halts the CPU.

This OS is not usable for anything productive. It was produced as a learning exercise. Feel free to consult it if you find any of the code helpful. I highly recommend the guide [OS Tutorial](https://github.com/cfenollosa/os-tutorial) that it was based off of.

Licensed under GLPv3.
