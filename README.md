<div align="center">
  <img width="187" src="https://user-images.githubusercontent.com/93570324/234292265-889228a8-7a68-4e2d-b891-f75262410af1.png"/>
</div>

<p align="center">
    <b>The AI-native database for next-generation AI apps</b>
</p>

## What is Infinity

Infinity is an AI-native database that can be used with large language models (LLMs) to meet typical To B (business-to-business) applications such as internal enterprise search, specific industry search, internal AI assistants, and chatbots. 

## Key features

- Mixed data type query: Besides embeddings generated by LLMs, Infinity also stores structured and semi-structured data and supports mixed data type queries on top of them. 

- Multi-collection query: Traditional vector databases only supports queries on one collection. 

- Multi-embedding query

- Unified APIs

- High concurrency query

## Build Infinity from source
### Requirements

-   Operating system: Ubuntu 22.04,  openSUSE Tumbleweed 20230828 or higher

-   GCC 13 / Clang-18 or higher to support C++23

-   CMake 3.10 or higher

-   Git

### Step1 Download Infinity source code

```shell
$ git clone https://github.com/infiniflow/infinity.git
```

### Step2 Install dependency

On Ubuntu 22.4 to 23.10,
```shell
# Clang 17 or 18 is required. GCC is not supported.
$ apt install clang-*-17
$ ln -s /usr/lib/llvm-17/bin/clang-scan-deps /usr/bin/clang-scan-deps

# CMake 3.28+ is requrired.
$ wget https://github.com/Kitware/CMake/releases/download/v3.28.0-rc5/cmake-3.28.0-rc5-linux-x86_64.tar.gz
$ tar xzvf cmake-3.28.0-rc5-linux-x86_64.tar.gz
$ sudo cp -r cmake-3.28.0-rc5-linux-x86_64/bin/* /usr/local/bin/

$ sudo apt install make ninja-build bison flex libomp-17-dev libblas-dev liblapack-dev libboost1.81-dev liburing-dev libgflags-dev libleveldb-dev

# iresearch requires lz4
$ git clone https://github.com/lz4/lz4.git
$ cd lz4
$ make
$ sudo make install
$ export LZ4_ROOT=/usr/local

# dependencies for thrift
$ sudo apt install thrift-compiler

```

### Step3 Build source code

```shell
$ git config --global --add safe.directory infinity
$ cd infinity && mkdir build && cd build
$ export CC=/usr/bin/clang-18
$ export CXX=/usr/bin/clang++-18
$ cmake -G Ninja ..
$ ninja -j 12
```

### Step4 Start up Infinity server

```shell
$ ./src/infinity_main
```

## Python SDK develop
### how to generate thrift rpc code
```shell
$ cd tools && python generate_rpc_code.py
```