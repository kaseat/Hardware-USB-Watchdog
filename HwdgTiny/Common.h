// Copyright 2017 Oleg Petrochenko
// 
// This file is part of HwdgTiny.
// 
// HwdgTiny is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or any
// later version.
// 
// HwdgTiny is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with HwdgTiny. If not, see <http://www.gnu.org/licenses/>.

#pragma once
typedef enum
{
	Busy = 0x20,
	EnableEventsOk = 0x21,
	DisableEventsOk = 0x22,
	TestHardResetOk = 0x23,
	TestSoftResetOk = 0x24,
	StartOk = 0x25,
	StopOk = 0x26,
	PingOk = 0x27,
	EnableHardResetOk = 0x28,
	DisableHardResetOk = 0x29,
	EnableLedOk = 0x2A,
	DisableLedOk = 0x2B,
	SetRebootTimeoutOk = 0x2C,
	SetResponseTimeoutOk = 0x2D,
	SetSoftResetAttemptsOk = 0x2E,
	SetHardResetAttemptsOk = 0x2F,

	SaveCurrentSettingsOk = 0x40,
	LoadDefaultSettingsAtStartupOk = 0x41,
	ApplyUserSettingsAtStartupOk = 0x42,

	PwrPulseOnStartupEnableOk = 0x43,
	PwrPulseOnStartupDisableOk = 0x44,
	RstPulseOnStartupEnableOk = 0x45,
	RstPulseOnStartupDisableOk = 0x46,

	SaveSettingsError = 0x47,
	PowerPulseOk = 0x48,

	UnknownCommand = 0x4F,

	FirstResetOccurred = 0x30,
	SoftResetOccurred = 0x31,
	HardResetOccurred = 0x32,
	MovedToIdle = 0x33,
	WatchdogOk = 0x34,
	SoftwareVersion = 0x55,
} Response_t;



/**
* \brief Represents actual HWDG status.
* \remarks See https://hwdg.ru/developer/hwdg-api/getstatus/
* for more detailed information. This is HWDG Tiny version.
*/
typedef struct
{
	/**
	* \brief Reserved.
	*/
	unsigned char : 1;
	/**
	* \brief System reboot timeout. If system does not
	* reboot within this time HWDG reboots the system again.
	*/
	unsigned char RebootTimeout : 7;



	/**
	* \brief System response timeout. If system does not
	* respond within this time HWDG reboots the system.
	*/
	unsigned char ResponseTimeout : 6;
	/**
	* \brief This flag is set when HWDG started to reboot system.
	*/
	unsigned char IsRebooting : 1;
	/**
	* \brief This flag is set when HWDG is running and awaiting Ping command.
	*/
	unsigned char IsMonitoring : 1;



	/**
	* \brief Reset attempts count before HWDG goes idle state or begins Hard reset routine.
	*/
	unsigned char ResetAttempts : 3;
	/**
	* \brief When this flag is set, HWDG loads user setting instead of default.
	*/
	unsigned char LoadUserSettings : 1;
	/**
	* \brief This flag is set when HWDG LED disabled.
	*/
	unsigned char IsLedDisabled : 1;
	/**
	* \brief Reserved.
	*/
	unsigned char : 3;



	/**
	* \brief Checksum.
	*/
	unsigned char Checksum : 8;


	/**
	* \brief Checksum.
	*/
	unsigned char LastCommandStatus : 8;
}Status_t;

///**
// * \brief Represents actual HWDG status.
// * \remarks See https://hwdg.ru/developer/hwdg-api/getstatus/
// * for more detailed information. This is full version.
// */
//typedef struct
//{
//	/**
//	 * \brief Last command status.
//	 */
//	Response_t LastCommandStatus : 8;
//	unsigned char : 1;
//	/**
//	 * \brief System reboot timeout. If system does not
//	 * reboot within this time HWDG reboots the system again.
//	 */
//	unsigned char RebootTimeout : 7;
//	/**
//	* \brief System response timeout. If system does not
//	* respond within this time HWDG reboots the system.
//	*/
//	unsigned char ResponseTimeout : 6;
//	/**
//	 * \brief This flag is set when HWDG started to reboot system.
//	 */
//	unsigned char IsRebooting : 1;
//	/**
//	 * \brief This flag is set when HWDG is running and awaiting Ping command.
//	 */
//	unsigned char IsMonitoring : 1;
//	/**
//	 * \brief Soft reset attempts count before HWDG goes idle state or begins Hard reset routine.
//	 */
//	unsigned char SoftResetAttempts : 3;
//	/**
//	* \brief Hard reset attempts count before HWDG goes idle state.
//	*/
//	unsigned char HardResetAttempts : 3;
//	unsigned char : 1;
//	/**
//	 * \brief This flag is set when HWDG hard reset enabled.
//	 */
//	unsigned char IsHardResetEnabled : 1;
//	unsigned char : 3;
//	/**
//	 * \brief This flag is set when HWDG sends Power button pulse after power on.
//	 */
//	unsigned char IsPwrPulseEnabled : 1;
//	/**
//	 * \brief This flag is set when HWDG sends Reset button pulse after power on.
//	 */
//	unsigned char IsRstulseEnabled : 1;
//	/**
//	 * \brief When this flag is set, HWDG loads user setting instead of default.
//	 */
//	unsigned char LoadUserSettings : 1;
//	/**
//	 * \brief This flag is set when HWDG events are enabled.
//	 */
//	unsigned char IsEventsEnabled : 1;
//	/**
//	 * \brief This flag is set when HWDG LED disabled.
//	 */
//	unsigned char IsLedDisabled : 1;
//	/**
//	 * \brief Checksum.
//	 */
//	unsigned char Checksum : 8;
//}Status_t;
