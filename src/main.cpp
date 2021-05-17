#include <Arduino.h>
#include <Wire.h>

// i2c pins
#define I2C_SDA 14
#define I2C_SCL 15

// slaves i2c address
// allow multiple slaves
int i2cSlave[1] = {0x08};

#ifndef LED_BUILTIN
  #define LED_BUILTIN PC13
#endif

#define LED_PIN LED_BUILTIN
bool ledon = false;

// handle serial numeric data
union u_tag {
  char b[4];
  int i;
  float f;
};

union u_tag i2c_data[1][8];

// debug
int receivedCount = 0;

// request num bytes (bytes) from slave (id)
void getSlavesUpdate(int id, int num) {
  
  char i2c_byte[32];
  int i;

  // init i2c receive buffer
  for(i=0; i < 32; i++) {
    i2c_byte[i] = 0;
  }

  // send request 
  Wire.requestFrom(i2cSlave[id], num);

  // receive data 
  i = 0;
  while (Wire.available()) {
    char c = Wire.read();
    i2c_byte[i] = c;
    i++;

    // debug
    receivedCount++;
  } 

  // store receive data to use later
  for(i=0; i < 8; i++) {
    i2c_data[id][i].b[0] = i2c_byte[i*4 + 0];
    i2c_data[id][i].b[1] = i2c_byte[i*4 + 1];
    i2c_data[id][i].b[2] = i2c_byte[i*4 + 2];
    i2c_data[id][i].b[3] = i2c_byte[i*4 + 3];
  }

}

void setup() {

  Serial.begin(115200);
  while (!Serial) { ; }
  Serial.println(F("Serial begin done"));

  Wire.begin(I2C_SDA, I2C_SCL, 4000000);
  Serial.println(F("Wire begin done"));

}

void loop() {

  static int i = 65500;
  static float f = 0.0;
  byte error;
  int slave_id = 0;

  Wire.beginTransmission(i2cSlave[slave_id]); 
  
  // send 12 bytes
  Wire.write(0);
  Wire.write(90);
  Wire.write(258);
  Wire.write("A");
  Wire.write((byte*)&i, 4);
  Wire.write((byte*)&f, 4);

  error = Wire.endTransmission();

  // debug check for error
  Serial.print(i);
  Serial.print("\t");
  Serial.print(error);
  Serial.print("  ");

  i++;
  f += 0.2;
  
  // request an update with n bytes from slave
  getSlavesUpdate(slave_id, 16);

  // display update from slave
  Serial.print( (char) i2c_data[slave_id][0].b[0]); Serial.print("\t");
  Serial.print( (char) i2c_data[slave_id][0].b[1]); Serial.print("\t");
  Serial.print( (char) i2c_data[slave_id][0].b[2]); Serial.print("\t");
  Serial.print( (char) i2c_data[slave_id][0].b[3]); Serial.print("\t");

  Serial.print( (char) i2c_data[slave_id][1].b[0]); Serial.print("\t");
  Serial.print( (byte) i2c_data[slave_id][1].b[1]); Serial.print("\t");
  Serial.print( (byte) i2c_data[slave_id][1].b[2]); Serial.print("\t");
  Serial.print( (char) i2c_data[slave_id][1].b[3]); Serial.print("\t");

  Serial.print( (int) i2c_data[slave_id][2].i); Serial.print("\t");
  Serial.print( (float) i2c_data[slave_id][3].f); Serial.print("\t");
 
  // debug
  Serial.print(receivedCount); Serial.print("\t");

  Serial.println("\t");

  delay(1000);

}