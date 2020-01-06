#include <ESP8266WiFi.h>
#include <TinyGPS++.h>
#include <SPI.h>
#include <SD.h>
#include <SoftwareSerial.h>

extern "C" {
  #include <user_interface.h>
}

#define IO0 0
#define IO2 2

static const int RXPin = 4, TXPin = 5;
static const uint32_t GPSBaud = 9600;
const uint8_t cs_SD = 15;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

// The TinyGPS++ object
// http://arduiniana.org/libraries/tinygpsplus/
TinyGPSPlus gps;

// 
//File myFile;
static char fileNameText[13] = "log_xxxx.txt";
String nmea_string;


#define ESQ_MGPS_WAIT_START 0
#define ESQ_MGPS_WAIT_CRLF  1
#define ESQ_MGPS_ENCODE     2



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 初期化関数
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup()
{
	// ポート設定
	// IO0  - Connected SW1
	pinMode(IO0, INPUT);
	// IO2  - Pull Up
	pinMode(IO2, OUTPUT);
	// IO4  - Connected GPS TXD
	// IO5  - Connected GPS RXD
	// IO12 - Connected SD MISO
	// IO13 - Connected SD MOSI
	// IO14 - Connected SD SCK
	// IO15 - Connected SD CS
	// IO16 - Nan
	
	// WIFIモード/SLEEPモード設定
	WiFi.mode(WIFI_STA);
	wifi_set_sleep_type(LIGHT_SLEEP_T);
	
	// UART設定
	Serial.begin(74880);
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
	
	// GPS タスク
	m_gps();
	
	if(gps.location.isValid())
	{
		displayInfo();
	}
	
	// SLEEP タスク
	m_sleep();
}

void m_gps()
{
	
	while(!gps.location.isUpdated())
	{
		// Software Serialのバッファサイズは64byte。
		// 9600bpsでGPSとは通信しているため、1バイト=約1ms
		// そのため、およそバッファサイズが満タンになる50ms弱 Sleepすることで、消費電力を抑える。
		// 心配なのは、Sleep中にSoftwareSerialがちゃんと受信できるかどうかなのだが・・・
		delay(50);
		
		if(ss.available() > 0)
		{
			Serial.print(F("Debug: ss.available() = "));
			Serial.println(ss.available());
		}
		
		while(ss.available() > 0)
		{
			gps.encode(ss.read());
		}
		
	}
}


void displayInfo()
{
	Serial.print(F("Location: ")); 
	if (gps.location.isValid()) {
		Serial.print(gps.location.lat(), 6);
		Serial.print(F(","));
		Serial.print(gps.location.lng(), 6);
	} else {
		Serial.print(F("INVALID"));
	}

	Serial.print(F("	Date/Time: "));
	if (gps.date.isValid()) {
		Serial.print(gps.date.year());
		Serial.print(F("-"));
		Serial.print(gps.date.month());
		Serial.print(F("-"));
		Serial.print(gps.date.day());
	} else {
		Serial.print(F("INVALID"));
	}

	Serial.print(F(" "));
	if (gps.time.isValid()) {
		if (gps.time.hour() < 10) Serial.print(F("0"));
		Serial.print(gps.time.hour());
		Serial.print(F(":"));
		if (gps.time.minute() < 10) Serial.print(F("0"));
		Serial.print(gps.time.minute());
		Serial.print(F(":"));
		if (gps.time.second() < 10) Serial.print(F("0"));
		Serial.print(gps.time.second());
		Serial.print(F("."));
		if (gps.time.centisecond() < 10) Serial.print(F("0"));
		Serial.print(gps.time.centisecond());
	} else {
		Serial.print(F("INVALID"));
	}
	
	Serial.print(F(" "));
	if (gps.speed.isValid()) {
		Serial.print(F("Speed:"));
		Serial.print(gps.speed.kmph());
		Serial.print(F("km/h"));
	} else {
		Serial.print(F("INVALID"));
	}
	
	Serial.print(F(" "));
	if (gps.altitude.isValid()) {
		Serial.print(F("Alt:"));
		Serial.print(gps.altitude.meters());
		Serial.print(F("m"));
	} else {
		Serial.print(F("INVALID"));
	}
	
	Serial.print(F(" "));
	if (gps.satellites.isValid()) {
		Serial.print(F("Sat:"));
		Serial.print(gps.satellites.value());
	} else {
		Serial.print(F("INVALID"));
	}
	
	Serial.print(F(" "));
	if (gps.hdop.isValid()) {
		Serial.print(F("Hdop:"));
		Serial.print(gps.hdop.value());
	} else {
		Serial.print(F("INVALID"));
	}
	Serial.println();
}

void m_sleep()
{
	delay(1000);
}