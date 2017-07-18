.PHONY: clean

./bin/obscura: main.c
	mkdir -p ./bin/
	gcc -Wall $< -o $@

clean:
	rm -rf ./bin/
