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
2. Clone the repository and its dependency Wool, using:
   ```
   git clone https://github.com/MarbleHE/Granite.git
   git clone https://github.com/MarbleHE/Wool
   ```
3. Install the latest version of Docker.
4. Execute the following commands:
    ```
   cd Wool
   docker pull marblehe/wool_library
   docker tag marblehe/wool_library wool_library
   docker build -t wool_library_ssh -f Dockerfile_wool_library_ssh .
   docker run -d --cap-add sys_ptrace -p 127.0.0.1:2222:22 wool_library_ssh
   ```
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