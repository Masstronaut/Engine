# Distro
FROM ubuntu:18.04

RUN apt-get update && \
  apt-get upgrade -y && \
  apt-get install -y software-properties-common \
  git \
  subversion \
  tar \
  wget \
  make \
  gnupg2

#install clang 6.0
RUN wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add - && \
  apt-add-repository "deb http://apt.llvm.org/bionic/ llvm-toolchain-bionic-6.0 main" && \
  apt-get update && \
  apt-get install -y clang-6.0 clang-tidy-6.0 clang-format-6.0 libc++1 libc++abi1
#install cmake 3.8.2

RUN ln -s /usr/bin/clang-6.0 /usr/bin/clang && \
  ln -s /usr/bin/clang++-6.0 /usr/bin/clang++ && \
  mkdir /cmake/ && \
  cd /cmake/ && \
  wget https://cmake.org/files/v3.8/cmake-3.8.2.tar.gz && \
  tar -xzvf cmake-3.8.2.tar.gz && \
  cd cmake-3.8.2/ && \
  env CC=clang && \
  env CX=clang && \
  ./bootstrap && \
  make -j4 && \
  make install


# Checkout and build libc++ to ensure full standard library support
#RUN mkdir /libc++/ && \
#  cd /libc++/ && \
#  svn co http://llvm.org/svn/llvm-project/llvm/trunk llvm && \
#  cd llvm/projects && \
#  svn co http://llvm.org/svn/llvm-project/libcxx/trunk libcxx && \
#  svn co http://llvm.org/svn/llvm-project/libcxxabi/trunk libcxxabi && \
#  mkdir /libc++/build/ && \
#  cd /libc++/build/ && \
#  cmake -G"Unix Makefiles" -DCMAKE_INSTALL_PREFIX=/usr/include/libc++ -DCMAKE_C_COMPILER=clang-6.0 -DCMAKE_CXX_COMPILER=clang++-6.0 ../llvm/ && \
#  make cxx && \
#  make install-cxx install-cxxabi

# library dependencies
RUN apt-get install -y libassimp-dev \
  libglfw3-dev \
  libgtest-dev \
  libfreetype6-dev \
  zlib1g-dev \
  libboost-all-dev \
  libssl-dev \
  libglu-dev && \
  cd / && \
  git clone https://github.com/Microsoft/cpprestsdk.git casablanca && \
  mkdir casablanca/Release/build.release && \
  cd casablanca/Release/build.release && \
  cmake ../ -DCMAKE_BUILD_TYPE=Release && \
  make && \
  make install


#remove before deployment
RUN apt-get install -y nano

RUN ln -s /usr/bin/clang-format-6.0 /usr/bin/clang-format && \
  ln -s /usr/bin/clang-tidy-6.0 /usr/bin/clang-tidy 