//#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define soilWet 500   // Define max value we consider soil 'wet'
#define soilDry 750 
#define sensorPower 7
#define sensorPin A0
int led = 13; 
int led_soilperfect = 12;
int led_soildry = 10;
int led_soilwet = 11;               // the pin that the LED is atteched to
int sensor = 2;              // the pin that the sensor is atteched to
int state = LOW;             // by default, no motion detected
int val = 0;   

// Set the LCD address to 0x27 for a 16 chars and 2 line display

LiquidCrystal_I2C lcd(0x27, 20, 4);

int i = 0;


void setup() {

  // initialize the LCD

  lcd.begin();

  // Turn on the blacklight and clear the display.

  lcd.backlight();

  lcd.clear();
  pinMode(sensorPower, OUTPUT);
	
	// Initially keep the sensor OFF
	digitalWrite(sensorPower, LOW);
  pinMode(led, OUTPUT);      // initalize LED as an output
  pinMode(sensor, INPUT);    // initialize sensor as an input
  pinMode(led_soilperfect, OUTPUT);
  pinMode(led_soildry, OUTPUT);
  pinMode(led_soilwet, OUTPUT);

  digitalWrite(led_soilperfect, LOW);
  digitalWrite(led_soildry, LOW);
  digitalWrite(led_soilwet, LOW);
	
	Serial.begin(9600);

}



void loop() {

  // put your main code here, to run repeatedly:

  lcd.setCursor(0,0);
//get the reading from the function below and print it
	int moisture = readSensor();
	Serial.print("Analog Output: ");
	Serial.println(moisture);

	// Determine status of our soil
	if (moisture < soilWet) {
		Serial.println("Status: Soil is too wet");
    lcd.println("Moisture Content " + String(moisture));
    lcd.println("Soil Type: Wet");
    digitalWrite(led_soilwet,HIGH);
    digitalWrite(led_soildry,LOW);
    digitalWrite(led_soilperfect,LOW);

	} else if (moisture >= soilWet && moisture < soilDry) {
		Serial.println("Status: Soil moisture is perfect");
    lcd.println("Moisture Content " + String(moisture));
    lcd.println("Soil Type: Perfect");
    digitalWrite(led_soilwet,LOW);
    digitalWrite(led_soildry,LOW);
    digitalWrite(led_soilperfect,HIGH);
	} else {
		Serial.println("Status: Soil is too dry - time to water!");
    lcd.println("Moisture Content "+String(moisture));
    lcd.println("Soil Type: Dry");
    digitalWrite(led_soilwet,LOW);
    digitalWrite(led_soildry,HIGH);
    digitalWrite(led_soilperfect,LOW);
	}
	
	delay(1000);	// Take a reading every second for testing
					// Normally you should take reading perhaps once or twice a day
	Serial.println();
  val = digitalRead(sensor);
  Serial.println(val);   // read sensor value
  if (val == HIGH) {           // check if the sensor is HIGH
    digitalWrite(led, HIGH);   // turn LED ON
    delay(500);                // delay 100 milliseconds 
    
    if (state == LOW) {
      Serial.println("Motion detected!"); 
      state = HIGH;       // update variable state to HIGH
    }
  } 
  else {
      digitalWrite(led, LOW); // turn LED OFF
      delay(500);             // delay 200 milliseconds 
      
      if (state == HIGH){
        Serial.println("Motion stopped!");
        state = LOW;       // update variable state to LOW
    }
  }
}
int readSensor() {
	digitalWrite(sensorPower, HIGH);	// Turn the sensor ON
	delay(10);							// Allow power to settle
	int val = analogRead(sensorPin);	// Read the analog value form sensor
	digitalWrite(sensorPower, LOW);		// Turn the sensor OFF
	return val;							// Return analog moisture value
}

