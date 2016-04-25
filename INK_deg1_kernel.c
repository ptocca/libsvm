// gcc -fPIC -g -c -Wall custom_kernel.c
// gcc -shared custom_kernel.o -Wl,-soname,libsvm_kernel.1 -o libsvm_kernel.so.1.0.1 -lc

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "svm.h"

double aParam = 3.0;

enum
{
  A_OPTION = 0,
  THE_END
};

char * const kernel_opts[] =
{
  [A_OPTION] = "a",
  [THE_END] = NULL
};


void init(char * params) {
	char *value;
	char *subopts = params;

	if (params==NULL)
		return;

	while (*subopts != '\0')
		switch (getsubopt(&subopts, kernel_opts, &value)) {
		case A_OPTION:
			aParam = strtod(value, NULL);
			break;
		default:
			/* Unknown sub-option. */
			printf("INK-Spline: Unknown kernel option `%s'\n", value);
			exit(1);
		}

	if (aParam == 0.0) {
		printf("Parameter 'a' for kernel not correctly set.");
		exit(1);
	}
}

double kernel(const struct svm_node *px, const struct svm_node *py) {
	double resXY = 1.0;
	double resXX = 1.0;
	double resYY = 1.0;
	double x = 0.0;
	double y = 0.0;
	while (px->index != -1 || py->index != -1) {
		if (px->index == -1)
			x = aParam;
		else {
			double v = px->value + aParam;
			px++;
			x = v>0.0 ? v : 0.0;
		}

		if (py->index == -1)
			y = aParam;
		else {
			double v = py->value + aParam;
			py++;
			y = v>0.0 ? v : 0.0;
		}

		double m = fmin(x,y);

		resXY *= (1.0 + x*y + 0.5*abs(x-y)*m*m + m*m*m*0.33333333333333);
		resXX *= (1.0 + x*x + x*x*x*0.33333333333333);
		resYY *= (1.0 + y*y + y*y*y*0.33333333333333);

	}
	return resXY/sqrt(resXX*resYY);
}
