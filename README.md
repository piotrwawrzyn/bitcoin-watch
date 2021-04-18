# bitcoin-watch

Bitcoin watch is a ESP8266 based project that allows you to monitor price action of all your favorite cryptocurrencies... on your desk.

I encourage anyone who's excited about crypto to go and build this project. It's really cool - it's a little bit like having a small binance widget right on your desk.

![Bitcoin watch preview 1](https://user-images.githubusercontent.com/42513971/115114340-0c463700-9f8f-11eb-8d2f-fdf4941b01d9.png)

## Table of Contents

1. [Features](#features)
2. [Hardware requirements](#hardware-requirements)
3. [Instalation and usage](#instalation-and-usage)
   - [Uploading the project to your board (step by step)](#uploading-the-project-to-your-board-step-by-step)
   - [Adding another cryptocurrency of your choice (step by step)](#adding-another-cryptocurrency-of-your-choice-step-by-step)
   - [Changing the theme to dark](#changing-the-theme-to-dark)
4. [Problems? Happy to help](#problems-happy-to-help)
5. [Credits](#credits)

## Features

- Monitor 10+ cryptocurrencies
- Add any cryptocurrency that is supported by [CoinGecko](https://www.coingecko.com/en) (over 6000 different coins)
- Pick image (logo) of your choice for every cryptocurrency
- Tons of information for every coin: **USD price**, **BTC price**, **ETH price**, **24h change**, **7 days change** all fitted on a compact 1"x1" screen
- Light theme and dark theme
- Connect to your home Wi-Fi network

![Bitcoin watch preview 2](https://user-images.githubusercontent.com/42513971/115114455-cccc1a80-9f8f-11eb-808a-844e0c1587e6.jpg)

## Hardware requirements

- [ESP8266 D1 Mini and D1 Mini TFT Shield](https://www.tindie.com/products/brianlough/d1-mini-tft-shield/)
- Some lego blocks? Just kidding, you can use anything to create a cover for the display. Just use your imagination!

## Instalation and usage

So you have the [ESP8266 D1 Mini and D1 Mini TFT Shield](https://www.tindie.com/products/brianlough/d1-mini-tft-shield/) already? Great, installation is straightforward and should only take you about 10 to 20 minutes. No deeper technical knowledge required. Below I will guide you step by step how to upload the software on your board and then how to add your own customized cryptos as well!

### Uploading the project to your board (step by step)

1. Download Arduino IDE software from [official arduino website](https://www.arduino.cc/en/software)

2. Download the project from github as a zip file

![Download project zip](https://user-images.githubusercontent.com/42513971/115114807-71029100-9f91-11eb-8991-8d7b9ff79264.png)

3. Unzip it in your desired location on your computer (for example on the desktop)

4. Follow [this little tutorial](https://randomnerdtutorials.com/how-to-install-esp8266-board-arduino-ide/) to install ESP8266 Add-on in Arduino IDE

5. Select the correct board in Arduino IDE

   - Go to `Tools > Board > ESP8266 Boards`
   - Select LOLIN(WEMOS) D1 R2 & mini

   ![Selecting the correct board](https://user-images.githubusercontent.com/42513971/115115313-0bfc6a80-9f94-11eb-9986-cb76401a8d2d.png)

6. Follow [this little tutorial](https://randomnerdtutorials.com/install-esp8266-filesystem-uploader-arduino-ide/) to install ESP8266 Filesystem Uploader in Arduino IDE. It is needed to upload images from your computer to your board

7. Install necessary libraries. We will need: `ArduinoJson`, `TFT_eSPI` and `TJpg_Decoder`

   - Download [zip file with these 3 libs](https://mega.nz/file/sPxQDSaY#1J7xJJsQ-JYTtRYMIueGydp0YMmAIVoMK3y47RF4u0w)
   - Place the libs from the zip file in the Arduino's `libraries` folder
   - The path to this folder is by default something like `C:\Users\<user>\Documents\Arduino\libraries`

   ![libraries folder](https://user-images.githubusercontent.com/42513971/115115939-5e8b5600-9f97-11eb-8dad-119e9db87f13.png)

8. Set up your Wi-fi connection in the project

   - Open `bitcoin-watch.ino` file in Arduino IDE
   - Put your Wi-fi ssid and password in configurations section.

   ![configurations](https://user-images.githubusercontent.com/42513971/115146247-f18ac580-a055-11eb-9cea-91bc02491a16.png)

9. Wire your ESP8266 D1 Mini board to your computer.

10. Upload images from the `data` folder to your ESP8266 D1 Mini

    - Click `tools > ESP8266 Sketch Data Upload`

    ![tools > ESP8266 Sketch Data Upload](https://user-images.githubusercontent.com/42513971/115118042-b75fec00-9fa1-11eb-81b1-b37a08d099b0.png)

    - It will automatically upload all the images from the data folder to your board
    - If you don't see the `ESP8266 Sketch Data Upload` in the menu it means that you didn't install ESP8266 Filesystem Uploader properly

11. Upload the project to your board

    - Click "Upload" button in the top menu

      ![Upload](https://user-images.githubusercontent.com/42513971/115117074-1cfda980-9f9d-11eb-9821-672d6c9618b0.png)

    - Uploading should take about 20-30 seconds

12. That's it. You should now see the application running on your board's screen. Good job! If you still have a problem or some of my explanations were not clear enough, consider opening and issue on GitHub or message me directly. I'm always happy to help.

### Adding another cryptocurrency of your choice (step by step)

You can track any cryptocurrency that is supported by CoinGecko

1. Get CoinGecko id of your cryptocurrency

   - Open [CoinGecko all coins list](https://api.coingecko.com/api/v3/coins/list?include_platform=false)
   - Use search to look for your cryptocurrency ticker. For example if you are looking to add VeChain (VET) look for `"VET"` (include quotation marks)

   ![Search for symbol](https://user-images.githubusercontent.com/42513971/115117474-00627100-9f9f-11eb-84e7-10f8c1c05d7a.png)

   - CoinGecko id is located next to the symbol. In our example the id is `vechain`

   ![VeChain id](https://user-images.githubusercontent.com/42513971/115117518-3f90c200-9f9f-11eb-8421-cefbaaf0fa92.png)

   - Copy this id, we will need it in the next steps

2. Add coin to the list in the project

   - Open `cryptos.h` file in the project
   - Find the `cryptos` array inside the file

     ![cryptos list](https://user-images.githubusercontent.com/42513971/115117565-88487b00-9f9f-11eb-8b42-7c646b9c69b6.png)

   - You can remove some of the already added coins from the array if you don't want to monitor their prices

   - In my case I will remove everything besides bitcoin and ethereum

   ![bitcoin and ethereum](https://user-images.githubusercontent.com/42513971/115117625-d78eab80-9f9f-11eb-8279-e9b90dfe5e71.png)

   - Add the id of the coin that you copied from CoinGecko's list in the same format as other coins

     ![image](https://user-images.githubusercontent.com/42513971/115117674-1886c000-9fa0-11eb-8f30-9bf8db12b5f1.png)

3. Add 80px x 80px coin logo to the `data` folder

   - Download logo image of your choice for your coin
   - Use a simple graphics editor tool to scale it to 80px x 80px. I recommend Paint for this one
   - Save it as **JPG** image with name as **id of the coin the logo is designed for** inside of **the data folder**. In my example I will save it as `vechain.jpg`
   - If the image is not in JPG format, it won't work. If the logo is not in the right dimensions it will look poor in the UI

4. Upload images from the `data` folder to your ESP8266 D1 Mini

   - Click `tools > ESP8266 Sketch Data Upload`
   - It will automatically upload all the images from the data folder to your board

5. Upload the project to your board

### Changing the theme to dark

1. Open `bitcoin-watch.ino` file in ArduinoIDE

2. Find the `Theme config` section inside the file

3. Comment out `#define LIGHT_THEME 1` and uncomment `#define DARK_THEME 1`

![Theme config](https://user-images.githubusercontent.com/42513971/115146368-8261a100-a056-11eb-9eab-1a3aa7d8be79.png)

4. Upload the project to your board

## Problems? Happy to help

If you have problems of any kind or need help with setting this thing up, I'm more than happy to help you! Consider either opening an issue on GitHub or message me directly.

## Credits

I would like to personally thank [Brian Lough](https://github.com/witnessmenow) for sending me the board and the display and giving the opportunity to work on this project. The Bitcoin Watch is also inspired by his old project [Simple Arduino Crypto Display](https://github.com/witnessmenow/simple-arduino-crypto-display) which used NodeMCU ESP8266 Board and OLED Display 1.3".
