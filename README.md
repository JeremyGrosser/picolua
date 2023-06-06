# Minimal Lua on RP2040

## Dependencies

    sudo apt update
    sudo apt install build-essential gcc-arm-none-eabi git cmake python3 tio

    git clone https://github.com/JeremyGrosser/picolua
    cd picolua

## Build

    mkdir build
    cd build
    cmake ..
    make

## Run

Hold BOOTSEL button, connect USB.

    sudo cp picolua.uf2 /dev/disk/by-label/RPI-RP2
    tio -b 115200 /dev/ttyACM0

## Usage

    Ctrl-C  Clear buffer
    Ctrl-D  Execute buffer
    Ctrl-L  Clear screen

## Examples

    lua> print("hello lua")
    <Ctrl-D>
    hello lua
    lua> a=2
    b=2
    print(a*b)
    <Ctrl-D>
    4

## Binding to Pico SDK
A few simple bindings for SDK functions have been added as examples. Here we turn the LED on:

    lua> LED=25
    set_output(LED, true)
    set_pin(LED, true)
    <Ctrl-D>

Blinking is an exercise left to the reader.

The `bootsel()` function will reset the chip for reflashing as if you'd held down the BOOTSEL button.

## References

- [Getting Started with Raspberry Pi Pico](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf)
- [Raspberry Pi Pico C/C++ SDK](https://datasheets.raspberrypi.com/pico/raspberry-pi-pico-c-sdk.pdf)
- [Lua 5.4 Reference Manual](https://www.lua.org/manual/5.4/manual.html)
- [Programming in Lua](https://www.lua.org/pil/)
