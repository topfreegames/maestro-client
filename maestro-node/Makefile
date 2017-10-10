.PHONY: build

IMAGE_NAME := local/maestro-example:v1
build-example-image: 
	@docker build \
		-f example/Dockerfile \
		-t $(IMAGE_NAME) \
		.

binary:
	@cd .. && node-gyp rebuild maestro-node

all:
	@cd .. && node-gyp build && node-gyp rebuild
