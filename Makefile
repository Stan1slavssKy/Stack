SOURCE  = main.cpp stack.cpp protection.cpp
OBJECT  = main.o   stack.o   protection.o
OUTPUT  = stack
LIBRARY = libstack.a

default: $(OUTPUT) library

$(OUTPUT): $(OBJECT)
	g++ $(OBJECT) -o $(OUTPUT)
	./$(OUTPUT)

main.o: main.cpp
	g++ main.cpp -c

stack.o: stack.cpp
	g++ stack.cpp -c

protection.o: protection.cpp
	g++ protection.cpp -c

library: $(OBJECT)
	ar cr $(LIBRARY) $(OBJECT)

clean:
	rm -f *.o *.a $(OUTPUT)

#прочитать про директиву .PHONY