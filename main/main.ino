#include <LiquidCrystal.h>
#include <Keypad.h>

// LCD setup
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
int display_boundry_low = 0;
int display_boundry_high = 16;

// array setup
#define BUFFER_ARRAY_SIZE 50 // amount of digits that are stored in memory
char enc_data[BUFFER_ARRAY_SIZE]; // array containing 50 last characters

// initialize potentiometer variables
int pot_A = 0; int val_A = 0;
int pot_B = 0; int val_B = 0;
int pot_C = 0; int val_C = 0;

// keyboard setup
const byte ROWS = 4;
const byte COLS = 3;

byte row_pins[ROWS] = {1, 8, 9, 10};
byte col_pins[COLS] = {11, 12, 13};

char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

bool is_setup_complete = false;

Keypad keyboard = Keypad(makeKeymap(keys), row_pins, col_pins, ROWS, COLS);

void setup() {
  // initialize LCD
  lcd.begin(16, 2);
  // initialize array with blank characters
  char placeholder_ = ' ';
  for (int i = 0; i < BUFFER_ARRAY_SIZE; i++) {
    enc_data[i] = placeholder_;
  }
  lcd.setCursor(4, 0);
  lcd.print("Welcome!");
  delay(1000);
  lcd.clear();
}
#define ROTOR_SIZE 10
const int rotA[5][ROTOR_SIZE] = { // TODO a kurła miało być pięknie
  {1, 3, 0, 7, 5, 4, 8, 2, 6, 9},
  {1, 9, 7, 6, 2, 3, 8, 0, 5, 4},
  {8, 7, 3, 2, 5, 9, 1, 6, 4, 0},
  {6, 3, 4, 5, 1, 9, 2, 8, 0, 7},
  {1, 2, 3, 9, 5, 6, 8, 7, 4, 0}
};
const int rotB[5][ROTOR_SIZE] = {
  {5, 9, 2, 6, 3, 8, 7, 4, 1, 0},
  {7, 2, 9, 0, 3, 4, 6, 5, 8, 1},
  {6, 5, 2, 7, 8, 1, 9, 4, 3, 0},
  {3, 9, 5, 6, 0, 4, 2, 1, 8, 7},
  {6, 4, 8, 0, 1, 5, 3, 2, 9, 7}
};
const int rotC[5][ROTOR_SIZE] = {
  {8, 7, 0, 6, 4, 1, 9, 5, 2, 3},
  {2, 5, 4, 1, 9, 6, 8, 7, 0, 3},
  {6, 3, 0, 2, 4, 5, 9, 7, 1, 8},
  {1, 0, 7, 6, 4, 2, 8, 9, 3, 5},
  {0, 6, 9, 5, 1, 2, 3, 4, 7, 8}
};

char crypt(char x, int a, int b, int c, int mode[]) { // ENCRYPTION ALGORITHM


  // offset rotors by a,b,c
  int tmpA[ROTOR_SIZE];
  for (int it = 0; it < ROTOR_SIZE; it++) {
    tmpA[it] = rotA[mode[0]][it];
  }
  for (int i = 0; i < a; i++) {
    int holder = tmpA[0];
    for (int j = 0; j < ROTOR_SIZE - 1; j++)
      tmpA[j] = tmpA[j + 1];
    tmpA[ROTOR_SIZE - 1] = holder;
  }

  int tmpB[ROTOR_SIZE];
  for (int it = 0; it < ROTOR_SIZE; it++) {
    tmpB[it] = rotB[mode[1]][it];
  }
  for (int i = 0; i < b; i++) {
    int holder = tmpB[0];
    for (int j = 0; j < ROTOR_SIZE - 1; j++)
      tmpB[j] = tmpB[j + 1];
    tmpB[ROTOR_SIZE - 1] = holder;
  }

  int tmpC[ROTOR_SIZE];
  for (int it = 0; it < ROTOR_SIZE; it++) {
    tmpC[it] = rotC[mode[2]][it];
  }
  for (int i = 0; i < c; i++) {
    int holder = tmpC[0];
    for (int j = 0; j < ROTOR_SIZE - 1; j++)
      tmpC[j] = tmpC[j + 1];
    tmpC[ROTOR_SIZE - 1] = holder;
  }

  // encrypt value x
  int r1 = tmpA[x-48];
  int r2 = tmpB[r1];
  int r3 = tmpC[r2];
  int r3rot = tmpC[ROTOR_SIZE - 1 - r2];
  int r2rot = tmpB[r3rot];
  int y = tmpA[r2rot];
  return char(y + 48); // ASCII offset



  /*
    int output = -1;
    int input = int(x);
    // do very complicated math algorithm magic
    a = a % 10;
    b = b % 5;
    c = c % 2;
    output = (input + a) % 10 + (input + b) % 10 + (input + c) % 10;

    while (true) {
    if (output > 9) { // recreate if out of [0,9] range
      int dec = output;
      int multiplier = dec / 10;
      output = output - 10 * multiplier;
      return char(output + 48); // it's ascii so add 48
    } else
      return char(output + 48);
    }*/


}

void mainFun(int mode[]) {
  
  // display key
  lcd.setCursor(1, 0);
  lcd.print("A:");

  lcd.setCursor(6, 0);
  lcd.print("B:");

  lcd.setCursor(11, 0);
  lcd.print("C:");

  pot_A = analogRead(A0);
  val_A = map(pot_A, 0, 1021, 0, 9);
  if (val_A < 10) {
    lcd.setCursor(3, 0);
    lcd.print("0");
    lcd.setCursor(4, 0);
  }
  else
    lcd.setCursor(3, 0);
  lcd.print(val_A);

  pot_B = analogRead(A1);
  val_B = map(pot_B, 0, 1021, 0, 9);
  if (val_B < 10) {
    lcd.setCursor(8, 0);
    lcd.print("0");
    lcd.setCursor(9, 0);
  }
  else
    lcd.setCursor(8, 0);
  lcd.print(val_B);

  pot_C = analogRead(A2);
  val_C = map(pot_C, 0, 1021, 0, 9);
  if (val_C < 10) {
    lcd.setCursor(13, 0);
    lcd.print("0");
    lcd.setCursor(14, 0);
  }
  else
    lcd.setCursor(13, 0);
  lcd.print(val_C);
  delay(250);

  // get input from keyboard
  char digit = keyboard.getKey();
  if (digit) {
    if (digit == '*' || digit == '#') { // shift last inputs
      switch (digit) {
        case '*':
          if (display_boundry_high > BUFFER_ARRAY_SIZE - 1) {
            break;
          } else {
            display_boundry_low++;
            display_boundry_high++;
          }
          break;
        case '#':
          if (display_boundry_low < 1) {
            break;
          } else {
            display_boundry_low--;
            display_boundry_high--;
          }
          break;
      }
    } else {
      // reset digits' shifting
      display_boundry_low = 0;
      display_boundry_high = 16;
      // move digits in array and add newest at index 0
      for (int k = BUFFER_ARRAY_SIZE - 2; k >= 0; k--) {
        int l = k;
        enc_data[l + 1] = enc_data[k];
      }
      enc_data[0] = crypt(digit, val_A, val_B, val_C, mode);
    }
  }

  // display encrypted data
  int j = 15;
  for (int i = display_boundry_low; i < display_boundry_high; i++) {
    lcd.setCursor(j, 1);
    int num = lcd.print(enc_data[i]);
    j--;
  }
}


bool flag = false;
int mode[] = {0, 0, 0};
int digit_counter = 0;

void loop() {
  if (!is_setup_complete) {
    //while (!is_setup_complete) {
    lcd.setCursor(1, 0);
    lcd.print("Pick 3 numbers");
    lcd.setCursor(0, 1);
    lcd.print("[0,4]:");
    char digit = keyboard.getKey();

    if (digit) {
      if (digit == '0' || digit == '1' || digit == '2' || digit == '3' || digit == '4') {
        mode[digit_counter] = int(digit)-48;
        digit_counter++;
      }
    }
    int j = 15;
    for (int i = 0; i < 3; i++) {
      lcd.setCursor(j, 1);
      int num = lcd.print(mode[i]);
      j--;
    }
    if (digit_counter > 2)
      flag = true;
    //}

    if (flag == true) {
      delay(750);
      lcd.clear();
      is_setup_complete = true;
    }

  } else {
    mainFun(mode);
  }
}
