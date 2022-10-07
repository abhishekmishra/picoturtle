FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

#RUN apt-get update && apt-get install -y \
#  bzr \
#  cvs \
#  git \
#  mercurial \
#  subversion \
#  && rm -rf /var/lib/apt/lists/*

RUN apt-get update
RUN apt-get install -y git g++
# RUN rm -rf /var/lib/apt/lists/*

WORKDIR /

RUN git clone https://github.com/Microsoft/vcpkg.git

RUN apt-get install curl zip unzip tar -y

ENV VCPKG_DISABLE_METRICS 1

RUN /vcpkg/bootstrap-vcpkg.sh

RUN apt-get install pkg-config -y

RUN /vcpkg/vcpkg install graphicsmagick

RUN /vcpkg/vcpkg install sdl2-image

WORKDIR /app

COPY . .
