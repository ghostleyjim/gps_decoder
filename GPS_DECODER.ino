#include <Arduino.h>
#include <SoftwareSerial.h>
#include <HardwareSerial.h>

char NMEAbuffer;
const int sentenceIndex = 128;
char sentenceBuffer[sentenceIndex];
int index;

String header;
String lat;
String NS;
String lon;
String EW;
String Checksum;


SoftwareSerial NMEA(3,4);

void setup(){
    Serial.begin(9600);
    NMEA.begin(9600);
}

void loop(){
    while (NMEA.available() > 0)
    {
       GPS(sentenceBuffer);
       NMEAbuffer = NMEA.read();
       if(NMEAbuffer == '\n')
       {
           index = 0;
        } 
        else if (NMEAbuffer == '$') 
        {
            sentenceBuffer[index] = NMEAbuffer;
            index++;
        }
    }
}

void GPS(String input){
    byte ind[19];
    byte index;
    
    index = input.length();

    if(input.startsWith("$GPGLL"))
    {
        for(int i = 0; i < index; i++){
          ind[i] = input.indexOf(',' , ind[i]+1);
        }
    
    header = input.substring(0, ind[0]);
    lat = input.substring(ind[0]+1, ind[1]);
    NS = input.substring(ind[1]+1, ind[2]);
    lon = input.substring(ind[2]+1, ind[3]);
    EW = input.substring(ind[3]+1, ind[4]);
    Checksum = input.substring(ind[4], index);
    
    Serial.print(header);
    Serial.print(lat);
    Serial.print(NS);
    Serial.print(lon);
    Serial.print(EW);
    Serial.print(Checksum);
    }
    
       
    
    }

