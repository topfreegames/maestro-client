FROM ubuntu:xenial
ARG RESTCLIENT_VERSION=0.4.4
ARG boost_version=1.63.0
ARG boost_dir=boost_1_63_0
ENV boost_version ${boost_version}
COPY . /app-src
WORKDIR /app-src
RUN apt-get update && apt-get -y install build-essential libboost-dev cmake curl wget autoconf autogen libtool \
    && cd /tmp && curl -L -o restclient.tar.gz https://github.com/mrtazz/restclient-cpp/archive/$RESTCLIENT_VERSION.tar.gz \
    && tar -zxf restclient.tar.gz && cd restclient-cpp-$RESTCLIENT_VERSION && ./autogen.sh \
    && ./configure && make install \
    && cd /tmp && wget http://www.cmake.org/files/v3.2/cmake-3.2.2.tar.gz \
    && cd /app-src && cmake . \
    && make \
    && rm -rf /tmp/* \
    && rm -rf /var/lib/apt/lists \
    && mkdir /app \
    && mv bin/example /app/example \
    && rm -rf /app-src
WORKDIR /app
CMD ["/bin/bash"]
