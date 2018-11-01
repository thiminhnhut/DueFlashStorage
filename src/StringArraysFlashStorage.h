#ifndef StringArraysFlashStorage_h
#define StringArraysFlashStorage_h

#include <Arduino.h>
#include <DueFlashStorage.h>

class StringArraysFlashStorage {
private:
    uint32_t _address;
    unsigned char _maxStorage;
    unsigned char _count;
    uint32_t _address_data;
    DueFlashStorage dueFlashStorage;

    void _resetAddressData();

public:
    enum ErrorFlashStorage {
        No_Error,
        Over_Error,
        Write_Error
    };
    StringArraysFlashStorage(uint32_t address, unsigned char maxStorage);

    void init();

    StringArraysFlashStorage::ErrorFlashStorage writeString(String data);
    void readString(String data[]);

    unsigned char getCount();
};

#endif
