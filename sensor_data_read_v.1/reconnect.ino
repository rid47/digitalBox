//------------------------------------------------While client not conncected---------------------------------//

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    //if your MQTT broker has clientID,username and password
    //please change following line to if (client.connect(clientId,userName,passWord))

    
    if (client.connect(clientId.c_str()))
    {
      Serial.println("connected");
     //once connected to MQTT broker, subscribe command if any
     //----------------------Subscribing to required topics-----------------------//
      client.subscribe("digibox/reset/");
      client.subscribe("digibox/sleep_time/");
      client.publish("digibox/status/","awake");
                   
    }else {

     
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again");
      mqttTryCounter++;
      Serial.println(mqttTryCounter);
    
    }


    if(mqttTryCounter==5){
      
      Serial.println(mqttTryCounter);
      mqttTryCounter=0;
      break;
    }
  }
}


