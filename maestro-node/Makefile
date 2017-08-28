.PHONY: build

setup:
	npm install

build:
	./node_modules/node-gyp/bin/node-gyp.js rebuild

build-linux:
	@cp -r ../cpplib cpplib
	@docker build -t local/maestro-node-builder:v1 .
	@rm -r cpplib
	@mkdir -p ./build/Release/linux
	@docker run \
		-v $$(pwd)/build/Release/linux:/app/bin \
		--rm \
		local/maestro-node-builder:v1

IMAGE_NAME := local/maestro-example:v1
MAESTRO_URL := maestro-api.maestro
build-example-image: build-linux
	@cp -r ../cpplib cpplib
	@docker build \
		-f example/Dockerfile \
		-t $(IMAGE_NAME) \
		.
	@rm -r cpplib
