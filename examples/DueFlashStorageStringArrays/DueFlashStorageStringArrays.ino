/* This example will write string arrays to address and print them to the serial monitor.
 */
#include <Arduino.h>
#include <StringArraysFlashStorage.h>

#define SerialMonitor Serial
#define __BAUDRATE__ 115200

#define __START_DATA__ 0

StringArraysFlashStorage stringArraysFlashStorage(__START_DATA__, 5);

void setup() {
    SerialMonitor.begin(__BAUDRATE__);
    delay(5000);
    SerialMonitor.println(F("Start!"));

    stringArraysFlashStorage.init();

    for (unsigned char i = 0; i < 2; i++) {
        String data = "There is " + String(i);
        StringArraysFlashStorage::ErrorFlashStorage error = stringArraysFlashStorage.writeString(data);
        if (error == StringArraysFlashStorage::No_Error) {
            SerialMonitor.println(F("Write Success!"));
        } else if (error == StringArraysFlashStorage::Write_Error){
            SerialMonitor.println(F("Write Fail!"));
        } else if (error == StringArraysFlashStorage::Over_Error) {
            SerialMonitor.println(F("Over Write!"));
        }
    }

    unsigned char count = stringArraysFlashStorage.getCount();
    SerialMonitor.print(F("Count = "));
    SerialMonitor.println(count);
    if (count > 0) {
        String data[count];
        stringArraysFlashStorage.readString(data);
        for (unsigned int i = 0; i < count; i++) {
            SerialMonitor.println(data[i]);
        }
    }
    SerialMonitor.println(F("Finish!"));
}

void loop() {

}
