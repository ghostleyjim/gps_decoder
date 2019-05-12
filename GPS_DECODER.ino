#include <SoftwareSerial.h>
#include <RF24Network.h>
#include<RF24.h>
#include <SPI.h>

RF24 radio(3,4);

RF24Network network(radio);
const uint16_t this_node = 00;
const uint16_t other_node = 01;

const unsigned long interval = 2000; //ms  // How often to send 'hello world to the other unit

unsigned long last_sent;             // When did we last send?

SoftwareSerial NMEA(3,4);
char GPS;
char GPSbuffer[150];
byte index;
String GPGLL = "$GPGLL";
String GPGGA = "$GPGGA";
int Bittotal;
int Check;
int hour;
int minute;
int seconds;
float lat;

struct payload_t
{
int time_h;
int time_m;
int time_s;
};


void setup() {
  // put your setup code here, to run once:
NMEA.begin(9600);
Serial.begin(57600);

SPI.begin();
radio.begin();
network.begin(90,this_node);

}

void loop() {
  
network.update();

unsigned long now = millis();              // If it's time to send a message, send it!
  if ( now - last_sent >= interval  )
  {
    last_sent = now;

    Serial.print("Sending...");
    payload_t payload = { hour, minute, seconds };
    RF24NetworkHeader header(/*to node*/ other_node);
    bool ok = network.write(header,&payload,sizeof(payload));
    if (ok)
      Serial.println("ok.");
    else
      Serial.println("failed.");
  }
}

while ((NMEA.available()) > 0){
  GPS = NMEA.read();
  //Serial.print(GPS);
  if (GPS == '\n')
  {
  
  /*for (int i = 1; i < strlen(GPSbuffer) - 3; i ++) { // checksum calculation need to make a function for it
        Bittotal ^= GPSbuffer[i];
  }
  
  Serial.print("calculated: ");
  Serial.println(Bittotal, HEX);
  */

    parsing(GPSbuffer);
    index = 0;
  }
  else{
    GPSbuffer[index] = GPS;
    index++;
    GPSbuffer[index] = '\0';
  }
}
}

void parsing(String GPSin){
  byte ind1;
  byte ind2;
  byte ind3;
  byte ind4;
  byte ind5;
  byte ind6;
  byte ind7;
  byte ind8;

  String header;
  String latbuf;
  String NS;
  String EW;
  String lon;
  String UTC;
  String status;
  String mode;
  String Checksum;
  String hourbuf;
  String minbuf;
  String secbuf;

  if (GPSin.startsWith(GPGLL) == true){
  
  

  ind1 = GPSin.indexOf(',');
  ind2 = GPSin.indexOf(',', ind1+1);
  ind3 = GPSin.indexOf(',', ind2+1);  
  ind4 = GPSin.indexOf(',', ind3+1);
  ind5 = GPSin.indexOf(',', ind4+1);
  ind6 = GPSin.indexOf(',', ind5+1);
  ind7 = GPSin.indexOf(',', ind6+1);
  ind8 = GPSin.indexOf('*', '\0');


  header = GPSin.substring(0,ind1);
  latbuf = GPSin.substring(ind1+1, ind2);
  lat = latbuf.toFloat();
  NS = GPSin.substring(ind2+1, ind3); 
  lon = GPSin.substring(ind3+1, ind4);
  EW =  GPSin.substring(ind4+1, ind5);
  UTC = GPSin.substring(ind5+1, ind6);
  hourbuf = UTC.substring(0, 2);
  minbuf = UTC.substring(2, 4);
  secbuf = UTC.substring(4,6);
  status = GPSin.substring(ind6+1, ind7);
  mode = GPSin.substring(ind7+1, ind8);
  Checksum = GPSin.substring(ind8);
  Check = Checksum.toInt();
  
  hour = hourbuf.toInt();
  minute = minbuf.toInt();
  seconds = secbuf.toInt();

  Serial.print("header: ");
  Serial.println(header);
  Serial.print("lat: ");
  Serial.println(lat);
  Serial.print("North or South: ");
  Serial.println(NS);
  Serial.print("lon: ");
  Serial.println(lon);
  Serial.print("East or West: ");
  Serial.println(EW);
  Serial.print("UTC time: ");
  Serial.println(UTC);
  Serial.print("status (A is valid, V not): ");
  Serial.println(status);
  Serial.print("mode: ");
  Serial.println(mode);
  Serial.print("checksum: ");
  Serial.println(Checksum);
  /*
  Serial.print("hour: ");
  Serial.println(hour);
  Serial.print("minutes: ");
  Serial.println(minute);
  Serial.print("seconds: ");
  Serial.println(seconds);
  */
  hour = hour +2;
  Serial.print("time: "); 
  Serial.print(hour); 
  Serial.print(":"); 
  Serial.print(minute); 
  Serial.print(":"); 
  if (seconds < 10){
    Serial.print("0");
  }
  Serial.print(seconds);
  Serial.println();
  Serial.println();
  

  
  }
/*
if (GPSin.startsWith(GPGGA) == true){
  ind1 = GPSin.indexOf(',');
  ind2 = GPSin.indexOf(',', ind1+1);
  ind3 = GPSin.indexOf(',', ind2+1);  
  ind4 = GPSin.indexOf(',', ind3+1);
  header = GPSin.substring(0,ind1);

  Serial.println(header);
}
*/
}
