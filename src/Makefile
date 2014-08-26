BigInt.o: BigInt.c ../includes/util.h ../includes/BigInt.h Environment.o
	gcc -I../includes/ -c BigInt.c

Environment.o: Environment.c ../includes/util.h
	gcc -I../includes/ -I../internal/ -c Environment.c

clean:
	rm *.o
