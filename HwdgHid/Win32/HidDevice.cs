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
using System.IO;
using System.Linq;
using System.Runtime.Remoting.Messaging;
using Microsoft.Win32.SafeHandles;

namespace HwdgHid.Win32
{
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

        public void WriteReport(Report report)
        {
            var buffer = new List<Byte>(Info.Capabilities.OutputReportByteLength) {report.ReportId};
            buffer.AddRange(report.Data);
            if (!Wrapper.HidD_SetOutputReport(deviceHandle, buffer.ToArray(),
                Math.Min(buffer.Count, Info.Capabilities.OutputReportByteLength)))
                throw new InvalidOperationException("Failed to write the report");
        }

        public Report ReadReport(Byte reportId)
        {
            var buff = new Byte[Info.Capabilities.InputReportByteLength];
            buff[0] = reportId;

            return Wrapper.HidD_GetInputReport(deviceHandle, out buff[0], buff.Length)
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