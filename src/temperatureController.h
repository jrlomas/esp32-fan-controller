void initTemperatureController(void);
void setFanPWMbasedOnTemperature(void);
float getTargetTemperature(void);
float getActualTemperature(void);
void setActualTemperatureAndPublishMQTT(double aActualTemperature);
void updatePWM_MQTT_Screen_withNewTargetTemperature(float aTargetTemperature, bool force);
void updatePWM_MQTT_Screen_withNewActualTemperature(float aActualTemperature, bool force);
