#include <bits/stdc++.h>
// #include <mpi.h>

using namespace std;

int main(int argc, char **argv) {
	int n,m;
	cin>>n>>m;

	double mat[n][2*m];

	memset(mat,0,sizeof(mat));

	for(int i=0;i<n;i++)
		for(int j=0;j<m;j++)
			cin>>mat[i][j];

	for(int i=0;i<n;i++)
		mat[i][m+i]=1;

	for(int i=0;i<n;i++) {
		double tmp=mat[i][i];
		// if(tmp==0) continue;
		for(int j=0;j<2*m;j++)
			mat[i][j]/=tmp;

		for(int k=i+1;k<n;k++) {
			double tmp=mat[k][i];;
			for(int j=0;j<2*m;j++) {
				mat[k][j]-=mat[i][j]*tmp;
			}
		}
	}

	for(int i=n-1;i>=0;i--) {
		for(int k=i-1;k>=0;k--) {
			double tmp=mat[k][i];
			for(int j=0;j<2*m;j++) {
				mat[k][j]-=mat[i][j]*tmp;
			}
		}
	}

	for(int i=0;i<n;i++) {
		for(int j=m;j<2*m;j++)
			cout<<mat[i][j]<<" ";
		cout<<endl;
	}

	return 0;

}