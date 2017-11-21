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
    public class HardResetAttempts
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
            hwdg.SetHardResetAttempts(DefaultSettings.HardResetAttempts);
        }

        [TestMethod]
        public void CanSetHardResetAttemptsWhenIdle()
        {
            const Byte minAttempts = 1;
            const Byte maxAttempts = 8;

            for (Byte attempts = minAttempts; attempts <= maxAttempts; attempts++)
            {
                Assert.AreEqual(Response.SetHardResetAttemptsOk, hwdg.SetHardResetAttempts(attempts));
                Assert.AreEqual(attempts, hwdg.GetStatus().HardResetAttempts);
                Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.IsRunning));
            }
        }

        [TestMethod]
        public void CannotSetHardResetAttemptsWhenMonitoring()
        {
            const Byte attempts = 7;

            hwdg.Start();
            Assert.AreEqual(Response.Busy, hwdg.SetHardResetAttempts(attempts));
            Assert.AreEqual(DefaultSettings.HardResetAttempts, hwdg.GetStatus().HardResetAttempts);
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.IsRunning));
        }

        [TestMethod]
        public void CannotSetHardResetAttemptsWhenWaitingForReboot()
        {
            const Byte attempts = 2;
            const Int32 timeout = 5000;

            hwdg.SetResponseTimeout(timeout);
            hwdg.Start();
            hwdg.WaitForFlag(WatchdogState.WaitingForReboot);

            Assert.AreEqual(Response.Busy, hwdg.SetHardResetAttempts(attempts));
            Assert.AreEqual(DefaultSettings.HardResetAttempts, hwdg.GetStatus().HardResetAttempts);
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.IsRunning));
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.WaitingForReboot));
        }
    }
}