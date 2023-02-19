# Cpp logging macro
A simple cpp logging macro configurable with log levels and stream output.

## Quick start

Copy the [`log.hpp`] file to your local project and include it. Or use the repository as a git submodule.

Use case:
```cpp
#include "log/log.hpp"

#include <iostream>

int main() {
    // Configure the logging macro (only needed once in the application).
    // Note: This is not necessarily needed, because default level and stream
    // are [`LogLevel::Debug`] and [`std::cerr`].
    LOG_CONFIGURE(Warn, std::cout)

    // Log to [`std::cout`] with log level [`LogLevel::Error`].
    LOG(Error) << "An error happened!" << std::endl;

    // Should not be printed because log level is lower than configured log level.
    LOG(Info) << "This should not be displayed..." << std::endl;

    return 0;
}
```
