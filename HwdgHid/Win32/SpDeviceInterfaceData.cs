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
using System.Runtime.InteropServices;

namespace HwdgHid.Win32
{
    /// <summary>
    /// An SP_DEVICE_INTERFACE_DATA structure defines
    /// a device interface in a device information set.
    /// </summary>
    /// <remarks>See https://msdn.microsoft.com/en-us/library/windows/hardware/ff552342.aspx
    /// for more info</remarks>
    [StructLayout(LayoutKind.Sequential)]
    internal struct SpDeviceInterfaceData
    {
        /// <summary>
        /// Get initialized instance of <see cref="SpDeviceInterfaceData"/> struct.
        /// </summary>
        /// <returns>Returns Initialized <see cref="SpDeviceInterfaceData"/> struct.</returns>
        internal static SpDeviceInterfaceData Initialize() =>
            new SpDeviceInterfaceData(Marshal.SizeOf(typeof(SpDeviceInterfaceData)));

        /// <summary>
        /// Create the struct with the specified cbSize value.
        /// </summary>
        /// <param name="size">cbSize value.</param>
        private SpDeviceInterfaceData(Int32 size)
        {
            InterfaceClassGuid = default(Guid);
            Flags = default(FlagsT);
            cbSize = size;
            Reserved = default(IntPtr);
        }

        /// <summary>
        /// The size, in bytes, of the SP_DEVICE_INTERFACE_DATA structure.
        /// For more information, see the Remarks section.
        /// </summary>
        /// <remarks>A SetupAPI function that takes an instance of the
        /// SP_DEVICE_INTERFACE_DATA structure as a parameter verifies
        /// whether the cbSize member of the supplied structure is
        /// equal to the size, in bytes, of the structure. If the
        /// cbSize member is not set correctly, the function will fail
        /// and set an error code of ERROR_INVALID_USER_BUFFER.</remarks>
        internal readonly Int32 cbSize;

        /// <summary>
        /// The GUID for the class to which the device interface belongs.
        /// </summary>
        internal readonly Guid InterfaceClassGuid;

        /// <summary>
        /// SP_DEVICE_INTERFACE_DATA Flags field
        /// </summary>
        internal readonly FlagsT Flags;

        /// <summary>
        /// Reserved. Do not use.
        /// </summary>
        internal readonly IntPtr Reserved;

        /// <summary>
        /// SP_DEVICE_INTERFACE_DATA Flags field
        /// </summary>
        internal enum FlagsT
        {
            /// <summary>
            /// The interface is active (enabled).
            /// </summary>
            SpintActive = 1,

            /// <summary>
            /// The interface is the default interface for the device class.
            /// </summary>
            SpintDefault = 2,

            /// <summary>
            /// The interface is removed.
            /// </summary>
            SpintRemoved = 4
        }
    }
}