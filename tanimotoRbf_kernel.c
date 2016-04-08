// gcc -fPIC -g -c -Wall custom_kernel.c
// gcc -shared custom_kernel.o -Wl,-soname,libsvm_kernel.1 -o libsvm_kernel.so.1.0.1 -lc

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "svm.h"

double gammaParam = 1.0;

void init() {
	const char *s = getenv("GAMMA");
	if (s==NULL)
		return;
	gammaParam = strtod(s,NULL);
	if (gammaParam==0.0) {
		printf("Parameter 'gamma' for kernel not correctly set.");
		exit(1);
	}
}

double kernel(const struct svm_node *px, const struct svm_node *py) {
	double sumMin = 0;
	double sumX = 0;
	double sumY = 0;
	while (px->index != -1 && py->index != -1) {
		if (px->index == py->index) {
			sumMin += ((px->value > py->value) ? py->value : px->value);
			sumX += px->value;
			sumY += px->value;
			++px;
			++py;
		} else {
			if (px->index > py->index) {
				sumY += py->value;
				++py;
			} else {
				sumX += px->value;
				++px;
			}
		}
	}
	return exp(-gammaParam * (2 - 2 * sumMin / (sumX + sumY - sumMin)));
}
