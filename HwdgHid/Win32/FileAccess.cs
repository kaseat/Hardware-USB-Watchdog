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

namespace HwdgHid.Win32
{
    /// <summary>
    /// Generic Access Rights.
    /// </summary>
    [Flags]
    internal enum FileAccess : UInt32
    {
        /// <summary>
        /// All possible access rights
        /// </summary>
        GenericAll = (UInt32) 1 << 28,

        /// <summary>
        /// 	Execute access
        /// </summary>
        GenericExecute = (UInt32) 1 << 29,

        /// <summary>
        /// Read access
        /// </summary>
        GenericWrite = (UInt32) 1 << 30,

        /// <summary>
        /// Write access
        /// </summary>
        GenericRead = (UInt32) 1 << 31,

        /// <summary>
        /// No access
        /// </summary>
        NoAccess = 0
    }
}