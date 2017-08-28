RESTCLIENT_VERSION=0.4.4
boost_version=1.63.0
boost_dir=boost_1_63_0

if [ $(uname) == "Darwin" ]; then
  brew tap mrtazz/oss
  brew install restclient-cpp boost
else
  dir=$(pwd)
  apt-get update
  apt-get -y install build-essential cmake libboost-dev curl autoconf autogen \ 
    libtool libcurl4-gnutls-dev 
  cd /tmp 
  curl -L -o restclient.tar.gz https://github.com/mrtazz/restclient-cpp/archive/$RESTCLIENT_VERSION.tar.gz 
  tar -zxf restclient.tar.gz && cd restclient-cpp-$RESTCLIENT_VERSION && ./autogen.sh
  ./configure 
  make install 
  rm -rf /tmp /var/lib/apt
  cd $dir
fi

npm install -g node-gyp
node-gyp rebuild
