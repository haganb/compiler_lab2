#include <stdio.h>
#include <stdlib.h>

int main(){
	// Variable and temporary variable initialization
	int A, B, C, D, E, F;
	int Tmp1, Tmp2, Tmp3, Tmp4;

	//Using printf and scanf to get user input values

	printf("A=");
	scanf("%d", &A);

	printf("B=");
	scanf("%d", &B);

	printf("C=");
	scanf("%d", &C);

	printf("D=");
	scanf("%d", &D);

	printf("E=");
	scanf("%d", &E);

	printf("F=");
	scanf("%d", &F);

	//Three-Address Code Representation
	A = B;
	Tmp1 = A + C;
	Tmp2 = Tmp1 * 1;
	B = D;
	Tmp3 = B / 2;
	C = Tmp3;
	D = 4;
	if(D){
		D = 5;
		Tmp4 = D;
	} else {
		Tmp4 = 0;
	}
		
	E = Tmp4;
	F = D;

	//Print final value of variables
	printf("A=%d\n", A);
	printf("B=%d\n", B);
	printf("C=%d\n", C);
	printf("D=%d\n", D);
	printf("E=%d\n", E);
	printf("F=%d\n", F);

	return 0;
}
