/*
	Simple script to count the number of special 
	pairs on a given set of series. It shows 
	both the number of the brute force approach
	but also use the internal (faster) approach.
	So one can compare the results and see if 
	everything is working properly.

 */

#include <cstdio>
#include <algorithm>
#include <cstdlib>
#include <cassert>
#include "heuristics/beginnings/range_list/fpair.cpp"
#include "heuristics/beginnings/range_list/fastSpecialPairs.cpp"


int brute_amount(vector<double>& A){
    int res = 0;
    for (int i = 0; i < (int)A.size() - 1; ++i){
        double menor = A[i + 1];
        double maior = A[i];
        for (int j = i + 1; j < A.size(); ++j){
            menor = min(menor, A[j]);
            if (A[i] < menor && A[j] > maior)
                  ++res;
            maior = max(maior, A[j]);
        }                       
    }
    return res;
}


void show_count(int n, vector<double>& B){
   int nsp = amount_fpairs(B);
   int nsp2 = amSpecialPairs(B);
   //int bnsp = brute_amount(B);
   int len = B.size();
   //printf("%d, %d, %d, %d, %.2lf\n", n, len, nsp, bnsp, nsp/(double)len); 
   //printf("%d, %d, %d, %.2lf\n", n, len, nsp, nsp/(double)len);  
   printf("%d, %d, %d, %d, %.2lf\n", n, len, nsp, nsp2, nsp/(double)len);  
}

int main(){
    string path = "../instances/real_data/data/";
    int serie = 1;
    printf("n, len, #sp, #sp_brute, ratio\n");  
    for (int n = 1; n <= 12; ++n){
        char cpath[150];
        sprintf(cpath, "%sserie%d.in", path.c_str(), n);
        FILE *fin = fopen(cpath, "r");
        if (fin == NULL){
            printf("File %s doesn't exist. Continuing..\n", cpath);
            continue;
            //exit(1);
        }
        vector<double> A;
        while (!feof(fin)){
            double a;
            fscanf(fin, "%lf ", &a);
            A.push_back(a);
        }
        fclose(fin);
        show_count(n, A);
    }
    return 0;
}
