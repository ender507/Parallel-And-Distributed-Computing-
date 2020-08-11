#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <time.h>
#include <Windows.h>
#include <cstdio>
#include "jacobi.h"
#include "error_checks.h"

// Change this to 0 if CPU reference result is not needed
#define COMPUTE_CPU_REFERENCE 1
#define MAX_ITERATIONS 3000

// CPU kernel
void sweepCPU(double* phi, const double* phiPrev, const double* source,
	double h2, int N)
{
	int i, j;
	int index, i1, i2, i3, i4;

	for (j = 1; j < N - 1; j++) {
		for (i = 1; i < N - 1; i++) {
			index = i + j * N;
			i1 = (i - 1) + j * N;
			i2 = (i + 1) + j * N;
			i3 = i + (j - 1) * N;
			i4 = i + (j + 1) * N;
			phi[index] = 0.25 * (phiPrev[i1] + phiPrev[i2] +
				phiPrev[i3] + phiPrev[i4] -
				h2 * source[index]);
		}
	}
}

// GPU kernel
__global__
void sweepGPU(double* phi, const double* phiPrev, const double* source,
	double h2, int N)
{
//#error Add here the GPU version of the update routine (see sweepCPU above)
	int idx = blockIdx.x * blockDim.x * blockDim.y + threadIdx.x;
	int i1, i2, i3, i4, i, j;
	if (idx !=0 && idx!=N-1 && idx%N!=0 && (idx+1)%N!=0) {
		j = idx / N;
		i = idx - j * N;
		i1 = (i - 1) + j * N;
		i2 = (i + 1) + j * N;
		i3 = i + (j - 1) * N;
		i4 = i + (j + 1) * N;
		phi[idx] = 0.25 * (phiPrev[i1] + phiPrev[i2] +
			phiPrev[i3] + phiPrev[i4] -
			h2 * source[idx]);
	}
}


double compareArrays(const double* a, const double* b, int N)
{
	double error = 0.0;
	int i;
	for (i = 0; i < N * N; i++) {
		error += fabs(a[i] - b[i]);
	}
	return error / (N * N);
}


double diffCPU(const double* phi, const double* phiPrev, int N)
{
	int i;
	double sum = 0;
	double diffsum = 0;

	for (i = 0; i < N * N; i++) {
		diffsum += (phi[i] - phiPrev[i]) * (phi[i] - phiPrev[i]);
		sum += phi[i] * phi[i];
	}

	return sqrt(diffsum / sum);
}


int main()
{
	SYSTEMTIME t1, t2; // Structs for timing
	const int N = 512;
	double h = 1.0 / (N - 1);
	int iterations;
	const double tolerance = 5e-4; // Stopping condition
	int i, j, index;

	double* phi = new double[N * N];
	double* phiPrev = new double[N * N];
	double* source = new double[N * N];
	double* phi_cuda = new double[N * N];

	double* phi_d, * phiPrev_d, * source_d;
	// Size of the arrays in bytes
	const int size = N * N * sizeof(double);
	double diff;

	// Source initialization
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			double x, y;
			x = (i - N / 2) * h;
			y = (j - N / 2) * h;
			index = j + i * N;
			if (((x - 0.25) * (x - 0.25) + y * y) < 0.1 * 0.1)
				source[index] = 1e10 * h * h;
			else if (((x + 0.25) * (x + 0.25) + y * y) < 0.1 * 0.1)
				source[index] = -1e10 * h * h;
			else
				source[index] = 0.0;
		}
	}

	CUDA_CHECK(cudaMalloc((void**)& source_d, size));
	CUDA_CHECK(cudaMemcpy(source_d, source, size, cudaMemcpyHostToDevice));

	// Reset values to zero
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			index = j + i * N;
			phi[index] = 0.0;
			phiPrev[index] = 0.0;
		}
	}

	CUDA_CHECK(cudaMalloc((void**)& phi_d, size));
	CUDA_CHECK(cudaMalloc((void**)& phiPrev_d, size));
	CUDA_CHECK(cudaMemcpy(phi_d, phi, size, cudaMemcpyHostToDevice));
	CUDA_CHECK(cudaMemcpy(phiPrev_d, phiPrev, size, cudaMemcpyHostToDevice));

	// CPU version 
	if (COMPUTE_CPU_REFERENCE) {
		GetLocalTime(&t1);

		// Do sweeps untill difference is under the tolerance
		diff = tolerance * 2;
		iterations = 0;
		while (diff > tolerance && iterations < MAX_ITERATIONS) {
			sweepCPU(phiPrev, phi, source, h * h, N);
			sweepCPU(phi, phiPrev, source, h * h, N);

			iterations += 2;
			if (iterations % 100 == 0) {
				diff = diffCPU(phi, phiPrev, N);
				printf("%d %g\n", iterations, diff);
			}
		}
		GetLocalTime(&t2);
		printf("CPU Jacobi: %g seconds, %d iterations\n",
			t2.wSecond - t1.wSecond +
			(t2.wMilliseconds - t1.wMilliseconds) / 1.0e3, iterations);
	}

	// GPU version
	const int blocksize = 4;
	const int gridsize = 128;
	dim3 dimBlock(blocksize, blocksize);
	dim3 dimGrid(gridsize, gridsize);

	//do sweeps until diff under tolerance
	diff = tolerance * 2;
	iterations = 0;

	GetLocalTime(&t1);

	while (diff > tolerance && iterations < MAX_ITERATIONS) {
		// See above how the CPU update kernel is called
		// and implement similar calling sequence for the GPU code

		//// Add routines here
//#error Add GPU kernel calls here (see CPU version above)
		sweepGPU << < gridsize * gridsize, blocksize* blocksize >> > (phiPrev_d, phi_d, source_d, h * h, N);
		sweepGPU << < gridsize * gridsize, blocksize* blocksize >> > (phi_d, phiPrev_d, source_d, h * h, N);

		iterations += 2;

		if (iterations % 100 == 0) {
			// diffGPU is defined in the header file, it uses
			// Thrust library for reduction computation
			diff = diffGPU<double>(phiPrev_d, phi_d, N);
			CHECK_ERROR_MSG("Difference computation");
			printf("%d %g\n", iterations, diff);
		}
	}

	//// Add here the routine to copy back the results
//#error Copy back the results
	CUDA_CHECK(cudaMemcpy(source, source_d, sizeof(double)* N, cudaMemcpyDeviceToHost));
	CUDA_CHECK(cudaMemcpy(phi, phi_d, sizeof(double)* N, cudaMemcpyDeviceToHost));
	CUDA_CHECK(cudaMemcpy(phiPrev, phiPrev_d, sizeof(double)* N, cudaMemcpyDeviceToHost));
	GetLocalTime(&t2);
	printf("GPU Jacobi: %g seconds, %d iterations\n",
		t2.wSecond - t1.wSecond +
		(t2.wMilliseconds - t1.wMilliseconds) / 1.0e3, iterations);

	//// Add here the clean up code for all allocated CUDA resources
//#error Add here the clean up code   
	CUDA_CHECK(cudaFree(source_d));
	CUDA_CHECK(cudaFree(phi_d));
	CUDA_CHECK(cudaFree(phiPrev_d));

	if (COMPUTE_CPU_REFERENCE) {
		printf("Average difference is %g\n", compareArrays(phi, phi_cuda, N));
	}

	delete[] phi;
	delete[] phi_cuda;
	delete[] phiPrev;
	delete[] source;

	return EXIT_SUCCESS;
}