int temp() {
  Serial.println("Inside temp()");
  for (int i = 1; i <= 10; i++) {
    Temperature = Temperature + myHDC1080.readTemperature();
    delay(100);
  }
  Temperature = ((Temperature / 10) - 3);
  return Temperature ;
}

int hum() {
   Serial.println("Inside hum()");
  for (int i = 1; i <= 10; i++) {
    Humidity = Humidity + myHDC1080.readHumidity();
    delay(100);
  }
  Humidity = (Humidity / 10);
  return Humidity;
}

int co2() {

  Serial.println("Inside co2()");
  for (int i = 1; i <= 10; i++) {
    carbon_dioxide = carbon_dioxide + myCCS811.getCO2();
    

  }
  carbon_dioxide = carbon_dioxide / 10;
  return carbon_dioxide;
}

int tvoc() {
  Serial.println("Inside Tvoc");
  for (int i = 1; i <= 10; i++) {
    
    total_voc = total_voc + myCCS811.getTVOC();
    delay(100);
  }
  total_voc = total_voc / 10;
  return total_voc;
}

int tgsVal() {
  Serial.println("Inside tgsVal()");
  delay(5000);      //.........5sec delay..........//
  for (int i = 1; i <= 10; i++) {
    int raw = analogRead(analogPin);
    methane = pow(10, .00390625 * (raw - 0));
    delay(100);
  }
  methane = methane / 10;
  return methane;
}

