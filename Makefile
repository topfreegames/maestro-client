build-mac:
	@rm -rf _builds/mac
	@conan install . -if _builds/mac
	@cmake -H. -B_builds/mac -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=../deps/restclient-cpp/mac
	@cmake --build _builds/mac

build-mac-unity:
	@rm -rf _builds/mac-unity
	@conan install . -if _builds/mac-unity
	@cmake -H. -B_builds/mac-unity -DCMAKE_BUILD_TYPE=Release -DBUILD_MACOSX_BUNDLE=ON -DCMAKE_PREFIX_PATH=../deps/restclient-cpp/mac
	@cmake --build _builds/mac-unity

build-linux:
	@rm -rf _builds/linux
	@conan install . -if _builds/linux
	@cmake -H. -B_builds/linux -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=../deps/restclient-cpp/linux
	@cmake --build _builds/linux

build-linux-docker:
	@docker run -v $(shell pwd):/app-src -it quay.io/tfgco/maestro-example-builder:v2.0.0 -c "cd /app-src && make build-linux"

build-all: build-mac-unity build-linux-docker
