/***************************************************************************
  This is a library for the AMG88xx GridEYE 8x8 IR camera

  This sketch tries to read the pixels from the sensor

  Designed specifically to work with the Adafruit AMG88 breakout
  ----> http://www.adafruit.com/products/3538

  These sensors use I2C to communicate. The device's I2C address is 0x69

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Dean Miller for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_AMG88xx.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>

const char *ssid = "TP-LINK 2";
const char *password = "asdf1234";

Adafruit_AMG88xx amg;

float pixels[AMG88xx_PIXEL_ARRAY_SIZE];

void setup()
{
  Serial.begin(115200);
  Serial.println(F("AMG88xx pixels"));

  bool status;

  // default settings
  status = amg.begin();
  if (!status)
  {
    Serial.println("Could not find a valid AMG88xx sensor, check wiring!");
    while (1)
      ;
  }

  delay(4000); //Delay needed before calling the WiFi.begin

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  { //Check for the connection
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");

  Serial.println("-- Pixels Test --");

  Serial.println();

  delay(100); // let sensor boot up
}

void loop()
{
  if (WiFi.status() == WL_CONNECTED)
  { //Check WiFi connection status

    HTTPClient http;

    http.begin("https://testexpressjsiot.herokuapp.com/subscribers"); //Specify destination for HTTP request
    http.addHeader("Content-Type", "application/json");               //Specify content-type header
    StaticJsonDocument<2000> doc;
    JsonArray baris1 = doc.createNestedArray("baris1");
    JsonArray baris2 = doc.createNestedArray("baris2");
    JsonArray baris3 = doc.createNestedArray("baris3");
    JsonArray baris4 = doc.createNestedArray("baris4");
    JsonArray baris5 = doc.createNestedArray("baris5");
    JsonArray baris6 = doc.createNestedArray("baris6");
    JsonArray baris7 = doc.createNestedArray("baris7");
    JsonArray baris8 = doc.createNestedArray("baris8");
    doc["name"] = "ESP Fadhil";
    doc["subscribedToChannel"] = "Sensor AMGxxx";
    int j = 0;
    //read all the pixels
    amg.readPixels(pixels);

    for (int i = 1; i <= AMG88xx_PIXEL_ARRAY_SIZE; i++)
    {
      float testPixelValue = pixels[i - 1];
      if ((j >= 0) and (j < 8))
      {
        baris1.add(testPixelValue);
      }
      else if ((j >= 8) and (j < 16))
      {
        baris2.add(testPixelValue);
      }
      else if (((j >= 16) and (j < 24)))
      {
        baris3.add(testPixelValue);
      }

      else if (((j >= 24) and (j < 32)))
      {
        baris4.add(testPixelValue);
      }

      else if (((j >= 32) and (j < 40)))
      {
        baris5.add(testPixelValue);
      }

      else if (((j >= 40) and (j < 48)))
      {
        baris6.add(testPixelValue);
      }
      else if (((j >= 48) and (j < 56)))
      {
        baris7.add(testPixelValue);
      }
      else if (((j >= 56) and (j < 64)))
      {
        baris8.add(testPixelValue);
      }
      else
      {
        Serial.println("Out of indeks");
      }
      j += 1;
    }
    String requestBody;
    serializeJsonPretty(doc, requestBody);
    //delay a second

    int httpResponseCode = http.POST(requestBody); //Send the actual POST request
    Serial.println(requestBody);
    if (httpResponseCode > 0)
    {

      String response = http.getString(); //Get the response to the request

      Serial.println(httpResponseCode); //Print return code
      Serial.println(response);         //Print request answer
    }
    else
    {

      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }

    http.end(); //Free resources
  }
  else
  {

    Serial.println("Error in WiFi connection");
  }
  delay(20000);
}
