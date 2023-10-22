void colorChoice(String& colorString, uint16_t& colorInt, String colorStringOld) {
  colorString.toUpperCase(); //All uppercase
  colorString.replace(" "," "); // Replace all spaces

  if (colorString == "WHITE"){
    colorInt = TFT_WHITE;  
  }
  else if(colorString == "BLACK"){
    colorInt = TFT_BLACK;  
  }
  else if(colorString == "NAVY"){
    colorInt = TFT_NAVY;    
  }
  else if(colorString ==  "DARKGREEN"){
    colorInt = TFT_DARKGREEN;    
  }
  else if(colorString ==  "DARKCYAN"){
    colorInt = TFT_DARKCYAN;    
  }
  else if(colorString ==  "MAROON"){
    colorInt = TFT_MAROON;    
  }
  else if(colorString ==  "PURPLE"){
    colorInt = TFT_PURPLE;    
  }
  else if(colorString ==  "OLIVE"){
    colorInt = TFT_OLIVE;    
  }
  else if(colorString ==  "LIGHTGREY"){
    colorInt = TFT_LIGHTGREY;    
  }
  else if(colorString ==  "DARKGREY"){
    colorInt = TFT_DARKGREY;    
  }
  else if(colorString ==  "BLUE"){
    colorInt = TFT_BLUE;    
  }
  else if(colorString ==  "GREEN"){
    colorInt = TFT_GREEN;    
  }
  else if(colorString ==  "CYAN"){
    colorInt = TFT_CYAN;    
  }
  else if(colorString ==  "RED"){
    colorInt = TFT_RED;    
  }
  else if(colorString ==  "MAGENTA"){
    colorInt = TFT_MAGENTA;    
  }
  else if(colorString ==  "YELLOW"){
    colorInt = TFT_YELLOW;    
  }
  else if(colorString ==  "ORANGE"){
    colorInt = TFT_ORANGE;    
  }
  else if(colorString ==  "GREENYELLOW"){
    colorInt = TFT_GREENYELLOW;    
  }
  else if(colorString ==  "PINK"){
    colorInt = TFT_PINK;    
  }
  else if(colorString ==  "BROWN"){
    colorInt = TFT_BROWN;    
  }
  else if(colorString ==  "GOLD"){
    colorInt = TFT_GOLD;    
  }
  else if(colorString ==  "SILVER"){
    colorInt = TFT_SILVER;    
  }
  else if(colorString ==  "SKYBLUE"){
    colorInt = TFT_SKYBLUE;    
  }
  else if(colorString ==  "VIOLET"){
    colorInt = TFT_VIOLET;
  }
  else{
    colorString = colorStringOld;
  }
  Serial.println(colorString + "Color is" + colorInt);
  
}