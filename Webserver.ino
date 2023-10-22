unsigned long timeNowWebserver = 0;
unsigned long timePrevWebserver = 0;
String bgColorInput;
String txtColorInput;

void WebserverClient() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (webserverActiveFirstCycle) {
    // Display a message on the screen when the web server is activated
    NewScreenMessage("Activated!", "Webserver", false);

    // Set some display-related settings
    spriteRay.setSwapBytes(true);
    spriteRay.pushSprite((screenWidth - 128), (screenHeight - (128 + rowHeight)), TFT_BLACK);

    // Print the local IP address on the TFT display
    tft.setCursor(screenEdge, screenHeight - rowHeight - screenEdge);
    tft.println(WiFi.localIP());

    // Update the flag to indicate that the web server is active
    webserverActiveFirstCycle = false;
  }

  if (client) {
    //Serial.println("Client is connected");

    // Read the first line of the HTTP request
    String request = client.readStringUntil('\r');

    if(!(request == "")){
      Serial.print(F("request = "));   
      Serial.println(request);

      int backgroundColorIndex = 0;
      int textColorIndex = 0;
      int endIndex = 0;

      backgroundColorIndex = request.indexOf("bgColorInput=");
      if(backgroundColorIndex > -1){
        backgroundColorIndex = backgroundColorIndex + 13; // Plus the length of bgColorInput= because it should not be included
        endIndex = request.indexOf("&", backgroundColorIndex); 
      
        backgroundColorString = request.substring(backgroundColorIndex, endIndex);
        Serial.println(backgroundColorString);
      }
      
      textColorIndex = request.indexOf("txtColorInput=");
      if(textColorIndex > -1){
        textColorIndex = textColorIndex + 14; // Plus the length of txtColorInput= because it should not be included
        endIndex = request.indexOf("&", textColorIndex); 
      
        textColorString = request.substring(textColorIndex, endIndex);
        Serial.println(textColorString);
      }
    }    

    // Send an HTTP response with a dropdown menu for both background and text color
    client.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
    client.print("<!DOCTYPE html><html><head><title>Settings</title></head><body><div id='main'><h2>CommuniRaytor Settings</h2>");
    
    // Dropdown menu for background color
    client.print("<p>Background Color: ");
    client.print("<select id='bgColorSelect' name='bgColorSelect'>");
    client.print(PrintColorSelection("BLACK", backgroundColorString));
    client.print(PrintColorSelection("WHITE", backgroundColorString));
    client.print(PrintColorSelection("LIGHTGREY", backgroundColorString));
    client.print(PrintColorSelection("SILVER", backgroundColorString));  
    client.print(PrintColorSelection("DARKGREY", backgroundColorString));  
    client.print(PrintColorSelection("RED", backgroundColorString));  
    client.print(PrintColorSelection("ORANGE", backgroundColorString));  
    client.print(PrintColorSelection("YELLOW", backgroundColorString));  
    client.print(PrintColorSelection("GREEN", backgroundColorString));  
    client.print(PrintColorSelection("CYAN", backgroundColorString));  
    client.print(PrintColorSelection("BLUE", backgroundColorString));  
    client.print(PrintColorSelection("MAGENTA", backgroundColorString));  
    client.print(PrintColorSelection("PURPLE", backgroundColorString));  
    client.print(PrintColorSelection("BROWN", backgroundColorString));  
    client.print(PrintColorSelection("GOLD", backgroundColorString));  
    client.print(PrintColorSelection("NAVY", backgroundColorString));
    client.print(PrintColorSelection("DARKGREEN", backgroundColorString));
    client.print(PrintColorSelection("DARKCYAN", backgroundColorString));
    client.print(PrintColorSelection("MAROON", backgroundColorString));
    client.print(PrintColorSelection("OLIVE", backgroundColorString));
    client.print(PrintColorSelection("GREENYELLOW", backgroundColorString));
    client.print(PrintColorSelection("SKYBLUE", backgroundColorString));
    client.print(PrintColorSelection("VIOLET", backgroundColorString));
    client.print("</select>");
    client.print("</p>");

    // Dropdown menu for text color
    client.print("<p>Text Color: ");
    client.print("<select id='txtColorSelect' name='txtColorSelect'>");
    client.print(PrintColorSelection("BLACK", textColorString));
    client.print(PrintColorSelection("WHITE", textColorString));
    client.print(PrintColorSelection("LIGHTGREY", textColorString));
    client.print(PrintColorSelection("SILVER", textColorString));  
    client.print(PrintColorSelection("DARKGREY", textColorString));  
    client.print(PrintColorSelection("RED", textColorString));  
    client.print(PrintColorSelection("ORANGE", textColorString));  
    client.print(PrintColorSelection("YELLOW", textColorString));  
    client.print(PrintColorSelection("GREEN", textColorString));  
    client.print(PrintColorSelection("CYAN", textColorString));  
    client.print(PrintColorSelection("BLUE", textColorString));  
    client.print(PrintColorSelection("MAGENTA", textColorString));  
    client.print(PrintColorSelection("PURPLE", textColorString));  
    client.print(PrintColorSelection("BROWN", textColorString));  
    client.print(PrintColorSelection("GOLD", textColorString));  
    client.print(PrintColorSelection("NAVY", textColorString));
    client.print(PrintColorSelection("DARKGREEN", textColorString));
    client.print(PrintColorSelection("DARKCYAN", textColorString));
    client.print(PrintColorSelection("MAROON", textColorString));
    client.print(PrintColorSelection("OLIVE", textColorString));
    client.print(PrintColorSelection("GREENYELLOW", textColorString));
    client.print(PrintColorSelection("SKYBLUE", textColorString));
    client.print(PrintColorSelection("VIOLET", textColorString));
    client.print("</select>");
    client.print("</p>");

    // Accept button to save the selected color options
    client.print("<button onclick='updateColors()'>Accept</button>");

    client.print("</div></body></html>");

    // JavaScript to save the selected color values to global strings
    client.print("<script>");
    client.print("function updateColors() {");
    client.print("  var selectedBgColor = document.getElementById('bgColorSelect').value;");
    client.print("  var selectedTxtColor = document.getElementById('txtColorSelect').value;");
    client.print("  var url = '/set_colors?bgColorInput=' + encodeURIComponent(selectedBgColor) + '&txtColorInput=' + encodeURIComponent(selectedTxtColor) + '&';");
    client.print("  var xhr = new XMLHttpRequest();");
    client.print("  xhr.open('GET', url, true);");
    client.print("  xhr.onload = function() {");
    client.print("    if (xhr.status === 200) {");
    client.print("      alert('Colors updated successfully.');");
    client.print("    } else {");
    client.print("      alert('Failed to update colors.');");
    client.print("    }");
    client.print("  };");
    client.print("  xhr.onerror = function() {");
    client.print("    alert('Error: Fetch failed.');");
    client.print("  };");
    client.print("  xhr.send();");
    client.print("  setTimeout(function() {");
    client.print("    location.reload();"); // Reload the page
    client.print("  }, 1000);"); // 2000 milliseconds (1 second) delay
    client.print("}");
    client.print("</script>");

    // Flush any remaining data from the client
    client.flush();
    client.stop();

    // Update the timePrevWebserver variable to track the time of the last client connection
    timePrevWebserver = millis();
  }

  // Check if a certain time interval has passed since the last client connection
  timeNowWebserver = millis();
  if (timeNowWebserver - timePrevWebserver > 300000) { //after 5 minutes
    // Perform some action when a certain time interval has passed (e.g., print "Looping")
    timePrevWebserver = millis();
    Serial.println("Looping");
    startWebClient = false;

    // Display a message on the screen when the web server is activated
    NewScreenMessage("Deactivated.", "Webserver", false);
  }
}

String PrintColorSelection(String color, String currentColor){
  String printString;
  if (color == currentColor){
    printString = ("      <option value='" + color + "' selected>" + color + "</option>");
  }
  else{
    printString = ("      <option value='" + color + "'>" + color + "</option>");
  } 
  return printString;
}

