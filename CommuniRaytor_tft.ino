#include <WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <WiFiManager.h>         // https://github.com/tzapu/WiFiManager
#include <IRCClient.h>           // Changed it so it gave me more info about the chatters
#include <TFT_eSPI.h> 
#include <SPI.h>
#include "pin_config.h"
#include "Ray.h"    // Image converter http://www.rinkydinkelectronics.com/t_imageconverter565.php

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
const String twitchChannelName = "Bloodylion666"; //this is case sensitive!

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
// Set character height
int charHeight = 30;
int rowHeight = (charHeight + 2);
int screenRow = 0;

uint16_t backgroundColor = TFT_WHITE;
uint16_t textColor = TFT_BLACK;

// put your setup code here, to run once:
void setup() {
  pinMode(PIN_POWER_ON, OUTPUT);
  digitalWrite(PIN_POWER_ON, HIGH);

  Serial.begin(115200);
  Serial.println();
 
  tft.begin();

  tft.setRotation(3);
  tft.fillScreen(backgroundColor);

  spriteRay.setSwapBytes(true);
  spriteRay.createSprite(128, 128); 
  spriteRay.pushImage(0, 0, 128, 128, Ray);
  spriteRay.setSwapBytes(true);
  spriteRay.pushSprite((screenWidth - 128), (screenHeight - 128), TFT_BLACK);
 

  // WiFiManager
  // Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  
  // Uncomment and run it once, if you want to erase all the stored information
  //wifiManager.resetSettings();

  // fetches ssid and pass from eeprom and tries to connect
  // if it does not connect it starts an access point with the specified name
  // here  "Connecting_Ray"
  // and goes into a blocking loop awaiting configuration
  NewScreenMessage("Attempting to connect to Wifi", "", false);  
  wifiManager.autoConnect("Connecting_Ray", "RayRulez123.");
  
  // if you get here you have connected to the WiFi
  Serial.println("Connected.");
  NewScreenMessage("Connected.", "", false);
  
  tft.setCursor(0, screenHeight - rowHeight);
  tft.println(WiFi.localIP());

  server.begin();
 
  ircChannel = "#" + twitchChannelName;
 
  client.setCallback(callback);
}
 
void loop() {
  // Try to connect to chat. If it loses connection try again
  if (!client.connected()) {
    Serial.println("Attempting to connect to " + ircChannel );
    NewScreenMessage("Attempting to connect to " + ircChannel , "", false);
    // Attempt to connect
    if (client.connect(TWITCH_USER, TWITCH_USER, "")) { // client.connect(TWITCH_BOT_NAME, "", TWITCH_OAUTH_TOKEN), 
      client.sendRaw("CAP REQ :twitch.tv/tags"); //Request tags so it also shows the messagers badge/modstatus
      client.sendRaw("JOIN " + ircChannel);
      Serial.println("CommuniRaytor activated!");
      //sendTwitchMessage("CommuniRaytor activated!");
      NewScreenMessage("Activated!", "CommuniRaytor", false);

      spriteRay.setSwapBytes(true);
      spriteRay.pushSprite((screenWidth - 128), (screenHeight - (128 + rowHeight)), TFT_BLACK);

      tft.setCursor(0, screenHeight - rowHeight);
      tft.println(WiFi.localIP());
    } 
    else {
      Serial.println("failed... try again in 5 seconds");
      NewScreenMessage("Connecting to " + ircChannel + "failed... try again in 5 seconds", "", false);
      // Wait 5 seconds before retrying
      delay(5000);
    }
    return;
  }

   client.loop();
  //  if (scrollActive) {
  //    NewScreenMessage(screenMessage, lastUser, true);
  //  }
}
 
void sendTwitchMessage(String message) {
  client.sendMessage(ircChannel, message);
}

void NewScreenMessage(String message, String user, bool dontClear) {
  screenMessage = message;
  lastUser = user;

  tft.setTextSize(charHeight/10);
  tft.setTextColor(textColor);

  if(!dontClear)
  {
    tft.fillScreen(backgroundColor);
    screenRow = 0;
    tft.setCursor(0, screenRow*rowHeight);
  }

  if(user != ""){
    tft.setTextWrap(false,false);
    tft.println(user);
    tft.setCursor(0, screenRow*rowHeight);
    screenRow++;
    Serial.println(message.substring(screenWidth));
  }

  int textWidth = tft.textWidth(message);

  if(textWidth > screenWidth){
    String messageLeft = message;
    Serial.println(messageLeft);
    bool messageSend = false;

    for (messageSend = false; messageSend == true;){
      tft.setCursor(0, screenRow*rowHeight);
      screenRow++;

      //Variables to make the next line
      int spaceAt = 0;
      int spaceAtprevious = 0;
      bool rowDone = false;

      for (rowDone = false; rowDone == true;){  // We are gonna find the part of the message that fits on a line
        String rowMessage; // Variable for the rowMessage
        spaceAt = messageLeft.indexOf(" ", spaceAtprevious); // Finding the position on of the next space
        Serial.println(spaceAt);

        if (spaceAt > -1){
          if (tft.textWidth(messageLeft.substring(0, spaceAt)) > screenWidth){
            rowMessage = messageLeft.substring(0, spaceAtprevious);
            messageLeft = messageLeft.substring(spaceAtprevious);
            tft.println(rowMessage);
            Serial.println(rowMessage);

            Serial.println(messageLeft);
            rowDone = true;            
          }
          else{
            spaceAtprevious = spaceAt;
            Serial.println(spaceAtprevious);
          }
        }
        else{
          rowDone = true;
          messageSend = true;
          
          tft.println(rowMessage);
          Serial.println(rowMessage);
        }
      }
    }
  }
  else{
    tft.setCursor(0, screenRow*rowHeight);
    screenRow++;
    tft.setTextWrap(true,false);
    tft.println(message);
  }
}

void screenBlink(int times){
  for (int i=0; i < times; i++){
    tft.fillScreen(TFT_WHITE);
    delay(200);
    tft.fillScreen(TFT_BLACK);
    delay(200);
  }
}

void callback(IRCMessage ircMessage) {
  /* ------------------ Put here for debugging     
  Serial.println("In CallBack"); 
  Serial.println(ircMessage.original);
  Serial.println(ircMessage.prefix);
  Serial.println(ircMessage.nick);
  Serial.println(ircMessage.user);
  Serial.println(ircMessage.host);
  Serial.println(ircMessage.command);
  Serial.println(ircMessage.parameters);
  Serial.println(ircMessage.text);
  Serial.println(ircMessage.badge);
  Serial.println(ircMessage.color);
  Serial.println(ircMessage.displayName);
  Serial.println(ircMessage.firstMsg);
  Serial.println(ircMessage.subscriber);
  Serial.println(ircMessage.userType); 
  ----------------------------------------------*/

  if (ircMessage.command == "PRIVMSG" && ircMessage.text[0] != '\001') 
  {
    //Serial.println("Passed private message."); Put here for debugging
    
    //Change to all upercase
    ircMessage.badge.toUpperCase();
    ircMessage.nick.toUpperCase();
  
    String message(ircMessage.text);
    String user("<" + ircMessage.nick + ">");
    
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
      if (messageUppercase.indexOf("CLEAR") > -1)
      {
        screenMessage = "";
        lastUser = "";
        tft.fillScreen(backgroundColor);
        Serial.println("Cleared");
        sendTwitchMessage("!CRF Cleared");
      }
      else
      {
      screenBlink(2);

      NewScreenMessage(message, user, false);
      }
    }
  }
}