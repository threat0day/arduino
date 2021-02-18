

#include <Wire.h>
#include <ErriezDS1302.h>

// Connect DS1302 data pin to Arduino DIGITAL pin

#define DS1302_CLK_PIN      15
#define DS1302_IO_PIN       17
#define DS1302_CE_PIN       16

// Create RTC object
ErriezDS1302 rtc = ErriezDS1302(DS1302_CLK_PIN, DS1302_IO_PIN, DS1302_CE_PIN);

#define DATE_STRING_SHORT           3

// Month names in flash
const char monthNames_P[] PROGMEM = "JanFebMarAprMayJunJulAugSepOctNovDec";

// Day of the week names in flash
const char dayNames_P[] PROGMEM= "SunMonTueWedThuFriSat";


void setup()
{
    // Initialize serial port
    delay(500);
    Serial.begin(115200);
    Serial.println(F("\n1111"));
    while (!Serial) {
        ;
    }
    Serial.println(F("\n2222"));

    // Initialize I2C
    Wire.begin();
    Wire.setClock(100000);

    // Initialize RTC
    while (!rtc.begin()) {
        Serial.println(F("RTC not found"));
        delay(3000);
    }

    // Set date/time: 12:34:56 31 December 2020 Sunday
    if (!rtc.setDateTime(1, 17, 00,  19, 2, 2021, 5)) {
        Serial.println(F("Set date/time failed"));
    }
    Serial.println(F("\n3333"));
}

void loop()
{
    char name[DATE_STRING_SHORT + 1];
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
    uint8_t mday;
    uint8_t mon;
    uint16_t year;
    uint8_t wday;

    // Read date/time
    if (!rtc.getDateTime(&hour, &min, &sec, &mday, &mon, &year, &wday)) {
        Serial.println(F("Read date/time failed"));
        return;
    }

    // Print time
    Serial.print(hour);
    Serial.print(F(":"));
    if (min < 10) {
        Serial.print(F("0"));
    }
    Serial.print(min);
    Serial.print(F(":"));
    if (sec < 10) {
        Serial.print(F("0"));
    }
    Serial.print(sec);

    delay(1000);
}