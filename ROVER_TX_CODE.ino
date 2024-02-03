/* 
 *  This is te transimmter algorithm definitions. 
 *  Potantiometers A0, A1 , A2, A3
 *  BUTTON DIGITAL 4
 *  JOYSTICK X-> A5  JOYSTİCK Y -> A4
 *  
 *  We need to send potantiometer values and joystick values by NRF. 
 *  We want to send button value also in the NRF Array. 
 *  
 *  When the button pressed, we need to toggle a variable to control zipzip algorithm on the car side. 
 *  
 *  Potantiometer values will be rearranged to control parazits on potatiometer.
 *  
 *
 *  
 *  
 */
//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
//Pin Definitions
#define JOY_X_PIN   A1  //A1          
#define JOY_Y_PIN   A0 // A0
#define JOY_SW_PIN  2           //Used for horn.
#define LF_POT_PIN  A4          //Left front shock absorber potantiometer pin.
#define RF_POT_PIN  A5          //Right front shock absorber potantiometer pin.
#define LB_POT_PIN  A2          //Left behindfshock absorber potantiometer pin.
#define RB_POT_PIN  A3          //Right behind shock absorber potantiometer pin.
#define JUMP_SWITCH 4
#define CE_PIN      9
#define CSN_PIN     8           
#define NRF_CORRECT_LED_PIN  7
#define NRF_FALSE_LED_PIN    6                                
int Joystick_x_value = 0;        // Car right-left
int Joystick_y_value = 0;        // Car forward-reverse
int left_front_absorber= 0;      // Shock absorber left front 
int right_front_absorber= 0;     // Shock absorber rigth front
int left_behind_absorber= 0;     // Shock absorber left behind
int right_behind_absorber= 0;    // Shock absorber rigth behind
int horn = 0;                    // Horn value which will be read joytsick switch.
int jump = 0;                    // This value will be controlled by external button to jumo the car. 
int NRF_ARRAY[8];                                     
                               
RF24 radio(CE_PIN, CSN_PIN);     //create an RF24 object

//address through which two modules communicate.
const byte nrf_address[6] = "00001";      // This adress should be change later.

void setup()
{
  //Serial.begin(9600);
  radio.begin();
   
  pinMode(JOY_X_PIN, INPUT);    // Joystick Right-Left Analog Input.
  pinMode(JOY_Y_PIN, INPUT);    // Joystick forward-reverse  Analog Input
  pinMode(JOY_SW_PIN,INPUT_PULLUP);    // Joystich switch digital input for horn. 

  pinMode(LF_POT_PIN, INPUT);   // Left  front absorber analog input.
  pinMode(RF_POT_PIN, INPUT);   // Right front absorber analog input.
  pinMode(LB_POT_PIN, INPUT);   // Left  behind absorber analog input. 
  pinMode(RB_POT_PIN, INPUT);   // Right behind absorber analog input.
  
  pinMode(JUMP_SWITCH, INPUT_PULLUP);  // Jumping digital input pin for external button. WHEN SWITH OFF, PIN GOES GND.

  pinMode(NRF_CORRECT_LED_PIN, OUTPUT);
  pinMode(NRF_FALSE_LED_PIN, OUTPUT);
  digitalWrite(NRF_CORRECT_LED_PIN, HIGH);
  digitalWrite(NRF_FALSE_LED_PIN, HIGH);
  //set the address
  radio.openWritingPipe(nrf_address);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate( RF24_250KBPS );
  //Set module as transmitter
  radio.stopListening();
  
}
void loop()
{ 
  
  //int x_data = analogRead(A0);
  //int y_data = analogRead(A1);
  Joystick_x_value      =      analogRead (JOY_X_PIN);
  Joystick_y_value      =      analogRead (JOY_Y_PIN);  
  left_front_absorber   =      analogRead (LF_POT_PIN);
  right_front_absorber  =      analogRead (RF_POT_PIN); 
  left_behind_absorber  =      analogRead (LB_POT_PIN);
  right_behind_absorber =      analogRead (RB_POT_PIN);
  horn                  =      !digitalRead(JOY_SW_PIN);
  jump                  =      !digitalRead(JUMP_SWITCH);

   NRF_ARRAY[0] = Joystick_x_value;//ReArrange(Joystick_x_value);
   NRF_ARRAY[1] = Joystick_y_value;//ReArrange(Joystick_y_value); 
   NRF_ARRAY[2] = ReArrange(left_front_absorber);
   NRF_ARRAY[3] = ReArrange(right_front_absorber);
   NRF_ARRAY[4] = ReArrange(left_behind_absorber);
   NRF_ARRAY[5] = ReArrange(right_behind_absorber);
   NRF_ARRAY[6] = horn; 
   NRF_ARRAY[7] = jump;
  //Send message to receiver
  //const char text[] = "hello world";
  if (radio.write(&NRF_ARRAY, sizeof(NRF_ARRAY)))
  {
  //digitalWrite(LED_BUILTIN, HIGH);
  //digitalWrite(NRF_FALSE_LED_PIN, LOW);
  }
  else 
  {
  //digitalWrite(NRF_CORRECT_LED_PIN, LOW);
  //digitalWrite(NRF_FALSE_LED_PIN, HIGH);
  }

//  for(int i=0;i<8;i++)
//  {
//    Serial.print(NRF_ARRAY[i]);
//    Serial.print("-");
//    
//  }
  //Serial.println();
// Serial.print("x_data:");
// Serial.print(x_data);
// Serial.print("\t");
// Serial.print("y_data:");
// Serial.print(y_data);
// Serial.print("\t");
// Serial.print("sw_data:");
// Serial.println(sw_data);
  
 //delay(100);
}



int ReArrange(int potValue)
{
 if (potValue >= 0 && potValue < 15) {
    return 0;
  } else if (potValue >= 15 && potValue < 20) {
    return 20;
  } else if (potValue >= 20 && potValue < 30) {
    return 25;
  } else if (potValue >= 30 && potValue < 40) {
    return 35;
  } else if (potValue >= 40 && potValue < 50) {
    return 45;
  } else if (potValue >= 50 && potValue < 60) {
    return 55;
  } else if (potValue >= 60 && potValue < 70) {
    return 65;
  } else if (potValue >= 70 && potValue < 80) {
    return 75;
  } else if (potValue >= 80 && potValue < 90) {
    return 85;
  } else if (potValue >= 90 && potValue < 100) {
    return 95;
  }
   else if (potValue >= 100 && potValue < 110) {
    return 105;
  } else if (potValue >= 110 && potValue < 120) {
    return 115;
  } else if (potValue >= 120 && potValue < 130) {
    return 125;
  } else if (potValue >= 130 && potValue < 140) {
    return 135;
  } else if (potValue >= 140 && potValue < 150) {
    return 145;
  } else if (potValue >= 150 && potValue < 160) {
    return 155;
  } else if (potValue >= 160 && potValue < 170) {
    return 165;
  } else if (potValue >= 170 && potValue < 180) {
    return 175;
  } else if (potValue >= 180 && potValue < 190) {
    return 185;
  }
    else if (potValue >= 190 && potValue < 200) {
    return 195;
  } else if (potValue >= 200 && potValue < 210) {
    return 205;
  } else if (potValue >= 210 && potValue < 220) {
    return 215;
  } else if (potValue >= 220 && potValue < 230) {
    return 225;
  } else if (potValue >= 230 && potValue < 240) {
    return 235;
  } else if (potValue >= 240 && potValue < 250) {
    return 245;
  } else if (potValue >= 250 && potValue < 260) {
    return 255;
  } else if (potValue >= 260 && potValue < 270) {
    return 265;
  } else if (potValue >= 270 && potValue < 280) {
    return 275;
  }
    else if (potValue >= 280 && potValue < 290) {
    return 285;
  } else if (potValue >= 290 && potValue < 300) {
    return 295;
  } else if (potValue >= 300 && potValue < 310) {
    return 305;
  } else if (potValue >= 310 && potValue < 320) {
    return 315;
  } else if (potValue >= 320 && potValue < 330) {
    return 325;
  } else if (potValue >= 330 && potValue < 340) {
    return 335;
  } else if (potValue >= 340 && potValue < 350) {
    return 345;
  } else if (potValue >= 350 && potValue < 360) {
    return 355;
  } else if (potValue >= 360 && potValue < 370) {
    return 365;
  } else if (potValue >= 370 && potValue < 380) {
    return 375;
  } else if (potValue >= 380 && potValue < 390) {
    return 385;
  } else if (potValue >= 390 && potValue < 400) {
    return 395;
  } else if (potValue >= 400 && potValue < 410) {
    return 405;
  } else if (potValue >= 410 && potValue < 420) {
    return 415;
  } else if (potValue >= 420 && potValue < 430) {
    return 425;
  } else if (potValue >= 430 && potValue < 440) {
    return 435;
  } else if (potValue >= 440 && potValue < 450) {
    return 445;
  } else if (potValue >= 450 && potValue < 460) {
    return 455;
  } else if (potValue >= 460 && potValue < 470) {
    return 465;
  } else if (potValue >= 470 && potValue < 480) {
    return 475;
  } else if (potValue >= 480 && potValue < 490) {
    return 485;
  } else if (potValue >= 490 && potValue < 500) {
    return 495;
  } else if (potValue >= 500 && potValue < 510) {
    return 505;
  } else if (potValue >= 510 && potValue < 520) {
    return 515;
  } else if (potValue >= 520 && potValue < 530) {
    return 525;
  } else if (potValue >= 530 && potValue < 540) {
    return 535;
  } else if (potValue >= 540 && potValue < 550) {
    return 545;
  } else if (potValue >= 550 && potValue < 560) {
    return 555;
  } else if (potValue >= 560 && potValue < 570) {
    return 565;
  } else if (potValue >= 570 && potValue < 580) {
    return 575;
  } else if (potValue >= 580 && potValue < 590) {
    return 585;
  } else if (potValue >= 590 && potValue < 600) {
    return 595;
  } else if (potValue >= 600 && potValue < 610) {
    return 605;
  } else if (potValue >= 610 && potValue < 620) {
    return 615;
  } else if (potValue >= 620 && potValue < 630) {
    return 625;
  } else if (potValue >= 630 && potValue < 640) {
    return 635;
  } else if (potValue >= 640 && potValue < 650) {
    return 645;
  } else if (potValue >= 650 && potValue < 660) {
    return 655;
  } else if (potValue >= 660 && potValue < 670) {
    return 665;
  } else if (potValue >= 670 && potValue < 680) {
    return 675;
  } else if (potValue >= 680 && potValue < 690) {
    return 685;
  } else if (potValue >= 690 && potValue < 700) {
    return 695;
  } else if (potValue >= 700 && potValue < 710) {
    return 705;
  } else if (potValue >= 710 && potValue < 720) {
    return 715;
  } else if (potValue >= 720 && potValue < 730) {
    return 725;
  } else if (potValue >= 730 && potValue < 740) {
    return 735;
  } else if (potValue >= 740 && potValue < 750) {
    return 745;
  } else if (potValue >= 750 && potValue < 760) {
    return 755;
  } else if (potValue >= 760 && potValue < 770) {
    return 765;
  } else if (potValue >= 770 && potValue < 780) {
    return 775;
  } else if (potValue >= 780 && potValue < 790) {
    return 785;
  } else if (potValue >= 790 && potValue < 800) {
    return 795;
  } else if (potValue >= 800 && potValue < 810) {
    return 805;
  } else if (potValue >= 810 && potValue < 820) {
    return 815;
  } else if (potValue >= 820 && potValue < 830) {
    return 825;
  } else if (potValue >= 830 && potValue < 840) {
    return 835;
  } else if (potValue >= 840 && potValue < 850) {
    return 845;
  } else if (potValue >= 850 && potValue < 860) {
    return 855;
  } else if (potValue >= 860 && potValue < 870) {
    return 865;
  } else if (potValue >= 870 && potValue < 880) {
    return 875;
  } else if (potValue >= 880 && potValue < 890) {
    return 885;
  } else if (potValue >= 890 && potValue < 900) {
    return 895;
  } else if (potValue >= 900 && potValue < 910) {
    return 905;
  } else if (potValue >= 910 && potValue < 920) {
    return 915;
  } else if (potValue >= 920 && potValue < 930) {
    return 925;
  } else if (potValue >= 930 && potValue < 940) {
    return 935;
  } else if (potValue >= 940 && potValue < 950) {
    return 945;
  } else if (potValue >= 950 && potValue < 960) {
    return 955;
  } else if (potValue >= 960 && potValue < 970) {
    return 965;
  } else if (potValue >= 970 && potValue < 980) {
    return 975;
  } else if (potValue >= 980 && potValue < 990) {
    return 985;
  } else if (potValue >= 990 && potValue < 1000) {
    return 995;
  } else if (potValue >= 1000 && potValue < 1024) {
    return 1020;
  }
  
}

/*   jumperlar sola bakacak şekilde tutulur. 
 *    joystick; 
 *    en sola çekilince x_data 0'a yaklaşıyor.
 *    en sağda x_data 1024.
 *    ortada 500-520 arasında.
 *    
 *    joystick;
 *    en yukarda y_data 0'a yaklaşıyor.
 *    en aşağıda y_data 1024'e.
 *    ortada 500-520 arasında.
 *    */
 
