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
using System.Runtime.InteropServices;

namespace HwdgHid.Win32
{
    /// <summary>
    /// An SP_DEVICE_INTERFACE_DETAIL_DATA structure contains the path for a device interface.
    /// </summary>
    /// <remarks>See https://msdn.microsoft.com/en-us/library/windows/hardware/ff552343.aspx
    /// for more info.
    /// An SP_DEVICE_INTERFACE_DETAIL_DATA structure identifies
    /// the path for a device interface in a device information set.
    ///
    /// SetupDiXxx functions that take an SP_DEVICE_INTERFACE_DETAIL_DATA
    /// structure as a parameter verify that the cbSize member of the
    /// supplied structure is equal to the size, in bytes, of the structure.
    /// If the cbSize member is not set correctly for an input parameter,
    /// the function will fail and set an error code of ERROR_INVALID_PARAMETER.
    /// If the cbSize member is not set correctly for an output parameter,
    /// the function will fail and set an error code of ERROR_INVALID_USER_BUFFER.</remarks>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Auto)]
    internal struct SpDeviceInterfaceDetailData
    {
        /// <summary>
        /// Get initialized instance of <see cref="SpDeviceInterfaceDetailData"/> struct.
        /// </summary>
        /// <returns>Returns Initialized <see cref="SpDeviceInterfaceDetailData"/> struct.</returns>
        internal static SpDeviceInterfaceDetailData Initialize() =>
            new SpDeviceInterfaceDetailData(IntPtr.Size == 4 ? 4 + Marshal.SystemDefaultCharSize : 8);

        /// <summary>
        /// Initialize this struct with cbSize field.
        /// </summary>
        /// <param name="size">cbSize value.</param>
        private SpDeviceInterfaceDetailData(Int32 size)
        {
            cbSize = size;
            DevicePath = null;
        }

        /// <summary>
        /// The size, in bytes, of the SP_DEVICE_INTERFACE_DETAIL_DATA
        /// structure. For more information, see the following Remarks section.
        /// </summary>
        internal readonly Int32 cbSize;

        /// <summary>
        /// A NULL-terminated string that contains the device interface path.
        /// This path can be passed to Win32 functions such as CreateFile.
        /// </summary>
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)] internal readonly String DevicePath;
    }
}