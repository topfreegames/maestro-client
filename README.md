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

## How to integrate maestro in your game room

### Unity instructions
* Include the files in folder maestro-unity into your Assets folder
* Initialize maestro like: MaestroClient.Initialize ("http://localhost:5000");
* You will have to keep calling the following methods to report the room's status to maestro:
  - RoomReady() //Every time the room is ready to receive new players, e.g. on the init or when a match has just ended
  - RoomOccupied() //When a match is happenning on this room
  - RoomTerminated() //When a room is to die (your room must die gracefully, you must catch SIGKILL and SIGTERM status and only let the room die when Maestro::RoomTerminated is called)
  - RoomTerminatind() //This should be the first method you call when the room is to die, it's to be called when a gracefull shutdown will occur
* You should also call Ping() method periodically, so that maestro knows that your room server is alive. Call it for example every 30 seconds after the first RoomReady (on room initialization)

### C++ instructions
* Same as unity, using the c++ sources inside cpplib directly or compiling the shared library with the instructions above.

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
    - [x] unity support
    - [ ] tests
    - [x] error handling
