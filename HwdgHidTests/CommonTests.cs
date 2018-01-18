using System;
using System.Linq;
using HwdgHid;
using HwdgHid.Win32;
using HwdgWrapper;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace HwdgHidTests
{
    [TestClass]
    public class CommonTests
    {
        [TestMethod]
        public void CheckHid()
        {
            var infos = HidDeviceFactory.GetDeviceInfos();
            using (var device = new HidDevice(infos.First(x => x.ProductId == 0x5711 && x.VendorId == 0x0483)))
            {
                var t = device.GetFeatureReport(1);
                device.SendReport(new Report {ReportId = 1, Data = new Byte[] {0x01}});
                t = device.GetFeatureReport(1);
                device.SendReport(new Report {ReportId = 1, Data = new Byte[] { 0xFA } });
                device.SendReport(new Report {ReportId = 1, Data = new Byte[] { 0x7F } });
                var rp = device.GetReport(1);
                device.SendFeatureReport(new Report { ReportId = 1, Data = new Byte[] { 00 } });
                rp = device.GetFeatureReport(1);
            }
        }

        [TestMethod]
        public void SendCmd()
        {
            var infos = HidDeviceFactory.GetDeviceInfos();
            using (var device = new HidDevice(infos.First(x => x.ProductId == 0x5711 && x.VendorId == 0x0483)))
            {
                var wrap = new HidWrapper(device);
                using (var hwdg = new SerialHwdg(wrap))
                {
                    var rsp = hwdg.DisableLed();
                    rsp = hwdg.EnableLed();
                    rsp = hwdg.DisableLed();
                    rsp = hwdg.DisableLed();
                    rsp = hwdg.EnableLed();
                    rsp = hwdg.DisableLed();
                }
            }
        }
    }
}
