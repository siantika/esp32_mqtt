

/* interface C++ ke C */

#ifdef __cplusplus

extern "C" {

#endif

uint8_t temprature_sens_read();

#ifdef __cplusplus

}

#endif
/* ********* berakhir di sini ********** */

uint8_t temprature_sens_read(); // jangan diubah!


// initialisasi nama fungsi

float getInternalTemperature(void);
float getInternalVoltage(void);
float getRawDataIr(void);





void setup() {
  Serial.begin(115200);

}

void loop() {

    
//  float temp = (float)(temprature_sens_read() - 32) / 1.8;

  Serial.println(temprature_sens_read());
  delay(500);

}
