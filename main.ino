#include <LiquidCrystal.h>
LiquidCrystal lcd(2,3,4,5,6,7);

void setup() {
  // initialization
  Serial.begin(9600);
  // initialize ADC
  
  
  // initialize keyboard

  // initialize LCD
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("key:");
  
  lcd.setCursor(5,0);
  lcd.print("A:");

  lcd.setCursor(9,0);
  lcd.print("B:");

  lcd.setCursor(13,0);
  lcd.print("C:");
 
  Serial.println("init -- DONE");
}
// initialize potentiometer variables
int pot_A = 0;
int pot_B = 0;
int pot_C = 0;

int val_A = 0;
int val_B = 0;
int val_C = 0;

#define buffer_array_size 50 // amount of digits that are remembered by program

void loop() {
  // display value of the key
  pot_A = analogRead(A0); 
  val_A = map(pot_A, 0, 1021, 9, 0); // CHANGE TO 0,9  [ ALL POTENTIOMETERS MUST BE ATTACHED TO VCC AND GND IN THE SAME ORDER ]
  lcd.setCursor(7,0);
  lcd.print(val_A);

  pot_B = analogRead(A1);
  val_B = map(pot_B, 0, 1021, 0, 9);
  lcd.setCursor(11,0);
  lcd.print(val_B);

  pot_C = analogRead(A2);
  val_C = map(pot_C, 0, 1021, 0, 9);
  lcd.setCursor(15,0);
  lcd.print(val_C);
  delay(250);
  
  // get input from keyboard

  int enc_data[buffer_array_size];
  
  // encrypt data
  
  
  // display encrypted data
  //enc_data[]
  
}
