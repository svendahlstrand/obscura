.PHONY: clean

./bin/obscura: main.c
	mkdir -p ./bin/
	gcc -std=c99 -Wall -Wextra -Wpedantic -Werror $< -o $@

clean:
	rm -rf ./bin/
