#include <SoftwareSerial.h>

SoftwareSerial NMEA(3,4);
char GPS;
char GPSbuffer[150];
byte index;
String GPGLL = "$GPGLL";
String GPGGA = "$GPGGA";

void setup() {
  // put your setup code here, to run once:
NMEA.begin(9600);
Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
while ((NMEA.available()) > 0){
  GPS = NMEA.read();
  //Serial.print(GPS);
  if (GPS == '\n')
  {
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
  String lat;
  String NS;
  String EW;
  String lon;
  String UTC;
  String status;
  String mode;
  String Checksum;
  int Check;
  int Bittotal;

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
  lat = GPSin.substring(ind1+1, ind2);
  lat = lat.toFloat();
  NS = GPSin.substring(ind2+1, ind3); 
  lon = GPSin.substring(ind3+1, ind4);
  EW =  GPSin.substring(ind4+1, ind5);
  UTC = GPSin.substring(ind5+1, ind6);
  status = GPSin.substring(ind6+1, ind7);
  mode = GPSin.substring(ind7+1, ind8);
  Checksum = GPSin.substring(ind8);
  Check = Checksum.toInt();
  
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
