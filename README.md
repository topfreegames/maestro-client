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
