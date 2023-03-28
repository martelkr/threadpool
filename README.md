# C++20 Thread Pool (header file only)
[![MIT license](https://img.shields.io/badge/license-MIT-blue.svg)](http://opensource.org/licenses/MIT)
![cmake Build](https://github.com/martelkr/threadpool/actions/workflows/cmake.yml/badge.svg)
![clang Build](https://github.com/martelkr/threadpool/actions/workflows/clang.yml/badge.svg)
![profile Build](https://github.com/martelkr/threadpool/actions/workflows/profile.yml/badge.svg)
![cppcheck Build](https://github.com/martelkr/threadpool/actions/workflows/cppcheck.yml/badge.svg)
[![Coverage Status](https://coveralls.io/repos/github/martelkr/threadpool/badge.svg?branch=main)](https://coveralls.io/github/martelkr/threadpool?branch=main)

## About
This is a header file only implementation of a C++20 style thread pool.

Compilation has been tested with:
- GCC 11.3.0 (GNU/Linux Ubuntu 22.04.1 LTS)
	- cmake 3.22.1
	- googletest 1.11.0-3
	- clang 14.0.0-1ubuntu1
- Visual Studio Community 2022 17.4.4 (64-bit) (Windows 11)
	- cmake 3.26.0-rc1
	- googletest 1.13.0

## Usage

Create a thread pool and add jobs to it to be performed by the running threads.

```cpp
void printVal(void)
{
    static int val = 1;
    std::cout << "Thread job: " << val++ << std::endl;
}

auto main(int argc, char* argv[]) -> int 
{
    com::thread::Pool p(1);

    for (auto i = 0; i < 5; ++i)
    {
        p.add(printVal);
    }
    ...
}
```
## Thread Safety

All methods of the thread pool are thread safe.

## Installation

Use the `ThreadPool.hpp` file in your source tree and include it in the file that need to use it.

## Run Unit Tests

Unit tests run with ctest:
```
ctest -VV -C debug
```

## Contribute
All contributions are highly appreciated.
