//Carol Arevalo 20461
//Corto 3 paralela

//compilacion
//mpicc ejercicio2.c -o ejercicio2
//mpirun -np 4 ./ejercicio2


#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 5000

int main(int argc, char* argv[]) {
    int rank, size;
    int local_sum = 0, global_sum = 0;
    int local_max = -1, global_max = -1;
    int local_min = 1000000, global_min = 1000000;
    int data[N];
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Solo el proceso maestro genera los datos
    if(rank == 0) {
        for(int i = 0; i < N; i++) {
            data[i] = rand() % 1000;  // números aleatorios entre 0 y 999
        }
    }

    // Distribuir el array entre todos los procesos
    int local_n = N / size;
    int local_data[local_n];
    MPI_Scatter(data, local_n, MPI_INT, local_data, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    // Cada proceso calcula su suma local máximo local y mínimo local
    for(int i = 0; i < local_n; i++) {
        local_sum += local_data[i];
        if(local_data[i] > local_max) {
            local_max = local_data[i];
        }
        if(local_data[i] < local_min) {
            local_min = local_data[i];
        }
    }

    // Reducción para la suma
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Reducción para el máximo
    MPI_Reduce(&local_max, &global_max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    // Reducción para el mínimo
    MPI_Reduce(&local_min, &global_min, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);

    if(rank == 0) {
        printf("Suma total: %d\n", global_sum);
        printf("Máximo valor: %d\n", global_max);
        printf("Mínimo valor: %d\n", global_min);
    }

    MPI_Finalize();
    return 0;
}
