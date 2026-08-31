.PHONY: test

BUILD_DIR := tests/build

test:
	cmake -S tests -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Release
	cmake --build $(BUILD_DIR) --config Release
	ctest --test-dir $(BUILD_DIR) --build-config Release --output-on-failure
