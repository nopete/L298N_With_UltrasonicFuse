const unsigned long MAX_DISTANCE = 15;
const unsigned long MAX_DURATION = MAX_DISTANCE*1000*1000*2/100/340;

//Pins 5 and 6: controlled by timer0 (8 bits)
//Pins 9 and 10: controlled by timer1 (16 bits)
//Pins 11 and 3: controlled by timer2 (8 bits)

//Define Pins

//Motor A
int enableA = 5;  // White
int pinA1 = 4;    //Yellow
int pinA2 = 2;    // Green

//Motor B
int enableB = 6;  // Gray
int pinB1 = 7;    // Purple
int pinB2 = 8;    // Blue

//Ultrasonic sensor
int pinEcho = 11; // Orange
int pinTrig = 12; // Brown

int pinLed=13;

int HIGH_LIMIT=255;
int LOW_LIMIT=130;
int STEP=15;


void pwm_go(int val, bool A, bool B)
{
  /*
  if (A)
  {
    if (B)
      Serial.print("BOTH ");
     else
      Serial.print("A ");
  }
  else
  {
    if (B)
      Serial.print("B ");
     else
     {
      Serial.println("NONE");
      return;
     }
  }
  Serial.println(val);
  */
  if (val > 0)
  {
    if (A)
    {
      analogWrite(enableA, val);
      digitalWrite(pinA1, HIGH);  
      digitalWrite(pinA2, LOW);  
    }
    if (B)
    {
      analogWrite(enableB, val);
      digitalWrite(pinB1, HIGH);  
      digitalWrite(pinB2, LOW);      
    }
  }
  else if (val < 0)
  {
    if (A)
    {
      analogWrite(enableA, -val);
      digitalWrite(pinA1, LOW);  
      digitalWrite(pinA2, HIGH);  
    }
    if (B)
    {
      analogWrite(enableB, -val);      
      digitalWrite(pinB1, LOW);  
      digitalWrite(pinB2, HIGH);      
    }
  }
  else
  {
    if (A)
    {
      digitalWrite(pinA2, HIGH);  
      digitalWrite(pinA1, HIGH);  
    }
    if (B)
    {
      digitalWrite(pinB2, HIGH);  
      digitalWrite(pinB1, HIGH);
    }
  }

}

int rpm = 0;
int last_inc = 0;

void Test(bool A, bool B)
{
  for (int i=LOW_LIMIT; i <= HIGH_LIMIT; i += STEP)
  {
    pwm_go(i, A, B);
    delay(1000);
  }
  
  pwm_go(0, A, B);
  delay(150);

  for (int i=LOW_LIMIT; i <= HIGH_LIMIT; i += STEP)
  {
    pwm_go(-i, A, B);
    delay(1000);
  }
  
  pwm_go(0, A, B);
  delay(150);
  
  for (int i=0; i < 2; ++i)
  {
    digitalWrite(pinLed,HIGH);
    delay(500);
    digitalWrite(pinLed,LOW);
    delay(500);
  }
}

void setup()
{
  Serial.begin (9600);

  //configure pin modes
  pinMode(pinLed, OUTPUT);
  digitalWrite(pinLed, LOW);
  
  pinMode (pinA1, OUTPUT);
  pinMode (pinA2, OUTPUT);
  pinMode (pinB1, OUTPUT);
  pinMode (pinB2, OUTPUT);

  pinMode(pinTrig, OUTPUT);
  pinMode(pinEcho, INPUT);

//  Test(true, false);
//  Test(false, true);

  Test(true, true);
  
  rpm = 0;
  last_inc = 0;

  Serial.println("Ready");

}



bool isTooClose()
{
  long duration;

  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(pinTrig, LOW);
  delayMicroseconds(5);
  digitalWrite(pinTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinTrig, LOW);
 
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  duration = pulseIn(pinEcho, HIGH, MAX_DURATION*10);

  Serial.println(duration);

  if (duration == 0)
    return false;
  if (duration > MAX_DURATION )
    return false;
  return true;
}

int speed = 0;

void loop()
{
  if (isTooClose())
  {
    digitalWrite(pinLed, HIGH);
    if (speed > 0)
    {
      pwm_go(0, true, true);
      speed = 0;
    }
  }
  else
  {
    digitalWrite(pinLed, LOW);
    if (speed == 0)
    {
      pwm_go(255, false, true);
      speed = 255;
    }
  }
}
