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
using System.Runtime.InteropServices;

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
                // ReSharper disable once InlineOutVariableDeclaration
                // ReSharper disable once RedundantAssignment
                var interfaceData = SpDeviceInterfaceData.Initialize();
                for (var j = 0;
                    Wrapper.SetupDiEnumDeviceInterfaces(deviceInfoSet, ref deviceInfoData,
                        ref hidClass, j, out interfaceData);
                    j++)
                {
                    var devicePath = deviceInfoSet.GetDevicePath(interfaceData);
                    var deviceHandle = devicePath.GetDeviceHandle();

                    if (deviceHandle == new IntPtr(-1)) continue;

                    var attributes = deviceHandle.GetDeviceAttributes();
                    var productName = deviceHandle.GetProductString();
                    var manufacturer = deviceHandle.GetManufacturerString();
                    var capabilities = deviceHandle.GetDeviceCapabilities();

                    deviceHandle.DisposeDeviceHandle();

                    yield return new DeviceInfo
                    {
                        Path = devicePath,
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
    }
}