#ifndef OS_IDE_H
#define OS_IDE_H

#include <stdint.h>

//status
#define ATA_SR_BSY     0x80    // Busy
#define ATA_SR_DRDY    0x40    // Drive ready
#define ATA_SR_DF      0x20    // Drive write fault
#define ATA_SR_DSC     0x10    // Drive seek complete
#define ATA_SR_DRQ     0x08    // Data request ready
#define ATA_SR_CORR    0x04    // Corrected data
#define ATA_SR_IDX     0x02    // Index
#define ATA_SR_ERR     0x01    // Error

//errors
#define ATA_ER_BBK      0x80    // Bad block
#define ATA_ER_UNC      0x40    // Uncorrectable data
#define ATA_ER_MC       0x20    // Media changed
#define ATA_ER_IDNF     0x10    // ID mark not found
#define ATA_ER_MCR      0x08    // Media change request
#define ATA_ER_ABRT     0x04    // Command aborted
#define ATA_ER_TK0NF    0x02    // Track 0 not found
#define ATA_ER_AMNF     0x01    // No address mark

//commands
#define ATA_CMD_READ_PIO          0x20
#define ATA_CMD_READ_PIO_EXT      0x24
#define ATA_CMD_READ_DMA          0xC8
#define ATA_CMD_READ_DMA_EXT      0x25
#define ATA_CMD_WRITE_PIO         0x30
#define ATA_CMD_WRITE_PIO_EXT     0x34
#define ATA_CMD_WRITE_DMA         0xCA
#define ATA_CMD_WRITE_DMA_EXT     0x35
#define ATA_CMD_CACHE_FLUSH       0xE7
#define ATA_CMD_CACHE_FLUSH_EXT   0xEA
#define ATA_CMD_PACKET            0xA0
#define ATA_CMD_IDENTIFY_PACKET   0xA1
#define ATA_CMD_IDENTIFY          0xEC

//identification space
#define ATA_IDENT_DEVICE_TYPE  0
#define ATA_IDENT_CYLINDERS    2
#define ATA_IDENT_HEADS        6
#define ATA_IDENT_SECTORS      12
#define ATA_IDENT_SERIAL       20
#define ATA_IDENT_MODEL        54
#define ATA_IDENT_CAPABILITIES 98
#define ATA_IDENT_FIELD_VALID  106
#define ATA_IDENT_MAX_LBA      120
#define ATA_IDENT_COMMAND_SETS 164
#define ATA_IDENT_MAX_LBA_EXT  200

#define IDE_ATA        0x00
#define IDE_ATAPI      0x01

#define ATA_CONTROLLER 0x00
#define ATA_PERIPHERAL 0x01

//registers
#define ATA_REG_ALTSTATUS  0x00
#define ATA_REG_CONTROL    0x00
#define ATA_REG_DATA       0x00
#define ATA_REG_ERROR      0x01
#define ATA_REG_FEATURES   0x01
#define ATA_REG_SECCOUNT0  0x02
#define ATA_REG_LBA0       0x03
#define ATA_REG_LBA1       0x04
#define ATA_REG_LBA2       0x05
#define ATA_REG_HDDEVSEL   0x06
#define ATA_REG_COMMAND    0x07
#define ATA_REG_STATUS     0x07
#define ATA_REG_SECCOUNT1  0x08
#define ATA_REG_LBA3       0x09
#define ATA_REG_LBA4       0x0A
#define ATA_REG_LBA5       0x0B
#define ATA_REG_DEVADDRESS 0x0D

// Channels:
#define      ATA_PRIMARY      0x00
#define      ATA_SECONDARY    0x01

typedef struct {
    uint16_t base;
    uint16_t controlBase;
    uint16_t busController;
    uint16_t noInterrupt;
} IdeChannelRegisters;

typedef struct {
    uint8_t exists;
    uint8_t channel;
    uint8_t drive;
    uint16_t type;
    uint16_t signature;
    uint16_t capabilities;
    uint32_t commandSets;
    uint32_t size;
    char model[41];
} IdeDevice;

typedef struct {
    IdeChannelRegisters channels[2];
    IdeDevice devices[4];
} Ide;

extern Ide ide;

void initializeIde(uint32_t bar0, uint32_t bar1,
                         uint32_t bar2, uint32_t bar3, uint32_t bar4);
void initializeIdeCompatibility();
uint8_t ideRead(uint8_t channel, uint8_t reg);
uint8_t ideReadControl(uint8_t channel, uint8_t reg);
void ideWrite(uint8_t channel, uint8_t reg, uint8_t value);
void ideWriteControl(uint8_t channel, uint8_t reg, uint8_t value);
int ataWaitUntilReady(int channel);
void ataIoWait(int channel);

/**
 * Read data from an ATA device.
 *
 * @param device
 * @param blockAddress the LBA to read from. Each LBA addresses 512 bytes
 * @param buffer the buffer to write data to
 * @param length the number of bytes to read
 */
void ataReadSectors(IdeDevice* device, uint32_t blockAddress, uint8_t* buffer, int length);

/**
 * Write data to an ATA drive
 *
 * @param device
 * @param blockAddress the LBA to write to. Each LBA addresses 512 bytes
 * @param buffer the buffer to write from
 * @param length the number of bytes to write
 */
void ataWriteSectors(IdeDevice* device, uint32_t blockAddress, uint8_t* buffer, int length);

#endif //OS_IDE_H
