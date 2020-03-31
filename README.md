# Granite

Granite is a framework containing libraries to support converting plaintext code into homomorphic encrypted computations with only little modification and knowledge of homomorphic encryption. It supports many different libraries by using SHEEP as a backend.

## High level design
![High level design](hld.jpg?raw=true "High level design")

Granite contains the main interface, which the user will use to write the plaintext computation. Its job is to step through the code and build an AST.

The middle part, Wool, will take that AST and transform it to a SHEEP circuit.

SHEEP will then be used to evaluate those circuits on any of its supported library.

### Granite

### Wool

### SHEEP

## Installation (Recommended)
1. Install the latest version of CLion.
2. Clone the repository, using:
   ```
   git clone --recurse-submodules -j8 https://github.com/MarbleHE/Granite.git
   ```
3. Install the latest version of Docker.
4. Execute the following commands:
    ```
    docker build -t clion/remote-cpp-env:0.5 -f Dockerfile.remote-cpp-env .
    docker run -d --cap-add sys_ptrace -p127.0.0.1:2222:22 --name clion_remote_env clion/remote-cpp-env:0.5
    ssh-keygen -f "$HOME/.ssh/known_hosts" -R "[localhost]:2222"
    ```
5. Go to CLion > Preferences > Toolchains
6. Add a new Remote Host with the Name: Docker_Granite, credentials are Host: localhost, Port: 2222, User name: user, Password: password
7. Select the correct CMake in path: /cmake-3.15.0/bin/cmake
8. Go to CLion > Preferences > CMake
9. Change Debug-Docker's Toolchain to Docker
10. Reload CMakeLists.txt in Granite root folder
## Installation (Manual, not tested)
Clone the repository, using:
```
git clone --recurse-submodules -j8 https://github.com/MarbleHE/Granite.git
```
Then install the following dependencies.
### SHEEP
```
brew install tbb
```

or for apt-based Linux installations:
```
apt-get -y install libtbb-dev
```
#### TFHE
To build TFHE, first install the fftw library (download source from http://www.fftw.org/download.html then follow the instructions on that page)

Then build TFHE as follows:
```
cd Wool/lib/SHEEP/backend/lib/tfhe
mkdir build
cd build
cmake ../src -DENABLE_TESTS=on -DENABLE_FFTW=on -DCMAKE_BUILD_TYPE=optim -DENABLE_NAYUKI_PORTABLE=off -DENABLE_SPQLIOS_AVX=off -DENABLE_SPQLIOS_FMA=off -DENABLE_NAYUKI_AVX=off
make
sudo make install
```

#### HElib
HElib depends on the GMP and NTL libraries. To install gmp on OSX do:
```
brew install gmp
```

this will install into /usr/local/Cellar/gmp - this location is needed when building ntl.)

For ntl, use `brew install ntl`??? or download and install from official resources, where at the ./configure step, add the argument GMP_PREFIX=/usr/local/Cellar/gmp if you installed gmp using homebrew, or the relevant location if you used another method.

Then build HElib as follows:
```
cd ../../HElib/src
make
mkdir build
cd build
cmake ..
make all
```