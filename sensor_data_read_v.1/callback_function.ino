//-----------------------Callback function-------------------------------------//

void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  
//-------------------------------------Getting config data---------------------//

if(strcmp(topic, "digibox/reset/") == 0){


  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    char data=payload[i];
    
  
      if (data=='1')
    {
      Serial.println("Reset Board");
      int result = client.publish("digibox/reset/ack/","Restarted");
      if (result == 1){
        Serial.println("Reset Ack sent successfully!");
        }else{
      Serial.println("Reset Ack failed to sent!");}
      delay(1500);
      ESP.reset();
      
     }
     }
     }

  if(strcmp(topic, "digibox/sleep_time/") == 0){

  memset(sleep_time,0, sizeof(sleep_time));// Emptying the char array
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    sleep_time[i] = payload[i];
    Serial.print("Received Sleeptime:");
    Serial.println(sleep_time);
    
    
      
     }
     }
     
     }
