//Variables to make the next Row whileloop
int spaceAt = 0;
int spaceAtprevious = 0;
bool rowDone = false;
int i = 10;
String rowMessage = ""; // Variable for the rowMessage

void NewScreenMessage(String message, String user, bool parsing) {
  String messageLeft = "";
  screenMessage = message;
  lastUser = user;
  //Serial.println("Screenmessage" + screenMessage);

// Setting up the screen, text color and size
  tft.setTextSize(charHeight/10); 
  tft.setTextColor(textColor);
  tft.fillScreen(backgroundColor);

  turnBacklightOn();

  screenRow = 0;
  tft.setCursor(screenEdge, (screenRow*rowHeight) + screenEdge);


  if(user != ""){
    tft.setTextWrap(false,false);
    tft.println(user);
    nextRowTFT();
  }

  int messageWidth = tft.textWidth(message);

  if((messageWidth > screenWidth) & (parsing == true)){
    //Serial.println("Message too wide");

    //Serial.println(message);
    messageLeft = message;
    //Serial.println("messageLeft " + messageLeft);

    bool messageSend = false;

    while(messageSend == false){   //While loop to create a message in rows
      //Serial.println("Message forloop ");
      //Serial.println("1 " + messageLeft);
      
      //Setting up the variables for the next while loop
      spaceAt = 0;
      spaceAtprevious = 0;
      rowDone = false;
      rowMessage = "";

      while(rowDone == false){  // We are gonna find the part of the message that fits on a line
     
        if (messageLeft.indexOf(" ") == 0) //if the first character is a space, make a new string after without that space
        {
          messageLeft = messageLeft.substring(1);
        }

        //Serial.println("SpaceAtprevious ");
        //Serial.println(spaceAtprevious);
        
        if (spaceAtprevious < 0){
          spaceAtprevious = 0;
        }

        if (spaceAt < 0){
          spaceAt = 0;
        }

        spaceAt = messageLeft.indexOf(" ", spaceAtprevious + 1); // Finding the position on of the next space
        //Serial.println("SpaceAt ");
        //Serial.println(spaceAt);
        if (spaceAt > -1){                                                      // A space is found.
          // rowMessage will be made from the message we have left to the found space. Then check the width of this message in pixels on the tft.
          rowMessage = messageLeft.substring(0, spaceAt);
          int textWidth = tft.textWidth(rowMessage);
          int textWidthPrevious = tft.textWidth(messageLeft.substring(0, spaceAtprevious));
          if (textWidth > screenWidth){                                         // Check if the message fits on the screen
            if((spaceAtprevious > 0) && ((textWidth - textWidthPrevious) < (screenWidth/2)) && ((screenWidth - textWidthPrevious) < (screenWidth/6))){                                            
              // If a previous space found and the distance between de spaces is smaller than a portion of the screenwidth,  then we cut off this row there. 
              rowMessage = messageLeft.substring(0, spaceAtprevious);  
              messageLeft = messageLeft.substring(spaceAtprevious + 1);
              tft.println(rowMessage);
              nextRowTFT();
              rowDone = true;
            }
            else{                                                               // If there was no previous space 
              bool parsed = false;
              int i = 1;
              while(parsed == false){
                // Checking where to parse the message if we find what part of the message including a - that still fits on the screen.
                rowMessage = messageLeft.substring(0, spaceAtprevious + i) + "-";
                if(tft.textWidth(rowMessage) > screenWidth){
                  rowMessage = messageLeft.substring(0, spaceAtprevious + i-1) + "-"; 
                  messageLeft = messageLeft.substring(spaceAtprevious + i-1);
                  Serial.println(rowMessage);
                  tft.println(rowMessage);
                  nextRowTFT();
                  parsed = true;
                  rowDone = true;
                }
                else if(i > 100){
                  tft.setTextWrap(true,false);
                  tft.println(messageLeft);
                  nextRowTFT();
                  parsed = true;
                  rowDone = true;
                  messageSend = true;
                }
                else{
                  i++;
                }
              }
            }
          }
          else{
            spaceAtprevious = spaceAt;
            spaceAt = 0;
          }
          //Serial.println(rowMessage);
          //Serial.println("textWidth");
          //Serial.println(textWidth);
        }
        else{ 
          tft.setTextWrap(true,false);
          tft.println(messageLeft);
          nextRowTFT();
          rowDone = true;
          messageSend = true;
        }
      }
    }
  }
  else{
    //Serial.println("Message not too wide");
    tft.setTextWrap(true,false);
    tft.println(message);
    nextRowTFT();
  }
}

void clearMessage(){
  screenMessage = "";
  lastUser = "";
  tft.fillScreen(backgroundColor);
}

void nextRowTFT(){
  screenRow++;
  tft.setCursor(screenEdge, (screenRow*rowHeight) + screenEdge);
}

void screenBlink(int times){
  turnBacklightOn();
  for (int i=0; i < times; i++){
    tft.fillScreen(TFT_WHITE);
    delay(200);
    tft.fillScreen(TFT_BLACK);
    delay(200);
  }
}
