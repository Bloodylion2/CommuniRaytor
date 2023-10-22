unsigned long timePrevWifi = 0; //Time when a recconect to wifi was tried previously
unsigned long intervalWifi = 5000; // 5 seconds

unsigned long timePrevChange; //Time when the message was previously changed
unsigned long intervalChange = 300000; // 5 minutes
String screenMessagePrevious;

void statusloop(){
  
  unsigned long timeNow = millis();
  // if WiFi is down, try reconnecting
  if ((WiFi.status() != WL_CONNECTED) && (timeNow - timePrevWifi >= intervalWifi)) {
    Serial.print(millis());
    Serial.println("Reconnecting to WiFi...");
    NewScreenMessage("Reconnecting to WiFi...", "", true);
    WiFi.disconnect();
    WiFi.reconnect();
    timePrevWifi = timeNow;
  }
  if (WiFi.status() != WL_CONNECTED){
    return; // Dont want to try and reconnect to twitch or turn off the screen if there is no internet connection
  }

  twitchConnect();

  if (screenMessage == ""){       // No message is currently displayed
    turnBacklightOff();
    if (!(screenMessagePrevious == screenMessage)) { //If the previous screenMessage is not the same as the current message
      screenMessagePrevious = screenMessage;
    }
  }
  else{                           // A message is currently displayed    
    turnBacklightOn();
    if (!(screenMessagePrevious == screenMessage)) { //If the previous screenMessage is not the same as the current message
      // Update the timePrevChange variable to track the time the message was empty
      timePrevChange = timeNow;
      screenMessagePrevious = screenMessage;
    }
    if (timeNow - timePrevChange > intervalChange) {  // Check if a certain time interval has passed since the message was empty
      // Perform some action when a certain time interval has passed (e.g., print "Looping")
      timePrevChange = timeNow;
      clearMessage(); // Auto clear message
    }
  }
}

void turnBacklightOn(){
  if (screenOn == false){ //Backlight is off
    // Set the backlight on
    screenOn = true;
    digitalWrite(PIN_LCD_BL, screenOn);
  }
}

void turnBacklightOff(){
  if (screenOn == true){ //Backlight is off
    // Set the backlight on
    screenOn = false;
    digitalWrite(PIN_LCD_BL, screenOn);
  }
}

void twitchConnect(){
  // Try to connect to chat. If it loses connection try again
  if (!client.connected()) {
    Serial.println("Attempting to connect to " + ircChannel );
    NewScreenMessage("Attempting to connect to " + ircChannel , "", true);
    // Attempt to connect
    if (client.connect(TWITCH_USER, TWITCH_USER, "")) { // client.connect(TWITCH_BOT_NAME, "", TWITCH_OAUTH_TOKEN), 
      client.sendRaw("CAP REQ :twitch.tv/tags "); //Request tags so it also shows the messagers badge/modstatus
      client.sendRaw("JOIN " + ircChannel);
      Serial.println("CommuniRaytor activated! ");
      //sendTwitchMessage("CommuniRaytor activated! ");
      NewScreenMessage("Activated!", "CommuniRaytor", false);

      spriteRay.setSwapBytes(true);
      spriteRay.pushSprite((screenWidth - 128), (screenHeight - (128)), TFT_BLACK);
    } 
    else {
      Serial.println("failed... try again in 5 seconds ");
      NewScreenMessage("Connecting to " + ircChannel + "failed... try again in 5 seconds", "", true);
      // Wait 5 seconds before retrying
      delay(5000);
    }
    return;
  }
}