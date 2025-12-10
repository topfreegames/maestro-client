
install:
	@conan install . -of _builds/conan -u

clean:
	@rm -rf _builds

build-mac:
	@rm -rf _builds/mac
	@cmake -H. -B_builds/mac -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=../deps/restclient-cpp/mac
	@cmake --build _builds/mac

build-mac-unity:
	@rm -rf _builds/mac-unity
	@cmake -H. -B_builds/mac-unity -DCMAKE_BUILD_TYPE=Release -DBUILD_MACOSX_BUNDLE=ON -DCMAKE_PREFIX_PATH=../deps/restclient-cpp/mac
	@cmake --build _builds/mac-unity

build-linux:
	@rm -rf _builds/linux
	@conan profile detect -f
	@conan install . -of _builds/linux -u -pr:b=default -pr:h=./profiles/linux
	@cmake -H. -B_builds/linux -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=../deps/restclient-cpp/linux -DBOOST_FIND_POLICY=OLD
	@cmake --build _builds/linux

build-linux-docker:
	@docker run -v $(shell pwd):/app-src -it quay.io/tfgco/maestro-example-builder -c "cd /app-src && make build-linux"

build-all: build-mac-unity build-linux-docker
