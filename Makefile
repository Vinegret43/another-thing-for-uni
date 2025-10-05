a.out: main.cpp lyrics.cpp generators.cpp config.cpp
	LANG=en_us g++ main.cpp
run: a.out
	./a.out
