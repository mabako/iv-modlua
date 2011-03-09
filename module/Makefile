GCC = g++
CFLAGS = -m32 -shared
LDFLAGS = 

OUTFILE = helloworld.so
OBJ = Main.o Functions.o SDK/SDK.o

kwlbot: $(OBJ)
	$(GCC) $(CFLAGS) -o $(OUTFILE) $(OBJ) $(LDFLAGS)
	strip --strip-unneeded $(OUTFILE)

%.o: %.c
	$(GCC) $(CFLAGS) -Wall -fPIC -c $<

clean:
	rm -f $(OBJ)
