PROJ_DIR := .
BUILD := build

compile: CPPUTEST_BUILD_RULE=start

$(BUILD):
	mkdir $(BUILD)

clean:
	rm -rf $(BUILD)ever

all: clean $(BUILD)
	cd $(BUILD) && cmake ..
	make -C $(BUILD) vertices_sdk_tests
