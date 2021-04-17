struct Price {
  double usd;
  String btc;
  String eth;
};

struct Crypto
{
  String apiName;
  String symbol;
  Price price;
  double dayChange;
  double weekChange;
};

// ----------------------------
// Coin id list - adjust it to meet your interests
//
// Put your cryptocurrencies in the array below.
// Get id of your coin here: https://api.coingecko.com/api/v3/coins/list?include_platform=false
// ----------------------------

Crypto cryptos[] = {
    {"bitcoin"},
    {"ethereum"},
    {"binancecoin"},
    {"cardano"},
    {"dogecoin"}
 };

int cryptosCount = (sizeof(cryptos) / sizeof(cryptos[0]));
