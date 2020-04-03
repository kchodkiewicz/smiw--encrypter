#include <LiquidCrystal.h>
#include <Keypad.h>

// LCD setup
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
int display_boundry_low = 0;
int display_boundry_high = 16;

// array setup
#define BUFFER_ARRAY_SIZE 50 // amount of digits that are remembered by program
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

Keypad keyboard = Keypad(makeKeymap(keys), row_pins, col_pins, ROWS, COLS);

void setup() {
  // initialize LCD
  lcd.begin(16, 2);

  lcd.setCursor(0, 0);
  lcd.print("key:");

  lcd.setCursor(5, 0);
  lcd.print("A:");

  lcd.setCursor(9, 0);
  lcd.print("B:");

  lcd.setCursor(13, 0);
  lcd.print("C:");

  // initialize array with blank characters
  char placeholder_ = ' ';
  for (int i = 0; i < BUFFER_ARRAY_SIZE; i++) {
    enc_data[i] = placeholder_;
  }
}

char encrypt(int x, int a, int b, int c) { // ENCRYPTION ALGORITHM
  int input = int(x);
  
  // TODO modify algorithm
  
  return char(input);
}


void loop() {
  // display value of the key
  pot_A = analogRead(A0);
  val_A = map(pot_A, 0, 1021, 0, 9);
  lcd.setCursor(7, 0);
  lcd.print(val_A);

  pot_B = analogRead(A1);
  val_B = map(pot_B, 0, 1021, 0, 9);
  lcd.setCursor(11, 0);
  lcd.print(val_B);

  pot_C = analogRead(A2);
  val_C = map(pot_C, 0, 1021, 0, 9);
  lcd.setCursor(15, 0);
  lcd.print(val_C);
  delay(250);

  // get input from keyboard
  char digit = keyboard.getKey();
  if (digit) {
    if (digit == '*' || digit == '#') { // shift last inputs
      switch (digit){
        case '*':
          if (display_boundry_high > BUFFER_ARRAY_SIZE-1){
            break;
          } else {
            display_boundry_low++;
            display_boundry_high++;
          }
          break;
        case '#':
          if (display_boundry_low < 1){
            break;
          } else {
            display_boundry_low--;
            display_boundry_high--;            
          }
          break;
      }
    } else {
      // move digits in array and add newest at index 0
      for (int k = BUFFER_ARRAY_SIZE - 2; k >= 0; k--) {
        int l = k;
        enc_data[l+1] = enc_data[k];
      }
      enc_data[0] = encrypt(digit, val_A, val_B, val_C);
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
// TODO add setting caret to default position
