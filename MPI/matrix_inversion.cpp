#include <bits/stdc++.h>
// #include <mpi.h>

using namespace std;

int main(int argc, char **argv) {
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

	// Reduction to identity matrix
	for(int i = 0; i < n; i++) {
		double tmp = mat[i][i];
		for(int j = 0; j < 2*n; j++)
			mat[i][j] /= tmp;
	}

	// Output
	for(int i = 0; i < n; i++) {
		for(int j = n; j < 2*n; j++)
			cout << mat[i][j]<<" ";
		cout << endl;
	}

	return 0;

}