# Snake for Raspberry Pi Pico

This is a recreation of the classic game Snake, running on a [Pimoroni Pico Explorer](https://shop.pimoroni.com/products/pico-explorer-base).

## Build

You'll need the [Pico Explorer](https://github.com/pimoroni/pimoroni-pico) library available locally to you. This project assumes this has been cloned in the directory next to this one. If this isn't true for you, you'll need to modify the `include()` statement in [`CMakeLists`](CMakeLists.txt) before compiling.

```bash
mkdir build
cd build

cmake ..
make
```

Then load the resulting `snake.uf2` file onto your Pico anyway you know how.