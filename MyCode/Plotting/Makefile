include Makefile.arch

SOURCES=$(wildcard *.cc) $(wildcard ../../MT2CORE/*.cc)

OBJECTS=$(SOURCES:.cc=.o)

LIB=libPlotting.so

EXE = Plotter

$(LIB): $(OBJECTS)
	$(LD) $(LDFLAGS) $(SOFLAGS) $(OBJECTS) $(ROOTLIBS) -o $@

$(EXE): $(LIB) 
	echo "$(CXX) -o $@ $(LIB) $(ROOTLIBS) -Wl,-rpath,./"; \
	$(CXX) -o $@ $(LIB) $(ROOTLIBS) -Wl,-rpath,./	

%.o:  %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

all: 	$(EXE)
libs: 	$(LIB)
clean:
	rm -f *.o \
	rm -f *.d \
	rm -f *.so \
