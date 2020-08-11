#include<stdio.h>
#include<time.h>
#include<mpi.h>
#include<string.h>
#include<Windows.h>

void main(int argc, char* argv[]){
	int id;
	char mes[100000];
	for (int i = 0; i < 100000; i++)mes[i] = '!';
	SYSTEMTIME local_time = { 0 };
	int tmp1, tmp2;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);

	if (id) {		//1号进程发送消息给0号
		GetLocalTime(&local_time);
		tmp1 = local_time.wSecond;
		tmp2 = local_time.wMilliseconds;
		MPI_Send(&tmp1, 4, MPI_INT, 0, 1, MPI_COMM_WORLD);
		MPI_Send(&tmp2, 4, MPI_INT, 0, 1, MPI_COMM_WORLD);
		for (int i = 0; i < 100000; i++)MPI_Send(mes, strlen(mes), MPI_CHAR, 0, 1, MPI_COMM_WORLD);
	}
	else {			//即0号进程接收消息
		MPI_Recv(&tmp1, 4, MPI_INT, 1, 1, MPI_COMM_WORLD, &status);
		MPI_Recv(&tmp2, 4, MPI_INT, 1, 1, MPI_COMM_WORLD, &status);
		for (int i = 0; i < 100000; i++)MPI_Recv(mes, strlen(mes), MPI_CHAR, 1, 1, MPI_COMM_WORLD, &status);
		GetLocalTime(&local_time);
		printf("用时%dms", 1000 * (local_time.wSecond - tmp1) + local_time.wMilliseconds - tmp2);
	}
	MPI_Finalize();
}