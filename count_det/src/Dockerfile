FROM ubuntu:20.04
WORKDIR /compiler
RUN apt-get -y update && apt-get -y install wget && apt-get -y install xz-utils && apt-get -y install qemu-user \
    && wget https://releases.linaro.org/components/toolchain/binaries/7.3-2018.05/arm-linux-gnueabi/gcc-linaro-7.3.1-2018.05-x86_64_arm-linux-gnueabi.tar.xz \
    && wget https://releases.linaro.org/components/toolchain/binaries/7.3-2018.05/arm-linux-gnueabi/sysroot-glibc-linaro-2.25-2018.05-arm-linux-gnueabi.tar.xz \
    && tar -xf gcc-linaro-7.3.1-2018.05-x86_64_arm-linux-gnueabi.tar.xz \
    && tar -xf sysroot-glibc-linaro-2.25-2018.05-arm-linux-gnueabi.tar.xz \
    && mv gcc-linaro-7.3.1-2018.05-x86_64_arm-linux-gnueabi gcc-linaro \
    && mv sysroot-glibc-linaro-2.25-2018.05-arm-linux-gnueabi sysroot
ENV PATH="/compiler/gcc-linaro/bin:${PATH}"
WORKDIR /app
COPY . .
RUN arm-linux-gnueabi-gcc -marm main.c main.s
CMD qemu-arm -L /compiler/sysroot ./a.out
