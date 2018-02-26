//Функция рандома
// Константы

#include "rnd.h"

#define RAND_NTAB 32
#define RAND_NWUP 8
#define RAND_EPS 1.2e-7
#define RAND_RNMX (1.0 - RAND_EPS)
#define RAND_IM1 2147483563
#define RAND_IM2 2147483399
#define RAND_AM (1./RAND_IM1)
#define RAND_IMM1 (RAND_IM1-1)
#define RAND_IA1 40014
#define RAND_IA2 40692
#define RAND_IQ1 53668
#define RAND_IQ2 52774
#define RAND_IR1 12211
#define RAND_IR2 3791
#define RAND_NDIV (1 + RAND_IMM1 / RAND_NTAB)


// Зерно
static long rand_dummy;
void rand_seed(long dum) {
	rand_dummy = dum;
}

float custom_rand() {
	int j;
	long k;
	static long dummy2 = 123456789;
	static long iy = 0;
	static long iv[RAND_NTAB];
	float temp;
	if(rand_dummy <= 0 || !iy) {
		if(rand_dummy < 0) {
			rand_dummy = -rand_dummy;
		} else if(rand_dummy == 0) {
			rand_dummy = 1;
		}
		dummy2 = rand_dummy;
		for(j = RAND_NTAB + RAND_NWUP - 1; j >= 0; j--) {
			k = rand_dummy / RAND_IQ1;
			if((rand_dummy = RAND_IA1 * (rand_dummy - k * RAND_IQ1) - RAND_IR1*k) < 0) {
				rand_dummy += RAND_IM1;
			}
			if(j < RAND_NTAB) {
				iv[j] = rand_dummy;
			}
		}
		iy = iv[0];
	}
	k = rand_dummy / RAND_IQ1;
	if((rand_dummy = RAND_IA1 * (rand_dummy - k * RAND_IQ1) - RAND_IR1 * k) < 0) {
		rand_dummy += RAND_IM1;
	}
	k = dummy2 / RAND_IQ2;
	if((dummy2 = RAND_IA2 * (dummy2 - k * RAND_IQ2) - RAND_IR2 * k) < 0) {
		dummy2 += RAND_IM2;
	}
	iy = iv[j = iy / RAND_NDIV] - dummy2;
	iv[j] = rand_dummy;
	if(iy < 1) {
		iy += RAND_IMM1;
	}
	if((temp = RAND_AM * iy) > RAND_RNMX) {
		return RAND_RNMX;
	}
	return temp;
}

int randomNumber(int m, int n)
 {
 return (custom_rand() * (n - m + 1)) + m;
}
