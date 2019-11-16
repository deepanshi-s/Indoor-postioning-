#include <SoftwareSerial.h>
SoftwareSerial _softSerial(2, 3);
SoftwareSerial bluetooth(6, 7);

#define TRGG 4
#define ECHO 5

void setup()
{
  pinMode(TRGG, OUTPUT);
  pinMode(ECHO, INPUT);
  digitalWrite(TRGG, LOW);
  
  Serial.begin(115200);
  bluetooth.begin(9600);
  _softSerial.begin(300);  // Hardware supports up to 2400
              //I found that lower baud rate improved accuracy,
              //so I got 300 giving best results
}

void loop()
{
  unsigned int _id = readUInt(true);
  long int _dist = getDist();

  // $ is used to identify packets by python
  // I put # bcoz I think the serial sends n-1 chars from a char array, so we need an extra
  String _data = "$" + String(_id) + "-" + String(_dist) + "#";
  char data[_data.length()];                //softwareserial can only take a char array so we need to convert
  _data.toCharArray(data, _data.length());
  
  bluetooth.write(data);
  Serial.println(data);
}

long int getDist()
{
  digitalWrite(TRGG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRGG, LOW);
  long duration = pulseIn(ECHO, HIGH);
  return duration*0.034/2;            //TODO: Some stuff is wrong with this calculation
}


//This is unchanged, we can keep the entire signature and checksum also
//there will be some packet loss, but here data accuracy is more important
#define NETWORK_SIG_SIZE 3
#define VAL_SIZE         2
#define CHECKSUM_SIZE    1
#define PACKET_SIZE      (NETWORK_SIG_SIZE + VAL_SIZE + CHECKSUM_SIZE)

// The network address byte and can be change if you want to run different devices in proximity to each other without interfearance
#define NET_ADDR 5
const byte g_network_sig[NETWORK_SIG_SIZE] = {0x8F, 0xAA, NET_ADDR};  // Few bytes used to initiate a transfer3

// Receives an unsigned int over the RF network
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
    while(_softSerial.available() == 0); // Wait until something is available
    c = _softSerial.read();

    if (c == g_network_sig[pos])
   {
     if (pos == NETWORK_SIG_SIZE-1)
      {
        byte checksum;

        while(_softSerial.available() < VAL_SIZE + CHECKSUM_SIZE); // Wait until something is available
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
