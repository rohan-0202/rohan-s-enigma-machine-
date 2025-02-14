bin:
	mkdir -p bin

enigma: bin
	g++ enigma.cpp -o bin/enigma

clean:
	rm -rf bin

enigma2: bin
	g++ enigma2.cpp -o bin/enigma2

clean2:
	rm -rf bin
