#include <ESP8266WiFi.h>
#include <TinyGPS++.h>
#include <SPI.h>
#include <SD.h>
#include <SoftwareSerial.h>

//extern "C" {
//  #include <user_interface.h>
//}

#define IO0 0
#define IO2 2
#define IO16 16

static const int RXPin = 4, TXPin = 5;
static const uint32_t GPSBaud = 9600;
const uint8_t cs_SD = 15;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

// The TinyGPS++ object
// http://arduiniana.org/libraries/tinygpsplus/
TinyGPSPlus gps;

// 
File myFile;
static char fileNameText[13] = "log_xxxx.txt";
String nmea_string;



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 初期化関数
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup()
{
	// ポート設定
	// IO0  - Connected SW1
	pinMode(IO0, INPUT);
	// IO2  - Pull Up
	pinMode(IO2, INPUT);
	// IO4  - Connected GPS TXD
	// IO5  - Connected GPS RXD
	// IO12 - Connected SD MISO
	// IO13 - Connected SD MOSI
	// IO14 - Connected SD SCK
	// IO15 - Connected SD CS
	// IO16 - Nan
	pinMode(IO16, OUTPUT);
	
	// WIFIモード/SLEEPモード設定
	WiFi.mode(WIFI_STA);
	wifi_set_sleep_type(LIGHT_SLEEP_T);
	
	// UART設定
	Serial.begin(19200);
	ss.begin(GPSBaud);
	delay(2);
	
	// GPS設定
	
	
	// SDカード設定
	
	// SDカードの挿入確認   
	Serial.print(analogRead(A0));
	if (analogRead(A0) < 512) {
		Serial.println(" : SDcard - exist.");
	} else {
		Serial.println(" : SDcard - none.");
	}
	
	// SDカード初期処理
	Serial.print("Initializing SD card...");
	if (!SD.begin(cs_SD)) {
		Serial.println("initialization failed!");
		return;
	}
	else {
		// Settings for name of thelog file.
		// Get the list of files. "log_xxxx.txt"
	}
	Serial.println("initialization done.");
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// メインループ
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop()
{
	
	while(1)
	{
		delay(100);
	}
}