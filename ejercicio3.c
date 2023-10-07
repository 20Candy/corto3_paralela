//Carol Arevalo 20461
//Corto 3 paralela

//compilacion
//mpicc ejercicio3.c -o ejercicio3
//mpirun -np 4 ./ejercicio3

#include <stdio.h>
#include <mpi.h>

// Fuunción a integrar de ejemplo: f(x) = x^2
double f(double x) {
    return x * x;
}

int main(int argc, char* argv[]) {
    int rank, size;
    double a = 0.0, b = 1.0;  // Intervalo [a, b]
    int n = 10000;  // Número total de subintervalos
    double h = (b - a) / n;  // Tamaño de un subintervalo
    double local_sum = 0.0;
    double integral;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int local_n = n / size;  // Número de subintervalos por proceso
    double local_a = a + rank * local_n * h;
    double local_b = local_a + local_n * h;

    for (int i = 0; i < local_n; i++) {
        double x = local_a + i * h;
        local_sum += f(x) * h;
    }

    if (rank != 0) {
        MPI_Send(&local_sum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    } else {
        integral = local_sum;
        for (int src = 1; src < size; src++) {
            MPI_Recv(&local_sum, 1, MPI_DOUBLE, src, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            integral += local_sum;
        }
    }

    if (rank == 0) {
        printf("Estimación de la integral de f(x) en el intervalo [%.2f, %.2f] = %f\n", a, b, integral);
    }

    MPI_Finalize();
    return 0;
}
