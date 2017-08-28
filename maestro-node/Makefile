.PHONY: build

IMAGE_NAME := local/maestro-example:v1
build-example-image: 
	@docker build \
		-f example/Dockerfile \
		-t $(IMAGE_NAME) \
		.
