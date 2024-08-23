#include <Wire.h>
#include <LiquidCrystal.h>
#include <Servo.h>

const unsigned long eventInterval = 1000;
unsigned previousTime  = 0;

// SERVO CODES //
Servo horizontal;             // horizontal servo
int servoh = 180;
int servohLimitHigh = 175;
int servohLimitLow = 0;

Servo vertical;            // vertical servo
int servov = 180;
int servovLimitHigh = 175;
int servovLimitLow = 1;
// LDR pin connections  // name = analogpin
int ldrlt = A3;        //LDR top left - BOTTOM LEFT <--- BDG
int ldrrt = A2;       //LDR top rigt - BOTTOM RIGHT
int ldrld = A0;      //LDR down left - TOP LEFT
int ldrrd = A1;     //ldr down rigt - TOP RIGHT


//LCD CODE//
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);       // pins of LCD module to be used
float voltage = 0.0;
float temp = 0.0;                         // variable to store digital vaue of the input
float current;
int power;
float r1 = 10.0;
float r2 = 10.0;
float value = r2 / (r1 + r2);
float current_value;
int volt ;
int cont = 0;
int analog_value;                    // variable to store analog value
int sesnor_value = 0;

void setup() {
  //SERVO CODE
  horizontal.attach(9);          //Connect pin 9 to horizontal servo
  vertical.attach(10);          //Connect pin 10 to vertical servo
  horizontal.write(180);
  vertical.write(180);
  delay(500);

  
  //LCD CODE
  Serial.begin(9600);
  Serial.println("CLEARDATA");
  Serial.println("LABEL,Time,VOLTAGE,CURRENT,POWER");
  
  lcd.begin(16,2);                      // start communication with LCD
  lcd.setCursor (0,0);                 // start the cursor from the beginning
  lcd.print(" SMART SOLAR ");         // Print text in first line
  lcd.setCursor(0,1);                // Move the cursoor to the next line
  lcd.print("  TRACKER  ");         // print text in second line
}

void loop() {
    int lt = analogRead(ldrlt)*15;                  // top left
    int rt = analogRead(ldrrt)*15;                 // top right
    int ld = analogRead(ldrld)*15;                // down left
    int rd = analogRead(ldrrd)*15;               // down right
    int dtime = 10;                             // dtime=diffirence time
    int tol = 90;                              //tol=tolerance
    int avt = (lt + rt);                      // average value top
    int avd = (ld + rd);                     // average value down
    int avl = (lt + ld);                    // average value left
    int avr = (rt + rd);                   // average value right
    int dvert = avt - avd;                // check the diffirence of up and down
    int dhoriz = avl - avr;              // check the diffirence og left and rigt
   
    if (-1*tol > dvert || dvert > tol) 
     {
     if (avt > avd)
     {
     servov = ++servov;
     if (servov > servovLimitHigh)
     {servov = servovLimitHigh;}
     }
     else if (avt < avd)
     {servov= --servov;
     if (servov < servovLimitLow)
     { servov = servovLimitLow;}
     }
     vertical.write(servov);
     }
    if (-1*tol > dhoriz || dhoriz > tol)        // check if the diffirence is in the tolerance else change horizontal angle
     {
     if (avl > avr)
     {
     servoh = --servoh;
     if (servoh < servohLimitLow)
     {
     servoh = servohLimitLow;
    }
   }
     else if (avl < avr)
     {
     servoh = ++servoh;
     if (servoh > servohLimitHigh)
     {
     servoh = servohLimitHigh;
     }
   }
     else if (avl = avr)
     {
     delay(5000);
    }
     horizontal.write(servoh);
  }
     
     delay(dtime);
     liquid_crystal();
}

void liquid_crystal(){
  
    analog_value = analogRead(A4);                          // Reading the analog value
    /*temp*/ voltage = 2*(analog_value * 5.0) / 1024.0;                // Converting the analog value in digital
   /* voltage = temp; */
    current = voltage/(r1+r2);
    //power = current*temp;
    power = sq(voltage)/current;
  
    if (voltage < 0.1) {
    voltage=0.0;                                          //Prints 0 value
 }  else{
    unsigned long currentTime = millis();
    if(currentTime - previousTime >= eventInterval){
                   
    previousTime = currentTime;
    lcd.clear();                                     // Clear any text on the LCD
    lcd.setCursor(0, 0);                            // MOve the cursor to the initial position
    lcd.print("V: ");                              // Print Voltgae
    lcd.print(voltage);                           // Print the final digital value of voltage
    lcd.print("v");                              // print the unit of voltage
    lcd.print(" P:");
    lcd.print(power);
    lcd.print("mW");
    lcd.setCursor(0,1);                      // move the cursor
    lcd.print("I: ");                       // Print the final digital value of voltage
    lcd.print(current);
    lcd.print("A");

   //SERIAL PRINT//

    analog_value = analogRead(A4);                          // Reading the analog value
    temp = 2*(analog_value * 5.0) / 1024.0;                // Converting the analog value in digital
     current = voltage/(r1+r2);
    //power = current*temp;
    power = sq(voltage)/current;
    
    Serial.print("DATA,TIME,");
    Serial.print(voltage);
    Serial.print(",");
    Serial.print(current);
    Serial.print(",");
    Serial.println(power);
//    delay(1000);
    }
 }
}
