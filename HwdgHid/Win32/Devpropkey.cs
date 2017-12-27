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
    /// In Windows Vista and later versions of Windows,
    /// the DEVPROPKEY structure represents a device
    /// property key for a device property in the unified
    /// device property model.
    /// </summary>
    /// <remarks>See https://docs.microsoft.com/en-us/windows-hardware/drivers/install/devpropkey
    /// for more info.
    /// The DEVPROPKEY structure is part of the unified
    /// device property model.
    ///
    /// The basic set of system-supplied device property
    /// keys are defined in Devpkey.h.
    /// The DEFINE_DEVPROPKEY macro creates an instance
    /// of a DEVPROPKEY structure that represents a
    /// device property key.</remarks>
    [StructLayout(LayoutKind.Sequential)]
    internal struct Devpropkey
    {
        /// <summary>
        /// A DEVPROPGUID-typed value that specifies a property category.
        /// </summary>
        public Guid fmtid;

        /// <summary>
        /// A DEVPROPID-typed value that uniquely identifies the property
        /// within the property category. For internal system reasons, a
        /// property identifier must be greater than or equal to two.
        /// </summary>
        public UInt64 pid;
    }
}