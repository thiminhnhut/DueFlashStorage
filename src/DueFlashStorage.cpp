#include "DueFlashStorage.h"

DueFlashStorage::DueFlashStorage() {
  uint32_t retCode;

  /* Initialize flash: 6 wait states for flash writing. */
  retCode = flash_init(FLASH_ACCESS_MODE_128, 6);
  if (retCode != FLASH_RC_OK) {
    _FLASH_DEBUG("Flash init failed\n");
  }
}

byte DueFlashStorage::read(uint32_t address) {
  return FLASH_START[address];
}
byte* DueFlashStorage::readAddress(uint32_t address) {
  return FLASH_START+address;
}

boolean DueFlashStorage::write(uint32_t address, byte value) {
  uint32_t retCode;
  uint32_t byteLength = 1;
  byte *data;

  retCode = flash_unlock((uint32_t)FLASH_START+address, (uint32_t)FLASH_START+address + byteLength - 1, 0, 0);
  if (retCode != FLASH_RC_OK) {
    _FLASH_DEBUG("Failed to unlock flash for write\n");
    return false;
  }

  // write data
  retCode = flash_write((uint32_t)FLASH_START+address, &value, byteLength, 1);
  //retCode = flash_write((uint32_t)FLASH_START, data, byteLength, 1);

  if (retCode != FLASH_RC_OK) {
    _FLASH_DEBUG("Flash write failed\n");
    return false;
  }

  // Lock page
  retCode = flash_lock((uint32_t)FLASH_START+address, (uint32_t)FLASH_START+address + byteLength - 1, 0, 0);
  if (retCode != FLASH_RC_OK) {
    _FLASH_DEBUG("Failed to lock flash page\n");
    return false;
  }
  return true;
}

boolean DueFlashStorage::write(uint32_t address, byte *data, uint32_t dataLength) {
  uint32_t retCode;

  if ((uint32_t)FLASH_START+address < IFLASH1_ADDR) {
    _FLASH_DEBUG("Flash write address too low\n");
    return false;
  }

  if ((uint32_t)FLASH_START+address >= (IFLASH1_ADDR + IFLASH1_SIZE)) {
    _FLASH_DEBUG("Flash write address too high\n");
    return false;
  }

  if (((uint32_t)FLASH_START+address & 3) != 0) {
    _FLASH_DEBUG("Flash start address must be on four byte boundary\n");
    return false;
  }

  // Unlock page
  retCode = flash_unlock((uint32_t)FLASH_START+address, (uint32_t)FLASH_START+address + dataLength - 1, 0, 0);
  if (retCode != FLASH_RC_OK) {
    _FLASH_DEBUG("Failed to unlock flash for write\n");
    return false;
  }

  // write data
  retCode = flash_write((uint32_t)FLASH_START+address, data, dataLength, 1);

  if (retCode != FLASH_RC_OK) {
    _FLASH_DEBUG("Flash write failed\n");
    return false;
  }

  // Lock page
    retCode = flash_lock((uint32_t)FLASH_START+address, (uint32_t)FLASH_START+address + dataLength - 1, 0, 0);
  if (retCode != FLASH_RC_OK) {
    _FLASH_DEBUG("Failed to lock flash page\n");
    return false;
  }
  return true;
}

boolean DueFlashStorage::write_unlocked(uint32_t address, byte value) {
  uint32_t retCode;
  uint32_t byteLength = 1;
  byte *data;

  // write data
  retCode = flash_write((uint32_t)FLASH_START+address, &value, byteLength, 1);
  //retCode = flash_write((uint32_t)FLASH_START, data, byteLength, 1);

  if (retCode != FLASH_RC_OK) {
    _FLASH_DEBUG("Flash write failed\n");
    return false;
  }

  return true;
}

boolean DueFlashStorage::write_unlocked(uint32_t address, byte *data, uint32_t dataLength) {
  uint32_t retCode;

  if ((uint32_t)FLASH_START+address < IFLASH1_ADDR) {
    _FLASH_DEBUG("Flash write address too low\n");
    return false;
  }

  if ((uint32_t)FLASH_START+address >= (IFLASH1_ADDR + IFLASH1_SIZE)) {
    _FLASH_DEBUG("Flash write address too high\n");
    return false;
  }

  if (((uint32_t)FLASH_START+address & 3) != 0) {
    _FLASH_DEBUG("Flash start address must be on four byte boundary\n");
    return false;
  }

  // write data
  retCode = flash_write((uint32_t)FLASH_START+address, data, dataLength, 1);

  if (retCode != FLASH_RC_OK) {
    _FLASH_DEBUG("Flash write failed\n");
    return false;
  }

  return true;
}

DueFlashStorage::MyString DueFlashStorage::readString(uint32_t address) {
  DueFlashStorage::MyString myString;
  myString.size = read(address);
  myString.data = "";

  for (unsigned int i = 1; i <= myString.size; i++) {
    myString.data += char(read(address + i));
  }

  return myString;
}

bool DueFlashStorage::writeString(uint32_t address, String data) {
  if ((uint32_t)FLASH_START+address < IFLASH1_ADDR) {
    _FLASH_DEBUG("Flash write address too low\n");
    return false;
  }

  if ((uint32_t)FLASH_START+address >= (IFLASH1_ADDR + IFLASH1_SIZE)) {
    _FLASH_DEBUG("Flash write address too high\n");
    return false;
  }

  bool status = write(address, data.length());
  if (!status) {
    _FLASH_DEBUG("Flash write failed\n");
    return false;
  }

  for (unsigned int i = 1; i <= data.length(); i++) {
    bool status = write(address + i, (char) data[i - 1]);
    if (!status) {
      _FLASH_DEBUG("Flash write failed\n");
      return false;
    }
  }

  return true;
}
