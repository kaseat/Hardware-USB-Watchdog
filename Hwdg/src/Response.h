// Copyright 2017 Oleg Petrochenko
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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
};
