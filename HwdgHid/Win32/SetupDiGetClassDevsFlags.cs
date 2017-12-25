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

namespace HwdgHid.Win32
{
    /// <summary>
    /// A variable of type DWORD that specifies control
    /// options that filter the device information elements
    /// that are added to the device information set. This
    /// parameter can be a bitwise OR of zero or more of the
    /// following flags. For more information about combining
    /// these flags, https://msdn.microsoft.com/en-us/library/windows/hardware/ff551069.aspx.
    /// </summary>
    [Flags]
    internal enum SetupDiGetClassDevsFlags
    {
        /// <summary>
        /// Return only the device that is associated
        /// with the system default device interface,
        /// if one is set, for the specified device
        /// nterface classes.
        /// </summary>
        DigcfDefault = 1 << 0,

        /// <summary>
        /// Return only devices that are currently
        /// present in a system.
        /// </summary>
        DigcfPresent = 1 << 1,

        /// <summary>
        /// Return a list of installed devices for all
        /// device setup classes or all device interface classes.
        /// </summary>
        DigcfAllclasses = 1 << 2,

        /// <summary>
        /// Return only devices that are a part of
        /// the current hardware profile.
        /// </summary>
        DigcfProfile = 1 << 3,

        /// <summary>
        /// Return devices that support device interfaces
        /// for the specified device interface classes.
        /// This flag must be set in the Flags parameter if
        /// the Enumerator parameter specifies a device instance ID.
        /// </summary>
        DigcfDeviceinterface = 1 << 4
    }
}