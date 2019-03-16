// RECIEVER code

#include <SoftwareSerial.h>
SoftwareSerial _softSerial(2, 3);

// Arduino digital pins
#define LED_PIN 13

#define SND_SPEED 343
#define NETWORK_SIG_SIZE 3
#define VAL_SIZE         2
#define CHECKSUM_SIZE    1
#define PACKET_SIZE      (NETWORK_SIG_SIZE + VAL_SIZE + CHECKSUM_SIZE)

// The network address byte and can be change if you want to run different devices in proximity to each other without interfearance
#define NET_ADDR 5
const byte g_network_sig[NETWORK_SIG_SIZE] = {0x8F, 0xAA, NET_ADDR};  // Few bytes used to initiate a transfer3
long duration;
int distance;
int r1=0;
int r2=0;
int r3=0;
const int trg = 5;
const int ech = 4;

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  pinMode(trg, OUTPUT); // Sets the trigPin as an Output
  pinMode(ech, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600);
  _softSerial.begin(1200);  // Hardware supports up to 2400, but 1200 gives longer range
}


void loop()
{
  unsigned int _id = readUInt(true);
  genPulse();
  duration = pulseIn(ech, HIGH);
  // Calculating the distance
  distance= duration*0.034/2;
  // Prints the distance on the Serial Monitor
  if(_id==1)
  r1=distance;
  else if (_id==2)
  r2=distance;
  else if (_id==3)
  r3=distance;
  Serial.print(r1);
  Serial.print(", ");
  Serial.print(r2);
  Serial.print(", ");
  Serial.println(r3);
  
}
unsigned int readUInt(bool wait)
{
  int pos = 0;          // Position in the network signature
  unsigned int val;     // Value of the unsigned int
  byte c = 0;           // Current byte  

  if((_softSerial.available() < PACKET_SIZE) && (wait == false))
  {
    return 0xFFFF;
  }
  

  while(pos < NETWORK_SIG_SIZE)

  { 
    while(_softSerial.available() == 0); // Wait until something is avalible
    c = _softSerial.read();

    if (c == g_network_sig[pos])
   {
     if (pos == NETWORK_SIG_SIZE-1)
      {
        byte checksum;

        while(_softSerial.available() < VAL_SIZE + CHECKSUM_SIZE); // Wait until something is avalible
        val      =  _softSerial.read();
        val      += ((unsigned int)_softSerial.read())*256;
        checksum =  _softSerial.read();

        if (checksum != ((val/256) ^ (val&0xFF)))
        {
          // Checksum failed
          pos = -1;
        }
      }
      ++pos;
    }
    else if (c == g_network_sig[0])
    {
      pos = 1;
    }
    else
    {
      pos = 0;
      
      if (!wait)
      {
        return 0xFFFF;
      }
    }
  }
  return val;
}



void genPulse()
{
  digitalWrite(trg, LOW);
  delayMicroseconds(2);
  digitalWrite(trg, HIGH);
  delayMicroseconds(10);
  digitalWrite(trg, LOW);
}
