using System;
using System.Linq;
using HwdgHid;
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
            using (var device = new HidDevice(infos.First(x => x.ProductId == 0x5711 && x.VendorId == 0x0483)))
            {
                device.WriteReport(new Report {ReportId = 1, Data = new Byte[] {33}});
                var rp = device.ReadReport(1);
            }
        }
    }
}
