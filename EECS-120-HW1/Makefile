CC = icpc
CFLAGS =
COPTFLAGS = -O3 -g  -qopenmp
LDFLAGS =
DEBUGFLAGS = 
# -g -DDEBUG

default:
	@echo "=================================================="
	@echo "To build your sorting code, use:"
	@echo "  make mergesort-omp    	 # For Mergesort"
	@echo ""
	@echo "To clean this subdirectory (remove object files"
	@echo "and other junk), use:"
	@echo "  make clean"
	@echo "=================================================="

# Mergesort driver
mergesort-omp: driver.o sort.o mergesort-omp.o
	$(CC) $(COPTFLAGS) -o $@ $^

mergesort-seq: driver.o sort.o mergesort-seq.o
	$(CC) $(COPTFLAGS) -o $@ $^

github: driver.o sort.o github.o
	$(CC) $(COPTFLAGS) -o $@ $^

%.o: %.cc
	$(CC)  $(CFLAGS) $(COPTFLAGS) $(DEBUGFLAGS) -o $@ -c $<

debug-mergesort: driver.o sort.o mergesort-omp.o
	$(CC) $(COPTFLAGS) $(DEBUGFLAGS) -o $@ $^

clean:
	rm -f core *.o *~ mergesort-omp

# eof
