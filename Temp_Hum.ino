#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHTesp.h"
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti WiFiMulti;
#define DHTpin 14                //D5 of NodeMCU is GPIO14 (pin sensor DHT)
DHTesp dht;
#define OLED_RESET 0             // GPIO0 SCL GPIO5  SDA GPIO4  (puertos display) 
Adafruit_SSD1306 display(OLED_RESET);
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16
WiFiServer server(80);


void setup()   {
  Serial.begin(115200);                //inicialización puerto serie
  delay(10);
  Serial.println();
  dht.setup(DHTpin, DHTesp::DHT11);    //for DHT11 Connect DHT sensor to GPIO 14 (d5)
                                       // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
    
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 64x48)
  display.display();                   //actualizar datos display (splash)
  delay(1000);
  display.clearDisplay();              //borrar datos display
                                        // display init done
  
                                          // We start by connecting to a WiFi network
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("Redmi", "perrete1"); //(SSID,PASSWD)



                                            //connection status (display + serial)
  display.setTextSize(1);                   //Tamaño
  display.setTextColor(WHITE);            //Color
  display.setCursor(0,0);                   //Posición
  Serial.println();
  display.println();
  Serial.print("Wait for WiFi... ");
  display.println("Wait for WiFi... ");
  display.display();
  while (WiFiMulti.run() != WL_CONNECTED) {   //Hasta que se conecte pone puntos.
    Serial.print(".");
    display.print(".");
    delay(200);
    display.display();
  }
  display.clearDisplay();                       //Una vez conectado
  display.setCursor(0,12); 
  Serial.println("** WiFi connected **");
  display.println("** WiFi connected **");    //Nos lo dice
  display.display();
  delay(500);
  display.clearDisplay(); 
  display.setCursor(0,0);  
  Serial.println("IP address: ");
  display.println("IP address: ");            //Y Muestra la IP
  Serial.println(WiFi.localIP());
  display.println(WiFi.localIP());
  delay(2000);
  display.display();  

  //FIN CONFIGURACION
  server.begin();                             //Inicializa el servidor
  Serial.println("Server started");
  display.println("Server started");
  display.display();  
} 
//Arranque servidor
   
void loop() {
  float humidity = dht.getHumidity();             // Toma dato de humedad
  float temperature = dht.getTemperature();      // Toma dato de temperatura
  int hum;
  int temp;
  temp = (int) temperature;                       //Lo pasa a valor entero
  hum = (int) humidity;   
  WiFiClient client = server.available();
  
  if (!client){                                   //Si no hay clientes
  display.clearDisplay(); 
//display.setCursor(15,12); 
//display.println("Sin clientes");                 //Lo dice
  Serial.println("Sin clientes");
//display.display();
  delay(1000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Temperatura: ");                 //Y saca datos por serial y display
  Serial.println("Temperatura: ");
  display.println(temp);
  Serial.println(temp);
  display.setCursor(0,17);  
  display.println("Humedad: ");
  Serial.println("Humedad: ");
  display.println(hum);
  Serial.println(hum);
  display.display();
  delay(1000);
}
//return;
  if(client){                                 //Si SI hay clientes
    String req = client.readStringUntil('\r');
    display.clearDisplay(); 
    display.setCursor(0,0);  
    Serial.println("Sending data...");        // Nos lo dice
    display.println("Sending data...");
    display.display();
    delay(1000);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("Temperatura: ");         //Saca datos por display y serial
    Serial.println("Temperatura: ");
    display.println(temp);
    Serial.println(temp);
    display.setCursor(0,17);  
    display.println("Humedad: ");
    Serial.println("Humedad: ");
    display.println(hum);
    Serial.println(hum);
    display.display();
    delay(1000); 
    
    if (req=="GET /temp HTTP/1.1"){        //Y responde a la petición   ("temp  hum  tym")
      String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\Temperatura: ";
      s += temp;
      s += "</html>\n";
      client.print(s);
      delay(1);
      // Formato=  "Temperatura: 47"
    }
    if (req=="GET /hum HTTP/1.1"){
      String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\Humedad: ";
      s += hum;
      s += "</html>\n";
      client.print(s);
      delay(1);
      
    }
    if (req=="GET /tym HTTP/1.1"){
      String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\Humedad: ";
      s += hum;
        s += " temperatura: ";
      s += temp;
      s += "</html>\n";
      client.print(s);
      delay(1);
    }
    display.display();
   delay(5000);

  } 
 }
