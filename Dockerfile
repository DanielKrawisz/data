# use this command to run the docker file with tests
#   docker build -f Dockerfile.test -t test .

FROM gigamonkey/gigamonkey-base-dev:v2.2.1

WORKDIR /home/data
COPY . .
RUN mkdir build
WORKDIR /home/data/build
RUN cmake -G Ninja -DPACKAGE_TESTS=OFF ../
RUN cmake --build . -j 4
RUN cmake --install .
