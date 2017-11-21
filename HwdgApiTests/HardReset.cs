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
using HwdgWrapper;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace HwdgApiTests
{
    [TestClass]
    public class HardReset
    {
        private static IWrapper hwdg;

        [ClassInitialize]
        public static void ClassInitialize(TestContext context)
        {
            Console.WriteLine($"Test class {context.TestName} begin");
            hwdg = AssemblyBase.Wrapper;
            hwdg.FactoryResetAndWaitForReady();
        }

        [TestCleanup]
        public void TestCleanup()
        {
            hwdg.Stop();
            hwdg.DisableHardReset();
        }

        [TestMethod]
        public void CanDisableHardResetWhenHardResetEnabled()
        {
            hwdg.EnableHardReset();
            Assert.AreEqual(Response.DisableHardResetOk, hwdg.DisableHardReset());
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.HardRersetEnabled));
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.IsRunning));
        }

        [TestMethod]
        public void CanDisableHardResetWhenHardResetDisabled()
        {
            Assert.AreEqual(Response.DisableHardResetOk, hwdg.DisableHardReset());
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.HardRersetEnabled));
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.IsRunning));
        }

        [TestMethod]
        public void CannotDisableHardResetWhenMonitoring()
        {
            hwdg.Start();
            Assert.AreEqual(Response.Busy, hwdg.DisableHardReset());
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.HardRersetEnabled));
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.IsRunning));
        }

        [TestMethod]
        public void CannotDisableHardResetWhenWaitingForReboot()
        {
            const Int32 timeout = 5000;

            hwdg.SetResponseTimeout(timeout);
            hwdg.Start();
            hwdg.WaitForFlag(WatchdogState.WaitingForReboot);

            Assert.AreEqual(Response.Busy, hwdg.DisableHardReset());
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.HardRersetEnabled));
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.IsRunning));
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.WaitingForReboot));
        }

        [TestMethod]
        public void CanEnableHardResetWhenHardResetEnabled()
        {
            hwdg.EnableHardReset();
            Assert.AreEqual(Response.EnableHardResetOk, hwdg.EnableHardReset());
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.HardRersetEnabled));
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.IsRunning));
        }

        [TestMethod]
        public void CanEnableHardResetWhenHardResetDisabled()
        {
            Assert.AreEqual(Response.EnableHardResetOk, hwdg.EnableHardReset());
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.HardRersetEnabled));
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.IsRunning));
        }

        [TestMethod]
        public void CannotEnableHardResetWhenMonitoring()
        {
            hwdg.Start();
            Assert.AreEqual(Response.Busy, hwdg.EnableHardReset());
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.HardRersetEnabled));
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.IsRunning));
        }

        [TestMethod]
        public void CannotEnableHardResetWhenWaitingForReboot()
        {
            const Int32 timeout = 5000;

            hwdg.SetResponseTimeout(timeout);
            hwdg.Start();
            hwdg.WaitForFlag(WatchdogState.WaitingForReboot);

            Assert.AreEqual(Response.Busy, hwdg.EnableHardReset());
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.HardRersetEnabled));
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.IsRunning));
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.WaitingForReboot));
        }
    }
}