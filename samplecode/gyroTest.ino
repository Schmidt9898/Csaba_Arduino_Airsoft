#include<Wire.h>
const int MPU = 0x68;
int Tmp, AcX, AcY, AcZ, GyX, GyY, GyZ, ujX, ujY;

void setup() {
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(9600);
}
void loop() {


  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 12, true);
  AcX = Wire.read() << 8 | Wire.read();
  AcY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();
  GyX = Wire.read() << 8 | Wire.read();
  GyY = Wire.read() << 8 | Wire.read();
  GyZ = Wire.read() << 8 | Wire.read();


  Serial.print("Gyorsulas: ");
  Serial.print("X = "); Serial.print(AcX);
  Serial.print(" | Y = "); Serial.print(AcY);
  Serial.println(" ");
  delay(500);

  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 12, true);
  ujX = Wire.read() << 8 | Wire.read();
  ujY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();
  GyX = Wire.read() << 8 | Wire.read();
  GyY = Wire.read() << 8 | Wire.read();
  GyZ = Wire.read() << 8 | Wire.read();


  Serial.print("Gyorsulas: ");
  Serial.print("ujX = "); Serial.print(ujX);
  Serial.print("ujY = "); Serial.print(ujY);
  Serial.println(" ");

  Serial.println(AcX - ujX);
  Serial.println(AcY - ujY);

  if (AcX - 1000 < ujX  and ujX < AcX + 1000 and AcY - 1000 < ujY  and ujY < AcY + 1000 )
  {
    Serial.println("Nyugi van");
  }
  else
  {
    Serial.println("ELMOZDÍTOTTAK");
  }

  Serial.println(" ");
  Serial.println(" ");






}
