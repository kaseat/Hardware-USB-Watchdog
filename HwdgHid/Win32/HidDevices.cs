// Copyright 2017 Oleg Petrochenko
//
// This file is part of HwdgHid.
//
// HwdgHid is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// HwdgHid is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace HwdgHid.Win32
{
    /// <summary>
    /// Represents HID devices communication class.
    /// </summary>
    public static class HidDevices
    {
        /// <summary>
        /// Get info about all connected USB HID devices.
        /// </summary>
        /// <returns>Returns collection of <see cref="DeviceInfo"/>.</returns>
        public static IEnumerable<DeviceInfo> GetDeviceInfos()
        {
            // Get GUID for HIDClass devices.
            var hidClass = Guid.Empty;
            Wrapper.HidD_GetHidGuid(ref hidClass);

            // Get a handle to a device information set of connected HID devices.
            var deviceInfoSet = Wrapper.SetupDiGetClassDevs(ref hidClass, IntPtr.Zero, IntPtr.Zero,
                SetupDiGetClassDevsFlags.DigcfPresent | SetupDiGetClassDevsFlags.DigcfDeviceinterface);

            // Exit if device information set fetching failed.
            if (deviceInfoSet == new IntPtr(-1)) yield break;

            // Now just iterate over all connected devices and all
            // device interfaces in order to get info about connected devices.
            var deviceInfoData = new SpDevinfoData(Marshal.SizeOf(typeof(SpDeviceInterfaceData)));
            for (var i = 0; Wrapper.SetupDiEnumDeviceInfo(deviceInfoSet, i, ref deviceInfoData); i++)
            {
                var interfaceData = SpDeviceInterfaceData.Initialize();
                for (var j = 0; Wrapper.SetupDiEnumDeviceInterfaces(deviceInfoSet, ref deviceInfoData,
                        ref hidClass, j, ref interfaceData); j++)
                {
                    var devicePath = GetDevicePath(deviceInfoSet, interfaceData);
                    var deviceHandle = GetDeviceHandle(devicePath);

                    if (deviceHandle == new IntPtr(-1)) continue;

                    var attributes = GetDeviceAttributes(deviceHandle);
                    var productName = GetProductString(deviceHandle);
                    var manufacturer = GetManufacturerString(deviceHandle);
                    var capabilities = GetDeviceCapabilities(deviceHandle);
                    DisposeDeviceHandle(deviceHandle);

                    yield return new DeviceInfo
                    {
                        Path = devicePath,
                        Handle = deviceHandle,
                        Version = attributes.VersionNumber,
                        VendorId = attributes.VendorID,
                        ProductId = attributes.ProductID,
                        ProductName = productName,
                        Manufacturer = manufacturer,
                        Capabilities = capabilities
                    };
                }
            }
            Wrapper.SetupDiDestroyDeviceInfoList(deviceInfoSet);
        }

        /// <summary>
        /// Get system HID device path.
        /// </summary>
        /// <param name="inf">A pointer to the device information set that
        /// contains the interface for which to retrieve details.</param>
        /// <param name="dat">SP_DEVICE_INTERFACE_DATA structure.</param>
        /// <returns>Returns device path.</returns>
        private static String GetDevicePath(IntPtr inf, SpDeviceInterfaceData dat)
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
        private static IntPtr GetDeviceHandle(String devicePath)
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
        private static HiddAttributes GetDeviceAttributes(IntPtr deviceHandle)
        {
            var attributes = HiddAttributes.Initialize();
            Wrapper.HidD_GetAttributes(deviceHandle, ref attributes);
            return attributes;
        }

        /// <summary>
        /// Close device handle.
        /// </summary>
        /// <param name="handle">Device handle to close.</param>
        private static void DisposeDeviceHandle(IntPtr handle)
        {
            if (Environment.OSVersion.Version.Major > 5)
                Wrapper.CancelIoEx(handle, IntPtr.Zero);
            Wrapper.CloseHandle(handle);
        }

        /// <summary>
        /// Get product string.
        /// </summary>
        /// <param name="deviceHandle">Device handle.</param>
        /// <returns>Returns product string</returns>
        private static String GetProductString(IntPtr deviceHandle)
        {
            var data = new Byte[254];
            return Wrapper.HidD_GetProductString(deviceHandle, ref data[0], data.Length)
                ? Encoding.Unicode.GetString(data).TrimEnd('\0')
                : null;
        }

        /// <summary>
        /// Get product string.
        /// </summary>
        /// <param name="deviceHandle">Device handle.</param>
        /// <returns>Returns product string</returns>
        private static String GetManufacturerString(IntPtr deviceHandle)
        {
            var data = new Byte[254];
            return Wrapper.HidD_GetManufacturerString(deviceHandle, ref data[0], data.Length)
                ? Encoding.Unicode.GetString(data).TrimEnd('\0')
                : null;
        }

        /// <summary>
        /// Get device capabilities.
        /// </summary>
        /// <param name="deviceHandle">Device handle.</param>
        /// <returns>Returns device capabilities.</returns>
        private static DeviceCapabilities GetDeviceCapabilities(IntPtr deviceHandle)
        {
            var capabilities = new HidpCaps();
            var preparsedData = default(IntPtr);

            if (Wrapper.HidD_GetPreparsedData(deviceHandle, ref preparsedData))
            {
                Wrapper.HidP_GetCaps(preparsedData, ref capabilities);
                Wrapper.HidD_FreePreparsedData(preparsedData);
            }
            return new DeviceCapabilities(capabilities);
        }
    }
}