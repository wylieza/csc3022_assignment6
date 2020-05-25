viteration: viteration.cpp
	g++ viteration.cpp -o viteration -std=c++11

clean:
	rm -rf viteration optimal_values.txt

run: viteration
	./viteration