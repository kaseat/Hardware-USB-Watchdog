using System;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using HwdgApi;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Moq;

namespace HwdgApiTests
{
    [TestClass]
    public class UnitTest1
    {
        [TestMethod]
        public void VerifyHwdgHardResetEnabled()
        {
            // Arrange
            var mock = new Mock<IHwdgProvider>();
            mock.Setup(x => x.Start()).Returns(Task.FromResult(Response.StartOk));
            mock.Setup(x => x.GetStatus()).Returns(Task.FromResult(new Status(new Byte[] {0x9C, 0x05, 0x0A, 0x3A})));
            var hw = new Hwdg(mock.Object);

            // Act
            hw.MonitoringEnabled = true;

            //Assert
            mock.Verify(x => x.Start(), Times.Once);
        }
    }
}
