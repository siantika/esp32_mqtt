#include <WiFi.h>
#include <PubSubClient.h>

/* const */
#define V_REF 3.3
#define MAX_ADC_RES 4096

const char* SSID_AP = "Redmi 9";
const char* PASSWORD = "qwerty123";
const char* MQTT_SERVER = "test.mosquitto.org";
const int PORT = 1883;
char* TOPIC_FLAME_SENSOR = "sian/interview/flame_sensor";
char* TOPIC_POWER_VOLTAGE = "sian/interview/power_voltage";
char* TOPIC_INTERNAL_TEMP = "sian/interview/internal_temp";


/* ********** pin ********** */
#define PIN_FLAME_SENSOR 35  // analog pin 
#define PIN_POWER_IN_VOLTAGE 34 // analog pin



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
char dataPowerVoltage[10];
char dataInternalTemp[10];
char dataFlameSensor[10];

float getDataPowerVoltage, getDataInternalTemp;
int getFlameSensor;


/* ********** inisialisasi nama fungsi ********** */
float getInternalTemperature(void);
float getInternalVoltage(void);
int getRawDataFlameSensor(void);
void conWifi(void);
void publishData(char* topicPub, char* dataSensor);

WiFiClient espClient;
PubSubClient client(espClient);

/* ********** Driver Code ********** */
void setup() {
  Serial.begin(115200);
  conWifi();
  client.setServer(MQTT_SERVER, PORT);


}

void loop() {
  
  getFlameSensor = getRawDataFlameSensor();
  getDataInternalTemp = getInternalTemperature();
  getDataPowerVoltage = getInternalVoltage();

  sprintf(dataFlameSensor, "%d", getFlameSensor);
  dtostrf(getDataInternalTemp, 4, 2, dataInternalTemp);
  dtostrf(getDataPowerVoltage, 4, 2, dataPowerVoltage);
  
  publishData(TOPIC_FLAME_SENSOR, dataFlameSensor);
  publishData(TOPIC_INTERNAL_TEMP, dataInternalTemp);
  publishData(TOPIC_POWER_VOLTAGE, dataPowerVoltage);

}

/* ********** Driver Code  Berakhir di sini ********** */


/* ********** Fungsi - Fungsi ********** */

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
    Return value: (int) nilai mentah sensor (ADC)
*/
int getRawDataFlameSensor()
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

/*  Nama        : conWifi
    Deskripsi   : menghubungkan ESP32 ke WiFi. Perlu SSID_AP dan PASSWORD WiFi. Tidak ada timeout jadi selama wifi belum terkoneksi, maka akan
                  terus mencoba terkoneksi.
    Param       : void
    Return value: -
*/
void conWifi()
{

  WiFi.begin(SSID_AP, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

/*  Nama        : publishData
    Deskripsi   : publish data dengan mqtt ke server online sekali. 
    Param       : (char*) topicPub: topik mqtt ; (char*) dataSensor: data sensor yang hendak dikirim 
    Return value: -
*/
void publishData(char* topicPub, char* dataSensor)
{
  
  if (!client.connected()) {
    Serial.println("Connecting to MQTT broker...");
    if (client.connect("espClient")) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.print("Failed to connect to MQTT broker, rc=");
      Serial.println(client.state());
      delay(1000);
      return;
    }
  }

  client.publish(topicPub, dataSensor);
}
