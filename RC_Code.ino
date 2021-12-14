#include <Stepper.h>
 

int strobePin  = 7;    // Strobe Pin on the MSGEQ7
int resetPin   = 8;    // Reset Pin on the MSGEQ7
int outPin     = A5;   // Output Pin on the MSGEQ7
int level[7];          // An array to hold the values from the 7 frequency bands
 

int largestLevel = 0;  // Largest measurement recieved from MSGEQ7
int largestLevelIndex = 0; // index of largest level array
int range      = 0;    // Range variable to determine between 3 bands
 

// Stepper motor initializations
Stepper stepper1(512, 22, 23, 24, 25); // Left stepper
Stepper stepper2(512, 26, 27, 28, 29); // right stepper
 

void setup() {
 

  Serial.begin (9600);
 

  // Define our pin modes
  pinMode      (strobePin, OUTPUT);
  pinMode      (resetPin,  OUTPUT);
  pinMode      (outPin,    INPUT);
 

  // Create an initial state for our pins
  digitalWrite (resetPin,  LOW);
  digitalWrite (strobePin, LOW);
  delay        (1);
 

  // Reset the MSGEQ7 as per the datasheet timing diagram
  digitalWrite (resetPin,  HIGH);
  delay        (1);
  digitalWrite (resetPin,  LOW);
  digitalWrite (strobePin, HIGH);
  delay        (1);
 

  // Define stepper RPM
  stepper1.setSpeed(40);
  stepper2.setSpeed(40);
}
 

void loop() {
  // Cycle through each frequency band by pulsing the strobe.
  for (int i = 0; i < 7; i++) {
    digitalWrite       (strobePin, LOW);
    delayMicroseconds  (100);                  // Delay necessary due to timing diagram
    level[i] =         analogRead (outPin);
    digitalWrite       (strobePin, HIGH);
    delayMicroseconds  (1);                    // Delay necessary due to timing diagram 
  }
 

  largestLevel = level[0];  // initialize largestLevel as first level
  for (int i = 1; i < 7; i++) {
    // Updates if new largestLevel is found
    if (level[i] > largestLevel)
    {
      largestLevel = level[i];
      largestLevelIndex = i;
    }
   }
 

  // Output largest level index to serial monitor
  Serial.print("Largest Level Index: ");
  Serial.print(largestLevelIndex);
  Serial.println ();  
 

  // Direction Setting
  if (largestLevelIndex < 2) // if the largest index is below band 3
  {
    // Turn left
    stepper1.step(50);
    stepper2.step(50);
    range = 1;
  }
  else if (largestLevelIndex > 2) // if the largest index is above band 3
  {
    // Turn right
    stepper1.step(-50);
    stepper2.step(-50);
    range = 3;
  }
  else // if the largest index is the third index
  {
    // Stay still
    range = 2;
  }
 

  // Print direction integer: 1 (left) 2 (nothing) 3 (right)
  Serial.print ("Range: ");
  Serial.print (range);
 

  Serial.println ();  
 

}
