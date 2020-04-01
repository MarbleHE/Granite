# CLion remote docker environment (How to build docker container, run and stop it)
#
# Build and run:
#   docker build -t clion/remote-cpp-env:0.5 -f Dockerfile .
#   docker run -d --cap-add sys_ptrace -p127.0.0.1:2222:22 --name clion_remote_env clion/remote-cpp-env:0.5
#   ssh-keygen -f "$HOME/.ssh/known_hosts" -R "[localhost]:2222"
#
# stop:
#   docker stop clion_remote_env
#
# ssh credentials (test user):
#   user@password

FROM ubuntu:16.04 as sheep_base

### get wget git etc

RUN apt-get update; apt-get -y install git
RUN apt-get update; apt-get -y install wget

### get gcc-9 (gcc >=6 needed for SEAL). # 7 produces errors.

RUN apt-get -y install software-properties-common
RUN add-apt-repository -y  ppa:ubuntu-toolchain-r/test
RUN apt-get update; apt-get -y install gcc-9 g++-9
RUN apt-get update; apt-get -y install build-essential

RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 10
RUN update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-9 10
RUN update-alternatives --set gcc /usr/bin/gcc-9
RUN update-alternatives --set g++ /usr/bin/g++-9

###### get fftw3 (needed for TFHE)
RUN apt-get install -y libfftw3-dev

###### build cmake from source (to get a new enough version for SEAL)
RUN wget https://cmake.org/files/v3.15/cmake-3.15.0.tar.gz
RUN tar -xvzf cmake-3.15.0.tar.gz
RUN cd cmake-3.15.0; export CC=gcc-9; export CXX=g++-9; ./bootstrap; make -j8; make install

####### install intel-tbb for parallelisation
RUN apt-get -y install libtbb-dev

###### install PALISADE
RUN apt-get -y install lzip
RUN apt-get -y install flex
RUN apt-get -y install bison
RUN apt-get -y install autoconf
RUN git clone --recursive https://gitlab.com/palisade/palisade-release.git
RUN cd palisade-release; mkdir build; cd build; cmake ..; make -j8; make install

# ###### get gmp (needed for HElib)
RUN apt-get -y install m4
RUN wget https://gmplib.org/download/gmp/gmp-6.1.0.tar.xz
RUN tar -xvf gmp-6.1.0.tar.xz
RUN cd gmp-6.1.0; export CC=gcc-9; export CXX=g++-9; ./configure; make -j8; make install

###### get ntl (needed for HElib)
RUN wget http://www.shoup.net/ntl/ntl-11.1.0.tar.gz
RUN tar -xvzf ntl-11.1.0.tar.gz
RUN cd ntl-11.1.0/src; export CC=gcc-9; export CXX=g++-9; ./configure NTL_GMP_LIP=on NTL_EXCEPTIONS=on; make -j8; make install

###### get cpprestsdk (for the REST API)
#RUN apt-get update
#RUN apt-get -y install libssl-dev
#RUN apt-get -y install libboost-all-dev
#RUN apt-get update
#RUN git clone --recurse-submodules  https://github.com/Microsoft/cpprestsdk.git casablanca
#RUN cd casablanca/Release; mkdir build.debug; cd build.debug; export CC=gcc-9; export CXX=g++-9; cmake .. -DCMAKE_BUILD_TYPE=Debug; make install

###### build SEAL
RUN git clone https://github.com/microsoft/SEAL.git
RUN cd SEAL/native/src; export CC=gcc-9; export CXX=g++-9 ; cmake .; make -j8; make install



###### get and build libpaillier
RUN wget http://hms.isi.jhu.edu/acsc/libpaillier/libpaillier-0.8.tar.gz
RUN tar -xvzf libpaillier-0.8.tar.gz
RUN cd libpaillier-0.8 ; ./configure; make -j8; make install

RUN mkdir -p SHEEP/backend/lib/

###### build TFHE
RUN cd SHEEP/backend/lib; git clone --recurse-submodules https://github.com/tfhe/tfhe.git
RUN cd SHEEP/backend/lib/tfhe; git reset --hard a65271bc8f5f0015c71351ed8746dd8eec051e29
RUN cd SHEEP/backend/lib/tfhe; git submodule update --init

RUN rm -fr SHEEP/backend/lib/tfhe/build
RUN mkdir -p SHEEP/backend/lib/tfhe/build
RUN cd SHEEP/backend/lib/tfhe/build; export CC=gcc-9; export CXX=g++-9; cmake ../src -DENABLE_TESTS=on -DENABLE_FFTW=on -DCMAKE_BUILD_TYPE=optim -DENABLE_NAYUKI_PORTABLE=off -DENABLE_SPQLIOS_AVX=off -DENABLE_SPQLIOS_FMA=off -DENABLE_NAYUKI_AVX=off
RUN cd SHEEP/backend/lib/tfhe/build; make -j8; make install;

###### build HElib
RUN cd SHEEP/backend/lib; git clone --recurse-submodules https://github.com/shaih/HElib.git
RUN cd SHEEP/backend/lib/HElib; git reset --hard 9c50908a3538f5df77df523e525e1f9841f22eb2
RUN cd SHEEP/backend/lib/HElib; git submodule update --init

RUN cd SHEEP/backend/lib/HElib/src ; export CC=gcc-9; export CXX=g++-9; make clean; make -j8;

RUN apt-get update \
  && apt-get install -y ssh \
      build-essential \
      gdb \
      clang \
      rsync \
      python \
  && apt-get clean

RUN ( \
    echo 'LogLevel DEBUG2'; \
    echo 'PermitRootLogin yes'; \
    echo 'PasswordAuthentication yes'; \
    echo 'Subsystem sftp /usr/lib/openssh/sftp-server'; \
  ) > /etc/ssh/sshd_config_test_clion \
  && mkdir /run/sshd

RUN useradd -m user \
  && yes password | passwd user

CMD ["/usr/sbin/sshd", "-D", "-e", "-f", "/etc/ssh/sshd_config_test_clion"]