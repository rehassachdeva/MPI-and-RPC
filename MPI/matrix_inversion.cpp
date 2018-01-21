#include <bits/stdc++.h>
#include <mpi.h>

using namespace std;

int main(int argc, char **argv) {
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if(rank == 0) {

		int n;
		cin >> n;

		double mat[n][2*n];

		memset(mat, 0, sizeof(mat));

		for(int i = 0; i < n; i++)
			for(int j = 0; j < n; j++)
				cin >> mat[i][j];

		// Append identity matrix to the right
		for(int i = 0; i < n; i++)
			mat[i][n + i] = 1;

		//Pivoting
		for(int k = 0; k < n; k++) {
			
			// Find the kth pivot.
			int i_max = k, mx = abs(mat[k][k]);
			for(int i = k + 1; i < n; i++) {
				if(abs(mat[i][k]) > mx) {
					i_max = i;
					mx = abs(mat[i][k]);
				}
			}

			// Swap current row with the pivot
			for(int j = 0; j < 2*n; j++) {
				double tmp = mat[k][j];
				mat[k][j] = mat[i_max][j];
				mat[i_max][j] = tmp;
			}
		}

		// Reduction to diagonal matrix
		if(size == 1) {
			for(int i = 0; i < n; i++) {
				for(int j = 0; j < n; j++) {
					if(j != i) {
						double tmp = mat[j][i] / mat[i][i];
						for(int k = 0; k < 2*n; k++) {
							mat[j][k] -= mat[i][k] * tmp;
						}
					}
				}
			}			
		}

		else {
			int ni = (2*n) / (size - 1);
			int spare = (2*n) % (size - 1);
			vector<int> nis;

			int prev = 0;
			for(int i = 1; i < size; i++) {
				int cur = ni;
				if(spare > 0) {
					cur++;
					spare--;
				}
				nis.push_back(cur);
				MPI_Send(&n, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
				MPI_Send(&cur, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
				prev += cur;
			}

			for(int i = 0; i < n; i++) {
				for(int j = 0; j < n; j++) {
					if(j != i) {

						double tmp = mat[j][i] / mat[i][i];
						for(int k = 1; k < size; k++)
							MPI_Send(&tmp, 1, MPI_DOUBLE, k, 0, MPI_COMM_WORLD);
						
						int ptr = 0;
						for(int k = 1; k < size; k++) {
							MPI_Send(&mat[i][ptr], nis[k - 1], MPI_DOUBLE, k, 0, MPI_COMM_WORLD);
							ptr += nis[k];
						}

						ptr = 0;
						for(int k = 1; k < size; k++) {
							MPI_Send(&mat[j][ptr], nis[k - 1], MPI_DOUBLE, k, 0, MPI_COMM_WORLD);
							ptr += nis[k];
						}

						ptr = 0;
						for(int k = 1; k < size; k++) {
							MPI_Recv(&mat[j][ptr], nis[k - 1], MPI_DOUBLE, k, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
							ptr += nis[k];
						}
					}
				}
			}
		}

		// Reduction to identity matrix
		for(int i = 0; i < n; i++) {
			double tmp = mat[i][i];
			for(int j = 0; j < 2*n; j++)
				mat[i][j] /= tmp;
		}

		// Output
		for(int i = 0; i < n; i++) {
			for(int j = n; j < 2*n; j++)
				cout << setprecision(2) << mat[i][j]<<" ";
			cout << endl;
		}

	}

	else {
		int ni, n;
		MPI_Recv(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&ni, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);


		for(int i = 0; i < n; i++) {
			for(int j = 0; j < n; j++) {
				if(i != j) {

					double tmp;
					MPI_Recv(&tmp, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

					double irow[ni], jrow[ni];
					MPI_Recv(&irow, ni, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					MPI_Recv(&jrow, ni, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					
					for(int i = 0; i < ni; i++)
						jrow[i] -= irow[i] * tmp;

					MPI_Send(&jrow, ni, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);				
				}
			}
		}
	}

	MPI_Finalize();

	return 0;

}