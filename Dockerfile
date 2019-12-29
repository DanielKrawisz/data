FROM ubuntu:bionic
RUN apt-get update && \
	apt-get install -y build-essential git cmake autoconf libtool pkg-config libboost-all-dev libcrypto++ wget curl unzip tar
RUN git clone https://github.com/Microsoft/vcpkg.git
WORKDIR vcpkg
RUN ./bootstrap-vcpkg.sh
RUN ./vcpkg integrate install
RUN ./vcpkg install ctre
RUN mkdir -p /src
WORKDIR /src
COPY . .
WORKDIR /src/build
RUN cmake ..
