/*
    YM2149F

    This is a simple Arduino sketch to test the YM2149F chip.

    The circuit: TODO 

    Created 09/02/2024
    By Daniele Galloppo
    Modified 10/02/2024
    By Daniele Galloppo

    https://github.com/DG266/YM2149F
*/
 
#include <SPI.h>

const int BC1_PIN = A5;
const int BDIR_PIN = A4;

const int SS_PIN = 10;  
const int LATCH_PIN = 4;       

const int RESET_PIN = 8;

void inactiveMode() {
  digitalWrite(BC1_PIN, LOW);
  digitalWrite(BDIR_PIN, LOW);
}

void writeMode() {
  digitalWrite(BC1_PIN, LOW);
  digitalWrite(BDIR_PIN, HIGH);
}

void addressMode() {
  digitalWrite(BC1_PIN, HIGH);
  digitalWrite(BDIR_PIN, HIGH);
}

void shiftRegisterOut(uint8_t data) {
  PORTD = PORTD & 0b11101111;  // D4 pin LOW
  SPI.transfer(data);
  PORTD = PORTD | 0b00010000;  // D4 pin HIGH
}

void writeRegister(uint8_t reg, uint8_t val) {
  addressMode();
  shiftRegisterOut(reg);
  inactiveMode();

  writeMode();
  shiftRegisterOut(val);
  inactiveMode();
}

void resetChip() {
  digitalWrite(RESET_PIN, LOW);
  delay(1);
  digitalWrite(RESET_PIN, HIGH);
}

void setup() {
  //// Generate a clock signal of 2 MHz on pin 9 (see ATmega328P datasheet!)
  // Toggle OC1A (pin 9) on compare match (Table 15-2 on the datasheet)
  TCCR1A = (1 << COM1A0);   
  // Enable CTC +  no prescaling (Table 15-5 and Table 15-6 on the datasheet)    
  // CTC = Clear Timer on Compare Match          
  TCCR1B = (1 << WGM12) | (1 << CS10);    
  // 00000011 = 3 -> 2 MHz (see section 15.9.2 for more details)
  OCR1A = 0b00000011;
  pinMode(9, OUTPUT);

  pinMode(RESET_PIN, OUTPUT);
  pinMode(BC1_PIN, OUTPUT);
  pinMode(BDIR_PIN, OUTPUT);

  pinMode(LATCH_PIN, OUTPUT);
  pinMode(SS_PIN, OUTPUT);

  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  Serial.begin(9600);


  inactiveMode();
  resetChip();

  // Mixer settings: the important bits are the first 6.
  // In this case, I want to generate sounds only on Channel A,
  // so I set the first bit to 0 (0 to enable, 1 to disable).
  writeRegister(0b00000111, 0b00111110);

  // Audio level control: maximum volume on Channel A
  writeRegister(0b00001000, 0b00001111);
}

void loop() {
  // Play some notes...
  // (If you can hear something, it works!)
  writeRegister(0b00000000, 12);
  writeRegister(0b00000001, 1);
  delay(500);
  
  writeRegister(0b00000000, 28);
  writeRegister(0b00000001, 1);
  delay(500);

  writeRegister(0b00000000, 12);
  writeRegister(0b00000001, 1);
  delay(500);

  writeRegister(0b00000000, 239);
  writeRegister(0b00000001, 0);
  delay(500);

  writeRegister(0b00000000, 213);
  writeRegister(0b00000001, 0);
  delay(500);

  writeRegister(0b00000000, 239);
  writeRegister(0b00000001, 0);
  delay(500);

  writeRegister(0b00000000, 12);
  writeRegister(0b00000001, 1);
  delay(500);

  writeRegister(0b00000000, 28);
  writeRegister(0b00000001, 1);
  delay(500);

  writeRegister(0b00000000, 63);
  writeRegister(0b00000001, 1);
  delay(500);

  writeRegister(0b00000000, 102);
  writeRegister(0b00000001, 1);
  delay(500);

  writeRegister(0b00000000, 63);
  writeRegister(0b00000001, 1);
  delay(500);

  writeRegister(0b00000000, 28);
  writeRegister(0b00000001, 1);
  delay(500);

  writeRegister(0b00000000, 12);
  writeRegister(0b00000001, 1);
  delay(500);

  writeRegister(0b00000000, 28);
  writeRegister(0b00000001, 1);
  delay(500);

  writeRegister(0b00000000, 63);
  writeRegister(0b00000001, 1);
  delay(500);

  writeRegister(0b00000000, 0);
  writeRegister(0b00000001, 0);
  delay(1500);
}
