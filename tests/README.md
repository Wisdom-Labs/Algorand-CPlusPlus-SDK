# Unit tests

## Setup

- Linux - `sudo apt-get install cpputest lcov`
- OSX - `brew install cpputest lcov`

## Running tests

```shell
make
```

## Directory structure

```
├── Makefile // Invokes all the unit tests
├── build
│   [...] // Where all the tests wind up
├── fakes
│   // fakes for unit tests
├── mocks
│   // mocks for unit tests
├── src // test source files
└── vertices_ports // HTTP Request
```

# Adding a test

- Make sure you have set `CPPUTEST_HOME` on `tests/CMakeLists.txt`. 
  * Linux - `set(CPPUTEST_HOME /home/ubuntu/cpputest)`
- Add a new test file under tests/src for the module you want to test
- `make`

---

Based on the work of our friends at Memfault ([SDK](https://github.com/memfault/memfault-firmware-sdk)). For more, checkout the [Interrupt](https://interrupt.memfault.com/) blog.