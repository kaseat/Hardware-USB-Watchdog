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
    internal static class Wrapper
    {
        [DllImport("kernel32.dll", SetLastError = true, ExactSpelling = true, CharSet = CharSet.Auto)]
        internal static extern Boolean CancelIoEx(IntPtr hFile, IntPtr lpOverlapped);

        [DllImport("kernel32.dll", SetLastError = true, ExactSpelling = true, CharSet = CharSet.Auto)]
        internal static extern Boolean CloseHandle(IntPtr hObject);

        /// <summary>
        /// Creates or opens a file or I/O device. The most commonly used I/O devices
        /// are as follows: file, file stream, directory, physical disk, volume,
        /// console buffer, tape drive, communications resource, mailslot, and pipe.
        /// </summary>
        ///
        ///
        /// <param name="lpFileName">The name of the file or device to be created or opened.
        /// You may use either forward slashes (/) or backslashes (\) in this name.
        ///
        /// In the ANSI version of this function, the name is limited to MAX_PATH characters.
        /// To extend this limit to 32,767 wide characters, call the Unicode version of the
        /// function and prepend "\\?\" to the path. For more information, see Naming Files,
        /// Paths, and Namespaces.
        ///
        /// For information on special device names, see Defining an MS-DOS Device Name.
        /// To create a file stream, specify the name of the file, a colon, and then
        /// the name of the stream.
        ///
        /// For more information, see File Streams.</param>
        ///
        ///
        /// <param name="dwDesiredAccess">The requested access to the file or device,
        /// which can be summarized as read, write, both or neither zero.
        ///
        /// The most commonly used values are GENERIC_READ, GENERIC_WRITE, or both
        /// (GENERIC_READ | GENERIC_WRITE). For more information, see Generic Access
        /// Rights, File Security and Access Rights, File Access Rights Constants,
        /// and ACCESS_MASK.
        ///
        /// If this parameter is zero, the application can query certain metadata such as
        /// file, directory, or device attributes without accessing that file or device,
        /// even if GENERIC_READ access would have been denied.
        ///
        /// You cannot request an access mode that conflicts with the
        /// sharing mode that is specified by the dwShareMode parameter in an open
        /// request that already has an open handle.</param>
        ///
        ///
        /// <param name="dwShareMode">The requested sharing mode of the file or
        /// device, which can be read, write, both, delete, all of these, or none
        /// (refer to the following table). Access requests to attributes or
        /// extended attributes are not affected by this flag.
        ///
        /// If this parameter is zero and CreateFile succeeds, the file or device cannot
        /// be shared and cannot be opened again until the handle to the file or device
        /// is closed. For more information, see the Remarks section.
        ///
        /// You cannotrequest a sharing mode that conflicts with the access mode that
        /// is specified in an existing request that has an open handle. CreateFile
        /// would fail and the GetLastError function would return ERROR_SHARING_VIOLATION.
        ///
        /// To enable a process to share a file or device while another process has
        /// the file or device open, use a compatible combination of one or more of
        /// the following values. For more information about valid combinations of
        /// this parameter with the dwDesiredAccess parameter, see Creating and
        /// Opening Files.</param>
        ///
        ///
        /// <param name="lpSecurityAttributes">A pointer to a SECURITY_ATTRIBUTES
        /// structure that contains two separate but related data members: an optional
        /// security descriptor, and a Boolean value that determines whether the
        /// returned handle can be inherited by child processes.
        ///
        /// This parameter can be NULL.
        ///
        /// If this parameter is NULL, the handle returned by CreateFile cannot be
        /// inherited by any child processes the application may create and the file
        /// or device associated with the returned handle gets a default security descriptor.
        ///
        /// The lpSecurityDescriptor member of the structure specifies a SECURITY_DESCRIPTOR
        /// for a file or device. If this member is NULL, the file or device associated with
        /// the eturned handle is assigned a default security descriptor.
        ///
        /// CreateFile ignores the lpSecurityDescriptor member when opening an existing file
        /// or device, but continues to use the bInheritHandle member.
        ///
        /// The bInheritHandlemember of the structure specifies whether the returned
        /// handle can be inherited.</param>
        ///
        ///
        /// <param name="dwCreationDisposition">An action to take on a file or device
        /// that exists or does not exist.
        ///
        /// For devices other than files, this parameter is usually set to OPEN_EXISTING.</param>
        ///
        ///
        /// <param name="dwFlagsAndAttributes">The file or device attributes and flags,
        /// FILE_ATTRIBUTE_NORMAL being the most common default value for files.
        ///
        /// This parameter can include any combination of the available file attributes
        /// (FILE_ATTRIBUTE_*). All other file attributes override FILE_ATTRIBUTE_NORMAL.
        ///
        /// This parameter can also contain combinations of flags (FILE_FLAG_*) for
        /// control of file or device caching behavior, access modes, and other
        /// special-purpose flags. These combine with any FILE_ATTRIBUTE_* values.
        ///
        /// This parameter can also contain Security Quality of Service (SQOS)
        /// information by specifying the SECURITY_SQOS_PRESENT flag. Additional
        /// SQOS-related flags information is presented in the table following the
        /// attributes and flags tables.</param>
        ///
        ///
        /// <param name="hTemplateFile">A valid handle to a template file with the
        /// GENERIC_READ access right. The template file supplies file attributes
        /// and extended attributes for the file that is being created.
        ///
        /// This parameter can be NULL.
        ///
        /// When opening an existing file, CreateFile ignores this parameter.</param>
        ///
        ///
        /// <returns>The function returns a handle that can be used to access the file
        /// or device for various types of I/O depending on the file or device and the
        /// flags and attributes specified.</returns>
        [DllImport("kernel32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        internal static extern IntPtr CreateFile(String lpFileName, FileAccess dwDesiredAccess, ShareMode dwShareMode,
            ref SecurityAttributes lpSecurityAttributes, OpenMode dwCreationDisposition, FileAttributes dwFlagsAndAttributes,
            IntPtr hTemplateFile);


        [DllImport("hid.dll")]
        internal static extern Boolean HidD_FreePreparsedData(IntPtr preparsedData);

        /// <summary>
        /// The HidD_GetAttributes routine returns the attributes of a specified top-level collection.
        /// </summary>
        /// <param name="hidDeviceObject">Specifies an open handle to a top-level collection.</param>
        /// <param name="attributes">Pointer to a caller-allocated HIDD_ATTRIBUTES structure that returns the attributes of the collection specified by HidDeviceObject.</param>
        /// <returns>HidD_GetAttributes returns TRUE if succeeds; otherwise, it returns FALSE.</returns>
        [DllImport("hid.dll")]
        internal static extern Boolean HidD_GetAttributes(IntPtr hidDeviceObject, ref HiddAttributes attributes);

        [DllImport("hid.dll")]
        internal static extern Boolean HidD_GetFeature(IntPtr hidDeviceObject, Byte[] lpReportBuffer,
            Int32 reportBufferLength);

        /// <summary>
        /// The HidD_GetHidGuid routine returns the device interfaceGUID for HIDClass devices.
        /// </summary>
        /// <param name="hidGuid">Pointer to a caller-allocated GUID buffer that
        /// the routine uses to return the device interface GUID for HIDClass devices.</param>
        [DllImport("hid.dll")]
        internal static extern void HidD_GetHidGuid(ref Guid hidGuid);

        [DllImport("hid.dll")]
        internal static extern Boolean HidD_GetInputReport(IntPtr hidDeviceObject, Byte[] lpReportBuffer,
            Int32 reportBufferLength);

        [DllImport("hid.dll", CharSet = CharSet.Unicode)]
        internal static extern Boolean HidD_GetManufacturerString(IntPtr hidDeviceObject, ref Byte lpReportBuffer,
            Int32 reportBufferLength);

        [DllImport("hid.dll")]
        internal static extern Boolean HidD_GetPreparsedData(IntPtr hidDeviceObject, ref IntPtr preparsedData);

        [DllImport("hid.dll", CharSet = CharSet.Unicode)]
        internal static extern Boolean HidD_GetProductString(IntPtr hidDeviceObject, ref Byte lpReportBuffer,
            Int32 reportBufferLength);

        [DllImport("hid.dll", CharSet = CharSet.Unicode)]
        internal static extern Boolean HidD_GetSerialNumberString(IntPtr hidDeviceObject, ref Byte lpReportBuffer,
            Int32 reportBufferLength);

        [DllImport("hid.dll")]
        internal static extern Boolean HidD_SetFeature(IntPtr hidDeviceObject, Byte[] lpReportBuffer,
            Int32 reportBufferLength);

        [DllImport("hid.dll")]
        internal static extern Boolean HidD_SetOutputReport(IntPtr hidDeviceObject, Byte[] lpReportBuffer,
            Int32 reportBufferLength);

        [DllImport("hid.dll")]
        internal static extern Int32 HidP_GetCaps(IntPtr preparsedData, ref HidpCaps capabilities);

        [DllImport("kernel32.dll", SetLastError = true)]
        internal static extern Boolean ReadFile(IntPtr hFile, IntPtr lpBuffer, UInt32 nNumberOfBytesToRead,
            out UInt32 lpNumberOfBytesRead, [In] ref System.Threading.NativeOverlapped lpOverlapped);

        [DllImport("setupapi.dll")]
        internal static extern Int32 SetupDiDestroyDeviceInfoList(IntPtr deviceInfoSet);

        [DllImport("setupapi.dll")]
        internal static extern Boolean SetupDiEnumDeviceInfo(IntPtr deviceInfoSet, Int32 memberIndex,
            ref SpDevinfoData deviceInfoData);

        [DllImport("setupapi.dll")]
        internal static extern Boolean SetupDiEnumDeviceInterfaces(IntPtr deviceInfoSet,
            ref SpDevinfoData deviceInfoData, ref Guid interfaceClassGuid, Int32 memberIndex,
            ref SpDeviceInterfaceData deviceInterfaceData);

        /// <summary>
        /// The SetupDiGetClassDevs function returns a handle to a device
        /// information set that contains requested device information
        /// elements for a local computer.
        /// </summary>
        /// <param name="classGuid">A pointer to the GUID for a device setup
        /// class or a device interface class. This pointer is optional and
        /// can be NULL. For more information about how to set ClassGuid,
        /// see the following Remarks section.</param>
        /// <param name="enumerator">A pointer to a NULL-terminated string
        /// that specifies:
        ///
        /// 1. An identifier (ID) of a Plug and Play (PnP) enumerator.
        /// This ID can either be the value's globally unique identifier
        /// (GUID) or symbolic name. For example, "PCI" can be used to
        /// specify the PCI PnP value. Other examples of symbolic names
        /// for PnP values include "USB," "PCMCIA," and "SCSI".
        ///
        /// 2. A PnP device instance ID. When specifying a PnP deviceinstance ID,
        /// DIGCF_DEVICEINTERFACE must be set in the Flags parameter.
        /// This pointer is optional and can be NULL. If an enumeration value
        /// is not used to select devices, set Enumerator to NULL.
        ///
        /// For more information about how to set the Enumerator value, see
        /// the following Remarks section.</param>
        /// <param name="hwndParent">A handle to the top-level window to be
        /// used for a user interface that is associated with installing a
        /// device instance in the device information set. This handle is
        /// optional and can be NULL.</param>
        /// <param name="flags">A variable of type DWORD that specifies
        /// control options that filter the device information elements
        /// that are added to the device information set. This parameter
        /// can be a bitwise OR of zero or more of the following flags.
        /// For more information about combining these flags, see the
        /// following Remarks section.</param>
        /// <returns>If the operation succeeds, SetupDiGetClassDevs returns
        /// a handle to a device information set that contains all installed
        /// devices that matched the supplied parameters. If the operation
        /// fails, the function returns INVALID_HANDLE_VALUE. To get extended
        /// error information, call GetLastError.</returns>
        /// <remarks>
        /// For more details see
        /// https://msdn.microsoft.com/en-us/library/windows/hardware/ff551069.aspx.
        /// </remarks>
        [DllImport("setupapi.dll", CharSet = CharSet.Auto)]
        internal static extern IntPtr SetupDiGetClassDevs(ref Guid classGuid,
            IntPtr enumerator, IntPtr hwndParent, SetupDiGetClassDevsFlags flags);

        /// <summary>
        /// The SetupDiGetDeviceInterfaceDetail function returns details about a device interface.
        /// </summary>
        /// <param name="deviceInfoSet">A pointer to the device information set that
        /// contains the interface for which to retrieve details. This handle is
        /// typically returned by <see cref="SetupDiGetClassDevs"/>.</param>
        /// <param name="deviceInterfaceData">A pointer to an <see cref="SpDeviceInterfaceData"/>
        /// structure that specifies the interface in <see cref="deviceInfoSet"/> for which to
        /// retrieve details. A pointer of this type is typically returned
        /// by <see cref="SetupDiEnumDeviceInterfaces"/>.</param>
        /// <param name="deviceInterfaceDetailData">A pointer to an <see cref="SpDeviceInterfaceDetailData"/>
        /// structure to receive information about the specified interface. This parameter is optional
        /// and can be NULL. This parameter must be NULL if DeviceInterfaceDetailSize is zero.
        /// If this parameter is specified, the caller must set DeviceInterfaceDetailData.cbSize
        /// to sizeof(<see cref="SpDeviceInterfaceDetailData"/>) before calling this function. The cbSize
        /// member always contains the size of the fixed part of the data structure, not a size
        /// reflecting the variable-length string at the end.</param>
        /// <param name="deviceInterfaceDetailDataSize">The size of the DeviceInterfaceDetailData buffer.
        /// The buffer must be at least (offsetof(SP_DEVICE_INTERFACE_DETAIL_DATA, DevicePath) + sizeof(TCHAR))
        /// bytes, to contain the fixed part of the structure and a single NULL to terminate an empty
        /// MULTI_SZ string. This parameter must be zero if <paramref name="deviceInterfaceDetailData"/>
        /// is NULL.</param>
        /// <param name="requiredSize">A pointer to a variable of type DWORD that receives the
        /// required size of the DeviceInterfaceDetailData buffer. This size includes the size
        /// of the fixed part of the structure plus the number of bytes required for the
        /// variable-length device path string. This parameter is optional and can be NULL.</param>
        /// <param name="deviceInfoData">A pointer to a buffer that receives information about
        /// the device that supports the requested interface. The caller must set
        /// DeviceInfoData.cbSize to sizeof(SP_DEVINFO_DATA). This parameter is
        /// optional and can be NULL.</param>
        /// <returns>SetupDiGetDeviceInterfaceDetail returns TRUE if the function completed
        /// without error. If the function completed with an error, FALSE is returned and
        /// the error code for the failure can be retrieved by calling GetLastError.</returns>
        [DllImport("setupapi.dll", CharSet = CharSet.Auto)]
        internal static extern Boolean SetupDiGetDeviceInterfaceDetail(IntPtr deviceInfoSet,
            ref SpDeviceInterfaceData deviceInterfaceData,
            ref SpDeviceInterfaceDetailData deviceInterfaceDetailData,
            Int32 deviceInterfaceDetailDataSize,
            IntPtr requiredSize,
            IntPtr deviceInfoData);

        [DllImport("kernel32.dll")]
        internal static extern Boolean WriteFile(IntPtr hFile, Byte[] lpBuffer, UInt32 nNumberOfBytesToWrite,
            out UInt32 lpNumberOfBytesWritten, [In] ref System.Threading.NativeOverlapped lpOverlapped);
    }
}