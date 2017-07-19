.PHONY: clean

./bin/obscura: main.c
	mkdir -p ./bin/
	gcc -Wall -Wextra -Wpedantic -Werror $< -o $@

clean:
	rm -rf ./bin/
