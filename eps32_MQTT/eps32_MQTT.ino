
/* ********** pin ********** */
#define PIN_FLAME_SENSOR 2  // analog pin 
#define PIN_POWER_IN_VOLTAGE 15
#define V_REF 3.3
#define MAX_ADC_RES 4096


/* interface C++ ke C */
#ifdef __cplusplus

extern "C" {

#endif

uint8_t temprature_sens_read();

#ifdef __cplusplus

}

#endif
/* ********** berakhir di sini ********** */


uint8_t temprature_sens_read(); // jangan diubah!


/* ********** inisialisasi nama fungsi ********** */
float getInternalTemperature(void);
float getInternalVoltage(void);
uint16_t getRawDataFlameSensor(void);


/* ********** Driver Code ********** */
void setup() {
  Serial.begin(115200);


}

void loop() {
  

}

/* ********** Driver Code  Berakhir di sini ********** */


/* Fungsi - Fungsi */

/*  Nama        : getInternalTemperature
    Deskripsi   : Membaca nilai temperature internal dari ESP32. Harus mendeklarasikan interface C ke C++ guard di atas
                  dan mendeklarasikan fungsi temperature_sens_read() dengan tipe data uint8_t
    Param       : void
    Return value: (float) nilai temperatur dalam celcius
*/
float getInternalTemperature()
{
  return (float) (temprature_sens_read() - 32) / 1.8;
}


/*  Nama        : getRawDataFlameSensor
    Deskripsi   : Mendapatkan nilai mentah dari flame sensor. Jika sensor mendeteksi api, maka nilai yang didapat akan menurun dan sebaliknya.
                  Rentang nilai (0-4095)
    Param       : void
    Return value: (uint16_t) nilai mentah sensor (ADC)
*/
uint16_t getRawDataFlameSensor()
{
  return analogRead(PIN_FLAME_SENSOR);
}

/*  Nama        : getInternalVoltage
    Deskripsi   : mendapatkan nilai tegangan dalam satuan volt dari sumber daya ESP32
    Param       : void
    Return value: (float) nilai tegangan dalam volt
*/
float getInternalVoltage()
{
  int _rawDataAdc = 0;
  float _powerInVoltage = 0;
  
  _rawDataAdc = analogRead(PIN_POWER_IN_VOLTAGE);
  _powerInVoltage = _rawDataAdc * V_REF / MAX_ADC_RES;

  return _powerInVoltage;

}
