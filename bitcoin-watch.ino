#define ARDUINOJSON_USE_DOUBLE 1
#define ARDUINOJSON_USE_LONG_LONG 1
#define ARDUINOJSON_NEGATIVE_EXPONENTIATION_THRESHOLD 1e-12
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WifiClientSecure.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <TJpg_Decoder.h>
#include <FS.h>
#define FS_NO_GLOBALS
#define FREE_SANS_12 &FreeSans12pt7b
#define FREE_SANS_18 &FreeSans18pt7b
#define ROBOTO_24 &Roboto_Thin_24
#include "cryptos.h"
#include "coingecko-api.h"

// ----------------------------
// Configurations - Update these
// ----------------------------

const char *ssid = "Your Wi-fi SSID (name)";
const char *password = "Your Wi-fi password";
unsigned long secondsForEachCrypto = 12;
int brightness = 250; // 1000 == 100%

// ----------------------------
// End of area you need to change
// ----------------------------

TFT_eSPI tft = TFT_eSPI();

void setup()
{
  Serial.begin(115200);
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  
  pinMode(D8, INPUT);
  pinMode(D3, INPUT);
  pinMode(D4, OUTPUT);
  analogWrite(D4, brightness);
  
  TJpgDec.setJpgScale(1);
  TJpgDec.setSwapBytes(true);
  TJpgDec.setCallback(displayOutput);

  if (!SPIFFS.begin())
  {
    while (1)
      yield();
  }

  connectToWifi();
  renderWelcomeMsg();
  delay(4000);
}

void loop()
{
  downloadBaseData("usd");
  delay(1000);
  downloadBtcAndEthPrice();
  for (int i = 0; i < cryptosCount; i++)
  {
    renderCryptoCard(cryptos[i]);
    delay(secondsForEachCrypto * 1000);
  }
}

void renderCryptoCard(Crypto crypto)
{
  tft.fillScreen(TFT_WHITE);

  TJpgDec.drawFsJpg(10, 10, "/" + crypto.apiName + ".jpg");
  tft.setFreeFont(ROBOTO_24);
  
  if (crypto.symbol.length() > 3) {
    tft.drawString(crypto.symbol, 15, 100);
  } else {
    tft.drawString(crypto.symbol, 25, 100);
  }
  
  
  tft.setTextColor(TFT_BLACK);
  tft.setFreeFont(ROBOTO_24);
  tft.drawString("$", 100, 24);
  tft.setFreeFont(FREE_SANS_18);
  tft.drawString(formatCurrency(crypto.price.usd), 116, 20);

  tft.setFreeFont(ROBOTO_24);
  tft.drawString("1D: ", 100, 68);
  tft.setFreeFont(FREE_SANS_12);
  tft.drawString(formatPercentageChange(crypto.dayChange), 160, 70);
  renderConditionalArrow(138, 70, crypto.dayChange);

  tft.setFreeFont(ROBOTO_24);
  tft.setTextColor(TFT_BLACK);
  tft.drawString("7D: ", 100, 103);
  tft.setFreeFont(FREE_SANS_12);
  tft.drawString(formatPercentageChange(crypto.weekChange), 160, 105);
  renderConditionalArrow(138, 105, crypto.weekChange);
  
  tft.setTextColor(TFT_BLACK);
  TJpgDec.setJpgScale(4);
  TJpgDec.drawFsJpg(14, 145, "/" + crypto.apiName + ".jpg");
  tft.setFreeFont(ROBOTO_24);
  tft.drawString("/", 35, 153);
  TJpgDec.drawFsJpg(42, 163, "/bitcoin.jpg");
  tft.setFreeFont(FREE_SANS_12);
  tft.drawString(crypto.price.btc, 78, 153);

  TJpgDec.drawFsJpg(14, 195, "/" + crypto.apiName + ".jpg");
  tft.setFreeFont(ROBOTO_24);
  tft.drawString("/", 35, 203);
  TJpgDec.drawFsJpg(42, 213, "/ethereum.jpg");
  tft.setFreeFont(FREE_SANS_12);
  tft.drawString(crypto.price.eth, 78, 203);
  TJpgDec.setJpgScale(1);

  tft.drawLine(5, 188, 235, 188, TFT_BLACK);
  tft.drawLine(70, 145, 70, 230, TFT_BLACK);
}

void renderConditionalArrow(int x, int y, double percentageChange) {
  if (percentageChange < 0)
  {
    TJpgDec.drawFsJpg(x, y, "/arrow_down.jpg");
  }
  else
  {
    TJpgDec.drawFsJpg(x, y, "/arrow_up.jpg");
  }
}

bool displayOutput(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t *bitmap)
{
  // Stop further decoding as image is running off bottom of screen
  if (y >= tft.height())
    return 0;

  // This function will clip the image block rendering automatically at the TFT boundaries
  tft.pushImage(x, y, w, h, bitmap);

  // Return 1 to decode next block
  return 1;
}

void renderWelcomeMsg()
{
  tft.fillScreen(TFT_WHITE);
  TJpgDec.drawFsJpg(58, 30, "/bitcoin_welcome.jpg");
  tft.setTextColor(TFT_ORANGE, TFT_WHITE);
  tft.drawCentreString("Bitcoin Watch", 120, 170, 4);
}

void connectToWifi()
{
  WiFi.begin(ssid, password);
  String dots[3] = {".", "..", "..."};
  int numberOfDots = 1;

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  while (WiFi.status() != WL_CONNECTED)
  {
    tft.drawCentreString("Connecting to WiFi " + dots[numberOfDots - 1], 120, 120, 2);

    if (numberOfDots == 3)
    {
      numberOfDots = 0;
    }
    else
    {
      numberOfDots++;
    }

    delay(300);
    tft.fillScreen(TFT_BLACK);
  }
}

String formatCurrency(double price)
{
  int digitsAfterDecimal = 3;

  if (price >= 1000)
  {
    digitsAfterDecimal = 0;
  }
  else if (price >= 100)
  {
    digitsAfterDecimal = 1;
  }
  else if (price >= 1)
  {
    digitsAfterDecimal = 2;
  }
  else if (price < 0.001)
  {
    digitsAfterDecimal = 4;
  }

  return String(price, digitsAfterDecimal);
}

String formatPercentageChange(double change)
{
  if (change >= 0)
  {
    tft.setTextColor(TFT_DARKGREEN);
  }
  else
  {
    tft.setTextColor(TFT_RED);
  }

  double absChange = change;

  if (change < 0)
  {
    absChange = -change;
  }
  
  if (absChange > 100) {
    return String(absChange, 0) + "%";
  } else if (absChange >= 10) {
    return String(absChange, 1) + "%";
  } else {
    return String(absChange) + "%";
  }
}
