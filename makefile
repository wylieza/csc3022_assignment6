viteration: viteration.cpp
	g++ viteration.cpp -o viteration -std=c++11

clean:
	rm -rf viteration

run: viteration
	./viteration