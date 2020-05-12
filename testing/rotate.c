#include <stdio.h>
#include <stdlib.h>
#define ROTOR_SIZE 10

/*
const int rotA[4][ROTOR_SIZE] = { // TODO a kurła miało być pięknie
  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
  {4, 0, 6, 3, 2, 1, 9, 7, 5, 8},
  {4, 3, 1, 5, 7, 0, 9, 8, 2, 6},
  {4, 0, 1, 5, 2, 9, 3, 6, 7, 8},
};
const int rotB[4][ROTOR_SIZE] = {
  {3, 4, 5, 6, 7, 8, 9, 0, 1, 2},
  {5, 0, 3, 1, 2, 4, 6, 9, 7, 8},
  {2, 4, 9, 1, 8, 3, 5, 7, 6, 0},
  {1, 0, 5, 3, 9, 4, 8, 6, 2, 7},
};
const int rotC[4][ROTOR_SIZE] = {
  {6, 7, 8, 9, 0, 1, 2, 3, 4, 5},
  {1, 3, 4, 2, 5, 0, 6, 8, 9, 7},
  {9, 3, 0, 5, 1, 6, 8, 7, 4, 2},
  {1, 0, 8, 3, 5, 2, 7, 9, 6, 4},
};
*/
/*
const int rotA[ROTOR_SIZE][2] = {{0, 5}, {1, 8}, {2, 3}, {3, 7}, {4, 9}, {5, 0}, {6, 2}, {7, 4}, {8, 1}, {9, 6}};
const int rotB[ROTOR_SIZE][2] = {{0, 8}, {1, 2}, {2, 6}, {3, 9}, {4, 3}, {5, 4}, {6, 7}, {7, 1}, {8, 0}, {9, 5}};
const int rotC[ROTOR_SIZE][2] = {{0, 7}, {1, 8}, {2, 0}, {3, 6}, {4, 3}, {5, 2}, {6, 5}, {7, 4}, {8, 9}, {9, 1}};
const int rotX[ROTOR_SIZE][2] = {{0, 5}, {1, 8}, {2, 3}, {3, 2}, {4, 7}, {5, 0}, {6, 9}, {7, 4}, {8, 1}, {9, 6}};
*/
const int rotor[4][ROTOR_SIZE][2] = {
    {{0, 5}, {1, 8}, {2, 3}, {3, 2}, {4, 7}, {5, 0}, {6, 9}, {7, 4}, {8, 1}, {9, 6}},
    {{0, 5}, {1, 8}, {2, 3}, {3, 7}, {4, 9}, {5, 0}, {6, 2}, {7, 4}, {8, 1}, {9, 6}},
    {{0, 8}, {1, 2}, {2, 6}, {3, 9}, {4, 3}, {5, 4}, {6, 7}, {7, 1}, {8, 0}, {9, 5}},
    {{0, 1}, {1, 0}, {2, 8}, {3, 5}, {4, 3}, {5, 2}, {6, 7}, {7, 9}, {8, 6}, {9, 4}},
    {{0, 9}, {1, 3}, {2, 0}, {3, 5}, {4, 1}, {5, 6}, {6, 8}, {7, 4}, {8, 7}, {9, 2}},
    {{0, 7}, {1, 8}, {2, 0}, {3, 6}, {4, 3}, {5, 2}, {6, 5}, {7, 4}, {8, 9}, {9, 1}}
    };

int run(int rotNo, int input, int shift, int mode)
{
  if (mode == 1)
  {
    input = (input + shift) % ROTOR_SIZE;

    return rotor[rotNo][input][1];
  }
  else
  {
    for (int i = 0; i < ROTOR_SIZE; i++)
    {
      if (input == rotor[rotNo][i][1])
      {
        int output = (rotor[rotNo][i][0] - shift);
        while (output < 0)
        {
          output = ROTOR_SIZE + output;
        }
        output = output % ROTOR_SIZE;

        return output;
      }
    }
  }
}

int reverse(int input)
{
  input = (input) % ROTOR_SIZE;
  return rotor[0][input][1];
}

char crypt(char x, int a, int b, int c, int rot_arr[])
{ // ENCRYPTION ALGORITHM
  int in = atoi(&x);
  in = run(rot_arr[0], in, a, 1);
  in = run(rot_arr[1], in, b, 1);
  in = run(rot_arr[2], in, c, 1);
  in = reverse(in);
  in = run(rot_arr[2], in, c, 0);
  in = run(rot_arr[1], in, b, 0);
  in = run(rot_arr[0], in, a, 0);
  char out = in;
  return out; // ASCII offset
}

int main()
{
  int arr_good = 0;
  char in[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
  int input[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int rot_arr[3] = {1,2,3};
  char out[ROTOR_SIZE];
  char check[ROTOR_SIZE];
  for (int aj = 0; aj < 10; aj++)
    for (int bj = 0; bj < 10; bj++)
      for (int cj = 0; cj < 10; cj++)
      {
        for (int i = 0; i < 10; i++)
          out[i] = crypt(in[i], aj, bj, cj, rot_arr);
        for (int g = 0; g < 10; g++)
          check[g] = crypt(out[g] + 48, aj, bj, cj, rot_arr);
        printf("RESULT: ");
        for (int j = 0; j < ROTOR_SIZE; j++)
          printf(" %d", out[j]);
        printf(" |  ");
      }

  printf("\n Result:\n");
  for (int j = 0; j < ROTOR_SIZE; j++)
    printf(" %d", input[j]);
  printf("\n");
  for (int j = 0; j < ROTOR_SIZE; j++)
    printf(" %d", out[j]);
  printf("\n");
  for (int j = 0; j < ROTOR_SIZE; j++)
    printf(" %d", check[j]);
}
