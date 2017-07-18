.PHONY: clean

./bin/gb-develop: main.c
	mkdir -p ./bin/
	gcc -Wall $< -o $@

clean:
	rm -rf ./bin/
