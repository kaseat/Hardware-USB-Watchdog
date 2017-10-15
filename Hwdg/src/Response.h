#pragma once
enum Response
{
	SetRebootTimeoutOk = 0x21,
	SetResponseTimeoutOk = 0x22,
	SetSoftResetAttemptsOk = 0x23,
	SetHardResetAttemptsOk = 0x24,
	DisableHardResetOk = 0x25,
	EnableHardResetOk = 0x26,
	StartOk = 0x27,
	StopOk = 0x28,
	PingOk = 0x29,
	Busy = 0x2A,
	Error = 0x2B,
	EnableHddMonitorOk = 0x2C,
	DisableHddMonitorOk = 0x2D,
	SetHddTimeoutOk = 0x2E,
};