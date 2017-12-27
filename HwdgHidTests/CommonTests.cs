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
            var infos = HidDevices.GetDeviceInfos();
            using (var device = new HidDevice(infos.FirstOrDefault(x => x.ProductId == 0x5711 && x.VendorId == 0x0483)))
            {
                var rp=device.ReadReport(1);
            }
        }
    }
}
