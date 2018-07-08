# coro
Example of writing asynchronous code in synchronous style using Boost.Coroutine2

## Description
This is an example of how to write asynchronous code in synchronous style using coroutines.

The program fetches a random number from random.org using the json-rpc API and prints it to the standard output. If the service is unavailable for whatever reason (e.g. no Internet connection) it falls back to a pseudorandom generator with simulated long computation time (a few seconds). This high-level flow is implemented using a coroutine.

## Compilation

To compile:

- install Boost
- install OpenSSL (e.g. sudo apt install libssl-dev)
- install CMake
- install Ninja
- from the main directory: `mkdir build`
- `cd build`
- `cmake .. -GNinja`
- `ninja`

## Running

To run:

- `./coro`
