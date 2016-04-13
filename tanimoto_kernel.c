// gcc -fPIC -g -c -Wall custom_kernel.c
// gcc -shared custom_kernel.o -Wl,-soname,libsvm_kernel.1 -o libsvm_kernel.so.1.0.1 -lc

#include "svm.h"

void init(char *params) {

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
	while (px->index!=-1) {
		sumX += px->value;
		px++;
	}
	while (py->index!=-1) {
		sumY += py->value;
		py++;
	}
	return sumMin / (sumX + sumY - sumMin);
}


