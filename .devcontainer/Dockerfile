# Image Variables
ARG UBUNTU_MAJOR="22"
ARG CUDA_MAJOR="12"
ARG CUDA_MINOR="2"
ARG CUDA_PATCH="2"
ARG CUDNN_VERSION="8"

# Base Image
FROM nvcr.io/nvidia/cuda:${CUDA_MAJOR}.${CUDA_MINOR}.${CUDA_PATCH}-cudnn${CUDNN_VERSION}-devel-ubuntu${UBUNTU_MAJOR}.04

# Set Non-Interactive Mode
ARG DEBIAN_FRONTEND=noninteractive

# Install apt packages.
RUN apt-get update -y
RUN apt-get upgrade -y
RUN apt-get update && apt-get install --no-install-recommends -y curl cmake \
    build-essential wget openssl git libeigen3-dev iputils-ping \
    build-essential gdb less udev zstd sudo libgomp1 libswscale-dev \
    libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev tzdata \
    libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev tzdata net-tools \
    yasm libatlas-base-dev gfortran libpq-dev libavutil-dev libpostproc-dev \
    libxine2-dev libglew-dev libtiff5-dev zlib1g-dev libpcl-dev nano libgomp1 \
    gcc-10

    # Set Timezone
RUN echo "${TZ}" > /etc/localtime && \
    echo "${TZ}" > /etc/timezone

# Install gcc/g++
RUN apt-get install  --no-install-recommends -y \
    gcc-12 g++-12 && \
    rm -rf /var/lib/apt/lists/* && \
    update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-12 100 --slave /usr/bin/g++ \
    g++ /usr/bin/g++-12 --slave /usr/bin/gcov gcov /usr/bin/gcov-12

# Install CMake
ARG CMAKE_VERSION="3.27.9"
COPY install-cmake.sh /tmp/
RUN if [ "${CMAKE_VERSION}" != "none" ]; then \
    chmod +x /tmp/install-cmake.sh && /tmp/install-cmake.sh ${CMAKE_VERSION}; \
    fi && \
    rm -f /tmp/install-cmake.sh

# Set Working Directory
WORKDIR /opt

# Install prebuilt OpenCV.
ARG OPENCV_VERSION="4.9.0"
RUN wget -q https://github.com/MissouriMRDT/Autonomy_Packages/raw/main/opencv/amd64/opencv_${OPENCV_VERSION}_amd64.deb && \
    dpkg -i opencv_${OPENCV_VERSION}_amd64.deb && \
    rm opencv_${OPENCV_VERSION}_amd64.deb