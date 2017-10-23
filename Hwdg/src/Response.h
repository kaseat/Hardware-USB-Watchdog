// Copyright (c) 2017, Oleg Petrochenko
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the HWDG nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
// OF SUCH DAMAGE.

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

	FirstResetOccurred = 0x30,
	SoftResetOccurred = 0x31,
	HardResetOccurred = 0x32,
	MovedToIdle = 0x33,
	WatchdogOk = 0x34,
	SoftwareVersion = 0x55,
};
