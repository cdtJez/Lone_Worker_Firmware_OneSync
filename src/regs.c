/**
 *  Register names for RFM69HW
 *
 *
**/
#include "main.h"

#ifdef DEBUG_REGS

/* Structure to hold the register address and name */
struct rfm69hw_regs {
  uint8_t address;		/* Register address */
	char*   reg_name;		/* Register name */
};


const static struct rfm69hw_regs registers[] = {
  { 0x00, "RegFifo" },
  { 0x01, "RegOpMode" },
  { 0x02, "RegDataModul" },
  { 0x03, "RegBitrateMsb" },
  { 0x04, "RegBitrateLsb" },
	{ 0x05, "RegFdevMsb" },
	{ 0x06, "RegFdevLsb" },
	{ 0x07, "RegFrfMsb" },
	{ 0x08, "RegFrfMid" },
	{ 0x09, "RegFrfLsb" },
	{ 0x0A, "RegOsc1" },
	{ 0x0B, "RegAfcCtrl" },
	{ 0x0D, "RegListen1" },
	{ 0x0E, "RegListen2" },
	{ 0x0F, "RegListen3" },
  { 0x10, "RegVersion" },
  { 0x11, "RegPaLevel" },
  { 0x12, "RegPaRamp" },
	{ 0x18, "RegLna" },
	{ 0x19, "RegRxBw" },
	{ 0x1A, "RegAfcBw" },
	{ 0x1B, "RegOokPeak" },
	{ 0x1C, "RegOokAvg" },
	{ 0x1D, "RegOokFix" },
	{ 0x1E, "RegAfcFei" },
	{ 0x1F, "RegAfcMsb" },
	{ 0x20, "RegAfcLsb" },
	{ 0x21, "RegFeiMsb" },
	{ 0x22, "RegFeiLsb" },
  { 0x23, "RegRssiConfig" },
  { 0x24, "RegRssiValue" },
  { 0x25, "RegDioMapping1" },
  { 0x26, "RegDioMapping2" },
  { 0x27, "RegIrqFlags1" },
	{ 0x28, "RegIrqFlags2" },
	{ 0x29, "RegRssiThresh" },
	{ 0x2A, "RegRxTimeout1" },
	{ 0x2B, "RegRxTimeout2" },
	{ 0x2C, "RegPreambleMsb" },
	{ 0x2D, "RegPreambleLsb" },
	{ 0x2E, "RegSyncConfig" },
	{ 0x2F, "RegSyncValue1" },
	{ 0x30, "RegSyncValue2" },
	{ 0x31, "RegSyncValue3" },
	{ 0x32, "RegSyncValue4" },
  { 0x33, "RegSyncValue5" },
  { 0x34, "RegSyncValue6" },
  { 0x35, "RegSyncValue7" },
  { 0x36, "RegSyncValue8" },
  { 0x37, "RegPacketConfig1" },
	{ 0x38, "RegPayloadLength" },
	{ 0x39, "RegNodeAdrs" },
	{ 0x3A, "RegBroadcastAdrs" },
	{ 0x3B, "RegAutoModes" },
	{ 0x3C, "RegFifoThresh" },
	{ 0x3D, "RegPacketConfig2" },
	{ 0x3E, "RegAesKey1" },
	{ 0x3F, "RegAesKey2" },
	{ 0x40, "RegAesKey3" },
	{ 0x41, "RegAesKey4" },
	{ 0x42, "RegAesKey5" },
  { 0x43, "RegAesKey6" },
  { 0x44, "RegAesKey7" },
  { 0x45, "RegAesKey8" },
  { 0x46, "RegAesKey9" },
  { 0x47, "RegAesKey10" },
	{ 0x48, "RegAesKey11" },
	{ 0x49, "RegAesKey12" },
	{ 0x4A, "RegAesKey13" },
	{ 0x4B, "RegAesKey14" },
	{ 0x4C, "RegAesKey15" },
	{ 0x4D, "RegAesKey16" },
	{ 0x4E, "RegTemp1" },
	{ 0x4F, "RegTemp2" },
	{ 0x58, "RegTestLna" },
	{ 0x5A, "RegTestPa1" },
	{ 0x5C, "RegTestPa2" },
	{ 0x6F, "RegTestDagc" },
	{ 0x71, "RegTestAfc" },
	{ 0xff, "" }
};


/**
 * @brief Look up a register address to find the name string
 *
 * @param  address  Register address to search for
 * @return pointer to the register name or "" for unknown
 *
**/
char* regs_lookup(int address)
{
  for (int i = 0; registers[i].address != 0xff; i++)
		if (registers[i].address == address)
      return(registers[i].reg_name);

	return("Reserved");
}

#endif
