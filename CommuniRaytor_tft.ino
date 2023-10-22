#include <WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include "FS.h"
//#include <LITTLEFS.h>
#include <WiFiManager.h>    // WiFiManager Library
#include <IRCClient.h>      // Changed it so it gave me more info about the chatters
#include <TFT_eSPI.h> 
//#include <SPI.h>
#include <WebServer.h>

#include "Files\pin_config.h"
#include "Files\Ray.h"            // Image converter http://www.rinkydinkelectronics.com/t_imageconverter565.php

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite spriteRay = TFT_eSprite(&tft);

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

//define your default values here, if there are different values in config.json, they are overwritten.
#define secret_ssid "my ssid" 
#define IRC_SERVER   "irc.chat.twitch.tv"
#define IRC_PORT     6667
 
//------- Replace the following! ------
char ssid[] = "";       // your network SSID (name)
char password[] = "";  // your network key
 
//The name of the channel that you want the bot to join
const String twitchChannelName = "CzechGamesEdition"; //this is case sensitive!

/*------ Using the Botname and OAuth key only when we want to send things in chat. Using a justinfanXYZ format (XYZ is any numbers) to be able to connect without a Authkey 
//The name that you want the bot to have
#define TWITCH_BOT_NAME "CommuniRaytor"

//OAuth Key for your twitch bot
// https://twitchapps.com/tmi/
#define TWITCH_OAUTH_TOKEN "oauth:tf6ej0p7ttoim10izy2nfklbi1zblb"
---------------------------------*/
 
String TWITCH_USER = "justinfan666";
 
String ircChannel = "";
 
WiFiClient wiFiClient;
IRCClient client(IRC_SERVER, IRC_PORT, wiFiClient);

// LCD message parts
String screenMessage, lastUser;
String botCommand = "!cr";

// set the screen number of pixels in length and height
int screenWidth = 320;
int screenHeight = 170;
// Border around, pixels from edge
int screenEdge = 2;
// Set character height
int charHeight = 30;
int rowHeight = (charHeight + 1);
int screenRow = 0;

bool screenOn = false; // Declaration to keep track of wether the screen is on or off

uint16_t backgroundColor = TFT_WHITE;
String backgroundColorString = "WHITE";
String backgroundColorStringOld = "WHITE";
uint16_t textColor = TFT_BLACK;
String textColorString = "BLACK";
String textColorStringOld = "BLACK";

bool startWebClient = false;
bool webserverActiveFirstCycle;

// WiFiManager
WiFiManager wfm;

// put your setup code here, to run once:
void setup() {

  pinMode(PIN_POWER_ON, OUTPUT);
  digitalWrite(PIN_POWER_ON, HIGH);

  Serial.begin(2000000); //115200
 
  tft.begin();

  tft.setRotation(3);
  
  spriteRay.setSwapBytes(true);
  spriteRay.createSprite(128, 128); 
  spriteRay.pushImage(0, 0, 128, 128, Ray);
  spriteRay.setSwapBytes(true);
  spriteRay.pushSprite((screenWidth - 128), (screenHeight - 128), TFT_BLACK);

  // Set the backlight pin as an OUTPUT
  pinMode(PIN_LCD_BL, OUTPUT);
  turnBacklightOn();
  
  // Supress Debug information
  wfm.setDebugOutput(false);

  // Uncomment and run it once, if you want to erase all the stored information
  //wfm.resetSettings();  

  // fetches ssid and pass from eeprom and tries to connect
  // if it does not connect it starts an access point with the specified name
  // here  "Connecting_Ray"
  // and goes into a blocking loop awaiting configuration
  tft.setCursor(screenEdge, screenHeight - rowHeight - screenEdge);
  NewScreenMessage("Attempting to connect to Wifi", "", false);  
  wfm.autoConnect("Connecting_Ray", "RayRulez123.");
  
  // if you get here you have connected to the WiFi
  Serial.println("Connected. ");
  NewScreenMessage("Connected.", "", false);
 
  ircChannel = "#" + twitchChannelName;
  
  client.setCallback(callback);

  server.begin();
}


void loop() {
  statusloop();

  client.loop();
  
  if(startWebClient){
    WebserverClient();
  }
  else{
    webserverActiveFirstCycle = true;
  }
  
    // Check background color
  if(!(backgroundColorString == backgroundColorStringOld)){
    colorChoice(backgroundColorString, backgroundColor, backgroundColorStringOld);
    backgroundColorStringOld = backgroundColorString;
    NewScreenMessage("changed", "Color", true);
  }

    // Check text color
  if(!(textColorString == textColorStringOld)){
    colorChoice(textColorString, textColor, textColorStringOld);
    textColorStringOld = textColorString;
    NewScreenMessage("changed", "Color", true);
  }

}

void sendTwitchMessage(String message) {
  client.sendMessage(ircChannel, message);
}

void callback(IRCMessage ircMessage) {
  /* ------------------ Put here for debugging *    
  Serial.println("In CallBack"); 
  Serial.println("original: " + ircMessage.original);
  Serial.println("prefix: " + ircMessage.prefix);
  Serial.println("nick: " + ircMessage.nick);
  Serial.println("host: " + ircMessage.host);
  Serial.println("command: " + ircMessage.command);
  Serial.println("parameters: " + ircMessage.parameters);
  Serial.println("text: " + ircMessage.text);
  Serial.println("badge: " + ircMessage.badge);
  Serial.println("color: " + ircMessage.color);
  Serial.println("firstMsg: " + ircMessage.firstMsg);
  Serial.println("subscriber: " + ircMessage.subscriber);
  Serial.println("userType: " + ircMessage.userType); 
  /*----------------------------------------------*/

  if (ircMessage.command == "PRIVMSG" && ircMessage.text[0] != '\001') 
  {
    //Serial.println("Passed private message. "); Put here for debugging
    
    //Change to all upercase
    ircMessage.badge.toUpperCase();
    ircMessage.nick.toUpperCase();
  
    String message(ircMessage.text);
    String user("<" + ircMessage.nick + "> ");
    
    //Finding the first comma and splitting the message there.
    int commandAt = -1;

    //Making a uppercase temp for the botCommand uppercase string for the message. To make sure any form of the command works
    String botCommandTemp(botCommand); //Temp string for use here
    botCommandTemp.toUpperCase();
    
    String messageUppercase(message); //String for use here
    messageUppercase.toUpperCase();

    commandAt = messageUppercase.indexOf(botCommandTemp);
    if (commandAt > -1)
    {
      message = message.substring(commandAt + 3); 
      if (message.indexOf(" ") == 0) //if the first character is a space, make a new string after without that space
      {
        message = message.substring(1);
      }
    }
  
    //prints chat to serial
    Serial.println(user + message);

    messageUppercase.replace(" ",""); // Replace all spaces with to able to find the command only if its the first thing in the sentence

    //this is where you would replace these elements to match your streaming configureation. 
    if (commandAt > -1 && (ircMessage.badge.indexOf("MODERATOR") > -1 || ircMessage.badge.indexOf("BROADCASTER") > -1)){
      if (messageUppercase.indexOf("CLEAR") == 3)
      {
        //Serial.println(messageUppercase);

        clearMessage();
        Serial.println("Cleared ");
      }
      else if (messageUppercase.indexOf("CONFIG") == 3) {
        if(!startWebClient){
          Serial.println("Config");
          
          wfm.startWebPortal();
          startWebClient = true;
        }
        else{
          Serial.println("Config off");

          wfm.stopWebPortal();
          startWebClient = false;
        }
        
      }
      else
      {
      //Serial.println(messageUppercase);
      screenBlink(2);

      NewScreenMessage(message, user, true);
      }
    }
  }
}

