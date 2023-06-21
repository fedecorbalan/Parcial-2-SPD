![image](https://github.com/fedecorbalan/Parcial-2-SPD/assets/123754871/2bf74583-9656-4795-92c2-f83aaf3ee3a3)

# Segundo parcial SPD: Sistema de incendio con Arduino 🔥
## Alumno: Federico Corbalán 1-J

### El objetivo de este proyecto es diseñar un sistema de incendio utilizando Arduino que puedadetectar cambios de temperatura y activar un servo motor en caso de detectar un incendio.Además, se mostrará la temperatura actual y la estación del año en un display LCD.

## Componentes utilizados 🤖
- Arduino UNO
- Sensor de temperatura
- Control remoto IR (Infrarrojo)
- Display LCD (16x2 caracteres)
- Servo motor
- Cables y 3 resistencias de 220 Ω
- Protoboard para realizar las conexiones
- Dos leds (uno verde y uno rojo).

## Funcionalidad 🤖
El control remoto maneja el sistema, el sistema por defecto viene apagado, por lo tanto, el led rojo va a estar encendido, pero si se presiona el boton 1 del control remoto, se va a prender el led verde y se va a encender el sistema, que dependiendo la temperatura del ambiente, se va a mostrar en un Display LCD 16x2, tanto la temperatura como la estacion correspondiente, pero si se presiona el boton 2, el sistema se apagará.
En el caso de que la temperatura sea mayor a 60 grados, se accionara el servomotor a modo de respuesta.

## Esquema 🤖
![image](https://github.com/fedecorbalan/Parcial-2-SPD/assets/123754871/dc4a76a6-3e35-4f71-be45-03b05453c4ec)

## Circuito 🤖
![image](https://github.com/fedecorbalan/Parcial-2-SPD/assets/123754871/fa11430b-69b4-4a26-a667-1effa2ac2ce4)

## Link al proyecto 🤖
https://www.tinkercad.com/things/9gqUO7eE3Jb

## Setup 🤖
```c++
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
```

## Funcion deteccionTemperatura() 🌡
### Esta funcion imprime en el Display LCD la temperatura y la estacion en la que se encuentra, tambien contiene la reaccion ante el incendio
```c++
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
```

## Funcion movimientoServo() ⚙
### Esta funcion hace posible el movimiento del servomotor, cuando se invoca, este se mueve de 0 a 180 grados.
```c++
void movimientoServo()
{
  servoMotor.write(0);
  delay(500);
  servoMotor.write(180);
}
```

## Funcion funcionamientoSistema() 🤖
### Esta funcion determina el funcionamiento del sistema, previamente se ha declarado una variable global llamada flag_sistema la cual cambia en base a la seleccion de los botones, por lo tanto, si la bandera esta en false, el sistema practicamente esta apagado, pero si esta en true, esta encendido.
```c++
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
```

## Loop ➰
### 
```c++
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
```
