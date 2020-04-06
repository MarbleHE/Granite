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
   git clone --recurse-submodules https://github.com/MarbleHE/Granite.git
   ```
3. Install the latest version of Docker.
4. Execute the following commands:
    ```
    docker build -t granite_ssh -f Dockerfile .
    docker run -d --cap-add sys_ptrace -p127.0.0.1:2222:22 --name granite_ssh granite_ssh
    ssh-keygen -f "$HOME/.ssh/known_hosts" -R "[localhost]:2222"
    ```
    The last command is expected to fail, if the host is not already known. So error messages can be ignored there.
5. Go to CLion > Preferences > Toolchains
6. Add a new Remote Host with the Name: Docker_Granite, credentials are Host: localhost, Port: 2222, User name: user, Password: password
7. Select the correct CMake in path: /cmake-3.15.0/bin/cmake
8. Go to CLion > Preferences > CMake
9. Change Debug-Docker's Toolchain to Docker
10. Reload CMakeLists.txt in Granite root folder

## Troubleshooting
If you encounter strange errors of targets being missing or not recompiling, try Build > Clean, or delete cmake-build-debug-docker in the root folder and reload the root CMakeLists.txt, or delete the .idea file in the root folder and restart CLion. (This means, you will have to change the CMake toolchain from default to Docker again. Also you will loose any other setting for the local project.)
## Benchmark
Look at the examples in folder Benchmark.
In the results folder, there is a convenient script for plotting.

To get the results from the Docker container, use Tools > Deployment > Browse Remote Host and connect to the container via ssh again.

The results from the example are stored in the folder tmp/tmp.lyA63QWUHt/Benchmark/results

They are not synced automatically.
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