FROM gigamonkey/gigamonkey-base-dev:v2.1

WORKDIR /home/data
COPY . .
RUN mkdir build
WORKDIR /home/data/build
RUN cmake -G Ninja -DPACKAGE_TESTS=OFF ../
RUN cmake --build . -j 4
RUN cmake --install .
