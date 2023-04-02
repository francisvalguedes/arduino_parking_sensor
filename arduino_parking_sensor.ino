/* Ultrasonic Car Parking Sensor
   * ------------
   * 
*/

#define START_MIN 600
#define START_MAX 2000

#define BIT_MIN 50
#define BIT_MAX 400
#define BIT_READ 192

//#define SENSOR_PIN 2 
#define SENSOR_PIN PA0

//#define OUTPUT_PIN 3
#define OUTPUT_PIN PA1
 
unsigned long time;

unsigned int pulseValue = 0;

unsigned int cont = 0;

unsigned int byte_rc = 0;

unsigned int sens_arr[] = {0, 0, 0, 0};
unsigned int cont_arr = 0;


volatile unsigned long pulseInTimeBegin = micros();
volatile unsigned long pulseInTimeEnd = micros();
volatile bool newPulseDurationAvailable = false;


void setup(void) {
  Serial.begin(115200);
  pinMode(SENSOR_PIN, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(OUTPUT_PIN, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(SENSOR_PIN),
                  sensor_interrupt,
                  CHANGE);
}
 
void loop(void) {
  // pulseValue = pulseIn(SENSOR_PIN, HIGH );
  // Serial.println(pulseValue); 
  // newPulseDurationAvailable = false;

  // message frequency ~ 38 ms, mensage length: read ~6ms, write ~6ms, free ~ 25ms
  if (newPulseDurationAvailable) {
    newPulseDurationAvailable = false;
    pulseValue = pulseInTimeEnd - pulseInTimeBegin;
    //Serial.println(pulseValue);  

    if (cont==0) {   // mensage 6ms
      if ((pulseValue<START_MIN)||(pulseValue>START_MAX)) cont=0;
      else cont+=1;
    }
    else if ((cont>0)&&(cont<17)){    
      if ((pulseValue<BIT_MIN)||(pulseValue>BIT_MAX)) cont=0;
      else {
        if (pulseValue>BIT_READ) {
          byte_rc |=  (1<<(16-cont));
        }
        cont+=1;    
      }
    } 
    else{      
      unsigned char sensor = (byte_rc & 255);
      unsigned char byte_control = ((byte_rc >> 8) & 255);
      unsigned char sens_num = (byte_control & 3);

      sens_arr[sens_num] = sensor;

      // Serial.print(millis() - time); 
      // time=millis();
      // Serial.print(",");

      Serial.print(sens_num);
      Serial.print(",");
      Serial.print(sensor);
      Serial.println("");      

      send_byte(byte_rc);
      byte_rc = 0;
      cont=0;
    }   
  }
  // if ( millis()>=(time+100)){
  //   Serial.print("time: ");
  //   Serial.println(time); 
  //   time=millis(); 
  // } 
}

void sensor_interrupt()
{
  unsigned long pulse_tmp = micros();
  if (digitalRead(SENSOR_PIN) == HIGH) {
    // start measuring
    pulseInTimeBegin = pulse_tmp;
  }
  else {
    // stop measuring
    pulseInTimeEnd = pulse_tmp;
    newPulseDurationAvailable = true;
  }
}

void send_byte(unsigned int byte_rc)
{
  bool bit_w;
  int on_time;
  int off_time;
  unsigned char sensor = (byte_rc & 255);
  if (sensor>150) sensor = 150;
  if (sensor<50) sensor = sensor +10;
  byte_rc = (byte_rc & 0xFF00);
  byte_rc = (byte_rc | sensor);
  digitalWrite(OUTPUT_PIN, HIGH);
  delayMicroseconds (900);
  digitalWrite(OUTPUT_PIN, LOW);
  delayMicroseconds (100);
  for (int i = 15; i >=0; i--) {
    bit_w = bitRead(byte_rc, i) ; // ((byte_rc >> i) & 255);
    if (bit_w) {
      on_time = 300;
      off_time = 100;
    }
    else {
      on_time = 84;
      off_time = 100+216;
    }  

    digitalWrite(OUTPUT_PIN, HIGH);
    delayMicroseconds(on_time);
    digitalWrite(OUTPUT_PIN, LOW);
    delayMicroseconds(off_time);
  }

  digitalWrite(OUTPUT_PIN, HIGH);
  delayMicroseconds(84);
  digitalWrite(OUTPUT_PIN, LOW);
}

// debug binario 
    // itoa(byte_rc,binary,2);
    // Serial.print(binary);