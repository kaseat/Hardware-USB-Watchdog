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

using System;
using System.IO.Ports;
using HwdgWrapper;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace HwdgApiTests
{
    [TestClass]
    public class EventsResponse
    {
        private const Byte Start = 0xF9;
        private const Byte EnableEvents = 0x02;
        private const Byte MinResponseTimeout = 0x40;
        private const Byte MinRebootTimeout = 0x80;
        private const Byte MinSoftResetAttempts = 0x10;
        private const Byte EnableHardReset = 0xFC;

        private static SerialPort port;

        [TestInitialize]
        public void TestInitialize()
        {
            AssemblyBase.Wrapper.FactoryResetAndWaitForReady();
            AssemblyBase.Wrapper.Dispose();
            port = AssemblyBase.GetHwdgPort();
            if (port == null) Assert.Fail("Could not find HWDG");
        }

        [TestCleanup]
        public void TestCleanup()
        {
            port.Dispose();
            AssemblyBase.Wrapper = new SerialWrapper();
        }

        [TestMethod]
        public void CanReceiveFirstResetBeganEvent()
        {
            port.BaseStream.WriteByte(EnableEvents);
            port.BaseStream.WriteByte(MinResponseTimeout);
            port.BaseStream.WriteByte(Start);

            port.WaitForEvent(Response.FirstResetOccurred, 5100);
        }

        [TestMethod]
        public void CanReceiveSoftResetBeganEvent()
        {
            port.BaseStream.WriteByte(EnableEvents);
            port.BaseStream.WriteByte(MinResponseTimeout);
            port.BaseStream.WriteByte(MinRebootTimeout);
            port.BaseStream.WriteByte(Start);

            port.WaitForEvent(Response.SoftResetOccurred, 15100);
        }

        [TestMethod]
        public void CanReceiveHardResetBeganEvent()
        {
            port.BaseStream.WriteByte(EnableEvents);
            port.BaseStream.WriteByte(MinResponseTimeout);
            port.BaseStream.WriteByte(MinRebootTimeout);
            port.BaseStream.WriteByte(MinSoftResetAttempts);
            port.BaseStream.WriteByte(EnableHardReset);
            port.BaseStream.WriteByte(Start);

            port.WaitForEvent(Response.HardResetOccurred, 15100);
        }

        [TestMethod]
        public void CanReceiveWatchdogGoesIdleEventWithSoftReset()
        {
            port.BaseStream.WriteByte(EnableEvents);
            port.BaseStream.WriteByte(MinResponseTimeout);
            port.BaseStream.WriteByte(MinRebootTimeout);
            port.BaseStream.WriteByte(MinSoftResetAttempts);
            port.BaseStream.WriteByte(Start);

            port.WaitForEvent(Response.MovedToIdle, 15100);
        }

        [TestMethod]
        public void CanReceiveWatchdogGoesIdleEventWithHardReset()
        {
            const Byte minHardResetAttempts = 0x18;

            port.BaseStream.WriteByte(EnableEvents);
            port.BaseStream.WriteByte(MinResponseTimeout);
            port.BaseStream.WriteByte(MinRebootTimeout);
            port.BaseStream.WriteByte(MinSoftResetAttempts);
            port.BaseStream.WriteByte(minHardResetAttempts);
            port.BaseStream.WriteByte(EnableHardReset);
            port.BaseStream.WriteByte(Start);

            port.WaitForEvent(Response.MovedToIdle, 25100);
        }

        [TestMethod]
        public void CanReceiveWatchdogOkEvent()
        {
            port.BaseStream.WriteByte(EnableEvents);
            port.WaitForEvent(Response.WatchdogOk, 1100);
        }
    }
}