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
    /// The SECURITY_ATTRIBUTES structure contains the security
    /// descriptor for an object and specifies whether the handle
    /// retrieved by specifying this structure is inheritable.
    /// This structure provides security settings for objects
    /// created by various functions, such as CreateFile, CreatePipe,
    /// CreateProcess, RegCreateKeyEx, or RegSaveKeyEx.
    /// </summary>
    /// <remarks>See https://msdn.microsoft.com/en-us/library/windows/desktop/aa379560.aspx
    /// for more info</remarks>
    [StructLayout(LayoutKind.Sequential)]
    internal struct SecurityAttributes
    {
        /// <summary>
        /// Get initialized instance of <see cref="SecurityAttributes"/> struct.
        /// </summary>
        /// <returns>Returns Initialized <see cref="SecurityAttributes"/> struct.</returns>
        internal static SecurityAttributes Initialize() =>
            new SecurityAttributes(Marshal.SizeOf(typeof(SecurityAttributes)));

        /// <summary>
        /// Create the struct with the specified nLength value.
        /// </summary>
        /// <param name="length">nLength value.</param>
        private SecurityAttributes(Int32 length)
        {
            nLength = length;
            lpSecurityDescriptor = default(IntPtr);
            bInheritHandle = true;
        }

        /// <summary>
        /// The size, in bytes, of this structure. Set this
        /// value to the size of the SECURITY_ATTRIBUTES structure.
        /// </summary>
        internal readonly Int32 nLength;

        /// <summary>
        /// A pointer to a SECURITY_DESCRIPTOR structure that
        /// controls access to the object. If the value of
        /// this member is NULL, the object is assigned the
        /// default security descriptor associated with the
        /// access token of the calling process. This is not
        /// the same as granting access to everyone by assigning
        /// a NULL discretionary access control list (DACL).
        /// By default, the default DACL in the access token
        /// of a process allows access only to the user
        /// represented by the access token.
        /// </summary>
        internal readonly IntPtr lpSecurityDescriptor;

        /// <summary>
        /// A Boolean value that specifies whether the returned
        /// handle is inherited when a new process is created.
        /// If this member is TRUE, the new process inherits
        /// the handle.
        /// </summary>
        internal readonly Boolean bInheritHandle;
    }
}