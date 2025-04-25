
#include <SimpleRotary.h>
#include <neotimer.h>
#include <PinButton.h>
const int Light = 9;
const int YellowLight = 10;
// Pin A, Pin B, Button Pin
SimpleRotary rotary(6, 7, 2);  //6+7 on CLK and DT of Rotary Encoder  the last number is for the button pin, but not used, could be any free GPIO
//button is connected to pin 5 and used by the PinButton library
PinButton myButton(5);
int k = 0;
int memo = 0;
int sensorValue = 0;
boolean countup = true;
boolean countdown = false;
boolean fadeOut = false;
boolean fadeIn = false;
boolean fastOn = false;
boolean startTimer = false;
boolean stopTimer = false;

Neotimer FadeIn1 = Neotimer(10);       //timer fade in time steps
Neotimer FadeOut1 = Neotimer(10);      //timer fade out time steps
Neotimer FastOn1 = Neotimer(5);        //timer fade in time steps
Neotimer ReadSensor = Neotimer(100);   //Read LDR Sensor
Neotimer timerDelay = Neotimer(6000);  // the timer delay
Neotimer timerOn = Neotimer(10);       // the timer delay
Neotimer timerOff = Neotimer(10);      // the timer delay

void setup() {
  Serial.begin(74880);
  while (!Serial) { ; }
  Serial.println("INFO: ");
  Serial.println(F(__FILE__ " " __DATE__ " " __TIME__));
  Serial.println("Nano old bootloader");
  Serial.println("Made for Rotary Encoder Board");
  Serial.println("LDR on GND -- LDR on A1 -- Resistor 470R on a! and 3V3");
  // Set the trigger to be either a HIGH or LOW pin (Default: HIGH)
  // Note this sets all three pins to use the same state.
  rotary.setTrigger(HIGH);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(Light, OUTPUT);        //Weiss
  pinMode(YellowLight, OUTPUT);  //Yellow
  ReadSensor.reset();
  ReadSensor.start();
}
void loop() {
  myButton.update();
  byte i;
  byte j;

  i = rotary.rotate();


  if (ReadSensor.done()) {  //Importand delay, without too much stress and breakdown
    sensorValue = analogRead(A1);
    //Serial.print(sensorValue);
    //sensorValue = map(sensorValue, 480, 1023, 255, 0);
    //sensorValue = map(sensorValue, 1, 1023, 255, 0);
    sensorValue = map(sensorValue, 500, 1023, 255, 0);
    //Serial.print(" >> ");
    //Serial.println(sensorValue);
    ReadSensor.reset();
    ReadSensor.start();
  }
  if (k == 0 && memo >= 1) {  //Yellow only on when Light is off
    analogWrite(YellowLight, sensorValue);
  } else {
    analogWrite(YellowLight, 0);
  }
  if (countdown) {
    if (i == 1) {
      Serial.println("");
      Serial.print("subtrahiere i = ");
      Serial.print(i);
      sensorValue = analogRead(A1);
      Serial.print(" sensorValue not mapped = ");
      Serial.print(sensorValue);
      i = 0;
      if (k <= 9) {
        k = k - 1;
      } else {
        k = k - 5;
      }
      Serial.print(" memo = ");
      Serial.print(memo);
      Serial.print(" <<");
      if (k <= 1) {
        k = 0;
        Serial.print(" <= 1  k is = ");
        countdown = false;
      }
      //Serial.print(k);
    }
  }
  if (countup) {
    if (i == 2) {
      Serial.println("");
      Serial.print("addiere i = ");
      Serial.print(i);
      Serial.print(" sensorValue = ");
      Serial.print(sensorValue);
      i = 0;
      if (k <= 9) {
        k = k + 1;
      } else {
        k = k + 5;
      }
      memo = k;
      Serial.print(" memo = ");
      Serial.print(memo);
      Serial.print(" <<");
      if (k >= 254) {
        k = 255;
        Serial.print(" k is >= 254 == ");
        countup = false;
      }
      //Serial.print(k);
    }
  }

  if (k >= 1) {
    countdown = true;
    if (k <= 0) {
      countdown = false;
    }
  }
  if (k <= 250) {
    countup = true;
  }

  // 0 = not pushed, 1 = pushed
  j = rotary.push();

  //if (j == 1) {
  if (myButton.isSingleClick()) {
    Serial.println("");
    Serial.print(">> single");
    i = 0;
    Serial.print(">> memo = ");
    Serial.print(memo);
    Serial.print(" <<");
    countdown = false;
    //Serial.println(k);
    if (k <= 0) {
      fadeIn = true;
      Serial.print(" >> fadeIn now >>");
      FadeIn1.reset();
      FadeIn1.start();
    }
    if (k >= 1) {
      fadeOut = true;
      FadeOut1 = Neotimer(10);
      Serial.print(" >> fadeOut now >>");
      FadeOut1.reset();
      FadeOut1.start();
    }
  }  //end of single click
  if (myButton.isDoubleClick()) {
    Serial.println("");
    Serial.print(">> double >> ");
    if (k <= 255) {
      fastOn = true;
      FastOn1.reset();
      FastOn1.start();
    }
  }
  //}  //end of delay
  if (fadeIn && memo >= 1) {
    //Serial.println("fadeIn");
    //Serial.print("k =========           ========= ");
    //Serial.println(k);
    if (FadeIn1.done()) {
      k = k + 1;
      //Serial.println(k);
      if (k >= memo) {
        //Serial.print("k = ");
        //Serial.println(k);
        //Serial.print("memo = ");
        //Serial.print(memo);
        fadeIn = false;
        FadeIn1.reset();
        Serial.print(">> fadeIn done <<");
      }
      FadeIn1.reset();
      FadeIn1.start();
    }
  }
  if (fadeOut) {
    if (FadeOut1.done()) {
      if (k <= 15) {
        FadeOut1 = Neotimer(200);  //slow down end of FadeOut
      }
      k = k - 1;
      if (k <= 0) {
        fadeOut = false;
        FadeOut1.reset();
        Serial.print(">> fadeOut done <<");
      }
      FadeOut1.reset();
      FadeOut1.start();
    }
  }
  if (fastOn) {
    if (FastOn1.done()) {
      k = k + 1;
      if (k >= 255) {
        Serial.print("full brightness = ");
        Serial.print(k);
        Serial.print(" <<");
        FastOn1.reset();
        fastOn = false;
        //timerDelay.reset();
      }
      FastOn1.reset();
      FastOn1.start();
    }
  }  //end of fastOn

  if (myButton.isLongClick()) {
    Serial.println("");
    Serial.print(">> long = ");
    Serial.print(k);
    startTimer = true;
    stopTimer = false;  ///////////////////////////
    timerOn.reset();
    timerOn.start();
    timerOff.reset();
  }
  if (startTimer) {
    if (timerOn.done()) {
      if (k <= 254) {
        k = k + 1;
        if (k == 255) {
          startTimer = false;
          stopTimer = true;
          Serial.print(" >> long click full on start timerDelay >>");
          timerDelay.reset();
          timerDelay.start();
          timerOff.reset();  ///////////////////////////////
        }
      }

      if (k >= 255) {
        //k = 255;
        startTimer = false;
        stopTimer = true;
        Serial.print(" long click full on start timerDelay >>");
        timerDelay.reset();
        timerDelay.start();
        timerOff.reset();  ///////////////////////////////
      }
      timerOn.reset();
      timerOn.start();

    }  //end of startTimer
  }    //end of startTimer

  if (timerDelay.done()) {
    timerOff.reset();
    timerOff.start();
    timerDelay.reset();
    Serial.print(" timer fade out >>");
    stopTimer = true;
  }
  if (timerOff.done()) {
    if (stopTimer) {
      k = k - 1;
      //delay(10);
      if (k <= 0) {
        stopTimer = false;
        Serial.print(" timer led off <<");
        Serial.print("");
        timerOff.reset();
        fadeOut = false;
      }
      timerOff.reset();
      timerOff.start();
    }  //end of stopTimer
  }

  if (myButton.isClick()) {
    //Serial.println("click 257");
  }
  if (myButton.isReleased()) {
    //Serial.println("up 263");
  }
  analogWrite(Light, k);  //write k value to LED Light
}  //end of loop
