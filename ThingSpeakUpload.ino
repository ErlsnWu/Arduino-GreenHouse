#include <SoftwareSerial.h>
#define _baudrate   115200
#define _rxpin      19
#define _txpin      18
//*-- IoT Information
#define SSID "ElectroBoom"
#define PASS "kuto9987"
#define IP "184.106.153.149" // ThingSpeak IP Address: 184.106.153.149
// 使用 GET 傳送資料的格式
// GET /update?key=[THINGSPEAK_KEY]&field1=[data 1]&filed2=[data 2]...;
String GET = "GET /update?key=TQU84QSJSOHTDVTB";

void setup() {
    randomSeed(analogRead(A0));
    Serial.begin( _baudrate );
    Serial1.begin( _baudrate );
    sendSerial1("AT");
    delay(1000);
    if(Serial1.find("OK"))
    {
        Serial.println("RECEIVED: OK\nData ready to sent!");
        connectWiFi();
    }
    else{
    Serial.println("NO RESEPONCE!");
  }
}
void loop() {
    
    long i = random(1,100);
    long j = random(0,51);
    delay(5000);   // 60 second
    SentOnCloud( String(i), String(j) );
}
void SentOnCloud( String T, String H )
{
    // 設定 ESP8266 作為 Client 端
    String cmd = "AT+CIPSTART=\"TCP\",\"";
    cmd += IP;
    cmd += "\",80";
    sendSerial1(cmd);
    if( Serial1.find( "Error" ) )
    {
        Serial.print( "RECEIVED: Error\nExit1" );
        return;
    }
    cmd = GET + "&field1=" + T + "&field2=" + H +"\r\n";
    Serial1.print( "AT+CIPSEND=" );
    Serial1.println( cmd.length() );
    if(Serial1.find( ">" ) )
    {
        Serial.print(">");
        Serial.print(cmd);
        Serial1.print(cmd);
    }
    else
    {
        Serial1.print( "AT+CIPCLOSE" );
    }
    if( Serial1.find("OK") )
    {
        Serial.println( "RECEIVED: OK" );
    }
    else
    {
        Serial.println( "RECEIVED: Error\nExit2" );
    }
}
void sendSerial1(String cmd)
{
    Serial.print("SEND: ");
    Serial.println(cmd);
    Serial1.println(cmd);
} 
 
boolean connectWiFi()
{
    Serial1.println("AT+CWMODE=1");
    delay(2000);
    String cmd="AT+CWJAP=\"";
    cmd+=SSID;
    cmd+="\",\"";
    cmd+=PASS;
    cmd+="\"";
    sendSerial1(cmd);
    delay(5000);
    if(Serial1.find("OK"))
    {
        Serial.println("RECEIVED: OK");
        return true;
    }
    else
    {
        Serial.println("RECEIVED: Error");
        return false;
    }
    cmd = "AT+CIPMUX=0";
    sendSerial1( cmd );
    if( Serial1.find( "Error") )
    {
        Serial.print( "RECEIVED: Error" );
        return false;
    }
}
