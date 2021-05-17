#include <Arduino.h>
#include <Wire.h>

/*
#define I2C_SDA 14 // SDA Connected to GPIO 14
#define I2C_SCL 15 // SCL Connected to GPIO 15
*/

// slaves i2c address
int i2cSlave = 0x08;


#ifndef LED_BUILTIN
  #define LED_BUILTIN PC13
#endif

#define LED_PIN LED_BUILTIN
bool ledon = false;


// serial data manipulation
union u_tag {
    byte b[4];
    int i;
    float f;
};

union u_tag i2c_data[1][16];

int receivedCount = 0;

void getSlavesUpdate() {
  // global union u_tag i2c_data[][];
  byte i2c_byte[32];
  int i;

  // master reader (first loop data request)
  for(i=0; i < 32; i++) {
    i2c_byte[i] = 0;
  }
  i = 0;
  Wire.requestFrom(i2cSlave, 6); // request 6 bytes from slave device #8
  while (Wire.available()) { // slave may send less than requested
    char c = Wire.read(); // receive a byte as character
    i2c_byte[i] = c;
    i++;
    receivedCount++;
  } 
  for(i=0; i < 8; i++) {
    i2c_data[0][i].b[0] = i2c_byte[i*4 + 0];
    i2c_data[0][i].b[1] = i2c_byte[i*4 + 1];
    i2c_data[0][i].b[2] = i2c_byte[i*4 + 2];
    i2c_data[0][i].b[3] = i2c_byte[i*4 + 3];
  }
}

void setup() {

  Wire.begin(14, 15, 4000000);

  Serial.begin(115200);
  while (!Serial)
  {
      ;
  }
  Serial.println(F("Serial begin done"));
  Serial.println(F("Wire begin done"));
}

void loop() {
  static int i = 0; 

  byte error;

  Wire.beginTransmission(i2cSlave); 
  Wire.write(0); // cmd
  Wire.write(90); // cmd
  Wire.write(32); // cmd
  Wire.write("0"); // cmd
  Wire.write((byte*)&i, 4);
  error = Wire.endTransmission();

  Serial.print(i);
  Serial.print("  ");
  Serial.println(error);

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (i2cSlave<16)
        Serial.print("0");
      Serial.print(i2cSlave,HEX);
      Serial.println("  !");
 
    }

  i++;
  

  getSlavesUpdate();

  Serial.print(i2c_data[0][0].b[0]); Serial.print("\t");
  Serial.print(i2c_data[0][0].b[1]); Serial.print("\t");
  Serial.print(i2c_data[0][0].b[2]); Serial.print("\t");
  Serial.print(i2c_data[0][0].b[3]); Serial.print("\t");

  Serial.print(i2c_data[0][1].b[0]); Serial.print("\t");
  Serial.print(i2c_data[0][1].b[1]); Serial.print("\t");
  Serial.print(i2c_data[0][1].b[2]); Serial.print("\t");
  Serial.print(i2c_data[0][1].b[3]); Serial.print("\t");

  Serial.print(i2c_data[0][2].f); Serial.print("\t");
  Serial.print(i2c_data[0][3].f); Serial.print("\t");
  Serial.print(i2c_data[0][4].f); Serial.print("\t");
  Serial.print(i2c_data[0][5].f); Serial.print("\t");
  Serial.print(i2c_data[0][6].i); Serial.print("\t");
  Serial.print(i2c_data[0][7].i); Serial.print("\t");
  Serial.print(receivedCount); Serial.print("\t");


  Serial.println("\t");

  delay(1000);
}