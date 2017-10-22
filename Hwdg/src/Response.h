#pragma once
enum Response
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
	EnableHddMonitorOk = 0x2A,
	DisableHddMonitorOk = 0x2B,
	SetRebootTimeoutOk = 0x2C,
	SetResponseTimeoutOk = 0x2D,
	SetSoftResetAttemptsOk = 0x2E,
	SetHardResetAttemptsOk = 0x2F,
	UnknownCommand = 0x4F,

	FirstResetOccured = 0x30,
	SoftResetOccured = 0x31,
	HardResetOccured = 0x32,
	SoftwareVersion = 0x55,
};