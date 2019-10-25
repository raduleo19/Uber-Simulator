
build:
	g++ --std=c++14 -Wall -Wextra -Wpedantic main.cpp -o tema2

.PHONY: clean

run:
	./main

clean:
	rm -r out
	rm tema2
	rm time.out
