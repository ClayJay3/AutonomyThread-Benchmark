FROM ubuntu:22.04

# Install apt packages.
RUN apt-get update -y
RUN apt-get upgrade -y
RUN apt-get install -y curl cmake build-essential

# Install prebuilt OpenCV.
ARG OPENCV_VERSION="4.9.0"
RUN wget -q https://github.com/MissouriMRDT/Autonomy_Packages/raw/main/opencv/amd64/opencv_${OPENCV_VERSION}_amd64.deb && \
    dpkg -i opencv_${OPENCV_VERSION}_amd64.deb && \
    rm opencv_${OPENCV_VERSION}_amd64.deb