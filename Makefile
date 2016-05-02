CXX ?= g++
CFLAGS = -Wall -Wconversion -O3 -fPIC -fopenmp
SHVER = 2
OS = $(shell uname)

all: svm-train svm-predict svm-scale

lib: svm.o
	if [ "$(OS)" = "Darwin" ]; then \
		SHARED_LIB_FLAG="-dynamiclib -Wl,-install_name,libsvm.so.$(SHVER)"; \
	else \
		SHARED_LIB_FLAG="-shared -Wl,-soname,libsvm.so.$(SHVER)"; \
	fi; \
	$(CXX) $${SHARED_LIB_FLAG} svm.o -o libsvm.so.$(SHVER)

gitversion.c: .git/HEAD .git/index
	echo "const char *gitversion = \"$(shell git describe --abbrev=4 --dirty --always --tags)\";" > $@
	
svm-predict: svm-predict.c svm.o gitversion.o
	$(CXX) $(CFLAGS) svm-predict.c svm.o gitversion.o -o svm-predict -lm -ldl
svm-train: svm-train.c svm.o gitversion.o
	$(CXX) $(CFLAGS) svm-train.c svm.o gitversion.o -o svm-train -lm -ldl
svm-scale: svm-scale.c gitversion.o
	$(CXX) $(CFLAGS) svm-scale.c gitversion.o -o svm-scale
svm.o: svm.cpp svm.h
	$(CXX) $(CFLAGS) -c svm.cpp
clean:
	rm -f *~ svm.o svm-train svm-predict svm-scale libsvm.so.$(SHVER) gitversion.o
