
#define p_echo 2
#define p_trig 3
#define s_echo 4
#define s_trig 5
#define pthresh 120
#define sthresh 150
#define base_idle_time 120000

// relay pin trigger
#define rp 6

// defines variables
long duration;  // variable for the duration of sound wave travel
int distance;  // variable for the distance measurement
long idle_time = 0;

void setup() {
  pinMode(p_trig, OUTPUT);
  pinMode(p_echo, INPUT);
  pinMode(s_trig, OUTPUT);
  pinMode(s_echo, INPUT);
  pinMode(rp, OUTPUT);
  Serial.begin(9600);
  Serial.println("Ultrasonic Sensor HC-SR04 Test");
  Serial.println("with Arduino UNO R3");
  digitalWrite(rp, HIGH);
}

int poll_distance(int echo, int trig) {
  // Clears the trigPin condition
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echo, HIGH);
  // Calculating the distance
  return duration * 0.034 / 2;  // Speed of sound wave divided by 2
}

void activate(bool state) {
  if (state) {
    // set/reset idle time
    idle_time = base_idle_time;
    digitalWrite(rp, LOW);
  } else {
    // do not deactivate
    if (idle_time <= 0) {
      digitalWrite(rp, HIGH);
    }
  }
  delay(1000);
  if (idle_time > 0) {
    idle_time -= 1000;
  }
  Serial.print("Idle Time: ");
  Serial.println(idle_time);
}

void loop() {
  int pd, sd;
  pd = poll_distance(p_echo, p_trig);
  // Displays the distance on the Serial Monitor
  Serial.print("Primary distance: ");
  Serial.print(pd);
  Serial.println(" cm");
  if (pd < pthresh) {
    activate(true);
  } else {
    sd = poll_distance(s_echo, s_trig);
    // Displays the distance on the Serial Monitor
    Serial.print("Secondary distance: ");
    Serial.print(sd);
    Serial.println(" cm");
    if (sd < sthresh) {
      activate(true);
    } else {
      activate(false);
    }
  }
}