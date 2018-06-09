
#使用变量
objects = HelloWorld.o

hello: $(objects)
	gcc -o HelloWorld $(objects)

HelloWorld.o: HelloWorld.c
	gcc -c HelloWorld.c

clean:
	rm HelloWorld $(objects)