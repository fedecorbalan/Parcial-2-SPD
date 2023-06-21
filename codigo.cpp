#include <IRremote.h>
#include <Servo.h>
#include <LiquidCrystal.h>
//se incluyen las bibliotecas correspondientes a los componetes
//utilizados y se definen las variables necesarias


int sensor_ir = 3;

LiquidCrystal lcd(4,5,6,7,8,9);
#define led_rojo 12
#define led_verde 11
#define servo 10

#define boton_1 0xEF10BF00
#define boton_2 0xEE11BF00

Servo servoMotor;

void movimientoServo();
void funcionemientoSistema();
void deteccionTemperatura(int heat);


bool flag_sistema = false;
  
void setup()
{
  lcd.begin(16,2);
  pinMode(led_rojo, OUTPUT);
  pinMode(led_verde, OUTPUT);
  
  servoMotor.attach(10);
 	
  Serial.begin(9600);
  IrReceiver.begin(sensor_ir, DISABLE_LED_FEEDBACK);
}

void loop()
{
  if (IrReceiver.decode())
  {
    Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
    if (IrReceiver.decodedIRData.decodedRawData == boton_1 && flag_sistema == false)
    {
      flag_sistema = true;
    }
    else if (IrReceiver.decodedIRData.decodedRawData == boton_2)
    {
      flag_sistema = false;
    }
   IrReceiver.resume();
  }
 funcionamientoSistema();
 delay(600);
}

void funcionamientoSistema()
{
 if(flag_sistema == false)
 {
   lcd.clear();
   digitalWrite(led_verde, LOW);
   digitalWrite(led_rojo, HIGH);
 }
 else
 {
   digitalWrite(led_rojo, LOW);
   digitalWrite(led_verde,HIGH);
   deteccionTemperatura();
 }
  
}

void deteccionTemperatura()
{
  int lector_sensor = analogRead(A0);
  int temperatura = map(lector_sensor, 20, 358, -40, 125);
  if (temperatura <= 14)
  {
    lcd.print(temperatura);
    lcd.print(" Grados");
    lcd.setCursor(0, 1);
    lcd.print("Invierno");
    delay(3000);
    lcd.clear();
  }
  else if(temperatura <= 24)
  {
    lcd.print(temperatura);
    lcd.print(" Grados");
    lcd.setCursor(0, 1);
    lcd.print("Otono/Primavera");
    delay(3000);
    lcd.clear();
  }
  else if(temperatura <= 45)
  {
    lcd.print(temperatura);
    lcd.print(" Grados");
    lcd.setCursor(0, 1);
    lcd.print("Verano");
    delay(3000);
    lcd.clear();
  }
  else if(temperatura <= 59)
  {
    lcd.print(temperatura);
    lcd.print(" Grados");
    lcd.setCursor(0, 1);
    lcd.print("Calor Excesivo");
    delay(3000);
    lcd.clear();
  }
  else if(temperatura >= 60)
  {
    lcd.print(temperatura);
    lcd.print(" Grados");
    lcd.setCursor(0, 1);
    lcd.print("INCENDIO");
    movimientoServo();
    delay(1000);
    lcd.clear();
  }
}

void movimientoServo()
{
  servoMotor.write(0);
  delay(500);
  servoMotor.write(180);
}
