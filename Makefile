.PHONY: all
all: bench

.PHONY: clean
clean:
	rm -rf build
	rm -f graphs/*.dot
	rm -f results/*.dot

.PHONY: build
build: clean
	cmake -B build -DCMAKE_BUILD_TYPE=Release
	cmake --build build -j$(nproc)

.PHONY: bench
bench: build
	python3 scripts/benchmark.py 10 60

.PHONY: bench-with-bt
bench-with-bt: build
	python3 scripts/benchmark.py 10 60 --include-bt

.PHONY: profile-build
profile-build:
	cmake -B build -DCMAKE_BUILD_TYPE=Release -DENABLE_PROFILING=ON
	cmake --build build -j$(nproc)

.PHONY: profile
profile: profile-build
	python3 scripts/profile.py $(filter-out $@,$(MAKECMDGOALS))

%:
	@: