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
    /// The HIDD_ATTRIBUTES structure contains vendor information about a HIDClass device.
    /// </summary>
    /// <remarks>See https://msdn.microsoft.com/en-us/library/windows/hardware/ff538868.aspx
    /// for more info.
    /// A caller of HidD_GetAttributes, uses this structure
    /// to obtain a device's vendor information.
    ///
    /// Before using a HIDD_ATTRIBUTES structure with HIDClass
    /// support routines, the caller must set the cbSize member.</remarks>
    [StructLayout(LayoutKind.Sequential)]
    internal struct HiddAttributes
    {
        /// <summary>
        /// Get initialized instance of <see cref="HiddAttributes"/> struct.
        /// </summary>
        /// <returns>Returns Initialized <see cref="HiddAttributes"/> struct.</returns>
        internal static HiddAttributes Initialize() =>
            new HiddAttributes(Marshal.SizeOf(typeof(HiddAttributes)));

        /// <summary>
        /// Create the struct with the specified Size value.
        /// </summary>
        /// <param name="size">Size value.</param>
        private HiddAttributes(Int32 size)
        {
            Size = size;
            VendorID = 0;
            ProductID = 0;
            VersionNumber = 0;
        }

        /// <summary>
        /// Specifies the size, in bytes, of a HIDD_ATTRIBUTES structure.
        /// </summary>
        internal readonly Int32 Size;

        /// <summary>
        /// Specifies a HID device's vendor ID.
        /// </summary>
        internal readonly UInt16 VendorID;

        /// <summary>
        /// Specifies a HID device's product ID.
        /// </summary>
        internal readonly UInt16 ProductID;

        /// <summary>
        /// Specifies the manufacturer's revision number for a HIDClass device.
        /// </summary>
        internal readonly Int16 VersionNumber;
    }
}