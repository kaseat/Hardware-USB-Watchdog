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
    /// The file or device attributes and flags.
    /// </summary>
    [Flags]
    internal enum FileAttributes : UInt32
    {
        /// <summary>
        /// No flags or attributes.
        /// </summary>
        NoFlags = 0,

        /// <summary>
        /// The file should be archived. Applications use this attribute
        /// to mark files for backup or removal.
        /// </summary>
        FileAttributeArchive = 0x20,

        /// <summary>
        /// The file or directory is encrypted. For a file, this means that
        /// all data in the file is encrypted. For a directory, this means
        /// that encryption is the default for newly created files and
        /// subdirectories. For more information, see File Encryption.
        /// This flag has no effect if FILE_ATTRIBUTE_SYSTEM is also specified.
        /// </summary>
        FileAttributeEncrypted = 0x4000,

        /// <summary>
        /// The file is hidden. Do not include it in an ordinary directory
        /// listing.
        /// </summary>
        FileAttributeHidden = 0x2,

        /// <summary>
        /// The file does not have other attributes set. This attribute is
        /// valid only if used alone.
        /// </summary>
        FileAttributeNormal = 0x80,

        /// <summary>
        /// The data of a file is not immediately available. This attribute
        /// indicates that file data is physically moved to offline storage.
        /// This attribute is used by Remote Storage, the hierarchical storage
        /// management software. Applications should not arbitrarily change
        /// this attribute.
        /// </summary>
        FileAttributeOffline = 0x1000,

        /// <summary>
        /// The file is read only. Applications can read the file, but
        /// cannot write to or delete it.
        /// </summary>
        FileAttributeReadonly = 0x1,

        /// <summary>
        /// The file is part of or used exclusively by an operating system.
        /// </summary>
        FileAttributeSystem = 0x4,

        /// <summary>
        /// The file is being used for temporary storage.
        /// </summary>
        FileAttributeTemporary = 0x100,

        /// <summary>
        /// The file is being opened or created for a backup or restore
        /// operation. The system ensures that the calling process overrides
        /// file security checks when the process has SE_BACKUP_NAME and
        /// SE_RESTORE_NAME privileges. For more information, see Changing
        /// Privileges in a Token. You must set this flag to obtain a handle
        /// to a directory. A directory handle can be passed to some functions
        /// instead of a file handle. For more information, see the Remarks section.
        /// </summary>
        FileFlagBackupSemantics = 0x02000000,

        /// <summary>
        /// The file is to be deleted immediately after all of its handles
        /// are closed, which includes the specified handle and any other
        /// open or duplicated handles. If there are existing open handles
        /// to a file, the call fails unless they were all opened with the
        /// FILE_SHARE_DELETE share mode. Subsequent open requests for the
        /// file fail, unless the FILE_SHARE_DELETE share mode is specified.
        /// </summary>
        FileFlagDeleteOnClose = 0x04000000,

        /// <summary>
        /// The file or device is being opened with no system caching for data
        /// reads and writes. This flag does not affect hard disk caching or
        /// memory mapped files.
        /// </summary>
        FileFlagNoBuffering = 0x20000000,

        /// <summary>
        /// If the file is not a reparse point, then this flag is ignored.
        /// </summary>
        FileFlagOpenNoRecall = 0x00100000,

        /// <summary>
        /// Normal reparse point processing will not occur; CreateFile will
        /// attempt to open the reparse point. When a file is opened, a file
        /// handle is returned, whether or not the filter that controls the
        /// reparse point is operational. This flag cannot be used with the
        /// CREATE_ALWAYS flag. If the file is not a reparse point, then this
        /// flag is ignored.
        /// </summary>
        FileFlagOpenReparsePoint = 0x00200000,

        /// <summary>
        /// The file or device is being opened or created for asynchronous
        /// I/O. When subsequent I/O operations are completed on this handle,
        /// the event specified in the OVERLAPPED structure will be set to
        /// the signaled state. If this flag is specified, the file can be
        /// used for simultaneous read and write operations. If this flag
        /// is not specified, then I/O operations are serialized, even if
        /// the calls to the read and write functions specify an OVERLAPPED
        /// structure.
        /// </summary>
        FileFlagOverlapped = 0x40000000,

        /// <summary>
        /// Access will occur according to POSIX rules. This includes
        /// allowing multiple files with names, differing only in case,
        /// for file systems that support that naming. Use care when
        /// using this option, because files created with this flag may
        /// not be accessible by applications that are written for MS-DOS
        /// or 16-bit Windows.
        /// </summary>
        FileFlagPosixSemantics = 0x0100000,

        /// <summary>
        /// Access is intended to be random. The system can use this as
        /// a hint to optimize file caching. This flag has no effect if
        /// the file system does not support cached I/O and
        /// FILE_FLAG_NO_BUFFERING.
        /// </summary>
        FileFlagRandomAccess = 0x10000000,

        /// <summary>
        /// The file or device is being opened with session awareness.
        /// If this flag is not specified, then per-session devices
        /// (such as a device using RemoteFX USB Redirection) cannot
        /// be opened by processes running in session 0. This flag has
        /// no effect for callers not in session 0. This flag is
        /// supported only on server editions of Windows.
        /// </summary>
        FileFlagSessionAware = 0x00800000,

        /// <summary>
        /// Access is intended to be sequential from beginning to
        /// end. The system can use this as a hint to optimize file
        /// caching. This flag should not be used if read-behind
        /// (that is, reverse scans) will be used. This flag has
        /// no effect if the file system does not support cached
        /// I/O and FILE_FLAG_NO_BUFFERING.
        /// </summary>
        FileFlagSequentialScan = 0x08000000,

        /// <summary>
        /// Write operations will not go through any intermediate
        /// cache, they will go directly to disk.
        /// </summary>
        FileFlagWriteThrough = 0x80000000
    }
}