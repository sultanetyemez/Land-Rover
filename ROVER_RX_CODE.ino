//Pin Definitions
#define DRIVER_R_PWM              7       // PWM Output       // TODO reverse
#define DRIVER_L_PWM              6       // PWM Output       // TODO forward
#define CE_PIN                    9           
#define CSN_PIN                   10          
#define BUZZER_HORN               8       // Output pin.    
#define DIRECTION_SERVO           11      // PWM Output       
#define LF_SERVO_ABSORBER         2       // PWM Output          
#define RF_SERVO_ABSORBER         3       // PWM Output       
#define LB_SERVO_ABSORBER         4       // PWM Output       
#define RB_SERVO_ABSORBER         5       // PWM Output       
#define JUMPING_INTERVAL          1000   // JUMP PROCESS WILL BE APPLIED IN 500ms.
#define TEST 0

#if !TEST
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>
Servo Direction; 
Servo LF_ABSORBER_SERVO;
Servo RF_ABSORBER_SERVO;
Servo LB_ABSORBER_SERVO ;
Servo RB_ABSORBER_SERVO; 
int NRF_ARRAY[8];
int DIRECTION_SERVO_DEGREE = 0;
int LF_ABSORBER_SERVO_DEGREE =0;
int RF_ABSORBER_SERVO_DEGREE =0;
int LB_ABSORBER_SERVO_DEGREE =0;
int RB_ABSORBER_SERVO_DEGREE =0;
bool Jumping_Flag = 0;
RF24 radio(CE_PIN, CSN_PIN); // CE, CSN

const byte address[6] = "00001";
unsigned long LastTime = 0;
unsigned long TimeNow = 0;
void setup() {
  Serial.begin(9600);
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate( RF24_250KBPS );
  radio.startListening();
  
  Direction.attach(DIRECTION_SERVO);
  LF_ABSORBER_SERVO.attach(LF_SERVO_ABSORBER);
  RF_ABSORBER_SERVO.attach(RF_SERVO_ABSORBER);
  LB_ABSORBER_SERVO.attach(LB_SERVO_ABSORBER);
  RB_ABSORBER_SERVO.attach(RB_SERVO_ABSORBER);
  pinMode(BUZZER_HORN, OUTPUT);
  pinMode(DRIVER_R_PWM,OUTPUT);
  pinMode(DRIVER_L_PWM,OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  if (radio.available()) {
    digitalWrite(LED_BUILTIN, 1);
    radio.read(&NRF_ARRAY, sizeof(NRF_ARRAY));    
    DIRECTION_SERVO_DEGREE  =  map(NRF_ARRAY[0],0,1023,0,180);
    LF_ABSORBER_SERVO_DEGREE=  map(NRF_ARRAY[2],0,1023,180,0);
    RF_ABSORBER_SERVO_DEGREE=  map(NRF_ARRAY[3],0,1023,0,180);
    LB_ABSORBER_SERVO_DEGREE=  map(NRF_ARRAY[4],0,1023,180,0);
    RB_ABSORBER_SERVO_DEGREE=  map(NRF_ARRAY[5],0,1023,0,180);
    Serial.println(NRF_ARRAY[0]);
    Serial.println(NRF_ARRAY[1]);
    Serial.println(NRF_ARRAY[2]);
    Serial.println(NRF_ARRAY[3]);
    Serial.println(NRF_ARRAY[4]);
    Serial.println(NRF_ARRAY[5]);
    HORN(NRF_ARRAY[6]);
    Direction.write(DIRECTION_SERVO_DEGREE);
    motor_drive(NRF_ARRAY[1]);
    if(NRF_ARRAY[7]== 1)
    {

       TimeNow = millis();
       if(TimeNow-LastTime>JUMPING_INTERVAL)
       {
        LastTime = TimeNow;
        //digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); 
        JUMP_CAR(Jumping_Flag);
        Jumping_Flag = !Jumping_Flag;
       }
    }

    else
    {
      //digitalWrite(LED_BUILTIN, 1);
    LF_ABSORBER_SERVO.write(LF_ABSORBER_SERVO_DEGREE);
    RF_ABSORBER_SERVO.write(RF_ABSORBER_SERVO_DEGREE);
    LB_ABSORBER_SERVO.write(LB_ABSORBER_SERVO_DEGREE);
    RB_ABSORBER_SERVO.write(RB_ABSORBER_SERVO_DEGREE);
      
    }
      for(int i=0;i<8;i++)
  {
    Serial.print(NRF_ARRAY[i]);
    Serial.print("-");
    
  }
  Serial.println();
    
  }
  else
  Serial.println("not available");
  digitalWrite(LED_BUILTIN, 0);
}



#endif

void HORN(int horn_trigger)
{
  if(horn_trigger == 1)
  {
    tone(BUZZER_HORN,255);
    
    
  }

  else
  {
     noTone(BUZZER_HORN);
  }
  
}

void JUMP_CAR(bool FLAG)
{
  if(FLAG == 1)
  {
    LF_ABSORBER_SERVO.write(180);
    RF_ABSORBER_SERVO.write(0);
    LB_ABSORBER_SERVO.write(180);
    RB_ABSORBER_SERVO.write(0);   
  }

  else
  {
    LF_ABSORBER_SERVO.write(0);
    RF_ABSORBER_SERVO.write(180);
    LB_ABSORBER_SERVO.write(0);
    RB_ABSORBER_SERVO.write(180);
  }


  
}

void motor_drive(int Y_Axis)
{

 if (Y_Axis < 512)
 {
 // reverse rotation
 int R_PWM = -(Y_Axis - 511) / 2;
 analogWrite(DRIVER_L_PWM, 0);
 analogWrite(DRIVER_R_PWM, R_PWM);
 //analogWrite(DRIVER_R_PWM,50);
 }
 else
 {
 // forward rotation
  int L_PWM = (Y_Axis - 512) / 2;
 analogWrite(DRIVER_L_PWM, L_PWM);
 //analogWrite(DRIVER_L_PWM, 5);// Sabit hız için, eğer normal çalışması istenirse üstteki yorum satırından cıkarılmalı burası yorum ssatırı olmalı.
 analogWrite(DRIVER_R_PWM, 0);
 }
}

  






























#if TEST
// 18 Mar 2018 - simple program to verify connection between Arduino
//      and nRF24L01+
//  This program does NOT attempt any communication with another nRF24

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <printf.h>

#define CE_PIN   9
#define CSN_PIN 10

const byte thisSlaveAddress[5] = {'R','x','A','A','A'};

RF24 radio(CE_PIN, CSN_PIN);

char dataReceived[10]; // this must match dataToSend in the TX
bool newData = false;


void setup() {
    Serial.begin(9600);
    printf_begin();
    delay(1000);
    Serial.println("CheckConnection Starting");
    Serial.println();
    Serial.println("FIRST WITH THE DEFAULT ADDRESSES after power on");
    Serial.println("  Note that RF24 does NOT reset when Arduino resets - only when power is removed");
    Serial.println("  If the numbers are mostly 0x00 or 0xff it means that the Arduino is not");
    Serial.println("     communicating with the nRF24");
    Serial.println();
    radio.begin();
    
    
    radio.printDetails();
    Serial.println();
    Serial.println();
    Serial.println("AND NOW WITH ADDRESS AAAxR  0x41 41 41 78 52   ON P1");
    Serial.println(" and 250KBPS data rate");
    Serial.println();
    radio.openReadingPipe(1,thisSlaveAddress);
    radio.setDataRate(RF24_250KBPS);
    radio.printDetails();
    Serial.println();
    Serial.println();
    

}


void loop() {

}
#endif
