// ----------------------------
// Functions used to download data from coingecko retrieve are separated in this file
// ----------------------------

HTTPClient http;
WiFiClientSecure client;

const char *coingeckoSslFingerprint = "8925605d5044fcc0852b98d7d3665228684de6e2";

String combineCryptoCurrencies()
{
  String cryptosString = "";

  for (int i = 0; i < cryptosCount; i++)
  {
    cryptosString += cryptos[i].apiName;

    if (i != cryptosCount - 1)
    {
      cryptosString += "%2C";
    }
  }

  return cryptosString;
}

int getCryptoIndexById(String id)
{
  for (int i = 0; i < cryptosCount; i++)
  {
    if (cryptos[i].apiName == id)
      return i;
  }
}

void downloadBtcAndEthPrice()
{
  client.setFingerprint(coingeckoSslFingerprint);
  http.useHTTP10(true);

  String apiUrl = "https://api.coingecko.com/api/v3/simple/price?ids=" + combineCryptoCurrencies() + "&vs_currencies=btc%2Ceth";

  client.connect("api.coingecko.com", 443);
  http.begin(client, apiUrl);

  int code = http.GET();
  if (code != HTTP_CODE_OK)
  {
    Serial.println("Error connecting to API while downloading BTC and ETH data");
    Serial.println(code);
    return;
  }

  Serial.println("Successfuly downloaded BTC and ETH data");

  StaticJsonDocument<512> filter;

  for (int i = 0; i < cryptosCount; i++)
  {
    filter[cryptos[i].apiName]["btc"] = true;
    filter[cryptos[i].apiName]["eth"] = true;
  }

  DynamicJsonDocument doc(4096);
  DeserializationError error = deserializeJson(doc, http.getStream(), DeserializationOption::Filter(filter));

  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
  }

  for (int i = 0; i < cryptosCount; i++)
  {
    JsonObject json = doc[cryptos[i].apiName];
    String btcPrice = json["btc"];
    String ethPrice = json["eth"];

    cryptos[i].price.btc = btcPrice;
    cryptos[i].price.eth = ethPrice;
  }

  http.end();
  client.stop();
}

void downloadBaseData(String vsCurrency)
{
  http.useHTTP10(true);
  client.setFingerprint(coingeckoSslFingerprint);

  String apiUrl = "https://api.coingecko.com/api/v3/coins/markets?vs_currency=" + vsCurrency + "&ids=" + combineCryptoCurrencies() + "&order=market_cap_desc&per_page=100&page=1&sparkline=false&price_change_percentage=24h%2C7d";

  client.connect("api.coingecko.com", 443);

  http.begin(client, apiUrl);

  int code = http.GET();
  if (code != HTTP_CODE_OK)
  {
    Serial.println("Error connecting to API while downloading base data");
    Serial.println(code);
    return;
  }

  Serial.println("Successfuly downloaded BASE data");

  StaticJsonDocument<512> filter;

  for (int i = 0; i < cryptosCount; i++)
  {
    filter[i]["id"] = true;
    filter[i]["symbol"] = true;
    filter[i]["current_price"] = true;
    filter[i]["price_change_percentage_24h_in_currency"] = true;
    filter[i]["price_change_percentage_7d_in_currency"] = true;
  }

  DynamicJsonDocument doc(4096);
  DeserializationError error = deserializeJson(doc, http.getStream(), DeserializationOption::Filter(filter));

  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
  }

  for (int i = 0; i < cryptosCount; i++)
  {
    JsonObject json = doc[i];
    String id = json["id"];
    int cryptoIndex = getCryptoIndexById(id);

    double currentPrice = json["current_price"];
    cryptos[cryptoIndex].price.usd = currentPrice;

    String symbol = json["symbol"];
    symbol.toUpperCase();
    double dayChange = json["price_change_percentage_24h_in_currency"];
    double weekChange = json["price_change_percentage_7d_in_currency"];

    cryptos[cryptoIndex].symbol = symbol;
    cryptos[cryptoIndex].dayChange = dayChange;
    cryptos[cryptoIndex].weekChange = weekChange;
  }

  http.end();
  client.stop();
}
