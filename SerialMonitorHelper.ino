void SerialMonitorHelper_Print(char* text) {
  if(mSerialMonitor) {
    Serial.println(text);
  }
}
