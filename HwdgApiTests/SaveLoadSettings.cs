// Copyright 2017 Anna Batyaeva
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
    public class SaveLoadSettings
    {
        private static IWrapper hwdg;

        [ClassInitialize]
        public static void ClassInitialize(TestContext context)
        {
            Console.WriteLine($"Test class {context.TestName} begin");
            hwdg = AssemblyBase.Wrapper;
        }

        [TestInitialize]
        public void TestInitialize()
        {
            hwdg.FactoryResetAndWaitForReady();
        }

        [TestMethod]
        public void VerifyHwdgDefaultSettings()
        {
            Assert.AreEqual(DefaultSettings.HardResetAttempts, hwdg.GetStatus().HardResetAttempts);
            Assert.AreEqual(DefaultSettings.SoftResetAttempts, hwdg.GetStatus().SoftResetAttempts);
            Assert.AreEqual(DefaultSettings.ResponseTimeout, hwdg.GetStatus().ResponseTimeout);
            Assert.AreEqual(DefaultSettings.RebootTimeout, hwdg.GetStatus().RebootTimeout);
            Assert.AreEqual(DefaultSettings.LedDisabled, hwdg.GetStatus().State.HasFlag(WatchdogState.LedDisabled));
            Assert.AreEqual(DefaultSettings.HardResetEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.HardResetEnabled));
            Assert.AreEqual(DefaultSettings.EventsEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.EventsEnabled));
            Assert.AreEqual(DefaultSettings.RstPulseEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.RstPulseEnabled));
            Assert.AreEqual(DefaultSettings.PwrPulseEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.PwrPulseEnabled));
            Assert.AreEqual(DefaultSettings.ApplySettingsAtStartup, hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
        }

        [TestMethod]
        public void SaveHardResetAttemptsAndApplyOnStartup()
        {
            const Byte attemptsToBeVerified = 6;
            const Byte attemptsVerifyNotApplied = 1;

            hwdg.SetHardResetAttempts(attemptsToBeVerified);
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
            Assert.AreEqual(Response.SaveCurrentSettingsOk, hwdg.SaveCurrentSettingsAsUsers());
            Assert.AreEqual(Response.ApplyUserSettingsAtStartupOk, hwdg.ApplyUserSettingsAtStartup());
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
            Assert.AreEqual(attemptsToBeVerified, hwdg.GetStatus().HardResetAttempts);
            hwdg.SetHardResetAttempts(attemptsVerifyNotApplied);
            Assert.AreEqual(attemptsVerifyNotApplied, hwdg.GetStatus().HardResetAttempts);
            hwdg.ChipResetAndWaitForReady();
            Assert.AreEqual(attemptsToBeVerified, hwdg.GetStatus().HardResetAttempts);
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
        }

        [TestMethod]
        public void SaveSoftResetAttemptsAndApplyOnStartup()
        {
            const Byte attemptsToBeVerified = 1;
            const Byte attemptsVerifyNotApplied = 2;

            hwdg.SetSoftResetAttempts(attemptsToBeVerified);
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
            Assert.AreEqual(Response.SaveCurrentSettingsOk, hwdg.SaveCurrentSettingsAsUsers());
            Assert.AreEqual(Response.ApplyUserSettingsAtStartupOk, hwdg.ApplyUserSettingsAtStartup());
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
            Assert.AreEqual(attemptsToBeVerified, hwdg.GetStatus().SoftResetAttempts);
            hwdg.SetSoftResetAttempts(attemptsVerifyNotApplied);
            Assert.AreEqual(attemptsVerifyNotApplied, hwdg.GetStatus().SoftResetAttempts);
            hwdg.ChipResetAndWaitForReady();
            Assert.AreEqual(attemptsToBeVerified, hwdg.GetStatus().SoftResetAttempts);
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
        }

        [TestMethod]
        public void SaveResponseTimeoutAndApplyOnStartup()
        {
            const Int32 timeoutToBeVerified = 55000;
            const Int32 timeoutVerifyNotApplied = 70000;

            hwdg.SetResponseTimeout(timeoutToBeVerified);
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
            Assert.AreEqual(Response.SaveCurrentSettingsOk, hwdg.SaveCurrentSettingsAsUsers());
            Assert.AreEqual(Response.ApplyUserSettingsAtStartupOk, hwdg.ApplyUserSettingsAtStartup());
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
            Assert.AreEqual(timeoutToBeVerified, hwdg.GetStatus().ResponseTimeout);
            hwdg.SetResponseTimeout(timeoutVerifyNotApplied);
            Assert.AreEqual(timeoutVerifyNotApplied, hwdg.GetStatus().ResponseTimeout);
            hwdg.ChipResetAndWaitForReady();
            Assert.AreEqual(timeoutToBeVerified, hwdg.GetStatus().ResponseTimeout);
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
        }

        [TestMethod]
        public void SaveRebootTimeoutAndApplyOnStartup()
        {
            const Int32 timeoutToBeVerified = 230000;
            const Int32 timeoutVerifyNotApplied = 220000;

            hwdg.SetRebootTimeout(timeoutToBeVerified);
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
            Assert.AreEqual(Response.SaveCurrentSettingsOk, hwdg.SaveCurrentSettingsAsUsers());
            Assert.AreEqual(Response.ApplyUserSettingsAtStartupOk, hwdg.ApplyUserSettingsAtStartup());
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
            Assert.AreEqual(timeoutToBeVerified, hwdg.GetStatus().RebootTimeout);
            hwdg.SetRebootTimeout(timeoutVerifyNotApplied);
            Assert.AreEqual(timeoutVerifyNotApplied, hwdg.GetStatus().RebootTimeout);
            hwdg.ChipResetAndWaitForReady();
            Assert.AreEqual(timeoutToBeVerified, hwdg.GetStatus().RebootTimeout);
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
        }

        [TestMethod]
        public void SaveLedDisabledAndApplyOnStartup()
        {
            hwdg.DisableLed();
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
            Assert.AreEqual(Response.SaveCurrentSettingsOk, hwdg.SaveCurrentSettingsAsUsers());
            Assert.AreEqual(Response.ApplyUserSettingsAtStartupOk, hwdg.ApplyUserSettingsAtStartup());
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.LedDisabled));
            hwdg.EnableLed();
            hwdg.ChipResetAndWaitForReady();
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.LedDisabled));
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
        }

        [TestMethod]
        public void SaveLedEnabledAndApplyOnStartup()
        {
            hwdg.EnableLed();
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
            Assert.AreEqual(Response.SaveCurrentSettingsOk, hwdg.SaveCurrentSettingsAsUsers());
            Assert.AreEqual(Response.ApplyUserSettingsAtStartupOk, hwdg.ApplyUserSettingsAtStartup());
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.LedDisabled));
            hwdg.DisableLed();
            hwdg.ChipResetAndWaitForReady();
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.LedDisabled));
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
        }

        [TestMethod]
        public void SaveHardResetDisabledAndApplyOnStartup()
        {
            hwdg.DisableHardReset();
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
            Assert.AreEqual(Response.SaveCurrentSettingsOk, hwdg.SaveCurrentSettingsAsUsers());
            Assert.AreEqual(Response.ApplyUserSettingsAtStartupOk, hwdg.ApplyUserSettingsAtStartup());
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.HardResetEnabled));
            hwdg.EnableHardReset();
            hwdg.ChipResetAndWaitForReady();
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.HardResetEnabled));
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
        }

        [TestMethod]
        public void SaveHardResetEnabledAndApplyOnStartup()
        {
            hwdg.EnableHardReset();
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
            Assert.AreEqual(Response.SaveCurrentSettingsOk, hwdg.SaveCurrentSettingsAsUsers());
            Assert.AreEqual(Response.ApplyUserSettingsAtStartupOk, hwdg.ApplyUserSettingsAtStartup());
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.HardResetEnabled));
            hwdg.DisableHardReset();
            hwdg.ChipResetAndWaitForReady();
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.HardResetEnabled));
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
        }

        [TestMethod]
        public void SaveEventsDisabledAndApplyOnStartup()
        {
            hwdg.DisableEvents();
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
            Assert.AreEqual(Response.SaveCurrentSettingsOk, hwdg.SaveCurrentSettingsAsUsers());
            Assert.AreEqual(Response.ApplyUserSettingsAtStartupOk, hwdg.ApplyUserSettingsAtStartup());
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.EventsEnabled));
            hwdg.EnableEvents();
            hwdg.ChipResetAndWaitForReady();
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.EventsEnabled));
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
        }

        [TestMethod]
        public void SaveEventsEnabledAndApplyOnStartup()
        {
            hwdg.EnableEvents();
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
            Assert.AreEqual(Response.SaveCurrentSettingsOk, hwdg.SaveCurrentSettingsAsUsers());
            Assert.AreEqual(Response.ApplyUserSettingsAtStartupOk, hwdg.ApplyUserSettingsAtStartup());
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.EventsEnabled));
            hwdg.DisableEvents();
            hwdg.ChipResetAndWaitForReady();
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.EventsEnabled));
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
        }

        [TestMethod]
        public void SaveRstDisabledNotAffectedByApplyOnStartup()
        {
            hwdg.RstPulseOnStartupDisable();
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
            Assert.AreEqual(Response.SaveCurrentSettingsOk, hwdg.SaveCurrentSettingsAsUsers());
            Assert.AreEqual(Response.ApplyUserSettingsAtStartupOk, hwdg.ApplyUserSettingsAtStartup());
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.RstPulseEnabled));
            hwdg.RstPulseOnStartupEnable();
            hwdg.ChipResetAndWaitForReady(3200);
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.RstPulseEnabled));
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
        }

        [TestMethod]
        public void SaveRstEnabledNotAffectedByApplyOnStartup()
        {
            hwdg.RstPulseOnStartupEnable();
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
            Assert.AreEqual(Response.SaveCurrentSettingsOk, hwdg.SaveCurrentSettingsAsUsers());
            Assert.AreEqual(Response.ApplyUserSettingsAtStartupOk, hwdg.ApplyUserSettingsAtStartup());
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.RstPulseEnabled));
            hwdg.RstPulseOnStartupDisable();
            hwdg.ChipResetAndWaitForReady();
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.RstPulseEnabled));
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
        }

        [TestMethod]
        public void SavePwrDisabledNotAffectedByApplyOnStartup()
        {
            hwdg.PwrPulseOnStartupDisable();
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
            Assert.AreEqual(Response.SaveCurrentSettingsOk, hwdg.SaveCurrentSettingsAsUsers());
            Assert.AreEqual(Response.ApplyUserSettingsAtStartupOk, hwdg.ApplyUserSettingsAtStartup());
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.PwrPulseEnabled));
            hwdg.PwrPulseOnStartupEnable();
            hwdg.ChipResetAndWaitForReady(3200);
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.PwrPulseEnabled));
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
        }

        [TestMethod]
        public void SavePwrEnabledNotAffectedByApplyOnStartup()
        {
            hwdg.PwrPulseOnStartupEnable();
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
            Assert.AreEqual(Response.SaveCurrentSettingsOk, hwdg.SaveCurrentSettingsAsUsers());
            Assert.AreEqual(Response.ApplyUserSettingsAtStartupOk, hwdg.ApplyUserSettingsAtStartup());
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.PwrPulseEnabled));
            hwdg.PwrPulseOnStartupDisable();
            hwdg.ChipResetAndWaitForReady();
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.PwrPulseEnabled));
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
        }

        [TestMethod]
        public void SaveHardResetAttemptsAndLoadDefaultOnStartup()
        {
            const Byte attempts = 6;

            hwdg.SetHardResetAttempts(attempts);
            Assert.AreEqual(Response.SaveCurrentSettingsOk, hwdg.SaveCurrentSettingsAsUsers());
            Assert.AreEqual(Response.LoadDefaultSettingsAtStartupOk, hwdg.LoadDefaultSettingsAtStartup());
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
            Assert.AreEqual(attempts, hwdg.GetStatus().HardResetAttempts);
            hwdg.ChipResetAndWaitForReady();
            Assert.AreEqual(DefaultSettings.HardResetAttempts, hwdg.GetStatus().HardResetAttempts);
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
        }

        [TestMethod]
        public void SaveSoftResetAttemptsAndLoadDefaultOnStartup()
        {
            const Byte attempts = 1;

            hwdg.SetSoftResetAttempts(attempts);
            Assert.AreEqual(Response.SaveCurrentSettingsOk, hwdg.SaveCurrentSettingsAsUsers());
            Assert.AreEqual(Response.LoadDefaultSettingsAtStartupOk, hwdg.LoadDefaultSettingsAtStartup());
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
            Assert.AreEqual(attempts, hwdg.GetStatus().SoftResetAttempts);
            hwdg.ChipResetAndWaitForReady();
            Assert.AreEqual(DefaultSettings.SoftResetAttempts, hwdg.GetStatus().SoftResetAttempts);
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
        }

        [TestMethod]
        public void SaveResponseTimeoutAndLoadDefaultOnStartup()
        {
            const Int32 timeout = 40000;

            hwdg.SetResponseTimeout(timeout);
            Assert.AreEqual(Response.SaveCurrentSettingsOk, hwdg.SaveCurrentSettingsAsUsers());
            Assert.AreEqual(Response.LoadDefaultSettingsAtStartupOk, hwdg.LoadDefaultSettingsAtStartup());
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
            Assert.AreEqual(timeout, hwdg.GetStatus().ResponseTimeout);
            hwdg.ChipResetAndWaitForReady();
            Assert.AreEqual(DefaultSettings.ResponseTimeout, hwdg.GetStatus().ResponseTimeout);
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
        }

        [TestMethod]
        public void SaveRebootTimeoutAndLoadDefaultOnStartup()
        {
            const Int32 timeout = 580000;

            hwdg.SetRebootTimeout(timeout);
            Assert.AreEqual(Response.SaveCurrentSettingsOk, hwdg.SaveCurrentSettingsAsUsers());
            Assert.AreEqual(Response.LoadDefaultSettingsAtStartupOk, hwdg.LoadDefaultSettingsAtStartup());
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
            Assert.AreEqual(timeout, hwdg.GetStatus().RebootTimeout);
            hwdg.ChipResetAndWaitForReady();
            Assert.AreEqual(DefaultSettings.RebootTimeout, hwdg.GetStatus().RebootTimeout);
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
        }

        [TestMethod]
        public void SaveLedDisabledAndLoadDefaultOnStartup()
        {
            hwdg.DisableLed();
            Assert.AreEqual(Response.SaveCurrentSettingsOk, hwdg.SaveCurrentSettingsAsUsers());
            Assert.AreEqual(Response.LoadDefaultSettingsAtStartupOk, hwdg.LoadDefaultSettingsAtStartup());
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.LedDisabled));
            hwdg.ChipResetAndWaitForReady();
            Assert.AreEqual(DefaultSettings.LedDisabled, hwdg.GetStatus().State.HasFlag(WatchdogState.LedDisabled));
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
        }

        [TestMethod]
        public void SaveLedEnabledAndLoadDefaultOnStartup()
        {
            hwdg.EnableLed();
            Assert.AreEqual(Response.SaveCurrentSettingsOk, hwdg.SaveCurrentSettingsAsUsers());
            Assert.AreEqual(Response.LoadDefaultSettingsAtStartupOk, hwdg.LoadDefaultSettingsAtStartup());
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.LedDisabled));
            hwdg.ChipResetAndWaitForReady();
            Assert.AreEqual(DefaultSettings.LedDisabled, hwdg.GetStatus().State.HasFlag(WatchdogState.LedDisabled));
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
        }

        [TestMethod]
        public void SaveHardResetDisabledAndLoadDefaultOnStartup()
        {
            hwdg.DisableHardReset();
            Assert.AreEqual(Response.SaveCurrentSettingsOk, hwdg.SaveCurrentSettingsAsUsers());
            Assert.AreEqual(Response.LoadDefaultSettingsAtStartupOk, hwdg.LoadDefaultSettingsAtStartup());
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.HardResetEnabled));
            hwdg.ChipResetAndWaitForReady();
            Assert.AreEqual(DefaultSettings.HardResetEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.HardResetEnabled));
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
        }

        [TestMethod]
        public void SaveHardResetEnabledAndLoadDefaultOnStartup()
        {
            hwdg.EnableHardReset();
            Assert.AreEqual(Response.SaveCurrentSettingsOk, hwdg.SaveCurrentSettingsAsUsers());
            Assert.AreEqual(Response.LoadDefaultSettingsAtStartupOk, hwdg.LoadDefaultSettingsAtStartup());
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.HardResetEnabled));
            hwdg.ChipResetAndWaitForReady();
            Assert.AreEqual(DefaultSettings.HardResetEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.HardResetEnabled));
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
        }

        [TestMethod]
        public void SaveEventsDisabledAndLoadDefaultOnStartup()
        {
            hwdg.DisableEvents();
            Assert.AreEqual(Response.SaveCurrentSettingsOk, hwdg.SaveCurrentSettingsAsUsers());
            Assert.AreEqual(Response.LoadDefaultSettingsAtStartupOk, hwdg.LoadDefaultSettingsAtStartup());
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.EventsEnabled));
            hwdg.ChipResetAndWaitForReady();
            Assert.AreEqual(DefaultSettings.EventsEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.EventsEnabled));
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
        }

        [TestMethod]
        public void SaveEventsEnabledAndLoadDefaultOnStartup()
        {
            hwdg.EnableEvents();
            Assert.AreEqual(Response.SaveCurrentSettingsOk, hwdg.SaveCurrentSettingsAsUsers());
            Assert.AreEqual(Response.LoadDefaultSettingsAtStartupOk, hwdg.LoadDefaultSettingsAtStartup());
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.EventsEnabled));
            hwdg.ChipResetAndWaitForReady();
            Assert.AreEqual(DefaultSettings.EventsEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.EventsEnabled));
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
        }

        [TestMethod]
        public void SaveRstDisabledNotAffectedByLoadDefaultOnStartup()
        {
            hwdg.RstPulseOnStartupDisable();
            Assert.AreEqual(Response.SaveCurrentSettingsOk, hwdg.SaveCurrentSettingsAsUsers());
            Assert.AreEqual(Response.LoadDefaultSettingsAtStartupOk, hwdg.LoadDefaultSettingsAtStartup());
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.RstPulseEnabled));
            hwdg.ChipResetAndWaitForReady();
            Assert.AreEqual(DefaultSettings.RstPulseEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.RstPulseEnabled));
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
        }

        [TestMethod]
        public void SaveRstEnabledNotAffectedByLoadDefaultOnStartup()
        {
            hwdg.RstPulseOnStartupEnable();
            Assert.AreEqual(Response.SaveCurrentSettingsOk, hwdg.SaveCurrentSettingsAsUsers());
            Assert.AreEqual(Response.LoadDefaultSettingsAtStartupOk, hwdg.LoadDefaultSettingsAtStartup());
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.RstPulseEnabled));
            hwdg.ChipResetAndWaitForReady(3200);
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.RstPulseEnabled));
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
        }

        [TestMethod]
        public void SavePwrDisabledNotAffectedByLoadDefaultOnStartup()
        {
            hwdg.PwrPulseOnStartupDisable();
            Assert.AreEqual(Response.SaveCurrentSettingsOk, hwdg.SaveCurrentSettingsAsUsers());
            Assert.AreEqual(Response.LoadDefaultSettingsAtStartupOk, hwdg.LoadDefaultSettingsAtStartup());
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.PwrPulseEnabled));
            hwdg.ChipResetAndWaitForReady();
            Assert.AreEqual(DefaultSettings.PwrPulseEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.PwrPulseEnabled));
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
        }

        [TestMethod]
        public void SavePwrEnabledNotAffectedByLoadDefaultOnStartup()
        {
            hwdg.PwrPulseOnStartupEnable();
            Assert.AreEqual(Response.SaveCurrentSettingsOk, hwdg.SaveCurrentSettingsAsUsers());
            Assert.AreEqual(Response.LoadDefaultSettingsAtStartupOk, hwdg.LoadDefaultSettingsAtStartup());
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.PwrPulseEnabled));
            hwdg.ChipResetAndWaitForReady(3200);
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.PwrPulseEnabled));
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
        }

        [TestMethod]
        public void SaveHardResetAttemptsOtherSettingsNotChanged()
        {
            const Byte attempts = 1;

            hwdg.SetHardResetAttempts(attempts);
            Assert.AreEqual(attempts, hwdg.GetStatus().HardResetAttempts);
            Assert.AreEqual(Response.SaveCurrentSettingsOk, hwdg.SaveCurrentSettingsAsUsers());
            Assert.AreEqual(DefaultSettings.SoftResetAttempts, hwdg.GetStatus().SoftResetAttempts);
            Assert.AreEqual(DefaultSettings.ResponseTimeout, hwdg.GetStatus().ResponseTimeout);
            Assert.AreEqual(DefaultSettings.RebootTimeout, hwdg.GetStatus().RebootTimeout);
            Assert.AreEqual(DefaultSettings.LedDisabled, hwdg.GetStatus().State.HasFlag(WatchdogState.LedDisabled));
            Assert.AreEqual(DefaultSettings.HardResetEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.HardResetEnabled));
            Assert.AreEqual(DefaultSettings.EventsEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.EventsEnabled));
            Assert.AreEqual(DefaultSettings.RstPulseEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.RstPulseEnabled));
            Assert.AreEqual(DefaultSettings.PwrPulseEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.PwrPulseEnabled));
            Assert.AreEqual(DefaultSettings.ApplySettingsAtStartup, hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
        }

        [TestMethod]
        public void SaveSoftResetAttemptsOtherSettingsNotChanged()
        {
            const Byte attempts = 6;

            hwdg.SetSoftResetAttempts(attempts);
            Assert.AreEqual(attempts, hwdg.GetStatus().SoftResetAttempts);
            Assert.AreEqual(Response.SaveCurrentSettingsOk, hwdg.SaveCurrentSettingsAsUsers());
            Assert.AreEqual(DefaultSettings.HardResetAttempts, hwdg.GetStatus().HardResetAttempts);
            Assert.AreEqual(DefaultSettings.ResponseTimeout, hwdg.GetStatus().ResponseTimeout);
            Assert.AreEqual(DefaultSettings.RebootTimeout, hwdg.GetStatus().RebootTimeout);
            Assert.AreEqual(DefaultSettings.LedDisabled, hwdg.GetStatus().State.HasFlag(WatchdogState.LedDisabled));
            Assert.AreEqual(DefaultSettings.HardResetEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.HardResetEnabled));
            Assert.AreEqual(DefaultSettings.EventsEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.EventsEnabled));
            Assert.AreEqual(DefaultSettings.RstPulseEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.RstPulseEnabled));
            Assert.AreEqual(DefaultSettings.PwrPulseEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.PwrPulseEnabled));
            Assert.AreEqual(DefaultSettings.ApplySettingsAtStartup, hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
        }

        [TestMethod]
        public void SaveResponseTimeoutOtherSettingsNotChanged()
        {
            const Int32 timeout = 20000;

            hwdg.SetResponseTimeout(timeout);
            Assert.AreEqual(timeout, hwdg.GetStatus().ResponseTimeout);
            Assert.AreEqual(Response.SaveCurrentSettingsOk, hwdg.SaveCurrentSettingsAsUsers());
            Assert.AreEqual(DefaultSettings.HardResetAttempts, hwdg.GetStatus().HardResetAttempts);
            Assert.AreEqual(DefaultSettings.SoftResetAttempts, hwdg.GetStatus().SoftResetAttempts);
            Assert.AreEqual(DefaultSettings.RebootTimeout, hwdg.GetStatus().RebootTimeout);
            Assert.AreEqual(DefaultSettings.LedDisabled, hwdg.GetStatus().State.HasFlag(WatchdogState.LedDisabled));
            Assert.AreEqual(DefaultSettings.HardResetEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.HardResetEnabled));
            Assert.AreEqual(DefaultSettings.EventsEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.EventsEnabled));
            Assert.AreEqual(DefaultSettings.RstPulseEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.RstPulseEnabled));
            Assert.AreEqual(DefaultSettings.PwrPulseEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.PwrPulseEnabled));
            Assert.AreEqual(DefaultSettings.ApplySettingsAtStartup, hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
        }

        [TestMethod]
        public void SaveRebootTimeoutOtherSettingsNotChanged()
        {
            const Int32 timeout = 335000;

            hwdg.SetRebootTimeout(timeout);
            Assert.AreEqual(timeout, hwdg.GetStatus().RebootTimeout);
            Assert.AreEqual(Response.SaveCurrentSettingsOk, hwdg.SaveCurrentSettingsAsUsers());
            Assert.AreEqual(DefaultSettings.HardResetAttempts, hwdg.GetStatus().HardResetAttempts);
            Assert.AreEqual(DefaultSettings.SoftResetAttempts, hwdg.GetStatus().SoftResetAttempts);
            Assert.AreEqual(DefaultSettings.ResponseTimeout, hwdg.GetStatus().ResponseTimeout);
            Assert.AreEqual(DefaultSettings.LedDisabled, hwdg.GetStatus().State.HasFlag(WatchdogState.LedDisabled));
            Assert.AreEqual(DefaultSettings.HardResetEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.HardResetEnabled));
            Assert.AreEqual(DefaultSettings.EventsEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.EventsEnabled));
            Assert.AreEqual(DefaultSettings.RstPulseEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.RstPulseEnabled));
            Assert.AreEqual(DefaultSettings.PwrPulseEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.PwrPulseEnabled));
            Assert.AreEqual(DefaultSettings.ApplySettingsAtStartup, hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
        }

        [TestMethod]
        public void SaveLedDisabledOtherSettingsNotChanged()
        {
            hwdg.DisableLed();
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.LedDisabled));
            Assert.AreEqual(Response.SaveCurrentSettingsOk, hwdg.SaveCurrentSettingsAsUsers());
            Assert.AreEqual(DefaultSettings.HardResetAttempts, hwdg.GetStatus().HardResetAttempts);
            Assert.AreEqual(DefaultSettings.SoftResetAttempts, hwdg.GetStatus().SoftResetAttempts);
            Assert.AreEqual(DefaultSettings.ResponseTimeout, hwdg.GetStatus().ResponseTimeout);
            Assert.AreEqual(DefaultSettings.RebootTimeout, hwdg.GetStatus().RebootTimeout);
            Assert.AreEqual(DefaultSettings.HardResetEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.HardResetEnabled));
            Assert.AreEqual(DefaultSettings.EventsEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.EventsEnabled));
            Assert.AreEqual(DefaultSettings.RstPulseEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.RstPulseEnabled));
            Assert.AreEqual(DefaultSettings.PwrPulseEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.PwrPulseEnabled));
            Assert.AreEqual(DefaultSettings.ApplySettingsAtStartup, hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
        }

        [TestMethod]
        public void SaveLedEnabledOtherSettingsNotChanged()
        {
            hwdg.EnableLed();
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.LedDisabled));
            Assert.AreEqual(Response.SaveCurrentSettingsOk, hwdg.SaveCurrentSettingsAsUsers());
            Assert.AreEqual(DefaultSettings.HardResetAttempts, hwdg.GetStatus().HardResetAttempts);
            Assert.AreEqual(DefaultSettings.SoftResetAttempts, hwdg.GetStatus().SoftResetAttempts);
            Assert.AreEqual(DefaultSettings.ResponseTimeout, hwdg.GetStatus().ResponseTimeout);
            Assert.AreEqual(DefaultSettings.RebootTimeout, hwdg.GetStatus().RebootTimeout);
            Assert.AreEqual(DefaultSettings.HardResetEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.HardResetEnabled));
            Assert.AreEqual(DefaultSettings.EventsEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.EventsEnabled));
            Assert.AreEqual(DefaultSettings.RstPulseEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.RstPulseEnabled));
            Assert.AreEqual(DefaultSettings.PwrPulseEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.PwrPulseEnabled));
            Assert.AreEqual(DefaultSettings.ApplySettingsAtStartup, hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
        }

        [TestMethod]
        public void SaveHardResetDisabledOtherSettingsNotChanged()
        {
            hwdg.DisableHardReset();
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.HardResetEnabled));
            Assert.AreEqual(Response.SaveCurrentSettingsOk, hwdg.SaveCurrentSettingsAsUsers());
            Assert.AreEqual(DefaultSettings.HardResetAttempts, hwdg.GetStatus().HardResetAttempts);
            Assert.AreEqual(DefaultSettings.SoftResetAttempts, hwdg.GetStatus().SoftResetAttempts);
            Assert.AreEqual(DefaultSettings.ResponseTimeout, hwdg.GetStatus().ResponseTimeout);
            Assert.AreEqual(DefaultSettings.RebootTimeout, hwdg.GetStatus().RebootTimeout);
            Assert.AreEqual(DefaultSettings.LedDisabled, hwdg.GetStatus().State.HasFlag(WatchdogState.LedDisabled));
            Assert.AreEqual(DefaultSettings.EventsEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.EventsEnabled));
            Assert.AreEqual(DefaultSettings.RstPulseEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.RstPulseEnabled));
            Assert.AreEqual(DefaultSettings.PwrPulseEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.PwrPulseEnabled));
            Assert.AreEqual(DefaultSettings.ApplySettingsAtStartup, hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
        }

        [TestMethod]
        public void SaveHardResetEnabledOtherSettingsNotChanged()
        {
            hwdg.EnableHardReset();
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.HardResetEnabled));
            Assert.AreEqual(Response.SaveCurrentSettingsOk, hwdg.SaveCurrentSettingsAsUsers());
            Assert.AreEqual(DefaultSettings.HardResetAttempts, hwdg.GetStatus().HardResetAttempts);
            Assert.AreEqual(DefaultSettings.SoftResetAttempts, hwdg.GetStatus().SoftResetAttempts);
            Assert.AreEqual(DefaultSettings.ResponseTimeout, hwdg.GetStatus().ResponseTimeout);
            Assert.AreEqual(DefaultSettings.RebootTimeout, hwdg.GetStatus().RebootTimeout);
            Assert.AreEqual(DefaultSettings.LedDisabled, hwdg.GetStatus().State.HasFlag(WatchdogState.LedDisabled));
            Assert.AreEqual(DefaultSettings.EventsEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.EventsEnabled));
            Assert.AreEqual(DefaultSettings.RstPulseEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.RstPulseEnabled));
            Assert.AreEqual(DefaultSettings.PwrPulseEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.PwrPulseEnabled));
            Assert.AreEqual(DefaultSettings.ApplySettingsAtStartup, hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
        }

        [TestMethod]
        public void SaveEventsDisabledOtherSettingsNotChanged()
        {
            hwdg.DisableEvents();
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.EventsEnabled));
            Assert.AreEqual(Response.SaveCurrentSettingsOk, hwdg.SaveCurrentSettingsAsUsers());
            Assert.AreEqual(DefaultSettings.HardResetAttempts, hwdg.GetStatus().HardResetAttempts);
            Assert.AreEqual(DefaultSettings.SoftResetAttempts, hwdg.GetStatus().SoftResetAttempts);
            Assert.AreEqual(DefaultSettings.ResponseTimeout, hwdg.GetStatus().ResponseTimeout);
            Assert.AreEqual(DefaultSettings.RebootTimeout, hwdg.GetStatus().RebootTimeout);
            Assert.AreEqual(DefaultSettings.LedDisabled, hwdg.GetStatus().State.HasFlag(WatchdogState.LedDisabled));
            Assert.AreEqual(DefaultSettings.HardResetEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.HardResetEnabled));
            Assert.AreEqual(DefaultSettings.RstPulseEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.RstPulseEnabled));
            Assert.AreEqual(DefaultSettings.PwrPulseEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.PwrPulseEnabled));
            Assert.AreEqual(DefaultSettings.ApplySettingsAtStartup, hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
        }

        [TestMethod]
        public void SaveEventsEnabledOtherSettingsNotChanged()
        {
            hwdg.EnableEvents();
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.EventsEnabled));
            Assert.AreEqual(Response.SaveCurrentSettingsOk, hwdg.SaveCurrentSettingsAsUsers());
            Assert.AreEqual(DefaultSettings.HardResetAttempts, hwdg.GetStatus().HardResetAttempts);
            Assert.AreEqual(DefaultSettings.SoftResetAttempts, hwdg.GetStatus().SoftResetAttempts);
            Assert.AreEqual(DefaultSettings.ResponseTimeout, hwdg.GetStatus().ResponseTimeout);
            Assert.AreEqual(DefaultSettings.RebootTimeout, hwdg.GetStatus().RebootTimeout);
            Assert.AreEqual(DefaultSettings.LedDisabled, hwdg.GetStatus().State.HasFlag(WatchdogState.LedDisabled));
            Assert.AreEqual(DefaultSettings.HardResetEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.HardResetEnabled));
            Assert.AreEqual(DefaultSettings.RstPulseEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.RstPulseEnabled));
            Assert.AreEqual(DefaultSettings.PwrPulseEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.PwrPulseEnabled));
            Assert.AreEqual(DefaultSettings.ApplySettingsAtStartup, hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
        }

        [TestMethod]
        public void SaveRstPulseDisabledOtherSettingsNotChanged()
        {
            hwdg.RstPulseOnStartupDisable();
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.RstPulseEnabled));
            Assert.AreEqual(Response.SaveCurrentSettingsOk, hwdg.SaveCurrentSettingsAsUsers());
            Assert.AreEqual(DefaultSettings.HardResetAttempts, hwdg.GetStatus().HardResetAttempts);
            Assert.AreEqual(DefaultSettings.SoftResetAttempts, hwdg.GetStatus().SoftResetAttempts);
            Assert.AreEqual(DefaultSettings.ResponseTimeout, hwdg.GetStatus().ResponseTimeout);
            Assert.AreEqual(DefaultSettings.RebootTimeout, hwdg.GetStatus().RebootTimeout);
            Assert.AreEqual(DefaultSettings.LedDisabled, hwdg.GetStatus().State.HasFlag(WatchdogState.LedDisabled));
            Assert.AreEqual(DefaultSettings.EventsEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.EventsEnabled));
            Assert.AreEqual(DefaultSettings.HardResetEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.HardResetEnabled));
            Assert.AreEqual(DefaultSettings.PwrPulseEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.PwrPulseEnabled));
            Assert.AreEqual(DefaultSettings.ApplySettingsAtStartup, hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
        }

        [TestMethod]
        public void SaveRstPulseEnabledOtherSettingsNotChanged()
        {
            hwdg.RstPulseOnStartupEnable();
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.RstPulseEnabled));
            Assert.AreEqual(Response.SaveCurrentSettingsOk, hwdg.SaveCurrentSettingsAsUsers());
            Assert.AreEqual(DefaultSettings.HardResetAttempts, hwdg.GetStatus().HardResetAttempts);
            Assert.AreEqual(DefaultSettings.SoftResetAttempts, hwdg.GetStatus().SoftResetAttempts);
            Assert.AreEqual(DefaultSettings.ResponseTimeout, hwdg.GetStatus().ResponseTimeout);
            Assert.AreEqual(DefaultSettings.RebootTimeout, hwdg.GetStatus().RebootTimeout);
            Assert.AreEqual(DefaultSettings.LedDisabled, hwdg.GetStatus().State.HasFlag(WatchdogState.LedDisabled));
            Assert.AreEqual(DefaultSettings.EventsEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.EventsEnabled));
            Assert.AreEqual(DefaultSettings.HardResetEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.HardResetEnabled));
            Assert.AreEqual(DefaultSettings.PwrPulseEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.PwrPulseEnabled));
            Assert.AreEqual(DefaultSettings.ApplySettingsAtStartup, hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
        }

        [TestMethod]
        public void SavePwrPulseDisabledOtherSettingsNotChanged()
        {
            hwdg.PwrPulseOnStartupDisable();
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.PwrPulseEnabled));
            Assert.AreEqual(Response.SaveCurrentSettingsOk, hwdg.SaveCurrentSettingsAsUsers());
            Assert.AreEqual(DefaultSettings.HardResetAttempts, hwdg.GetStatus().HardResetAttempts);
            Assert.AreEqual(DefaultSettings.SoftResetAttempts, hwdg.GetStatus().SoftResetAttempts);
            Assert.AreEqual(DefaultSettings.ResponseTimeout, hwdg.GetStatus().ResponseTimeout);
            Assert.AreEqual(DefaultSettings.RebootTimeout, hwdg.GetStatus().RebootTimeout);
            Assert.AreEqual(DefaultSettings.LedDisabled, hwdg.GetStatus().State.HasFlag(WatchdogState.LedDisabled));
            Assert.AreEqual(DefaultSettings.EventsEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.EventsEnabled));
            Assert.AreEqual(DefaultSettings.HardResetEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.HardResetEnabled));
            Assert.AreEqual(DefaultSettings.RstPulseEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.RstPulseEnabled));
            Assert.AreEqual(DefaultSettings.ApplySettingsAtStartup, hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
        }

        [TestMethod]
        public void SavePwrPulseEnabledOtherSettingsNotChanged()
        {
            hwdg.PwrPulseOnStartupEnable();
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.PwrPulseEnabled));
            Assert.AreEqual(Response.SaveCurrentSettingsOk, hwdg.SaveCurrentSettingsAsUsers());
            Assert.AreEqual(DefaultSettings.HardResetAttempts, hwdg.GetStatus().HardResetAttempts);
            Assert.AreEqual(DefaultSettings.SoftResetAttempts, hwdg.GetStatus().SoftResetAttempts);
            Assert.AreEqual(DefaultSettings.ResponseTimeout, hwdg.GetStatus().ResponseTimeout);
            Assert.AreEqual(DefaultSettings.RebootTimeout, hwdg.GetStatus().RebootTimeout);
            Assert.AreEqual(DefaultSettings.LedDisabled, hwdg.GetStatus().State.HasFlag(WatchdogState.LedDisabled));
            Assert.AreEqual(DefaultSettings.EventsEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.EventsEnabled));
            Assert.AreEqual(DefaultSettings.HardResetEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.HardResetEnabled));
            Assert.AreEqual(DefaultSettings.RstPulseEnabled, hwdg.GetStatus().State.HasFlag(WatchdogState.RstPulseEnabled));
            Assert.AreEqual(DefaultSettings.ApplySettingsAtStartup, hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
        }

        [TestMethod]
        public void VerifyRstAndPwrEnabledOnStartup()
        {
            hwdg.RstPulseOnStartupEnable();
            hwdg.PwrPulseOnStartupEnable();
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.RstPulseEnabled));
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.PwrPulseEnabled));
            hwdg.ChipResetAndWaitForReady(6200);
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.RstPulseEnabled));
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.PwrPulseEnabled));
        }
    }
}