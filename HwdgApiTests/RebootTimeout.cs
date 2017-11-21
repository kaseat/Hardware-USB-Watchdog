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
    public class RebootTimeout
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
            hwdg.ChipResetAndWaitForReady();
        }

        [TestMethod]
        public void CanSetRebootTimeoutWhenIdle()
        {
            const Int32 timeoutStep = 5000;
            const Int32 minTimeout = 10000;
            const Int32 maxTimeout = 600000;

            for (var timeout = minTimeout; timeout <= maxTimeout; timeout += timeoutStep)
            {
                Assert.AreEqual(Response.SetRebootTimeoutOk, hwdg.SetRebootTimeout(timeout));
                Assert.AreEqual(timeout, hwdg.GetStatus().RebootTimeout);
                Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.IsRunning));
            }
        }

        [TestMethod]
        public void CannotSetRebootTimeoutWhenMonitoring()
        {
            const Int32 timeout = 425000;

            hwdg.Start();
            Assert.AreEqual(Response.Busy, hwdg.SetRebootTimeout(timeout));
            Assert.AreEqual(DefaultSettings.RebootTimeout, hwdg.GetStatus().RebootTimeout);
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.IsRunning));
        }

        [TestMethod]
        public void CannotSetRebootTimeoutWhenWaitingForReboot()
        {
            const Int32 timeout = 5000;
            const Int32 rebootTimeout = 25000;

            hwdg.SetResponseTimeout(timeout);
            hwdg.Start();
            hwdg.WaitForFlag(WatchdogState.WaitingForReboot);

            Assert.AreEqual(Response.Busy, hwdg.SetRebootTimeout(rebootTimeout));
            Assert.AreEqual(DefaultSettings.RebootTimeout, hwdg.GetStatus().RebootTimeout);
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.IsRunning));
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.WaitingForReboot));
        }
    }
}