using System;
using HwdgWrapper;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace HwdgApiTests
{
    [TestClass]
    public class GetStatus
    {
        private static IWrapper hwdg;
        private const String InitialStatus = "1C444800";

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
        public void VerifyInitialStatus()
        {
            Assert.AreEqual(InitialStatus, hwdg.GetStatus().RawData.ToString("X"));
        }

        [TestMethod]
        public void VerifyStatusIsChangedWhenSetHardResetAttempts()
        {
            const String status = "1C445800";
            const Byte attempts = 7;

            hwdg.SetHardResetAttempts(attempts);
            Assert.AreEqual(status, hwdg.GetStatus().RawData.ToString("X"));
            Assert.AreEqual(attempts, hwdg.GetStatus().HardResetAttempts);
        }

        [TestMethod]
        public void VerifyStatusIsChangedWhenSetSoftResetAttempts()
        {
            const String status = "1C442800";
            const Byte attempts = 2;

            hwdg.SetSoftResetAttempts(attempts);
            Assert.AreEqual(status, hwdg.GetStatus().RawData.ToString("X8"));
            Assert.AreEqual(attempts, hwdg.GetStatus().SoftResetAttempts);
        }

        [TestMethod]
        public void VerifyStatusIsChangedWhenSetResponseTimeout()
        {
            const String status = "1CB04800";
            const Int32 timeout = 225000;

            hwdg.SetResponseTimeout(timeout);
            Assert.AreEqual(status, hwdg.GetStatus().RawData.ToString("X8"));
            Assert.AreEqual(timeout, hwdg.GetStatus().ResponseTimeout);
        }

        [TestMethod]
        public void VerifyStatusIsChangedWhenSetRebootTimeout()
        {
            const String status = "0D444800";
            const Int32 timeout = 75000;

            hwdg.SetRebootTimeout(timeout);
            Assert.AreEqual(status, hwdg.GetStatus().RawData.ToString("X8"));
            Assert.AreEqual(timeout, hwdg.GetStatus().RebootTimeout);
        }

        [TestMethod]
        public void VerifyStatusIsChangedWhenDisableLed()
        {
            const String status = "1C444801";

            hwdg.DisableLed();
            Assert.AreEqual(status, hwdg.GetStatus().RawData.ToString("X8"));
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.LedDisabled));
        }

        [TestMethod]
        public void VerifyStatusIsChangedWhenEnableHardReset()
        {
            const String status = "1C444900";

            hwdg.EnableHardReset();
            Assert.AreEqual(status, hwdg.GetStatus().RawData.ToString("X8"));
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.HardResetEnabled));
        }

        [TestMethod]
        public void VerifyStatusIsNotChangedWhenSendPing()
        {
            hwdg.Ping();
            Assert.AreEqual(InitialStatus, hwdg.GetStatus().RawData.ToString("X8"));
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.IsRunning));
        }

        [TestMethod]
        public void VerifyStatusIsNotChangedWhenSendStop()
        {
            hwdg.Stop();
            Assert.AreEqual(InitialStatus, hwdg.GetStatus().RawData.ToString("X8"));
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.IsRunning));
        }

        [TestMethod]
        public void VerifyStatusIsChangedWhenSendStart()
        {
            const String status = "1C454800";

            hwdg.Start();
            Assert.AreEqual(status, hwdg.GetStatus().RawData.ToString("X8"));
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.IsRunning));
        }

        [TestMethod]
        public void VerifyStatusIsNotChangedWhenSendIsAlive()
        {
            hwdg.IsAlive();
            Assert.AreEqual(InitialStatus, hwdg.GetStatus().RawData.ToString("X8"));
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.IsRunning));
        }

        [TestMethod]
        public void VerifyStatusIsNotChangedWhenTestSoftReset()
        {
            hwdg.TestSoftReset();
            Assert.AreEqual(InitialStatus, hwdg.GetStatus().RawData.ToString("X8"));
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.IsRunning));
        }

        [TestMethod]
        public void VerifyStatusIsNotChangedWhenTestHardReset()
        {
            hwdg.TestHardReset();
            Assert.AreEqual(InitialStatus, hwdg.GetStatus().RawData.ToString("X8"));
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.IsRunning));
        }

        [TestMethod]
        public void VerifyStatusIsNotChangedWhenGetStatus()
        {
            hwdg.GetStatus();
            Assert.AreEqual(InitialStatus, hwdg.GetStatus().RawData.ToString("X8"));
        }

        [TestMethod]
        public void VerifyStatusIsChangedWhenRstPulseOnStartupEnable()
        {
            const String status = "1C444808";

            hwdg.RstPulseOnStartupEnable();
            Assert.AreEqual(status, hwdg.GetStatus().RawData.ToString("X8"));
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.RstPulseEnabled));
        }

        [TestMethod]
        public void VerifyStatusIsChangedWhenPwrPulseOnStartupEnable()
        {
            const String status = "1C444810";

            hwdg.PwrPulseOnStartupEnable();
            Assert.AreEqual(status, hwdg.GetStatus().RawData.ToString("X8"));
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.PwrPulseEnabled));
        }

        [TestMethod]
        public void VerifyStatusIsChangedWhenLoadUserSettingsAtStartup()
        {
            const String status = "1C444804";

            hwdg.ApplyUserSettingsAtStartup();
            Assert.AreEqual(status, hwdg.GetStatus().RawData.ToString("X8"));
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
        }

        [TestMethod]
        public void VerifyStatusIsNotChangedWhenLoadDefaultSettingsAtStartup()
        {
            hwdg.LoadDefaultSettingsAtStartup();
            Assert.AreEqual(InitialStatus, hwdg.GetStatus().RawData.ToString("X8"));
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
        }

        [TestMethod]
        public void VerifyStatusIsNotChangedWhenSaveCurrentSettingsAsUsers()
        {
            hwdg.SaveCurrentSettingsAsUsers();
            Assert.AreEqual(InitialStatus, hwdg.GetStatus().RawData.ToString("X8"));
            Assert.IsFalse(hwdg.GetStatus().State.HasFlag(WatchdogState.LoadUserSettings));
        }

        [TestMethod]
        public void VerifyStatusIsChangedToInitialWhenRestoreFactory()
        {
            const Byte attempts = 4;
            const Int32 timeout = 300000;
            
            hwdg.SetSoftResetAttempts(attempts);
            hwdg.SetHardResetAttempts(attempts);
            hwdg.SetResponseTimeout(timeout);
            hwdg.SetRebootTimeout(timeout);
            hwdg.DisableLed();
            hwdg.EnableHardReset();
            hwdg.RstPulseOnStartupEnable();
            hwdg.PwrPulseOnStartupEnable();
            hwdg.SaveCurrentSettingsAsUsers();
            hwdg.ApplyUserSettingsAtStartup();

            hwdg.FactoryResetAndWaitForReady();
            Assert.AreEqual(InitialStatus, hwdg.GetStatus().RawData.ToString("X8"));
        }

        [TestMethod]
        public void VerifyStatusIsNotChangedWhenSendReservedCommand()
        {
            const Byte reserved = 0x20;

            hwdg.SendCommand(reserved);
            Assert.AreEqual(InitialStatus, hwdg.GetStatus().RawData.ToString("X8"));
        }

        [TestMethod]
        public void VerifyStatusIsNotChangedWhenSendUnknownCommand()
        {
            const Byte unknown = 0x05;

            hwdg.SendCommand(unknown);
            Assert.AreEqual(InitialStatus, hwdg.GetStatus().RawData.ToString("X8"));
        }

        [TestMethod]
        public void VerifyStatusIsChangedWhenWaitingForReboot()
        {
            const Int32 timeout = 5000;
            const String status = "1C034800";

            hwdg.SetResponseTimeout(timeout);
            hwdg.Start();
            hwdg.WaitForFlag(WatchdogState.WaitingForReboot);

            Assert.AreEqual(status, hwdg.GetStatus().RawData.ToString("X8"));
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.IsRunning));
            Assert.IsTrue(hwdg.GetStatus().State.HasFlag(WatchdogState.WaitingForReboot));
        }

        [TestMethod]
        public void VerifyStatusIsChangedWhenSetMultipleSettings()
        {
            const Byte attempts = 5;
            const Int32 timeout = 480000;
            const String status = "5E448804";

            hwdg.SetSoftResetAttempts(attempts);
            hwdg.SetRebootTimeout(timeout);
            hwdg.EnableLed();
            hwdg.DisableHardReset();
            hwdg.PwrPulseOnStartupDisable();
            hwdg.RstPulseOnStartupDisable();
            hwdg.ApplyUserSettingsAtStartup();

            Assert.AreEqual(status, hwdg.GetStatus().RawData.ToString("X8"));
        }

        [TestMethod]
        public void VerifyStatusIsChangedWithUnsavedSettingsAfterChipReset()
        {
            const Byte attempts = 1;
            const Int32 timeout = 80000;
            const String status = "0E444100";

            hwdg.SetHardResetAttempts(attempts);
            hwdg.SetRebootTimeout(timeout);
            hwdg.EnableHardReset();
            Assert.AreEqual(status, hwdg.GetStatus().RawData.ToString("X8"));

            hwdg.ChipResetAndWaitForReady();
            Assert.AreEqual(InitialStatus, hwdg.GetStatus().RawData.ToString("X8"));
        }

        [TestMethod]
        public void VerifyStatusIsNotChangedWhenSaveLoadUserSettings()
        {
            const Byte attempts = 1;
            const Int32 timeout = 80000;
            const String status = "1C3C0805";

            hwdg.SetSoftResetAttempts(attempts);
            hwdg.SetResponseTimeout(timeout);
            hwdg.DisableLed();
            hwdg.SaveCurrentSettingsAsUsers();
            hwdg.ApplyUserSettingsAtStartup();
            Assert.AreEqual(status, hwdg.GetStatus().RawData.ToString("X8"));

            hwdg.ChipResetAndWaitForReady();
            Assert.AreEqual(status, hwdg.GetStatus().RawData.ToString("X8"));
        }

        [TestMethod]
        public void VerifyStatusIsChangedWhenSaveLoadDefaultSettings()
        {
            const Byte attempts = 2;
            const Int32 timeout = 470000;
            const String status = "5C442900";

            hwdg.SetSoftResetAttempts(attempts);
            hwdg.SetRebootTimeout(timeout);
            hwdg.EnableHardReset();
            hwdg.SaveCurrentSettingsAsUsers();
            hwdg.LoadDefaultSettingsAtStartup();
            Assert.AreEqual(status, hwdg.GetStatus().RawData.ToString("X8"));

            hwdg.ChipResetAndWaitForReady();
            Assert.AreEqual(InitialStatus, hwdg.GetStatus().RawData.ToString("X8"));
        }
    }
}