# AVaRICE for Windows

This is a fork of AVaRICE 2.13 from <https://sourceforge.net/projects/avarice/>.

The purpose of this fork is to add better support for Windows.

Noteable changes include:

- Support Visual Studio

## Feature Details

### Support Visual Studio

This build adds support for **Microsoft Visual Studio**. Building AVRDUDE with Microsoft Visual C/C++ will give you the best user and debugging experience while working on Windows.

## Releases

You can find the latest releases of **AVaRICE for Windows** here:

<https://github.com/mariusgreuel/avarice/releases>

## Users manual

You can find the original documentation for AVaRICE here:

<http://avarice.sourceforge.net/>

## Build

### Building AVaRICE for Windows

#### Prerequisites

In order to build AVaRICE, you need:

- Microsoft Visual Studio 2019 with 'Desktop development with C++' enabled

#### Instructions

To build AVaRICE on Windows, do the following:

- `git clone --branch windows https://github.com/mariusgreuel/avarice`
- Open the solution `avarice.sln` and build the project.

Note: The folder `msvc\generated` includes pre-built files from the AVaRICE configure and grammar generation.

### Building AVaRICE for Linux

#### Prerequisites

In order to build AVaRICE, you need the following packages:

- make gcc automake libtool flex bison
- libusb-dev libelf-dev libftdi1-dev libhidapi-dev

#### Instructions

To build AVaRICE on Linux, run the following commands:

```
git clone --branch master https://github.com/mariusgreuel/avarice
./bootstrap
./configure
make
```

## Troubleshooting Tips & Tricks

### TODO

## License

AVaRICE is licensed under the GNU GPLv2.
