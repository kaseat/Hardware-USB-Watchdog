﻿using System;
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
            var infos = HidDeviceFactory.GetDeviceInfos();
            using (var device = new HidDevice(infos.First(x => x.ProductId == 0x5711 && x.VendorId == 0x0483)))
            {
                device.SendReport(new Report {ReportId = 1, Data = new Byte[] {1}});
                device.SendReport(new Report {ReportId = 1, Data = new Byte[] {2}});
                device.SendReport(new Report {ReportId = 1, Data = new Byte[] {3}});
                var rp = device.GetReport(1);
                device.SendFeatureReport(new Report { ReportId = 1, Data = new Byte[] { 00 } });
                rp = device.GetFeatureReport(1);
            }
        }
    }
}
