#define NUM_MOTORS 4 // Number of motors
#define SONG_LEN 58 // number of periods

int i; // Counter variable, reused a couple of times

// hold motor pin numbers
int motorPins[NUM_MOTORS] = {2, 3, 4, 5};
int selectPin = 7;
byte (*song)[NUM_MOTORS][SONG_LEN] = NULL;

// Time:
// Stores time in milliseconds - ~60 days for this variable to overflow
unsigned long tPrevious;
// A note will play every interval millisecondsj
const long interval = 125;
// The cursor indicates which part of the song we will play
unsigned int cursor = 0;
// patterns - a list of lists of on-off pattersn - 1 for each motor
// Plays the percussion version of "hot cross buns"
byte hot_cross_buns[NUM_MOTORS][SONG_LEN] = {
  {1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0},
  {0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0},
  {0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,1,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

//imperial march
byte imp_march[NUM_MOTORS][SONG_LEN] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0}, 
  {1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0}, 
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

int m = 0;

/**
 * Setup code for Arduino, runs once at start. It is very important to set
 * the pins to OUTPUT, otherwise they will be very weak due to pullups being
 * enabled by default.
 */
void setup() {
  Serial.begin(9600);
  for (i=0; i<NUM_MOTORS; i++) {
     pinMode(motorPins[i], OUTPUT); 
  }
  pinMode(selectPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT); 
  // The select pin should be jumper'd high or low to select a song
  if (digitalRead(selectPin) == HIGH) {
    song = &imp_march;
  } else {
    song = &hot_cross_buns;
  }
}

/**
 * Enters an infinite loop when the song is over
 */
void stop() {
  Serial.print("Song Complete");
  while(1);
}

/**
 * This code will run until the song is complete
 */
void loop() {
  Serial.print("Iteration");
  // Get the current time, then compare with the last recorded time
  unsigned long tCurrent = millis();
  // If it has been more than an interval, advance to the next "note"
  if (tCurrent - tPrevious >= interval) {
    tPrevious = tCurrent;
    cursor++;
    // When the song is over, go to the stop() function
    if (cursor >= SONG_LEN) { stop(); }
    // For every motor, we will play out the current note
    for (i=0; i<NUM_MOTORS; i++) {
      Serial.print("Iteration");
      digitalWrite(motorPins[i], (*song)[i][cursor]);
      
    }
    m = !m;
      digitalWrite(LED_BUILTIN, m);

  }
}
