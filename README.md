# data

Tools and high-level programming constructs in c++, including 
functional data structures, higher-order functions, big numbers, and mathematics. 

See [the standard](doc/standard.md) for all supported features and how to use them. 
The standard will be gradually improved as we work toward a beta version.

Current version is 0.1 beta.

## Features 

  * Functional data structures
    * stack
    * queue
    * list
    * tree
    * map
    * set
    * priority queue
  * Functional programming
    * reduce
    * fold
    * for_each
    * lift
  * Mathematical types
    * signed and unsigned int types of every size.
    * natural numbers
    * integers
    * rational numbers
    * Caylay-Dickson rationals (complex, quaternion, and octonion)
    * prime fields
    * polynomials
    * permutations
    * AKS algorithm
  * Encodings
    * hex
    * base58
    * base64
    
# Requirements
  * GCC v13+
  * GMP 
  
# Dependencies
  * compile-time-regular-expressions https://github.com/hanickadot/compile-time-regular-expressions (included)
  * Google test (only for tests)
  * NTL (built specially)

# Building from Source

Alternatively, the entire build and installation process can be performed using the `build_and_install.sh` script in the repository root.

The script automatically downloads, builds, and installs GF2X and NTL before configuring, building, testing, and installing `data`.

Run:

```bash
./build_and_install.sh
```

Use `./build_and_install.sh --help` to see the available options.

The following sections describe the manual build and installation procedure.

## 1. Check and install dependencies

The following dependencies are required:

| Dependency | Purpose |
|---|---|
| GCC 13 or equivalent | C and C++ compiler with C++20 support |
| CMake | Build configuration |
| Ninja | Build system |
| GMP | Arbitrary-precision integer arithmetic |
| Google Test | Unit testing (optional) |
| Autoconf, Automake, Libtool | Building GF2X |
 
On Ubuntu/Debian, install the system dependencies:

```bash
sudo apt update
sudo apt install \
    build-essential \
    gcc-13 \
    g++-13 \
    cmake \
    ninja-build \
    libgmp-dev \
    autoconf \
    automake \
    libtool \
    git \
    wget \
    tar
```

If you want to build the tests (with `-DPACKAGE_TESTS=ON`), install Google Test:

```bash
sudo apt install libgtest-dev
```

## 2. Build GF2X

We use GF2X 1.3.0, installed under `/opt/ntl`.

Download and extract GF2X:

```bash
wget https://gitlab.inria.fr/gf2x/gf2x/-/archive/gf2x-1.3.0/gf2x-gf2x-1.3.0.tar.gz
tar -xf gf2x-gf2x-1.3.0.tar.gz
cd gf2x-gf2x-1.3.0
```

Generate the configuration script, configure, compile, test, and install:

```bash
autoreconf -i

./configure \
    --prefix=/opt/ntl

make -j$(nproc)
make check
sudo make install
```

## 3. Build NTL

We use NTL 11.6.0, also installed under `/opt/ntl`.

Download and extract NTL:

```bash
wget https://libntl.org/ntl-11.6.0.tar.gz
tar -xf ntl-11.6.0.tar.gz
cd ntl-11.6.0/src
```

Configure NTL to use the system GMP installation and the GF2X installation:

```bash
./configure \
    CXX=g++-13 \
    PREFIX=/opt/ntl \
    GMP_PREFIX=/usr \
    GF2X_PREFIX=/opt/ntl \
    NTL_GF2X_LIB=on \
    SHARED=on
```

Compile, test, and install:

```bash
make -j$(nproc)
make check
sudo make install
```

NTL uses its own `variable=value` configuration syntax rather than the usual Autoconf `--option` syntax.

The `NTL_GF2X_LIB=on` option enables GF2X support.

## 4. CMAKE

CMake provides several configuration options that control how `data` is built and installed.

### Build configuration

**`CMAKE_BUILD_TYPE`**

Specifies the build configuration. The available options are:

| Configuration | Description |
|---|---|
| `Debug` | Includes debugging information and disables most optimizations. |
| `Release` | Enables optimizations for production builds. |
| `RelWithDebInfo` | Enables optimizations while retaining debugging information. |
| `MinSizeRel` | Optimizes for executable size. |

The default configuration used in our build instructions is `Release`.

**`PACKAGE_TESTS`**

Controls whether the test suite is built.

- `ON`: Builds the test suite and downloads Google Test using CMake FetchContent.
- `OFF`: Disables the test suite. Google Test is not required.

The default is `ON`.

### Dependency configuration

**`NTL_PREFIX`**

Specifies the installation prefix containing NTL and GF2X.

The default is `/opt/ntl`.

Both libraries must be installed under this prefix. CMake uses it to locate their headers and libraries.

For example:

```bash
-DNTL_PREFIX="$HOME/local/ntl"
```

### Installation configuration

**`CMAKE_INSTALL_PREFIX`**

Specifies where `data` will be installed.

The default is `/usr/local`.

For example:

```bash
-DCMAKE_INSTALL_PREFIX="$HOME/local"
```

### Compiler configuration

**`CMAKE_C_COMPILER`** and **`CMAKE_CXX_COMPILER`**

Specify the C and C++ compilers used to build the project.

Our reference compiler is GCC 13:

```bash
-DCMAKE_C_COMPILER=gcc-13
-DCMAKE_CXX_COMPILER=g++-13
```

Other compilers supporting the required C++ standard may also be used.

### Configure the project

Once the dependencies have been installed, configure the project using CMake and Ninja:

```bash
cmake -S . -B build -G Ninja \
    -DCMAKE_C_COMPILER=gcc-13 \
    -DCMAKE_CXX_COMPILER=g++-13 \
    -DCMAKE_BUILD_TYPE=Debug \
    -DNTL_PREFIX=/opt/ntl \
    -DCMAKE_INSTALL_PREFIX=/usr/local \
    -DPACKAGE_TESTS=ON
```

## 5. Build and install data

Compile:

```bash
cmake --build build
```

Run the tests:

```bash
ctest --test-dir build --output-on-failure
```

Install:

```bash
sudo cmake --install build
```

By default, CMake installs to `/usr/local`.

To choose another installation directory, specify `CMAKE_INSTALL_PREFIX` during configuration:

```bash
-DCMAKE_INSTALL_PREFIX=/some/installation/path
```
  
## Examples

### Basic Utils

#### Logical types

#### Infinite Limits

### Functional Programming 

#### Functional Data Structures 

#### Higher-Order Functions 

### Combinatorics

#### Figurate Numbers

#### Permutations

#### Iterating over Permutations, subsets, etc

### Linear Algebra

#### constexpr arrays

### Numbers 

#### Fixed-width Integral types

#### N and Z

#### N_bytes and Z_bytes

#### String number types

### Algebra

#### Rational 

#### Modular

#### Cayley Dickson 

#### Polynomials

## For version 1:

This library is an alpha product. This is what is planned for version 1.
  * functional RB map algorithms
    * remove
    * merge
    * intersection
    * complements
  * list works more like Mathematica's List. 
    * inner
    * outer
    * partition
  * use size_in_base in the division algorithm.
  * Polynomial algorithms
    * minimal polynomials.
    * irreducible polynomial.
  * finite fields. 
    * Generate irreducible polynomials for arbitrary finite fields. 
  * geometry
    * Affine space using real, complex, and quaternionic numbers.
    * Euclidian space
    * Projective space
    * Hyperbolic space
    * infinite classes of compact Lie Groups (no exceptionals for now)
