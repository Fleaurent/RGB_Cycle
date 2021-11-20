.PHONY: hello build_image publish_image create_doxygen_config doxygen

hello:
	@echo "hello"

build_image:
	docker build -t doxygen_image .
	
publish_image:
	docker build -t ghcr.io/fleaurent/doxygen_image:latest .
	docker push ghcr.io/fleaurent/doxygen_image:latest
	
create_doxygen_config:
	docker run --rm -v $(pwd):/tmp doxygen_image doxygen -g
	
doxygen:
	docker run --rm -v $(pwd):/tmp doxygen_image doxygen Doxyfile