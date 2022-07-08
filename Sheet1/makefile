SOURCES=$(HOME)/physicist/Desktop/Monte\ Carlo/Sheet1
vpath %.cc $(SOURCES)
vpath %.cpp $(SOURCES)
vpath %.c $(SOURCES)

CC = gcc
CXX = g++
CXXFLAGS = -fopenmp -O3 -std=c++11 -I. -I $(SOURCES)/lib
LIBS = -lgsl -lgslcblas

PROGRAM = sheet1
OBJS = sheet1.o statistics.o

$(PROGRAM): $(OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^ $(LIBS)

clean:
	rm -f $(PROGRAMS) $(OBJS)

%.o: %.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -c $<

%.o: %.cc $(HDRS)
	$(CXX) $(CXXFLAGS) -c $<

%.o: %.c $(HDRS)
	$(CC) $(CXXFLAGS) -c $<
