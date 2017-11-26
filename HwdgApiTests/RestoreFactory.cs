using System;
using HwdgWrapper;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace HwdgApiTests
{
    [TestClass]
    public class RestoreFactory
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
            const Byte attempts = 5;
            const Int32 timeout = 125000;

            hwdg.FactoryResetAndWaitForReady();
            hwdg.SetSoftResetAttempts(attempts);
            hwdg.SetHardResetAttempts(attempts);
            hwdg.SetResponseTimeout(timeout);
            hwdg.SetRebootTimeout(timeout);
            hwdg.DisableLed();
            //No events here, as they cannot be used with HWDG wrapper.
            hwdg.EnableHardReset();
            hwdg.RstPulseOnStartupEnable();
            hwdg.PwrPulseOnStartupEnable();
            hwdg.SaveCurrentSettingsAsUsers();
            hwdg.ApplyUserSettingsAtStartup();
        }

        [TestMethod]
        public void CanRestoreFactoryWhenIdle()
        {
            hwdg.FactoryResetAndWaitForReady();
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.IsRunning));
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
        public void CanRestoreFactoryWhenMonitoring()
        {
            hwdg.Start();
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.IsRunning));
            hwdg.FactoryResetAndWaitForReady();
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.IsRunning));
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
        public void CanRestoreFactoryWhenWaitingForReboot()
        {
            const Int32 timeout = 5000;

            hwdg.SetResponseTimeout(timeout);
            hwdg.Start();
            hwdg.WaitForFlag(WatchdogState.WaitingForReboot);

            hwdg.FactoryResetAndWaitForReady();
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.IsRunning));
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.WaitingForReboot));
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
    }
}