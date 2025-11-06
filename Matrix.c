#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
                                                            /* original code: H. Hasegawa; 2024.10. 4. */
																														/* updated code: K. Suzuta; 2025.11.6 */
// TO ENSURE N is equal to or bigger than input-n
#define TIME_LIMIT 160
#define N 2500

double A[N+1][N+1], B[N+1][N+1], C[N+1][N+1];

void mm_ijk(int m, int p, int n);
void mm_ikj(int m, int p, int n);
void mm_jik(int m, int p, int n);
void mm_jki(int m, int p, int n);
void mm_kij(int m, int p, int n);
void mm_kji(int m, int p, int n);

void mm_i2kj(int m, int p, int n);
void mm_ik2j(int m, int p, int n);
void mm_ikj2(int m, int p, int n);

void mm_k2ij(int m, int p, int n);
void mm_ki2j(int m, int p, int n);
void mm_kij2(int m, int p, int n);

void mm_ik4j(int m, int p, int n);
void mm_ik8j(int m, int p, int n);
void mm_ik16j(int m, int p, int n);
void mm_ik10j(int m, int p, int n);
void mm_ik20j(int m, int p, int n);

void mm_k4ij(int m, int p, int n);
void mm_k8ij(int m, int p, int n);
void mm_k16ij(int m, int p, int n);
void mm_k10ij(int m, int p, int n);
void mm_k20ij(int m, int p, int n);

void mm_ijk_block2(int m, int p, int n);
void mm_ijk_block4(int m, int p, int n);
void mm_ijk_block5(int m, int p, int n);
void mm_ijk_block10(int m, int p, int n);
void mm_ijk_block20(int m, int p, int n);
void mm_ijk_block50(int m, int p, int n);

void mm_ikj_block2(int m, int p, int n);
void mm_ikj_block4(int m, int p, int n);
void mm_ikj_block5(int m, int p, int n);
void mm_ikj_block10(int m, int p, int n);
void mm_ikj_block20(int m, int p, int n);
void mm_ikj_block50(int m, int p, int n);

void measure_time(void (*mm)(int, int, int), char *name, int n, FILE* fp);
void Input_Matrix(int m, int p, int n);
void Print_Matrix(int m, int p, int n);
int max(int a, int b);

clock_t start;

int main(int argc, char *argv[])
{
	FILE* fp = stdout;
	if (argc == 2) {
		fp = fopen(argv[1], "a");
		if (!fp) {
			perror("fopen");
			exit(1);
		}
	}

	int i, j, n;
	// csv header
	// fprintf(fp, "Alg,N,Time(sec),GFLOPS\n");

	start = clock();

	for (n = 200; n <= 2500; n += 100) {
		// n = N;
		// measure_time(mm_ijk, "ijk", n, fp);
		// measure_time(mm_ikj, "ikj", n, fp);
		// measure_time(mm_jik, "jik", n, fp);
		// measure_time(mm_jki, "jki", n, fp);
		// measure_time(mm_kij, "kij", n, fp);
		// measure_time(mm_kji, "kji", n, fp);
		// measure_time(mm_i2kj, "i2kj", n, fp);
		// measure_time(mm_ik2j, "ik2j", n, fp);
		// measure_time(mm_ikj2, "ikj2", n, fp);
		// measure_time(mm_k2ij, "k2ij", n, fp);
		// measure_time(mm_ki2j, "ki2j", n, fp);
		// measure_time(mm_kij2, "kij2", n, fp);
		// measure_time(mm_ik4j, "ik4j", n, fp);
		// measure_time(mm_ik8j, "ik8j", n, fp);
		// measure_time(mm_ik16j, "ik16j", n, fp);
		// measure_time(mm_ijk_block2, "ijk_block2", n, fp);
		// measure_time(mm_ijk_block4, "ijk_block4", n, fp);
		// measure_time(mm_ijk_block5, "ijk_block5", n, fp);
		// measure_time(mm_ijk_block10, "ijk_block10", n, fp);
		// measure_time(mm_ijk_block20, "ijk_block20", n, fp);
		// measure_time(mm_ijk_block50, "ijk_block50", n, fp);

		// measure_time(mm_ikj_block2, "ikj_block2", n, fp);
		// measure_time(mm_ikj_block4, "ikj_block4", n, fp);
		// measure_time(mm_ikj_block5, "ikj_block5", n, fp);
		// measure_time(mm_ikj_block10, "ikj_block10", n, fp);
		// measure_time(mm_ikj_block20, "ikj_block20", n, fp);
		// measure_time(mm_ikj_block50, "ikj_block50", n, fp);

		// measure_time(mm_k4ij, "k4ij", n, fp);
		// measure_time(mm_k8ij, "k8ij", n, fp);
		// measure_time(mm_k16ij, "k16ij", n, fp);

		measure_time(mm_ik10j, "ik10j", n, fp);
		measure_time(mm_ik20j, "ik20j", n, fp);
		measure_time(mm_k10ij, "k10ij", n, fp);
		measure_time(mm_k20ij, "k20ij", n, fp);
	}

	if (n<11) {
		Print_Matrix( n, n, n );
	}
	
	return 0;
}

void measure_time(void (*mm)(int, int, int), char *name, int n, FILE *fp) {
	if ((double)(clock() - start) / CLOCKS_PER_SEC >= TIME_LIMIT) exit(0);

	fprintf(stderr, "Start Measure %s with N=%d\n", name, n);

	// initialize matrix
	Input_Matrix(n, n, n);
	
	double t, Gflops;
	clock_t tic, toc;

	tic = clock();
	mm(n, n, n);
	toc = clock();
	t = (double)(toc - tic)/CLOCKS_PER_SEC;
	Gflops = (2.0*n)*n*n/t/1e+9;
	fprintf(fp, "%s,%d,%.2f,%.2f\n", name, n, t, Gflops);

	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			if (i == j) {
				assert(C[i][j] == 1);
			} else {
				assert(C[i][j] == 0);
			}
		}
	}
}

void mm_ijk(int m, int p, int n)
{
	int i, j, k;

	for (i = 1; i <= m; i++) {
		for (j = 1; j <= n; j++) {
			for (k = 1; k <= p; k++) {
				C[i][j] += A[i][k]*B[k][j];
			}
		}
	}
}

void mm_ikj(int m, int p, int n)
{
	int i, j, k;

	for (i = 1; i <= m; i++) {
		for (k = 1; k <= p; k++) {
			for (j = 1; j <= n; j++) {
				C[i][j] += A[i][k]*B[k][j];
			}
		}
	}
}

void mm_jik(int m, int p, int n)
{
	int i, j, k;

	for (j = 1; j <= n; j++) {
		for (i = 1; i <= m; i++) {
			for (k = 1; k <= p; k++) {
				C[i][j] += A[i][k]*B[k][j];
			}
		}
	}
}

void mm_jki(int m, int p, int n)
{
	int i, j, k;

	for (j = 1; j <= n; j++) {
		for (k = 1; k <= p; k++) {
			for (i = 1; i <= m; i++) {
				C[i][j] += A[i][k]*B[k][j];
			}
		}
	}
}

void mm_kij(int m, int p, int n)
{
	int i, j, k;

	for (k = 1; k <= p; k++) {
		for (i = 1; i <= m; i++) {
			for (j = 1; j <= n; j++) {
				C[i][j] += A[i][k]*B[k][j];
			}
		}
	}
}

void mm_kji(int m, int p, int n)
{
	int i, j, k;

	for (k = 1; k <= p; k++) {
		for (j = 1; j <= n; j++) {
			for (i = 1; i <= m; i++) {
				C[i][j] += A[i][k]*B[k][j];
			}
		}
	}
}

// i2kj => loop unrolling by 2 for i
// ik2j => loop unrolling by 2 for k
// ikj2 => loop unrolling by 2 for j
// k2ij => loop unrolling by 2 for k
// kj2i => loop unrolling by 2 for j
// kji2 => loop unrolling by 2 for i

void mm_i2kj(int m, int p, int n)
{
	int i, j, k;

	for (i = 1; i+1 <= m; i += 2) {
		for (k = 1; k <= p; k++) {
			for (j = 1; j <= n; j++) {
				C[i][j] += A[i][k]*B[k][j];
				C[i+1][j] += A[i+1][k]*B[k][j];
			}
		}
	}

	// remains
	int rem_i = i;
	for (i = rem_i; i <= m; i++) {
		for (k = 1; k <= p; k++) {
			for (j = 1; j <= n; j++) {
				C[i][j] += A[i][k]*B[k][j];
			}
		}
	}
}	

void mm_ik2j(int m, int p, int n)
{
	int i, j, k;

	for (i = 1; i <= m; i++) {
		for (k = 1; k+1 <= p; k+=2) {
			for (j = 1; j <= n; j++) {
				C[i][j] += A[i][k]*B[k][j];
				C[i][j] += A[i][k+1]*B[k+1][j];
			}
		}
	}

	// remains
	int rem_k = k;
	for (i = 1; i <= m; i++) {
		for (k = rem_k; k <= p; k++) {
			for (j = 1; j <= n; j++) {
				C[i][j] += A[i][k]*B[k][j];
			}
		}
	}
}

void mm_ikj2(int m, int p, int n)
{
	int i, j, k;

	for (i = 1; i <= m; i++) {
		for (k = 1; k <= p; k++) {
			for (j = 1; j+1 <= n; j+=2) {
				C[i][j] += A[i][k]*B[k][j];
				C[i][j+1] += A[i][k]*B[k][j+1];
			}
		}
	}

	// remains
	int rem_j = j;
	for (i = 1; i <= m; i++) {
		for (k = 1; k <= p; k++) {
			for (j = rem_j; j <= n; j++) {
				C[i][j] += A[i][k]*B[k][j];
			}
		}
	}
}

void mm_k2ij(int m, int p, int n)
{
	int i, j, k;

	for (k = 1; k+1 <= p; k += 2) {
		for (i = 1; i <= m; i++) {
			for (j = 1; j <= n; j++) {
				C[i][j] += A[i][k]*B[k][j];
				C[i][j] += A[i][k+1]*B[k+1][j];
			}
		}
	}

	// remains
	int rem_k = k;
	for (k = rem_k; k <= p; k++) {
		for (i = 1; i <= m; i++) {
			for (j = 1; j <= n; j++) {
				C[i][j] += A[i][k]*B[k][j];
			}
		}
	}
}

void mm_ki2j(int m, int p, int n)
{
	int i, j, k;

	for (k = 1; k <= p; k++) {
		for (i = 1; i+1 <= m; i+=2) {
			for (j = 1; j <= n; j++) {
				C[i][j] += A[i][k]*B[k][j];
				C[i+1][j] += A[i+1][k]*B[k][j];
			}
		}
	}

	// remains
	int rem_i = i;
	for (k = 1; k <= p; k++) {
		for (i = rem_i; i <= m; i++) {
			for (j = 1; j <= n; j++) {
				C[i][j] += A[i][k]*B[k][j];
			}
		}
	}
}

void mm_kij2(int m, int p, int n)
{
	int i, j, k;

	for (k = 1; k <= p; k++) {
		for (i = 1; i <= m; i++) {
			for (j = 1; j+1 <= n; j+=2) {
				C[i][j] += A[i][k]*B[k][j];
				C[i][j+1] += A[i][k]*B[k][j+1];
			}
		}
	}

	// remains
	int rem_j = j;
	for (k = 1; k <= p; k++) {
		for (i = 1; i <= m; i++) {
			for (j = rem_j; j <= n; j++) {
				C[i][j] += A[i][k]*B[k][j];
			}
		}
	}
}

void mm_ik4j(int m, int p, int n)
{
	int i, j, k;

	for (i = 1; i <= m; i++) {
		for (k = 1; k+3 <= p; k+=4) {
			for (j = 1; j <= n; j++) {
				C[i][j] += A[i][k]*B[k][j];
				C[i][j] += A[i][k+1]*B[k+1][j];
				C[i][j] += A[i][k+2]*B[k+2][j];
				C[i][j] += A[i][k+3]*B[k+3][j];
			}
		}
	}

	// remains
	int rem_k = k;
	for (i = 1; i <= m; i++) {
		for (k = rem_k; k <= p; k++) {
			for (j = 1; j <= n; j++) {
				C[i][j] += A[i][k]*B[k][j];
			}
		}
	}
}

void mm_ik8j(int m, int p, int n)
{
	int i, j, k;

	for (i = 1; i <= m; i++) {
		for (k = 1; k+7 <= p; k+=8) {
			for (j = 1; j <= n; j++) {
				C[i][j] += A[i][k]*B[k][j];
				C[i][j] += A[i][k+1]*B[k+1][j];
				C[i][j] += A[i][k+2]*B[k+2][j];
				C[i][j] += A[i][k+3]*B[k+3][j];
				C[i][j] += A[i][k+4]*B[k+4][j];
				C[i][j] += A[i][k+5]*B[k+5][j];
				C[i][j] += A[i][k+6]*B[k+6][j];
				C[i][j] += A[i][k+7]*B[k+7][j];
			}
		}
	}

	// remains
	int rem_k = k;
	for (i = 1; i <= m; i++) {
		for (k = rem_k; k <= p; k++) {
			for (j = 1; j <= n; j++) {
				C[i][j] += A[i][k]*B[k][j];
			}
		}
	}
}

void mm_ik16j(int m, int p, int n)
{
	int i, j, k;

	for (i = 1; i <= m; i++) {
		for (k = 1; k+15 <= p; k+=16) {
			for (j = 1; j <= n; j++) {
				C[i][j] += A[i][k]*B[k][j];
				C[i][j] += A[i][k+1]*B[k+1][j];
				C[i][j] += A[i][k+2]*B[k+2][j];
				C[i][j] += A[i][k+3]*B[k+3][j];
				C[i][j] += A[i][k+4]*B[k+4][j];
				C[i][j] += A[i][k+5]*B[k+5][j];
				C[i][j] += A[i][k+6]*B[k+6][j];
				C[i][j] += A[i][k+7]*B[k+7][j];
				C[i][j] += A[i][k+8]*B[k+8][j];
				C[i][j] += A[i][k+9]*B[k+9][j];
				C[i][j] += A[i][k+10]*B[k+10][j];
				C[i][j] += A[i][k+11]*B[k+11][j];
				C[i][j] += A[i][k+12]*B[k+12][j];
				C[i][j] += A[i][k+13]*B[k+13][j];
				C[i][j] += A[i][k+14]*B[k+14][j];
				C[i][j] += A[i][k+15]*B[k+15][j];
			}
		}
	}

	// remains
	int rem_k = k;
	for (i = 1; i <= m; i++) {
		for (k = rem_k; k <= p; k++) {
			for (j = 1; j <= n; j++) {
				C[i][j] += A[i][k]*B[k][j];
			}
		}
	}
}

void mm_ik10j(int m, int p, int n)
{
	int i, j, k;

	for (i = 1; i <= m; i++) {
		for (k = 1; k+9 <= p; k+=10) {
			for (j = 1; j <= n; j++) {
				C[i][j] += A[i][k]*B[k][j];
				C[i][j] += A[i][k+1]*B[k+1][j];
				C[i][j] += A[i][k+2]*B[k+2][j];
				C[i][j] += A[i][k+3]*B[k+3][j];
				C[i][j] += A[i][k+4]*B[k+4][j];
				C[i][j] += A[i][k+5]*B[k+5][j];
				C[i][j] += A[i][k+6]*B[k+6][j];
				C[i][j] += A[i][k+7]*B[k+7][j];
				C[i][j] += A[i][k+8]*B[k+8][j];
				C[i][j] += A[i][k+9]*B[k+9][j];
			}
		}
	}
}

void mm_ik20j(int m, int p, int n)
{
	int i, j, k;
	for (i = 1; i <= m; i++) {
		for (k = 1; k+19 <= p; k+=20) {
			for (j = 1; j <= n; j++) {
				C[i][j] += A[i][k]*B[k][j];
				C[i][j] += A[i][k+1]*B[k+1][j];
				C[i][j] += A[i][k+2]*B[k+2][j];
				C[i][j] += A[i][k+3]*B[k+3][j];
				C[i][j] += A[i][k+4]*B[k+4][j];
				C[i][j] += A[i][k+5]*B[k+5][j];
				C[i][j] += A[i][k+6]*B[k+6][j];
				C[i][j] += A[i][k+7]*B[k+7][j];
				C[i][j] += A[i][k+8]*B[k+8][j];
				C[i][j] += A[i][k+9]*B[k+9][j];
				C[i][j] += A[i][k+10]*B[k+10][j];
				C[i][j] += A[i][k+11]*B[k+11][j];
				C[i][j] += A[i][k+12]*B[k+12][j];
				C[i][j] += A[i][k+13]*B[k+13][j];
				C[i][j] += A[i][k+14]*B[k+14][j];
				C[i][j] += A[i][k+15]*B[k+15][j];
				C[i][j] += A[i][k+16]*B[k+16][j];
				C[i][j] += A[i][k+17]*B[k+17][j];
				C[i][j] += A[i][k+18]*B[k+18][j];
				C[i][j] += A[i][k+19]*B[k+19][j];
			}
		}
	}
}

void mm_k4ij(int m, int p, int n)
{
	int i, j, k;

	for (k = 1; k+3 <= p; k+=4) {
		for (i = 1; i <= m; i++) {
			for (j = 1; j <= n; j++) {
				C[i][j] += A[i][k]*B[k][j];
				C[i][j] += A[i][k+1]*B[k+1][j];
				C[i][j] += A[i][k+2]*B[k+2][j];
				C[i][j] += A[i][k+3]*B[k+3][j];
			}
		}
	}

	// remains
	int rem_k = k;
	for (k = rem_k; k <= p; k++) {
		for (i = 1; i <= m; i++) {
			for (j = 1; j <= n; j++) {
				C[i][j] += A[i][k]*B[k][j];
			}
		}
	}
}
void mm_k8ij(int m, int p, int n)
{
	int i, j, k;

	for (k = 1; k+7 <= p; k+=8) {
		for (i = 1; i <= m; i++) {
			for (j = 1; j <= n; j++) {
				C[i][j] += A[i][k]*B[k][j];
				C[i][j] += A[i][k+1]*B[k+1][j];
				C[i][j] += A[i][k+2]*B[k+2][j];
				C[i][j] += A[i][k+3]*B[k+3][j];
				C[i][j] += A[i][k+4]*B[k+4][j];
				C[i][j] += A[i][k+5]*B[k+5][j];
				C[i][j] += A[i][k+6]*B[k+6][j];
				C[i][j] += A[i][k+7]*B[k+7][j];
			}
		}
	}

	// remains
	int rem_k = k;
	for (k = rem_k; k <= p; k++) {
		for (i = 1; i <= m; i++) {
			for (j = 1; j <= n; j++) {
				C[i][j] += A[i][k]*B[k][j];
			}
		}
	}
}

void mm_k16ij(int m, int p, int n)
{
	int i, j, k;

	for (k = 1; k+15 <= p; k+=16) {
		for (i = 1; i <= m; i++) {
			for (j = 1; j <= n; j++) {
				C[i][j] += A[i][k]*B[k][j];
				C[i][j] += A[i][k+1]*B[k+1][j];
				C[i][j] += A[i][k+2]*B[k+2][j];
				C[i][j] += A[i][k+3]*B[k+3][j];
				C[i][j] += A[i][k+4]*B[k+4][j];
				C[i][j] += A[i][k+5]*B[k+5][j];
				C[i][j] += A[i][k+6]*B[k+6][j];
				C[i][j] += A[i][k+7]*B[k+7][j];
				C[i][j] += A[i][k+8]*B[k+8][j];
				C[i][j] += A[i][k+9]*B[k+9][j];
				C[i][j] += A[i][k+10]*B[k+10][j];
				C[i][j] += A[i][k+11]*B[k+11][j];
				C[i][j] += A[i][k+12]*B[k+12][j];
				C[i][j] += A[i][k+13]*B[k+13][j];
				C[i][j] += A[i][k+14]*B[k+14][j];
				C[i][j] += A[i][k+15]*B[k+15][j];
			}
		}
	}

	// remains
	int rem_k = k;
	for (k = rem_k; k <= p; k++) {
		for (i = 1; i <= m; i++) {
			for (j = 1; j <= n; j++) {
				C[i][j] += A[i][k]*B[k][j];
			}
		}
	}
}

void mm_k10ij(int m, int p, int n)
{
	int i, j, k;
	for (k = 1; k+9 <= p; k+=10) {
		for (i = 1; i <= m; i++) {
			for (j = 1; j <= n; j++) {
				C[i][j] += A[i][k]*B[k][j];
				C[i][j] += A[i][k+1]*B[k+1][j];
				C[i][j] += A[i][k+2]*B[k+2][j];
				C[i][j] += A[i][k+3]*B[k+3][j];
				C[i][j] += A[i][k+4]*B[k+4][j];
				C[i][j] += A[i][k+5]*B[k+5][j];
				C[i][j] += A[i][k+6]*B[k+6][j];
				C[i][j] += A[i][k+7]*B[k+7][j];
				C[i][j] += A[i][k+8]*B[k+8][j];
				C[i][j] += A[i][k+9]*B[k+9][j];
			}
		}
	}
}

void mm_k20ij(int m, int p, int n)
{
	int i, j, k;
	for (k = 1; k+19 <= p; k+=20) {
		for (i = 1; i <= m; i++) {
			for (j = 1; j <= n; j++) {
				C[i][j] += A[i][k]*B[k][j];
				C[i][j] += A[i][k+1]*B[k+1][j];
				C[i][j] += A[i][k+2]*B[k+2][j];
				C[i][j] += A[i][k+3]*B[k+3][j];
				C[i][j] += A[i][k+4]*B[k+4][j];
				C[i][j] += A[i][k+5]*B[k+5][j];
				C[i][j] += A[i][k+6]*B[k+6][j];
				C[i][j] += A[i][k+7]*B[k+7][j];
				C[i][j] += A[i][k+8]*B[k+8][j];
				C[i][j] += A[i][k+9]*B[k+9][j];
				C[i][j] += A[i][k+10]*B[k+10][j];
				C[i][j] += A[i][k+11]*B[k+11][j];
				C[i][j] += A[i][k+12]*B[k+12][j];
				C[i][j] += A[i][k+13]*B[k+13][j];
				C[i][j] += A[i][k+14]*B[k+14][j];
				C[i][j] += A[i][k+15]*B[k+15][j];
				C[i][j] += A[i][k+16]*B[k+16][j];
				C[i][j] += A[i][k+17]*B[k+17][j];
				C[i][j] += A[i][k+18]*B[k+18][j];
				C[i][j] += A[i][k+19]*B[k+19][j];
			}
		}
	}
}

void mm_ijk_block2(int m, int p, int n)
{
	for (int i = 1; i <= m; i += 2) {
		for (int j = 1; j <= n; j += 2) {
			for (int k = 1; k <= p; k += 2) {
				for (int ii = i; ii < i + 2; ii++) {
					for (int jj = j; jj < j + 2; jj++) {
						for (int kk = k; kk < k + 2; kk++) {
							C[ii][jj] += A[ii][kk]*B[kk][jj];
						}
					}
				}
			}
		}
	}
}

void mm_ijk_block4(int m, int p, int n)
{
	for (int i = 1; i <= m; i += 4) {
		for (int j = 1; j <= n; j += 4) {
			for (int k = 1; k <= p; k += 4) {
				for (int ii = i; ii < i + 4; ii++) {
					for (int jj = j; jj < j + 4; jj++) {
						for (int kk = k; kk < k + 4; kk++) {
							C[ii][jj] += A[ii][kk]*B[kk][jj];
						}
					}
				}
			}
		}
	}
}

void mm_ijk_block5(int m, int p, int n)
{
	for (int i = 1; i <= m; i += 5) {
		for (int j = 1; j <= n; j += 5) {
			for (int k = 1; k <= p; k += 5) {
				for (int ii = i; ii < i + 5; ii++) {
					for (int jj = j; jj < j + 5; jj++) {
						for (int kk = k; kk < k + 5; kk++) {
							C[ii][jj] += A[ii][kk]*B[kk][jj];
						}
					}
				}
			}
		}
	}
}

void mm_ijk_block10(int m, int p, int n)
{
	for (int i = 1; i <= m; i += 10) {
		for (int j = 1; j <= n; j += 10) {
			for (int k = 1; k <= p; k += 10) {
				for (int ii = i; ii < i + 10; ii++) {
					for (int jj = j; jj < j + 10; jj++) {
						for (int kk = k; kk < k + 10; kk++) {
							C[ii][jj] += A[ii][kk]*B[kk][jj];
						}
					}
				}
			}
		}
	}
}

void mm_ijk_block20(int m, int p, int n)
{
	for (int i = 1; i <= m; i += 20) {
		for (int j = 1; j <= n; j += 20) {
			for (int k = 1; k <= p; k += 20) {
				for (int ii = i; ii < i + 20; ii++) {
					for (int jj = j; jj < j + 20; jj++) {
						for (int kk = k; kk < k + 20; kk++) {
							C[ii][jj] += A[ii][kk]*B[kk][jj];
						}
					}
				}
			}
		}
	}
}

void mm_ijk_block50(int m, int p, int n)
{
	for (int i = 1; i <= m; i += 50) {
		for (int j = 1; j <= n; j += 50) {
			for (int k = 1; k <= p; k += 50) {
				for (int ii = i; ii < i + 50; ii++) {
					for (int jj = j; jj < j + 50; jj++) {
						for (int kk = k; kk < k + 50; kk++) {
							C[ii][jj] += A[ii][kk]*B[kk][jj];
						}
					}
				}
			}
		}
	}
}

void mm_ikj_block2(int m, int p, int n)
{
	for (int i = 1; i <= m; i += 2) {
		for (int k = 1; k <= p; k += 2) {
			for (int j = 1; j <= n; j += 2) {
				for (int ii = i; ii < i + 2; ii++) {
					for (int kk = k; kk < k + 2; kk++) {
						for (int jj = j; jj < j + 2; jj++) {
							C[ii][jj] += A[ii][kk]*B[kk][jj];
						}
					}
				}
			}
		}
	}
}

void mm_ikj_block4(int m, int p, int n)
{
	for (int i = 1; i <= m; i += 4) {
		for (int k = 1; k <= p; k += 4) {
			for (int j = 1; j <= n; j += 4) {
				for (int ii = i; ii < i + 4; ii++) {
					for (int kk = k; kk < k + 4; kk++) {
						for (int jj = j; jj < j + 4; jj++) {
							C[ii][jj] += A[ii][kk]*B[kk][jj];
						}
					}
				}
			}
		}
	}
}

void mm_ikj_block5(int m, int p, int n)
{
	for (int i = 1; i <= m; i += 5) {
		for (int k = 1; k <= p; k += 5) {
			for (int j = 1; j <= n; j += 5) {
				for (int ii = i; ii < i + 5; ii++) {
					for (int kk = k; kk < k + 5; kk++) {
						for (int jj = j; jj < j + 5; jj++) {
							C[ii][jj] += A[ii][kk]*B[kk][jj];
						}
					}
				}
			}
		}
	}
}

void mm_ikj_block10(int m, int p, int n)
{
	for (int i = 1; i <= m; i += 10) {
		for (int k = 1; k <= p; k += 10) {
			for (int j = 1; j <= n; j += 10) {
				for (int ii = i; ii < i + 10; ii++) {
					for (int kk = k; kk < k + 10; kk++) {
						for (int jj = j; jj < j + 10; jj++) {
							C[ii][jj] += A[ii][kk]*B[kk][jj];
						}
					}
				}
			}
		}
	}
}

void mm_ikj_block20(int m, int p, int n)
{
	for (int i = 1; i <= m; i += 20) {
		for (int k = 1; k <= p; k += 20) {
			for (int j = 1; j <= n; j += 20) {
				for (int ii = i; ii < i + 20; ii++) {
					for (int kk = k; kk < k + 20; kk++) {
						for (int jj = j; jj < j + 20; jj++) {
							C[ii][jj] += A[ii][kk]*B[kk][jj];
						}
					}
				}
			}
		}
	}
}

void mm_ikj_block50(int m, int p, int n)
{
	for (int i = 1; i <= m; i += 50) {
		for (int k = 1; k <= p; k += 50) {
			for (int j = 1; j <= n; j += 50) {
				for (int ii = i; ii < i + 50; ii++) {
					for (int kk = k; kk < k + 50; kk++) {
						for (int jj = j; jj < j + 50; jj++) {
							C[ii][jj] += A[ii][kk]*B[kk][jj];
						}
					}
				}
			}
		}
	}
}

void Input_Matrix(int m, int p, int n)
		/* Frank matrix a_{i,j} = n+1-max(i,j) */
{	int i, j, k;	
	    
	for (i = 1; i <= m; i++) {
		for (j = 1; j <= p; j++) {
			A[i][j] = n+1-max(i,j);
		}
	}
	for (i = 1; i <= p; i++) {
		for (j = 1; j <= n; j++) {
			B[i][j] = 0.0;
		}
		if (i == 1) {
			B[i][i]   =  1.0;
			B[i][i+1] = -1.0;}
		else if (i == p) {
			B[i][i-1] = -1.0;
			B[i][i]   =  2.0; }
		else {
			B[i][i-1] = -1.0;
			B[i][i]   =  2.0;
			B[i][i+1] = -1.0;
		}
	}

	for (i = 1; i <= m; i++) {
		for (j = 1; j <= n; j++) {
				C[i][j] = 0.0;
			}
	}
}

void Print_Matrix(int m, int p, int n)
{
	int i, j, k;

	printf("A  =\n");
	for (i = 1; i <= n; i++) {
		for (j = 1; j <= n; j++) {
			printf(" %7.2f", A[i][j]);
			/* printf("i = %5d, j = %5d, value = %.5f\n", i, j, A[i][j]); */
		}
		printf("\n");
	}

	printf("B  =\n");
	for (i = 1; i <= n; i++) {
		for (j = 1; j <= n; j++) {
			printf(" %7.2f", B[i][j]);	
		}
		printf("\n");
	} 

	printf("AB =\n");for (i = 1; i <= n; i++) {
		for (j = 1; j <= n; j++) {
			printf(" %7.2f", C[i][j]);	
		}
		printf("\n");
	} 
}

int max(int a, int b)
{
	int maxvalue;

	maxvalue = a;
	if (b>a) {maxvalue = b;}
	return maxvalue;
}