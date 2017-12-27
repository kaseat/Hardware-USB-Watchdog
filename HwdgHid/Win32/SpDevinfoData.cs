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
    /// An SP_DEVINFO_DATA structure defines a device
    /// instance that is a member of a device information set.
    /// </summary>
    /// <remarks>See https://msdn.microsoft.com/en-us/library/windows/hardware/ff552344.aspx
    /// for more info</remarks>
    [StructLayout(LayoutKind.Sequential)]
    internal struct SpDevinfoData
    {
        /// <summary>
        /// Create the struct with the specified cbSize value.
        /// </summary>
        /// <param name="size">cbSize value.</param>
        public SpDevinfoData(Int32 size)
        {
            cbSize = size;
            ClassGuid = default(Guid);
            DevInst = default(Int32);
            Reserved = default(IntPtr);
        }

        /// <summary>
        /// The size, in bytes, of the SP_DEVINFO_DATA
        /// structure. For more information, see the
        /// following Remarks section.
        /// </summary>
        internal readonly Int32 cbSize;

        /// <summary>
        /// The GUID of the device's setup class.
        /// </summary>
        internal readonly Guid ClassGuid;

        /// <summary>
        /// An opaque handle to the device instance
        /// (also known as a handle to the devnode).
        ///
        /// Some functions, such as SetupDiXxx functions,
        /// take the whole SP_DEVINFO_DATA structure as
        /// input to identify a device in a device information
        /// set. Other functions, such as CM_Xxx functions
        /// like CM_Get_DevNode_Status, take this DevInst
        /// handle as input.
        /// </summary>
        internal readonly Int32 DevInst;

        /// <summary>
        /// Reserved. For internal use only.
        /// </summary>
        internal readonly IntPtr Reserved;
    }
}