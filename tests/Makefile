PROJ_DIR := .
BUILD := build

$(BUILD):
	mkdir $(BUILD)

clean:
	rm -rf $(BUILD)

all: clean $(BUILD)
	cd $(BUILD) && cmake ..
	make -C $(BUILD) vertices_sdk_tests