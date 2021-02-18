#include <Wire.h>
#include <ErriezDS1302.h>
#include <string.h>

#define DS1302_CLK_PIN      15
#define DS1302_IO_PIN       17
#define DS1302_CE_PIN       16

// Create RTC object
ErriezDS1302 rtc = ErriezDS1302(DS1302_CLK_PIN, DS1302_IO_PIN, DS1302_CE_PIN);


String shedule[5] = {
        "9:00", "12:30", "16:00", "19:30",
        "23:00"
    };
String buffer_str;
#define DATE_STRING_SHORT           3

void setup()
{
    // Initialize serial port
    delay(500);
    Serial.begin(115200);
    rtc.begin();
    Serial.println(F("\n setup finish"));
}


String get_current_time(ErriezDS1302 rtc)
{
    char name[DATE_STRING_SHORT + 1];
    uint8_t hour;
    uint8_t min;
    uint8_t sec;

    // Read date/time
    if (!rtc.getDateTime(&hour, &min, &sec)) {
        Serial.println(F("Read date/time failed"));
        return "";
    }
    buffer_str = String( hour );
    buffer_str+=":";
    if (min < 10) {
         buffer_str+="0";
    }
    buffer_str+= String( min );
    delay(1000);
    return buffer_str;
}

void start_feeder()
{
  Serial.println(F("Feed \n"));
  delay(60001);
}


void loop()
{
  auto result = get_current_time(rtc);
  for (int i = 0; i < 5; i++) {
      if (shedule[i]==result)
        {
            start_feeder();
            break;
        } 
  }

   Serial.print(result.c_str());
}
