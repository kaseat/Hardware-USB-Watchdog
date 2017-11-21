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
    public class ResponseTimeout
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
        public void CanSetResponseTimeoutWhenIdle()
        {
            const Int32 timeoutStep = 5000;
            const Int32 minTimeout = 5000;
            const Int32 maxTimeout = 300000;

            for (var timeout = minTimeout; timeout <= maxTimeout; timeout += timeoutStep)
            {
                Assert.AreEqual(Response.SetResponseTimeoutOk, hwdg.SetResponseTimeout(timeout));
                Assert.AreEqual(timeout, hwdg.GetStatus().ResponseTimeout);
                Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.IsRunning));
            }
        }

        [TestMethod]
        public void CannotSetResponseTimeoutWhenMonitoring()
        {
            const Int32 timeout = 250000;

            hwdg.Start();
            Assert.AreEqual(Response.Busy, hwdg.SetResponseTimeout(timeout));
            Assert.AreEqual(DefaultSettings.ResponseTimeout, hwdg.GetStatus().ResponseTimeout);
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.IsRunning));
        }

        [TestMethod]
        public void CannotSetResponseTimeoutWhenWaitingForReboot()
        {
            const Int32 timeout = 5000;
            const Int32 responseTimeout = 35000;

            hwdg.SetResponseTimeout(timeout);
            hwdg.Start();
            hwdg.WaitForFlag(WatchdogState.WaitingForReboot);

            Assert.AreEqual(Response.Busy, hwdg.SetResponseTimeout(responseTimeout));
            //Note: response timeout is not default, it was configured above
            Assert.AreEqual(timeout, hwdg.GetStatus().ResponseTimeout);
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.IsRunning));
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.WaitingForReboot));
        }
    }
}