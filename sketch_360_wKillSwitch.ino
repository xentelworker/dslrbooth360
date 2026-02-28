// State machine variables
bool boothRunning = false;
unsigned long stepStart = 0;
int step = 0;

// Hardware kill switch pin
const int killPin = 3;

void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);

  pinMode(killPin, INPUT_PULLUP);  // Button wired to GND
}

void loop() {

  // --- SERIAL COMMAND HANDLING ---
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');

    if (cmd == "BOOTH_ON" && !boothRunning) {
      boothRunning = true;
      step = 1;
      stepStart = millis();
      Serial.println("BOOTH sequence started");
    }
  }

  // --- HARDWARE KILL SWITCH ---
  if (digitalRead(killPin) == LOW) {  // Button pressed
    abortSequence();
  }

  // --- RUN SEQUENCE ---
  if (boothRunning) {
    unsigned long now = millis();

    switch (step) {

      case 1: // First button press ON
        digitalWrite(2, HIGH);
        if (now - stepStart >= 200) {
          digitalWrite(2, LOW);
          step = 2;
          stepStart = now;
        }
        break;

      case 2: // Wait 10 seconds
        if (now - stepStart >= 10000) {
          step = 3;
          stepStart = now;
        }
        break;

      case 3: // Second button press ON
        digitalWrite(2, HIGH);
        if (now - stepStart >= 200) {
          digitalWrite(2, LOW);
          boothRunning = false;
          step = 0;
          Serial.println("BOOTH sequence completed");
        }
        break;
    }
  }
}

// --- ABORT FUNCTION WITH BUTTON PRESS ---
void abortSequence() {

  // Send a relay pulse BEFORE aborting
  digitalWrite(2, HIGH);
  delay(200);          // momentary press
  digitalWrite(2, LOW);

  // Now abort the sequence
  boothRunning = false;
  step = 0;
  Serial.println("BOOTH sequence aborted (hardware button)");
}