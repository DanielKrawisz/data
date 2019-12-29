FROM ubuntu:bionic
RUN apt-get update && \
	apt-get install -y build-essential git cmake autoconf libtool pkg-config libcrypto++ wget
WORKDIR /
RUN git clone https://github.com/hanickadot/compile-time-regular-expressions.git ctre
WORKDIR ctre/build
RUN cmake ..
RUN make
RUN make install
WORKDIR /
RUN git clone --recurse-submodules -j8 https://github.com/boostorg/boost.git boost 
WORKDIR boost
RUN ./bootstrap.sh
RUN ./b2 install
RUN mkdir -p /src
WORKDIR /src
COPY . .
WORKDIR /src/build
RUN cmake ..
RUN make
