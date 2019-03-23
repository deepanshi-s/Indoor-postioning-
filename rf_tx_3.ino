#include <VirtualWire.h>
#include <string.h>
const int led_pin = 11;
const int transmit_pin = 12;
const int receive_pin = 2;
const int transmit_en_pin = 3;
const int trigger1=5;
const int trigger2=6;
const int trigger3=7;
void setup()
{
  pinMode(led_pin, OUTPUT);
  pinMode(trigger1, OUTPUT); 
  pinMode(trigger2, OUTPUT);
  pinMode(trigger3, OUTPUT);// Sets the trigPin as an Output
  // Initialise the IO and ISR
  vw_set_tx_pin(transmit_pin);
  vw_set_rx_pin(receive_pin);
  vw_set_ptt_pin(transmit_en_pin);
//  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(2000);   // Bits per sec
  
}
char msg[4];
int i=0;
void loop()
{ i=i%3;
  itoa(i,msg,10);
  digitalWrite(led_pin, HIGH); // Flash a light to show transmitting
  vw_send((uint8_t *)msg, strlen(msg));
  

  vw_wait_tx(); // Wait until the whole message is gone
  if(i==0)
  genPulse1();
  else if(i==1)
  genPulse2();
  else if(i==2)
  genPulse3();
  digitalWrite(led_pin, LOW);
  delay(3000);
  i++;
}
void genPulse1()
{
  digitalWrite(trigger1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger1, LOW);
}

void genPulse2()
{
  digitalWrite(trigger2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger2, LOW);
}


void genPulse3()
{
  digitalWrite(trigger3, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger3, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger3, LOW);
}
