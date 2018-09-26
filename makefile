# super simple makefile
# call it using 'make NAME=name_of_code_file_without_extension'
# (assumes a .cpp extension)
EXECNAME = "breakout"
NAME = "main"
OBJECTS = main.o breakout.o block.o paddle.o ball.o
SOURCES = main.cpp breakout.cpp block.cpp paddle.cpp ball.cpp

# Add $(MAC_OPT) to the compile line for macOS 
# (should be ignored by Linux, set to nothing if causing problems)
MAC_OPT = -I/opt/X11/include 

all:
	@echo "Compiling..."
	g++ -std=c++11 -c -L/opt/X11/lib -lX11 -lstdc++ $(SOURCES);
	g++ $(OBJECTS) -std=c++11 -o $(EXECNAME) -L/opt/X11/lib -lX11 -lstdc++ $(MAC_OPT)

run: all
	@echo "Running..."
	./$(EXECNAME) 

clean:
	-rm -f *.o
