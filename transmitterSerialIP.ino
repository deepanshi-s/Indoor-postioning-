#include <SoftwareSerial.h>
SoftwareSerial _softSerial(2, 3);

void setup()
{
  Serial.begin(9600);
  _softSerial.begin(300);  // Hardware supports up to 2400, but 1200 gives longer range
            //Mentioned in recv code, 300 is better for us
}

void loop()
{
  static unsigned int i = 0;
  Serial.println(writeUInt(i));

  i++;
  if (i > 2)i = 0;
  delay(500);       //This value is quite important
                    //the packet loss depends on this.   500 seemed best to me
}

#define NETWORK_SIG_SIZE 3
#define VAL_SIZE         2
#define CHECKSUM_SIZE    1
#define PACKET_SIZE      (NETWORK_SIG_SIZE + VAL_SIZE + CHECKSUM_SIZE)

// The network address byte and can be change if you want to run different devices in proximity to each other without interfearance
#define NET_ADDR 5
const byte g_network_sig[NETWORK_SIG_SIZE] = {0x8F, 0xAA, NET_ADDR};  // Few bytes used to initiate a transfer3

// Sends an unsigned int over the RF network
byte writeUInt(unsigned int val)
{
  byte checksum = (val/256) ^ (val&0xFF);

  _softSerial.write(0xF0);  // This gets reciever in sync with transmitter
  _softSerial.write(g_network_sig, NETWORK_SIG_SIZE);
  _softSerial.write((byte*)&val, VAL_SIZE);
  _softSerial.write(checksum); //CHECKSUM_SIZE
  return checksum;
}
