# Start with the base Ubuntu image
FROM ubuntu:20.04

# Avoid prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Set the timezone
ENV TZ=UTC
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# Install necessary dependencies
RUN apt-get update -yqq \
    && apt-get install -yqq --no-install-recommends \
    software-properties-common \
    sudo curl wget cmake make pkg-config locales git \
    && add-apt-repository ppa:ubuntu-toolchain-r/test \
    && apt-get update -yqq \
    && apt-get install -yqq --no-install-recommends \
    gcc-10 g++-10 openssl libssl-dev libjsoncpp-dev uuid-dev \
    zlib1g-dev libc-ares-dev postgresql-server-dev-all \
    libmariadb-dev libsqlite3-dev libhiredis-dev \
    && rm -rf /var/lib/apt/lists/* \
    && locale-gen en_US.UTF-8

# Set environment variables for localization
ENV LANG=en_US.UTF-8 \
    LANGUAGE=en_US:en \
    LC_ALL=en_US.UTF-8 \
    CC=gcc-10 \
    CXX=g++-10 \
    AR=gcc-ar-10 \
    RANLIB=gcc-ranlib-10 \
    IROOT=/install

# Clone Drogon repository - use a specific version tag for stability
ENV DROGON_ROOT="$IROOT/drogon"
RUN git clone --depth=1 --branch v1.7.5 https://github.com/drogonframework/drogon $DROGON_ROOT

# Initialize and update git submodules
WORKDIR $DROGON_ROOT
RUN git submodule update --init

# Create a simple build script
RUN echo '#!/bin/bash\n\
mkdir -p build && cd build\n\
cmake .. -DBUILD_TESTING=OFF -DBUILD_EXAMPLES=OFF -DCMAKE_BUILD_TYPE=Release -DBUILD_DROGON_SHARED=OFF -DBUILD_TRANTOR_SHARED=OFF\n\
make -j$(nproc)\n\
make install\n\
' > build_custom.sh && chmod +x build_custom.sh

# Build Drogon with custom build script
RUN ./build_custom.sh

# Create app directory
WORKDIR /app

# For development, we'll mount the source code instead of copying it
CMD ["/bin/bash"]
