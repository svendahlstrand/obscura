.PHONY: all test clean

all: ./bin/obscura

./bin/obscura: ./src/main.c
	mkdir -p ./bin/
	gcc -std=c99 -Wall -Wextra -Wpedantic -Werror $< -o $@

test: ./bin/obscura
	./test/run

clean:
	rm -rf ./bin/
