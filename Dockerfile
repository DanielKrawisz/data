FROM gigamonkey/gigamonkey-base-dev:v1.1.3

WORKDIR /home/data
COPY . .
RUN mkdir build
WORKDIR /home/data/build
RUN cmake -DPACKAGE_TESTS=OFF ../
RUN cmake --build .
RUN cmake --install .