#include <Servo.h>

Servo virtual_finger;
#define virtual_finger_pin 9
#define VIRTUAL_FINGER_MIN 50
#define VIRTUAL_FINGER_MAX 180

#define actuating_finger A0
#define ACTUATING_FINGER_MIN 770
#define ACTUATING_FINGER_THRESHOLD 830
#define ACTUATING_FINGER_MAX 900

#define release_finger A1
#define RELEASE_FINGER_MIN 0
//#define RELEASE_FINGER_THRESHOLD 512
#define RELEASE_FINGER_THRESHOLD 900
#define RELEASE_FINGER_MAX 1023


#define pressure_finger A2
#define PRESSURE_FINGER_MIN 0
#define PRESSURE_FINGER_THREASHOLD 20
#define PRESSURE_FINGER_MAX 730

#define BIN1 5
#define BIN2 6

int pressure_finger_reading=0;
int virtual_finger_position_raw =0;
byte virtual_finger_position_mapped=0;

// red left; black right --> bottom cold
// orange left; brown right --> bottom cold
// red left; white right --> bottom cold

void setup() {
  // put your setup code here, to run once:
  virtual_finger.attach(virtual_finger_pin);
  virtual_finger.write(90);
  Serial.begin(9600);

  analogWrite(BIN1,0);
  analogWrite(BIN2,0);

  delay(3000);
}

void loop() {
//  analogReference(EXTERNAL);
  // put your main code here, to run repeatedly:

  pressure_finger_reading = analogRead(pressure_finger);
//  Serial.println(pressure_finger_reading);
  if(pressure_finger_reading > PRESSURE_FINGER_THREASHOLD) {
    Serial.println("Force sensitive resistor triggered...");
    // Actuate the TEC to harden finger
    int release_finger_position_raw = analogRead(release_finger);
    Serial.println(release_finger_position_raw);
    if(release_finger_position_raw > RELEASE_FINGER_THRESHOLD) {
      release_tec();
    } else if (release_finger_position_raw > RELEASE_FINGER_THRESHOLD && virtual_finger_position_raw > ACTUATING_FINGER_THRESHOLD) {
    } else {
      actuate_tec();
    }
    
  } else {
    neutralize_tec();
  }

  

  if(pressure_finger_reading > PRESSURE_FINGER_THREASHOLD)
    virtual_finger.write(VIRTUAL_FINGER_MAX);
  else {
    virtual_finger_position_raw = analogRead(actuating_finger);
    virtual_finger_position_mapped = map(virtual_finger_position_raw,ACTUATING_FINGER_MIN,ACTUATING_FINGER_MAX,VIRTUAL_FINGER_MIN,VIRTUAL_FINGER_MAX);
//  Serial.println(virtual_finger_position_raw);
    virtual_finger.write(virtual_finger_position_mapped);
//  Serial.println(virtual_finger_position_mapped);
  }
}

void release_tec() {
  Serial.println("\nReleasing TEC...");
  virtual_finger.write(VIRTUAL_FINGER_MIN);
  pressure_finger_reading=0;
}

void actuate_tec() {
  Serial.println("\nActuating TEC...");

  analogWrite(BIN1,0);
  analogWrite(BIN2,255); // cool down
  
}

void neutralize_tec() {
  Serial.println("Neutralizing TEC...");
  analogWrite(BIN1,175); // warm up
  analogWrite(BIN2,0);
}

