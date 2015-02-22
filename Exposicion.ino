#include <Matriz.h>
Matriz matriz(6, 9, 5);

//74HC595 controla la matriz led
int pinLatch = 6; //Pin para el latch de los 74CH495
int pinDatos = 5; //Pin para Datos serie del 74CH495
int pinReloj = 9; //Pin para reloj del 74CH495
//Variables para leer el Sensor de temperatura
int ldrPin=A0;
long ldrValue=0;

byte columnaa[7]= {
  B1000000, B0100000, B0010000, B0001000, B0000100, B0000010, B0000001};
byte fila[7]= {
  B1111110, B1111100, B1111100, B1111000, B1111000, B1110000, B1100000};

//TIMER
volatile uint8_t columna=0;

volatile int a = 0;

int buzzerPin = 10;

//Variables para sustituir delay
long previousMillis = 0;
long currentMillis = 0;
long interval = 1000;

//Variables necesarias para imprimir en la matriz
int numero=0;

//Variables relacionadas al botón
int estadoBoton=0;
int botonPin = 3;
long TInicial = 0;
long TActual = 0;
int onOff=1;

//Variables para sustituir delay
volatile long tiempo;

//Variables para leer el Sensor de distancia HC-SN04
volatile int trigPin=11;
volatile int echoPin=12;

int boton1Pin=2;
int boton1State;
int state=0;

int temperaturaPin=A1;
long temperaturaValue=0;    
long temperatura=0;

long tiempoInicial = 0;
long tiempoActual = 0;

void setup(){
  pinMode(botonPin, INPUT);

  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);

  pinMode(buzzerPin, OUTPUT);

  pinMode(pinLatch,OUTPUT);
  pinMode(pinDatos,OUTPUT);
  pinMode(pinReloj,OUTPUT);
}

void loop(){
  boton1State=digitalRead(boton1Pin);
  if(boton1State==1){
    state++;
    if(state>2){
      state=0;
    }
    while(boton1State==1){
      boton1State=digitalRead(boton1Pin);
    }
  }
  if(state==0){
    tiempoActual = millis();
    if(tiempoActual - tiempoInicial > 500) { 
      tiempoInicial = tiempoActual;   
      temperaturaValue= analogRead(temperaturaPin);
      temperatura=(temperaturaValue*5*100)/1024;
    }
    matriz.printNumber(temperatura);
  }
  ///////////////////////////////////////////////
  if(state==1){
    estadoBoton=digitalRead(botonPin);
    if(estadoBoton == HIGH){
      a=estadoBoton;
    }

    if(a == HIGH){
      for(int x; x<128; x++){
        int x1=x/16;
        matriz.printLine(3, 1, 3, x1);
      }
      for(int i=0; i<5; i++){
        digitalWrite(buzzerPin, HIGH);
        delayMicroseconds(956);
        digitalWrite(buzzerPin, LOW);
        delayMicroseconds(956);
      }
      digitalWrite(trigPin,LOW); 
      delayMicroseconds(5);
      digitalWrite(trigPin, HIGH); 
      delayMicroseconds(10);
      tiempo=pulseIn(echoPin, HIGH);
      numero = int(0.017*tiempo);
      a=LOW;
    }
    while(estadoBoton==HIGH){
      estadoBoton=digitalRead(botonPin);
    }
    matriz.printNumber(numero);
  }
  if(state==2){
    ldrValue=analogRead(ldrPin)/145;
    for(int i=0; i<ldrValue; i++){
      digitalWrite(pinLatch, LOW);
      shiftOut(pinDatos, pinReloj, MSBFIRST, fila[i]);
      shiftOut(pinDatos, pinReloj, MSBFIRST, columnaa[i]);
      digitalWrite(pinLatch, HIGH);
    }
  }
}



