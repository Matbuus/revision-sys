BIN=branch_prediction

# if PAPI is available on your computer, update the CFLAGS/LDFLAGS
# PAPI_ROOT=$(HOME)/Soft/compat/papi
# CFLAGS=-I$(PAPI_ROOT)/include -Wall -DHAVE_PAPI=1
# LDFLAGS=-L$(PAPI_ROOT)/lib -lpapi

CFLAGS= -Wall
LDFLAGS=

all: $(BIN)

branch_prediction: branch_prediction.c libbranch.h libbranch.so
	gcc -o branch_prediction branch_prediction.c -L. -lbranch $(LDFLAGS)  $(CFLAGS)

libbranch.so: libbranch.o
	gcc -shared libbranch.o -o libbranch.so

libbranch.o: libbranch.c libbranch.h
	gcc -c libbranch.c -fPIC -o libbranch.o

clean:
	rm -f $(BIN) *.o *.so

cleanall: clean
