# set the C compiler, for C++ use g++
# use this setting on most sane machines
CC = gcc

# use this line on the engin linux machines
# CC = gcc296

# set the path to the include directory
INCDIR =../include

# set the flags for the C and C++ compiler to give lots of warnings
CFLAGS = -I$(INCDIR) -O2 -Wall -Wstrict-prototypes -Wnested-externs -Wmissing-prototypes -Wmissing-declarations
CPPFLAGS = $(CFLAGS)

# path to the object file directory
ODIR = obj

# path to the lib directory
LIBDIR =../lib

# path to the bin directory
BINDIR =../bin

# libraries to include
LIBS = -lm -limageIO
LFLAGS = -L$(LIBDIR)

# put all of the relevant include files here
_DEPS = graphics.h

# convert them to point to the right place
DEPS = $(patsubst %,$(INCDIR)/%,$(_DEPS))

# put a list of the executables here
EXECUTABLES = test5a test5d test5b test5c test5vt part4cb space part5cb test6a test6b cube christest test7a test7b test7c teapot jetman test6sphere test8a cubism

# put a list of all the object files here for all executables (with .o endings)
_OBJ = test5a.o test5d.o test5b.o test5c.o test5vt.o part4cb.o space.o part5cb.o test6a.o test6b.o cube.o christest.o test7a.o test7b.0 test7c.o teapot.o jetman.o test6sphere test8a.o cubism.o

# convert them to point to the right place
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

# patterns for compiling source code
# $< is the file that caused the action to occur
$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: %.C $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


all:
	for i in $(EXECUTABLES); do (make $$i) ; done


# put the specific executable rules here
test5a: $(ODIR)/test5a.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test5d: $(ODIR)/test5d.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test5b: $(ODIR)/test5b.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test5c: $(ODIR)/test5c.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test5vt: $(ODIR)/test5vt.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

part4cb: $(ODIR)/part4cb.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

part5cb: $(ODIR)/part5cb.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

space: $(ODIR)/space.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test4vt: $(ODIR)/test4vt.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)    

test6a: $(ODIR)/test6a.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test6b: $(ODIR)/test6b.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

cube: $(ODIR)/cube.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

christest: $(ODIR)/christest.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test7a: $(ODIR)/test7a.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test7b: $(ODIR)/test7b.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test7c: $(ODIR)/test7c.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

teapot: $(ODIR)/teapot.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

jetman: $(ODIR)/jetman.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test6sphere: $(ODIR)/test6sphere.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test8a: $(ODIR)/test8a.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

cubism: $(ODIR)/cubism.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 

