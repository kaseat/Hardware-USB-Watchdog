// Copyright 2017 Oleg Petrochenko
//
// This file is part of HwdgHid.
//
// HwdgHid is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or any
// later version.
//
// HwdgHid is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with HwdgHid. If not, see <http://www.gnu.org/licenses/>.

using System;
using System.Collections.Generic;
using System.Linq;

namespace HwdgHid.Win32
{
    /// <summary>
    /// Represents Win32 IHidDevice implementation.
    /// </summary>
    public class HidDevice : IHidDevice, IDisposable
    {
        private readonly IntPtr deviceHandle;

        public HidDevice(DeviceInfo deviceInfo)
        {
            Info = deviceInfo;

            if ((deviceHandle = OpenDeviceReadWrite(Info.Path)) == new IntPtr(-1))
                throw new InvalidOperationException("Could not connect the device!");
        }

        public DeviceInfo Info { get; }

        public void SendReport(Report report)
        {
            if (report == null) throw new NullReferenceException(nameof(report));

            var expectedDataLength = Info.Capabilities.OutputReportByteLength;
            if (expectedDataLength <= 0) throw new NotSupportedException("The device not support output reports.");

            var reportDataLength = report.Data.Count() + 1;
            if (reportDataLength != expectedDataLength) throw new ArgumentOutOfRangeException(
                $"report.Data size is {reportDataLength}, but expected {expectedDataLength}");

            var buffer = new List<Byte>(expectedDataLength) {report.ReportId};
            buffer.AddRange(report.Data);
            if (!Wrapper.HidD_SetOutputReport(deviceHandle, buffer.ToArray(),
                Math.Min(buffer.Count, expectedDataLength)))
                throw new InvalidOperationException("Failed to write the report");
        }

        public Report GetReport(Byte reportId)
        {
            var buff = new Byte[Info.Capabilities.InputReportByteLength];
            buff[0] = reportId;

            return Wrapper.HidD_GetInputReport(deviceHandle, out buff[0], buff.Length)
                ? new Report { Data = buff.Skip(1), ReportId = buff[0] }
                : null;
        }

        public void SendFeatureReport(Report report)
        {
            if (report == null) throw new NullReferenceException(nameof(report));

            var expectedDataLength = Info.Capabilities.FeatureReportByteLength;
            if (expectedDataLength <= 0)
                throw new InvalidOperationException("The device not support output feature reports.");

            var reportDataLength = report.Data.Count() + 1;
            if (reportDataLength != expectedDataLength) throw new ArgumentOutOfRangeException(
                $"report.Data size is {reportDataLength}, but expected {expectedDataLength}");

            var buffer = new List<Byte>(expectedDataLength) { report.ReportId };
            buffer.AddRange(report.Data);
            if (!Wrapper.HidD_SetFeature(deviceHandle, buffer.ToArray(),
                Math.Min(buffer.Count, expectedDataLength)))
                throw new InvalidOperationException("Failed to send feature report");
        }

        public Report GetFeatureReport(Byte reportId)
        {
            var buff = new Byte[Info.Capabilities.InputReportByteLength];
            buff[0] = reportId;

            return Wrapper.HidD_GetFeature(deviceHandle, out buff[0], buff.Length)
                ? new Report { Data = buff.Skip(1), ReportId = buff[0] }
                : null;
        }

        private void ReleaseUnmanagedResources()
        {
            deviceHandle.DisposeDeviceHandle();
        }

        public void Dispose()
        {
            ReleaseUnmanagedResources();
            GC.SuppressFinalize(this);
        }

        ~HidDevice()
        {
            ReleaseUnmanagedResources();
        }

        /// <summary>
        /// Get device handle.
        /// </summary>
        /// <param name="devicePath">Path to the device.</param>
        /// <returns>Returns device handle.</returns>
        private static IntPtr OpenDeviceReadWrite(String devicePath)
        {
            var securityAttributes = SecurityAttributes.Initialize();
            return Wrapper.CreateFile(devicePath, FileAccess.GenericRead | FileAccess.GenericWrite,
                ShareMode.Exclusive, ref securityAttributes, OpenMode.OpenExisting,
                FileAttributes.FileFlagOverlapped, IntPtr.Zero);
        }
    }
}