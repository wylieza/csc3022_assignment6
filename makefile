viteration: viteration.cpp
	g++ viteration.cpp -o viteration -std=c++11

clean:
	rm -rf viteration results.txt

run: viteration
	./viteration