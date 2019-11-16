#include <VirtualWire.h>
int data;
char msgr[4];
const int led_pin = 6;
const int transmit_pin = 12;
const int receive_pin = 11;
const int transmit_en_pin = 3;
const int trigger = 5;
const int echo = 4;
long duration;
int distance;
void setup()
{
    
    Serial.begin(9600);  // Debugging only
    Serial.println("setup");
    pinMode(trigger,OUTPUT);
    pinMode(echo,INPUT);
    // Initialise the IO and ISR
    vw_set_tx_pin(transmit_pin);
    vw_set_rx_pin(receive_pin);
    vw_set_ptt_pin(transmit_en_pin);
    vw_set_ptt_inverted(true); // Required for DR3100
    vw_setup(2000);  // Bits per sec

    vw_rx_start();       // Start the receiver PLL running
    
}

void loop()
{
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;

    if (vw_get_message(buf, &buflen)) // Non-blocking
    {
      genPulse();
     
      
      duration = pulseIn(echo, HIGH);
      // Calculating the distance
      distance= (duration*0.034)+8;
      int i;

      digitalWrite(led_pin, HIGH); // Flash a light to show received good message
      // Message with a good checksum received, print it.
      for (i=0;i<buflen;i++)
      {
        msgr[i]=char(buf[i]);
      }
      msgr[buflen]='\0';
      data=atoi(msgr);
       
      //duration = pulseIn(echo, HIGH);
      // Calculating the distance
      //distance= duration*0.034;
      Serial.print("Got: ");
      Serial.println(data);
      Serial.print("distance:");
      Serial.println(distance);
      
      digitalWrite(led_pin, LOW);
      
    }
}
void genPulse()
{
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
}
