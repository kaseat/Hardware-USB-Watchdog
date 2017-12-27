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
    /// The requested sharing mode of the file or device, which
    /// can be read, write, both, delete, all of these, or none.
    /// </summary>
    [Flags]
    internal enum ShareMode
    {
        /// <summary>
        /// Prevents other processes from opening a file or device if
        /// they request delete, read, or write access.
        /// </summary>
        Exclusive = 0,

        /// <summary>
        /// Enables subsequent open operations on a file or device to
        /// request read access. Otherwise, other processes cannot open
        /// the file or device if they request read access. If this flag
        /// is not specified, but the file or device has been opened for
        /// read access, the function fails.
        /// </summary>
        ShareRead = 1 << 0,

        /// <summary>
        /// Enables subsequent open operations on a file or device to
        /// request write access. Otherwise, other processes cannot open
        /// the file or device if they request write access. If this flag
        /// is not specified, but the file or device has been opened for
        /// write access or has a file mapping with write access, the
        /// function fails.
        /// </summary>
        ShareWrite = 1 << 2,

        /// <summary>
        /// Enables subsequent open operations on a file or device
        /// to request delete access. Otherwise, other processes
        /// cannot open the file or device if they request delete
        /// access. If this flag is not specified, but the file or
        /// device has been opened for delete access, the function fails.
        /// </summary>
        ShareDelete = 1 << 3
    }
}