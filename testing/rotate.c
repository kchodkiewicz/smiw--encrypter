#include <stdio.h>
#include <stdlib.h>
#define ROTOR_SIZE 10
const int rotA[ROTOR_SIZE] = {1, 2, 3, 9, 5, 6, 8, 7, 4, 0};
const int rotB[ROTOR_SIZE] = {6, 4, 8, 0, 1, 5, 3, 2, 9, 7};
const int rotC[ROTOR_SIZE] = {0, 6, 9, 5, 1, 2, 3, 4, 7, 8};
char crypt(char x, int a, int b, int c, char mode) { // ENCRYPTION ALGORITHM
    // offset rotors by a,b,c
    int tmpA[ROTOR_SIZE];
    for (int it=0; it<ROTOR_SIZE;it++){
      tmpA[it] = rotA[it];
    }
    for (int i=0;i<a;i++){
      int holder = tmpA[0];
      for (int j=0;j<ROTOR_SIZE-1;j++)
        tmpA[j] = tmpA[j+1];
      tmpA[ROTOR_SIZE-1] = holder;
    }

    int tmpB[ROTOR_SIZE];
    for (int it=0; it<ROTOR_SIZE;it++){
      tmpB[it] = rotB[it];
    }
    for (int i=0;i<b;i++){
      int holder = tmpB[0];
       for (int j=0;j<ROTOR_SIZE-1;j++)
        tmpB[j] = tmpB[j+1];
      tmpB[ROTOR_SIZE-1] = holder;
    }

    int tmpC[ROTOR_SIZE];
    for (int it=0; it<ROTOR_SIZE;it++){
      tmpC[it] = rotC[it];
    }
    for (int i=0;i<c;i++){
      int holder = tmpC[0];
       for (int j=0;j<ROTOR_SIZE-1;j++)
        tmpC[j] = tmpC[j+1];
      tmpC[ROTOR_SIZE-1] = holder;
    }
    printf("RotorA");
    for (int w=0; w<ROTOR_SIZE;w++)
        printf(" %d", tmpA[w]);
    printf("RotorB");
    for (int w=0; w<ROTOR_SIZE;w++)
        printf(" %d", tmpB[w]);
    printf("RotorC");
    for (int w=0; w<ROTOR_SIZE;w++)
        printf(" %d", tmpC[w]);
    
    // encrypt value x
    int in = atoi(&x);
    int r1 = tmpA[in];
    int r2 = tmpB[r1];
    int r3 = tmpC[r2];
    int r3rot = tmpC[ROTOR_SIZE - 1 - r2]; //r2
    int r2rot = tmpB[r3rot];
    int y = tmpA[r2rot];
    char out = y;
    return out; // ASCII offset

}

int main(){
    char in[] = {'0','1','2','3','4','5','6','7','8','9'};
    int input[] = {0,1,2,3,4,5,6,7,8,9};
    char out[ROTOR_SIZE];
    //for (int a = 0; a < 10; a++)
        //for (int b = 0; b < 10; b++)
            //for (int c = 0; c < 10; c++)
    for (int i = 0; i < 10; i++)
        out[i] = crypt(in[i], 2, 1, 0, 'e');
    
    printf("\n Result:\n");
    for (int j = 0; j<ROTOR_SIZE;j++)        
        printf(" %d", input[j]);
        printf("\n");
    for (int j = 0; j<ROTOR_SIZE;j++)        
        printf(" %d", out[j]);



}