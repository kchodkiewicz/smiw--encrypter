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
}


char crypt(char x, int a, int b, int c, char mode) { // ENCRYPTION ALGORITHM
  if (mode == 'e') {
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
    }
  } else if (mode == 'd') {
    // TODO add decyption xd no clue how fukme
  }

}

void mainFun(char mode) {
  lcd.noBlink();
  // display key
  lcd.setCursor(1, 0);
  lcd.print("A:");

  lcd.setCursor(6, 0);
  lcd.print("B:");

  lcd.setCursor(11, 0);
  lcd.print("C:");

  pot_A = analogRead(A0);
  val_A = map(pot_A, 0, 1021, 0, 15);
  if (val_A < 10) {
    lcd.setCursor(3, 0);
    lcd.print("0");
    lcd.setCursor(4, 0);
  }
  else
    lcd.setCursor(3, 0);
  lcd.print(val_A);

  pot_B = analogRead(A1);
  val_B = map(pot_B, 0, 1021, 0, 15);
  if (val_B < 10) {
    lcd.setCursor(8, 0);
    lcd.print("0");
    lcd.setCursor(9, 0);
  }
  else
    lcd.setCursor(8, 0);
  lcd.print(val_B);

  pot_C = analogRead(A2);
  val_C = map(pot_C, 0, 1021, 0, 15);
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
char mode = 'e';
void loop() {
  if (is_setup_complete == false) {
    lcd.setCursor(2, 0);
    lcd.print("Choose mode!");
    delay(1000);
    lcd.clear();

    while (!is_setup_complete) {
      lcd.setCursor(0, 0);
      lcd.print("Encrypt");
      lcd.setCursor(9, 0);
      lcd.print("Decrypt");

      lcd.setCursor(2, 1);
      lcd.print("[*]");
      lcd.setCursor(11, 1);
      lcd.print("[#]");


      char digit = keyboard.getKey();

      if (digit) {
        if (digit == '*') {
          mode = 'e';
          flag = true;
        } else if (digit == '#') {
          mode = 'd';
          flag = true;
        } else
          flag = false;
      }

      if (flag == true) {
        delay(750);
        lcd.clear();
        is_setup_complete = true;
      }

    }
  } else {
    mainFun(mode);
  }
}
