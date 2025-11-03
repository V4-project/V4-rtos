.PHONY: help build test clean format esp32c6

help:
	@echo "V4 RTOS Build System"
	@echo ""
	@echo "Targets:"
	@echo "  build       - Build all components"
	@echo "  test        - Run all tests"
	@echo "  clean       - Clean build artifacts"
	@echo "  format      - Format all source code"
	@echo "  esp32c6     - Build ESP32-C6 hello-rtos example"
	@echo ""

build:
	cmake -B build -DCMAKE_BUILD_TYPE=Release
	cmake --build build -j

test:
	cmake -B build -DCMAKE_BUILD_TYPE=Debug -DV4_BUILD_TESTS=ON
	cmake --build build -j
	cd build && ctest --output-on-failure

clean:
	rm -rf build

format:
	find kernel hal compiler shell protocol -name '*.c' -o -name '*.cpp' -o -name '*.h' -o -name '*.hpp' | xargs clang-format -i
	find . -name CMakeLists.txt | xargs cmake-format -i

esp32c6:
	cd bsp/esp32c6/examples/nanoc6/hello-rtos && idf.py build
