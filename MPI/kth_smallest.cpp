#include <bits/stdc++.h>
#include <mpi.h>

using namespace std;

int main(int argc, char **argv) {
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if(rank==0) {
		int n,k;
		cin>>n>>k;
		vector<int> arr(n);
		vector<int> res(n);
		for(int i=0;i<n;i++)
			cin>>arr[i];

		if(size==1) {
			sort(arr.begin(), arr.end());
			cout<<arr[k-1]<<endl;
		}

		else {

			int ni=n/(size-1);
			int spare=n%(size-1);

			vector<int> nis;

			int prev=0;
			for(int i=1;i<size;i++) {
				int cur=ni;
				if(spare>0) {
					cur++;
					spare--;
				}
				nis.push_back(cur);
				MPI_Send(&cur, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
				for(int j=prev;j<prev+cur;j++)
					MPI_Send(&arr[j], 1, MPI_INT, i, 0, MPI_COMM_WORLD);
				prev+=cur;
			}

			vector< vector<int> > local_res;
			for(int i=1;i<size;i++) {
				vector<int> tmp(nis[i-1]);
				for(int j=0;j<nis[i-1];j++)
					MPI_Recv(&tmp[j], 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				local_res.push_back(tmp);
			}

			vector<int> ptr(size-1, 0);
			for(int i=0;i<n;i++) {
				int mn=INT_MAX;
				int idx=-1;
				for(int j=0;j<size-1;j++) {
					int tmp=ptr[j];
					if(tmp==local_res[j].size()) continue;
					if(idx==-1 or local_res[j][tmp]<mn) {
						mn=local_res[j][tmp];
						idx=j;
					}
				}
				if(i==k-1) cout<<mn<<endl;
				ptr[idx]++;
			}
		}
	}
	
	else {
		int ni;
		MPI_Recv(&ni, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);


		vector<int> local_arr(ni);
		for(int i=0;i<ni;i++)
			MPI_Recv(&local_arr[i], 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		sort(local_arr.begin(), local_arr.end());

		for(int i=0;i<ni;i++)
			MPI_Send(&local_arr[i], 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}

	MPI_Finalize();

	return 0;
}
