#include <LiquidCrystal.h>
#include <Keypad.h>


// -------------------------------------------------------
// ------------------------ SETUP ------------------------
// LCD setup
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

// array setup
#define BUFFER_ARRAY_SIZE 500 // amount of digits that are stored in memory
char enc_data[BUFFER_ARRAY_SIZE]; // array containing <BUFFER_ARRAY_SIZE> last characters

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

// ------------------------------------------------------------
// ------------------------ ENCRYPTION ------------------------
#define ROTOR_SIZE 10
const int rotor[6][ROTOR_SIZE][2] = {
  {{0, 5}, {1, 8}, {2, 3}, {3, 2}, {4, 7}, {5, 0}, {6, 9}, {7, 4}, {8, 1}, {9, 6}},
  {{0, 5}, {1, 8}, {2, 3}, {3, 7}, {4, 9}, {5, 0}, {6, 2}, {7, 4}, {8, 1}, {9, 6}},
  {{0, 8}, {1, 2}, {2, 6}, {3, 9}, {4, 3}, {5, 4}, {6, 7}, {7, 1}, {8, 0}, {9, 5}},
  {{0, 1}, {1, 0}, {2, 8}, {3, 5}, {4, 3}, {5, 2}, {6, 7}, {7, 9}, {8, 6}, {9, 4}},
  {{0, 9}, {1, 3}, {2, 0}, {3, 5}, {4, 1}, {5, 6}, {6, 8}, {7, 4}, {8, 7}, {9, 2}},
  {{0, 7}, {1, 8}, {2, 0}, {3, 6}, {4, 3}, {5, 2}, {6, 5}, {7, 4}, {8, 9}, {9, 1}}
};
/*
 * move value according to rotors
 */
int rotate(int rotNo, int input, int shift, int mode) {
  if (mode == 1) {
    input = (input + shift) % ROTOR_SIZE;
    return rotor[rotNo][input][1];
  } else {
    for (int i = 0; i < ROTOR_SIZE; i++) {
      if (input == rotor[rotNo][i][1]) {
        int output = (rotor[rotNo][i][0] - shift);
        while (output < 0) 
          output = ROTOR_SIZE + output;
        output = output % ROTOR_SIZE;
        return output;
      }
    }
  }
}
/*
 * reverse the path of return
 */
int reverse(int input) {
  input = (input) % ROTOR_SIZE;
  return rotor[0][input][1];
}
/*
 * change value
 */
char crypt(char x, int a, int b, int c, int rot_arr[]) {
  int in = (x - 48); // ASCII offset
  //encrypt data
  in = rotate(rot_arr[0], in, a, 1);
  in = rotate(rot_arr[1], in, b, 1);
  in = rotate(rot_arr[2], in, c, 1);
  in = reverse(in);
  in = rotate(rot_arr[2], in, c, 0);
  in = rotate(rot_arr[1], in, b, 0);
  in = rotate(rot_arr[0], in, a, 0);
  return char(in + 48); // ASCII offset
}

// ------------------------------------------------------
// ------------------------ MAIN ------------------------
// initialize potentiometer variables
int pot_A = 0; int val_A = 0;
int pot_B = 0; int val_B = 0;
int pot_C = 0; int val_C = 0;

int display_boundry_low = 0;
int display_boundry_high = 16;

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

// ------------------------------------------------------
// ------------------------ LOOP ------------------------
bool flag = false;
int mode[] = {0, 0, 0};
int digit_counter = 0;
bool is_setup_complete = false;

void loop() {
  if (!is_setup_complete) {
    lcd.setCursor(0, 0);
    lcd.print("3 unique numbers");
    lcd.setCursor(0, 1);
    lcd.print("[1,5]:");
    char digit = keyboard.getKey();

    if (digit) {
      if (digit == '1' || digit == '2' || digit == '3' || digit == '4' || digit == '5') {
        // check if unique numbers
        if (((int(digit) - 48) != mode[0]) && ((int(digit) - 48) != mode[1]) && ((int(digit) - 48) != mode[2])) {
          mode[digit_counter] = int(digit) - 48; // ASCII offset
          digit_counter++;
        } else {
          // if not unique -> print error
          lcd.setCursor(0, 0);
          lcd.print("Incorrect number");
          delay(750);
        }
      } else {
          // if number out of range -> print error
          lcd.setCursor(0, 0);
          lcd.print("Incorrect number");
          delay(750);
        }
    }
    int j = 13;
    for (int i = 0; i < 3; i++) {
      lcd.setCursor(j, 1);
      if (mode[i] == 0) lcd.print("_");
      else lcd.print(mode[i]);
      j++;
    }
    if (digit_counter > 2)
      flag = true;

    if (flag == true) {
      delay(750);
      lcd.clear();
      // if rotors not chosen -> set default values
      if (mode[0] == 0 || mode[1] == 0 || mode[2] == 0)
        mode[0] = 1,
                  mode[1] = 2,
                            mode[2] = 3;
      is_setup_complete = true;
    }
  } else {
    mainFun(mode);
  }
}
