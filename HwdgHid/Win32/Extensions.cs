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
using System.Text;

namespace HwdgHid.Win32
{
    internal static class Extensions
    {
        /// <summary>
        /// Get system HID device path.
        /// </summary>
        /// <param name="inf">A pointer to the device information set that
        /// contains the interface for which to retrieve details.</param>
        /// <param name="dat">SP_DEVICE_INTERFACE_DATA structure.</param>
        /// <returns>Returns device path.</returns>
        internal static String GetDevicePath(this IntPtr inf, SpDeviceInterfaceData dat)
        {
            const Int32 bSize = 256;
            var dDat = SpDeviceInterfaceDetailData.Initialize();
            return Wrapper.SetupDiGetDeviceInterfaceDetail(inf, ref dat, ref dDat, bSize, IntPtr.Zero, IntPtr.Zero)
                ? dDat.DevicePath
                : null;
        }

        /// <summary>
        /// Get device handle.
        /// </summary>
        /// <param name="devicePath">Path to the device.</param>
        /// <returns>Returns device handle.</returns>
        internal static IntPtr GetDeviceHandle(this String devicePath)
        {
            var securityAttributes = SecurityAttributes.Initialize();
            return Wrapper.CreateFile(devicePath, FileAccess.NoAccess, ShareMode.ShareRead | ShareMode.ShareWrite,
                ref securityAttributes, OpenMode.OpenExisting, FileAttributes.FileFlagOverlapped, IntPtr.Zero);
        }

        /// <summary>
        /// Get device attributes.
        /// </summary>
        /// <param name="deviceHandle">Device handle.</param>
        /// <returns>Returns device attributes.</returns>
        internal static HiddAttributes GetDeviceAttributes(this IntPtr deviceHandle)
        {
            var attributes = HiddAttributes.Initialize();
            Wrapper.HidD_GetAttributes(deviceHandle, ref attributes);
            return attributes;
        }

        /// <summary>
        /// Close device handle.
        /// </summary>
        /// <param name="handle">Device handle to close.</param>
        internal static void DisposeDeviceHandle(this IntPtr handle)
        {
            if (handle == new IntPtr(-1) || handle == IntPtr.Zero) return;
            if (Environment.OSVersion.Version.Major > 5)
                Wrapper.CancelIoEx(handle, IntPtr.Zero);
            Wrapper.CloseHandle(handle);
        }

        /// <summary>
        /// Get product string.
        /// </summary>
        /// <param name="deviceHandle">Device handle.</param>
        /// <returns>Returns product string</returns>
        internal static String GetProductString(this IntPtr deviceHandle)
        {
            var data = new Byte[254];
            return Wrapper.HidD_GetProductString(deviceHandle, out data[0], data.Length)
                ? Encoding.Unicode.GetString(data).TrimEnd('\0')
                : null;
        }

        /// <summary>
        /// Get product string.
        /// </summary>
        /// <param name="deviceHandle">Device handle.</param>
        /// <returns>Returns product string</returns>
        internal static String GetManufacturerString(this IntPtr deviceHandle)
        {
            var data = new Byte[254];
            return Wrapper.HidD_GetManufacturerString(deviceHandle, out data[0], data.Length)
                ? Encoding.Unicode.GetString(data).TrimEnd('\0')
                : null;
        }

        /// <summary>
        /// Get device capabilities.
        /// </summary>
        /// <param name="deviceHandle">Device handle.</param>
        /// <returns>Returns device capabilities.</returns>
        internal static DeviceCapabilities GetDeviceCapabilities(this IntPtr deviceHandle)
        {
            var capabilities = new HidpCaps();

            if (Wrapper.HidD_GetPreparsedData(deviceHandle, out var preparsedData))
            {
                Wrapper.HidP_GetCaps(preparsedData, out capabilities);
                Wrapper.HidD_FreePreparsedData(preparsedData);
            }
            return new DeviceCapabilities(capabilities);
        }
    }
}