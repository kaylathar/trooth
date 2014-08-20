test: ../src/BigInt.o BigInt_Test.o
	gcc -o Test ../src/BigInt.o BigInt_Test.o

BigInt_Test.o: BigInt_Test.c ../includes/BigInt.h
	gcc -I../includes/ -c BigInt_Test.c

clean:
	rm Test *.o
