maestro-client
==============

maestro-client is [maestro's](https://github.com/topfreegames/maestro) framework that should be included in game rooms

## Dependencies
  * [restclient-cpp](https://github.com/mrtazz/restclient-cpp)
  * [boost](http://www.boost.org/)
  * C++11 or newer

## Building
lib and example-room
```
make
```
lib
```
make lib
```
example-room
```
make example-room
```
the ouput files will be at at ```cpplib/lib/libmaestro.a``` and ```example-room/bin/example```

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
