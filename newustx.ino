
//transmitter code

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

const int trg[3] = {4,5,6};
//const int ech[3] = {7,8,9};
int b;
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
  int i=0;
  while(i<3)
{int a[3]={1,2,3};
  writeUInt(a[i]);
  genPulse(i);
  Serial.println(a[i]);
  i++;
  delay(3000);
} 
}

void writeUInt(unsigned int val)
{
  byte checksum = (val/256) ^ (val&0xFF);
  
  _softSerial.write(0xF0);  // This gets reciever in sync with transmitter
  _softSerial.write(g_network_sig, NETWORK_SIG_SIZE);
  _softSerial.write((byte*)&val, VAL_SIZE);
  _softSerial.write(checksum); //CHECKSUM_SIZE
}

void genPulse(int b)
{
  digitalWrite(trg[b], LOW);
  delayMicroseconds(2);
  digitalWrite(trg[b], HIGH);
  delayMicroseconds(10);
  digitalWrite(trg[b], LOW);
}
