.PHONY: build

IMAGE_DEBIAN_NAME := local/maestro-debian-example:v1
build-example-debian-image: 
	@docker build \
		-f example/Dockerfile.debian \
		-t $(IMAGE_DEBIAN_NAME) \
		.

IMAGE_ALPINE_NAME := local/maestro-alpine-example:v1
build-example-alpine-image: 
	@docker build \
		-f example/Dockerfile.alpine \
		-t $(IMAGE_ALPINE_NAME) \
		.

binary:
	@cd .. && node-gyp rebuild maestro-node

all:
	@cd .. && node-gyp build && node-gyp rebuild
