/* This example will write string to address and print them to the serial monitor.
 */
#include <Arduino.h>
#include <DueFlashStorage.h>
DueFlashStorage dueFlashStorage;

#define SerialMonitor Serial
#define __BAUDRATE__ 115200

#define __START_DATA__ 0

void setup() {
    SerialMonitor.begin(__BAUDRATE__);

    String dataStorage = "a=10.00076";
    dueFlashStorage.writeString(__START_DATA__, dataStorage);
    DueFlashStorage::MyString myString = dueFlashStorage.readString(__START_DATA__);
    SerialMonitor.println(myString.data);

    dataStorage = "b=100.005";
    dueFlashStorage.writeString(__START_DATA__ + myString.size, dataStorage);
    myString = dueFlashStorage.readString(__START_DATA__ + myString.size);
    SerialMonitor.println(myString.data);

    dataStorage = "c=using vs 2010 and even using vs 2013 same error";
    dueFlashStorage.writeString(__START_DATA__ + myString.size, dataStorage);
    myString = dueFlashStorage.readString(__START_DATA__ + myString.size);
    SerialMonitor.println(myString.data);
}

void loop() {

}
