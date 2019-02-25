FROM ubuntu:16.04

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update\
 && apt-get install -y --no-install-recommends\
 g++\
 make\
 cmake\
 uuid-dev\
 libboost-filesystem-dev\
 omniorb-nameserver\
 libomniorb4-dev\
 omniidl\
 && apt-get clean\
 && rm -rf /var/lib/apt/lists/*

COPY OpenRTM-aist /root/OpenRTM-aist
WORKDIR /root/OpenRTM-aist
RUN ln -s . src/lib/coil/common/coil\
 && ln -s ../../build/yat.py utils/rtm-skelwrapper/yat.py
RUN mkdir build_openrtm && cd build_openrtm\
 && cmake -DCORBA=omniORB ..\
 && make -j$(nproc)