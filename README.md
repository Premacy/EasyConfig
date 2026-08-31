# EasyConfig

A small header-only library for reading simple configuration files in C++.
The entire implementation lives in a single header and has no external dependencies.

## Installation

Add `src/easyConfig.h` to your project's include path:

```cpp
#include "easyConfig.h"
```

A compiler with C++20 support is required.

## Configuration format

Each line contains one `key: value` pair:

```text
port: 8080
host: localhost
timeout: 2.5
```

Empty keys or values, malformed lines, and duplicate keys are treated as errors.

## Usage

```cpp
#include "easyConfig.h"
#include <iostream>

int main()
{
    ConfigReader config("config.txt");

    if (auto port = config.get<int>("port")) {
        std::cout << *port << '\n';
    }

    auto host = config.get<std::string>("host");
    auto timeout = config.get<double>("timeout");
}
```

`ConfigReader` reads and parses the file during construction. If the file cannot be
opened or contains invalid data, the constructor throws `std::logic_error`.
`get<T>()` returns `std::optional<T>`; an empty result means that the key was not
found or its value could not be converted.

The currently supported types are `std::string`, `int`, and `double`.

## Tests

The test suite uses GoogleTest. Make sure Make, CMake, and a C++20 compiler are
available, then run this command from the project root:

```sh
make test
```

The command configures the test project, builds it, and runs all tests with CTest.
GoogleTest is downloaded automatically during the first build, so an internet
connection is required for that run.

See [CHANGELOG.md](CHANGELOG.md) for a summary of project changes.
