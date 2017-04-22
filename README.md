maestro-client
==============

maestro-client is [maestro's](https://github.com/topfreegames/maestro) framework that should be included in game rooms

## Dependencies
  * [restclient-cpp](https://github.com/mrtazz/restclient-cpp)
  * [boost](http://www.boost.org/)
  * [cmake](https://cmake.org) for generating the Makefiles
  * C++11 or newer for building

## Building
generate the makefiles using cmake
```
cmake .
```
then make the lib and example room with make
```
make
```
if everything went well then, the ouput files should be at at ```lib/libmaestro-client.a``` and ```bin/example```

##### Docker image to build for linux
There's also a docker image available to build the lib and the room-example for linux if you are running OS X for example or simply don't want to install the dependencies to compile or haven't got it to work, use it like that:
```
$ docker run -v $(pwd):/app-src -it quay.io/tfgco/maestro-example-builder:v1.0.0
## inside the container
$ rm -f CMakeCache.txt
$ cmake .
$ make
```
building the image for the builder:
```
docker build -f Dockerfile-builder -t maestro-example-builder .
```
exit from the container and, as before, if everything went well then, the ouput files should be at at ```lib/libmaestro-client.a``` and ```bin/example``` but will only work on linux.

## Release Map

* ### Milestone 1

- [ ] maestro-client
    - [ ] docs
    - [x] fake http server
    - [x] configuration (maestro url / ping interval)
    - [x] http client
    - [x] polling to retrieve (host / port)
    - [ ] deal with connection errors
    - [x] catch sigterm/sigkill and handle graceful shutdown
    - [ ] unity support
