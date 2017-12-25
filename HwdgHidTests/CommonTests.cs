using System;
using System.Linq;
using HwdgHid.Win32;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace HwdgHidTests
{
    [TestClass]
    public class CommonTests
    {
        [TestMethod]
        public void CheckHid()
        {
            var infos = HidDevices.GetDeviceInfos().ToList();
        }
    }
}
