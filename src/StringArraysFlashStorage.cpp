#include "StringArraysFlashStorage.h"

StringArraysFlashStorage::StringArraysFlashStorage(uint32_t address, unsigned char maxStorage) {
    _address = address;
    _address_data = address + 1;
    _maxStorage = maxStorage;
}

void StringArraysFlashStorage::init() {
    _count = StringArraysFlashStorage::dueFlashStorage.read(_address);

    if (_count == 255) {
        _count = 0;
    }

    if (_count > 0) {
        String data[_count];
        readString(data);
    };
}

StringArraysFlashStorage::ErrorFlashStorage StringArraysFlashStorage::writeString(String data) {
    if (_count < _maxStorage) {
        _count++;
        StringArraysFlashStorage::dueFlashStorage.write(_address, _count);
        if (StringArraysFlashStorage::dueFlashStorage.writeString(_address_data, data)) {
            _address_data += data.length() + 1;
            return ErrorFlashStorage::No_Error;
        } else {
            return ErrorFlashStorage::Write_Error;
        }
    } else {
        return ErrorFlashStorage::Over_Error;
    }
}


void StringArraysFlashStorage::readString(String data[]) {
    _resetAddressData();
    unsigned char k = 0;
    while (k < _count) {
        DueFlashStorage::MyString _myString = StringArraysFlashStorage::dueFlashStorage.readString(_address_data);
        _address_data += _myString.size + 1;
        data[k] = _myString.data;
        k++;
    }
}

unsigned char StringArraysFlashStorage::getCount() {
    return _count;
}

void StringArraysFlashStorage::_resetAddressData() {
    if (_address_data != _address + 1) {
        _address_data = _address + 1;
    }
}
