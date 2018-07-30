int get_wakeup_reason()
{
  //Why did the chip wake up
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();
  Serial.print("wakeup_reason "); Serial.println(wakeup_reason);
  return wakeup_reason;
}

void putMeToSleep()
{
  Serial.println("Going to sleep");
  delay(100);
  esp_deep_sleep_start();
}
