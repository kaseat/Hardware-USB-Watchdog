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
using System.Threading;
using HwdgWrapper;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace HwdgApiTests
{
    [TestClass]
    public class TestSoftReset
    {
        private static IWrapper hwdg;

        [ClassInitialize]
        public static void ClassInitialize(TestContext context)
        {
            Console.WriteLine($"Test class {context.TestName} begin");
            hwdg = AssemblyBase.Wrapper;
            hwdg.FactoryResetAndWaitForReady();
        }

        [TestInitialize]
        public void TestInitialize()
        {
            hwdg.FactoryResetAndWaitForReady();
        }

        [TestMethod]
        public void CanTestSoftResetWhenIdle()
        {
            Assert.AreEqual(Response.TestSoftResetOk, hwdg.TestSoftReset());
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.IsRunning));
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.WaitingForReboot));
        }

        [TestMethod]
        public void CanTestSoftResetWhenMonitoring()
        {
            hwdg.Start();
            Assert.AreEqual(Response.TestSoftResetOk, hwdg.TestSoftReset());
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.IsRunning));
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.WaitingForReboot));
        }

        [TestMethod]
        public void CanTestSoftResetWhenWaitingForReboot()
        {
            const Int32 timeout = 5000;

            hwdg.SetResponseTimeout(timeout);
            hwdg.Start();
            hwdg.WaitForFlag(WatchdogState.WaitingForReboot);

            // Wait 200 ms until SoftReset operation finishes.
            // See fig.1 https://hwdg.ru/developer/reboot-timings/ for more details.
            Thread.Sleep(200);

            Assert.AreEqual(Response.TestSoftResetOk, hwdg.TestSoftReset());
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.IsRunning));
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.WaitingForReboot));
        }
    }
}