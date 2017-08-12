# Maestro-Client Node

This is a node wrapper for client.cpp

## Usage
As example of how to use it, look in example/app.js

## Build
For osx platform, follow these steps to build:

* Download npm dependencies
```
make setup
```

* Build node dependency
```
make build
```

* The binary dependency will be ./build/Release/maestro.node, import it with `require`.

For linux platform, the next command creates maestro.node binary file at `./build/Release/linux`
```
make build-linux
```

To create an example image that uses maestro.node, run:
```
make build-example-linux
```
