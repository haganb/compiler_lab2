#include <stdio.h>
#include <stdlib.h>

int main(){
	// Variable and temporary variable initialization
	int A, B, C, D, E, F;
	int Tmp1, Tmp2, Tmp3, Tmp4;

	//Using printf and scanf to get user input values

	printf("B=");
	scanf("%d", &B);

	printf("C=");
	scanf("%d", &C);

	printf("D=");
	scanf("%d", &D);

	printf("E=");
	scanf("%d", &E);

	//Three-Address Code Representation
	S1:	A = B;
	S2:	Tmp1 = A + C;
	S3:	Tmp2 = Tmp1 * 1;
	S4:	B = D;
	S5:	Tmp3 = B / 2;
	S6:	C = Tmp3;
	S7:	D = 4;
	S8:	if(D){
	S9:		D = 5;
	S10:		Tmp4 = D;
	S11:	} else {
	S12:		Tmp4 = 0;
		}
	S13:	E = Tmp4;
	S14:	F = D;

	//Print final value of variables
	printf("A=%d\n", A);
	printf("B=%d\n", B);
	printf("C=%d\n", C);
	printf("D=%d\n", D);
	printf("E=%d\n", E);
	printf("F=%d\n", F);

	return 0;
}
