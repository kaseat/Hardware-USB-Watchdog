using HwdgWrapper;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Rhino.Mocks;

namespace HwdgWrapperTests
{
    [TestClass]
    public class SerialHwdgTests
    {
        [TestMethod]
        public void VerifyEnableHardResetSendsCorrectCommand()
        {
            var wrapper = MockRepository.GenerateMock<IWrapper>();
            using (var hwdg = new SerialHwdg(wrapper))
            {
                hwdg.EnableHardReset();
                wrapper.AssertWasCalled(x => x.SendCommand(0x03), z => z.Repeat.Once());
            }
        }

        [TestMethod]
        public void VerifyDisableHardResetSendsCorrectCommand()
        {
            var wrapper = MockRepository.GenerateMock<IWrapper>();
            using (var hwdg = new SerialHwdg(wrapper))
            {
                hwdg.DisableHardReset();
                wrapper.AssertWasCalled(x => x.SendCommand(0x04), z => z.Repeat.Once());
            }
        }

        [TestMethod]
        public void VerifySetRebootTimeoutConversionCorrect()
        {
            var wrapper = MockRepository.GenerateMock<IWrapper>();
            using (var hwdg = new SerialHwdg(wrapper))
            {
                hwdg.SetRebootTimeout(440000);
                wrapper.AssertWasCalled(x => x.SendCommand(0xD6), z => z.Repeat.Once());
            }
        }

        [TestMethod]
        public void VerifySetResponseTimeoutConversionCorrect()
        {
            var wrapper = MockRepository.GenerateMock<IWrapper>();
            using (var hwdg = new SerialHwdg(wrapper))
            {
                hwdg.SetResponseTimeout(100000);
                wrapper.AssertWasCalled(x => x.SendCommand(0x53), z => z.Repeat.Once());
            }
        }

        [TestMethod]
        public void VerifySetHardResetAttemptsConversionCorrect()
        {
            var wrapper = MockRepository.GenerateMock<IWrapper>();
            using (var hwdg = new SerialHwdg(wrapper))
            {
                hwdg.SetHardResetAttempts(6);
                wrapper.AssertWasCalled(x => x.SendCommand(0x1D), z => z.Repeat.Once());
            }
        }

        [TestMethod]
        public void VerifySetSoftResetAttemptsConversionCorrect()
        {
            var wrapper = MockRepository.GenerateMock<IWrapper>();
            using (var hwdg = new SerialHwdg(wrapper))
            {
                hwdg.SetSoftResetAttempts(4);
                wrapper.AssertWasCalled(x => x.SendCommand(0x13), z => z.Repeat.Once());
            }
        }
    }
}
