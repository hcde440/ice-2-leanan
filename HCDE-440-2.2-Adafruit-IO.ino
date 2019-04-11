// Adafruit IO Digital Input Example
// Tutorial Link: https://learn.adafruit.com/adafruit-io-basics-digital-input
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/**************************** Dashboard  ***********************************/

// LINK TO ADAFRUIT IO DASHBOARD:
//     https://io.adafruit.com/leana_n/dashboards/hcde440-ice2
//


/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"

/************************ Example Starts Here *******************************/

// digital pin 2
#define BUTTON_PIN 2
// Analog pin A0
#define SENSOR_PIN A0

// button state
bool current = false;
bool last = false;

// set up the 'digital' feed in Adafruit IO and reference it here with io.feed("[NAME OF FEED]");
AdafruitIO_Feed *digital = io.feed("button");

// photocell state
int current2 = 0;
int last2 = -1;

// set up the 'analog' feed in Adafruit IO and reference it here with io.feed("[NAME OF FEED]");
AdafruitIO_Feed *analog = io.feed("light sensor");


void setup() {

  // set button pin as an input
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  //Set LDR sensor as input
  pinMode(SENSOR_PIN, INPUT);

  // start the serial connection
  Serial.begin(115200);
  Serial.print("This board is running: ");
  Serial.println(F(__FILE__));
  Serial.print("Compiled: ");
  Serial.println(F(__DATE__ " " __TIME__));
   
  // wait for serial monitor to open
  while(! Serial);

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.println(io.statusText());
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

/*   Below is the code for sending the button feedback to the
 *   Adafruit IO dashboard. Once the button is pressed, the dashboard Gauge 
 *   lights up blue and displays "1". 
 *   When unpressed/release, it turns white and displays "0"
 *   
 */

  // grab the current state of the button.
  // we have to flip the logic because we are
  // using a pullup resistor.
  
  if(digitalRead(BUTTON_PIN) == LOW) {
    current = true;
  } else {
    current = false;
  }
  
  // return if the value hasn't changed
  if(current == last){
    return;
  }
  
  // save the current state to the 'digital' feed on adafruit io
  Serial.print("sending button -> ");
  Serial.println(current);
  digital->save(current);

  // store last button state
  last = current;

//-------------------------------------------------------//

/*   Below is the code for sending the Light Sensor's feedback to the
 *   Adafruit IO dashboard. Once light is registered, the dashboard Line Graph  
 *   drops to lower than 700. When it is dark or less light is registered,
 *   the numbers climb to 700 and higher.
 *   
 */

   // grab the current state of the photocell
  current2 = analogRead(SENSOR_PIN);
 
  // return if the value hasn't changed
  if(current2 == last2){
    return;
  }
  
  // save the current state to the analog feed
  Serial.print("sending -> ");
  Serial.println(current2);
  analog->save(current2);
 
  // store last photocell state
  last2 = current2;
 
  // wait one second (1000 milliseconds == 1 second)
  delay(1000);
 
}
