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
    internal static class Wrapper
    {
        /// <summary>
        /// Marks any outstanding I/O operations for the specified file handle.
        /// The function only cancels I/O operations in the current process,
        /// regardless of which thread created the I/O operation.
        /// </summary>
        /// <param name="hFile">A handle to the file.</param>
        /// <param name="lpOverlapped">A pointer to an OVERLAPPED data structure
        /// that contains the data used for asynchronous I/O. If this parameter
        ///is NULL, all I/O requests for the hFile parameter are canceled. If
        /// this parameter is not NULL, only those specific I/O requests that
        /// were issued for the file with the specified lpOverlapped overlapped
        /// structure are marked as canceled, meaning that you can cancel one or
        /// more requests, while the CancelIo function cancels all outstanding
        /// requests on a file handle.</param>
        /// <returns>If the function succeeds, the return value is nonzero. The
        /// cancel operation for all pending I/O operations issued by the calling
        /// process for the specified file handle was successfully requested. The
        /// application must not free or reuse the OVERLAPPED structure associated
        /// with the canceled I/O operations until they have completed. The thread
        /// can use the GetOverlappedResult function to determine when the I/O
        /// operations themselves have been completed. If the function fails, the
        /// return value is 0 (zero). To get extended error information, call the
        /// GetLastError function. If this function cannot find a request to cancel,
        /// the return value is 0 (zero), and GetLastError returns ERROR_NOT_FOUND.</returns>
        [DllImport("kernel32.dll", SetLastError = true, ExactSpelling = true, CharSet = CharSet.Auto)]
        internal static extern Boolean CancelIoEx(IntPtr hFile, IntPtr lpOverlapped);

        /// <summary>
        /// Closes an open object handle.
        /// </summary>
        /// <param name="hObject">A valid handle to an open object.</param>
        /// <returns>If the function succeeds, the return value is nonzero. If the
        /// function fails, the return value is zero. To get extended error information,
        /// call GetLastError. If the application is running under a debugger, the function
        /// will throw an exception if it receives either a handle value that is not valid
        /// or a pseudo-handle value. This can happen if you close a handle twice, or if you
        /// call CloseHandle on a handle returned by the FindFirstFile function instead of
        /// calling the FindClose function.</returns>
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
        internal static extern IntPtr CreateFile(String lpFileName, FileAccess dwDesiredAccess,
            ShareMode dwShareMode, ref SecurityAttributes lpSecurityAttributes,
            OpenMode dwCreationDisposition, FileAttributes dwFlagsAndAttributes, IntPtr hTemplateFile);

        /// <summary>
        /// The HidD_FreePreparsedData routine releases the resources that the
        /// HID class driver allocated to hold a top-level collection's preparsed data.
        /// </summary>
        /// <param name="preparsedData">Pointer to the buffer, returned by
        /// HidD_GetPreparsedData, that is freed.</param>
        /// <returns>HidD_FreePreparsedData returns TRUE if it succeeds.
        /// Otherwise, it returns FALSE if the buffer was not a preparsed
        /// data buffer.</returns>
        [DllImport("hid.dll")]
        internal static extern Boolean HidD_FreePreparsedData(IntPtr preparsedData);

        /// <summary>
        /// The HidD_GetAttributes routine returns the attributes of a specified top-level collection.
        /// </summary>
        /// <param name="hidDeviceObject">Specifies an open handle to a top-level collection.</param>
        /// <param name="attributes">Pointer to a caller-allocated HIDD_ATTRIBUTES structure that
        /// returns the attributes of the collection specified by HidDeviceObject.</param>
        /// <returns>HidD_GetAttributes returns TRUE if succeeds; otherwise, it returns FALSE.</returns>
        [DllImport("hid.dll")]
        internal static extern Boolean HidD_GetAttributes(IntPtr hidDeviceObject, ref HiddAttributes attributes);

        /// <summary>
        /// The HidD_GetFeature routine returns a feature report from a specified top-level collection.
        /// </summary>
        /// <param name="hidDeviceObject">pecifies an open handle to a top-level collection.</param>
        /// <param name="lpReportBuffer">Pointer to a caller-allocated HID report buffer that the
        /// caller uses to specify a report ID. HidD_GetFeature uses ReportBuffer to return the
        /// specified feature report.</param>
        /// <param name="reportBufferLength">Specifies the size, in bytes, of the report buffer.
        /// The report buffer must be large enough to hold the feature report - excluding its
        /// report ID, if report IDs are used - plus one additional byte that
        /// specifies a nonzero report ID or zero.</param>
        /// <returns>If HidD_GetFeature succeeds, it returns TRUE; otherwise, it returns FALSE.</returns>
        [DllImport("hid.dll")]
        internal static extern Boolean HidD_GetFeature(IntPtr hidDeviceObject, out Byte[] lpReportBuffer,
            Int32 reportBufferLength);

        /// <summary>
        /// The HidD_GetHidGuid routine returns the device interfaceGUID for HIDClass devices.
        /// </summary>
        /// <param name="hidGuid">Pointer to a caller-allocated GUID buffer that
        /// the routine uses to return the device interface GUID for HIDClass devices.</param>
        [DllImport("hid.dll")]
        internal static extern void HidD_GetHidGuid(ref Guid hidGuid);

        /// <summary>
        /// The HidD_GetInputReport routine returns an input reports from a top-level collection.
        /// </summary>
        /// <param name="hidDeviceObject">Specifies an open handle to a top-level collection.</param>
        /// <param name="reportBuffer">Pointer to a caller-allocated input report buffer that
        /// the caller uses to specify a HID report ID and HidD_GetInputReport uses to return
        /// the specified input report.</param>
        /// <param name="reportBufferLength">Specifies the size, in bytes, of the report buffer.
        /// The report buffer must be large enough to hold the input report - excluding its
        ///report ID, if report IDs are used - plus one additional byte that specifies a
        /// nonzero report ID or zero.</param>
        /// <returns>HidD_GetInputReport returns TRUE if it succeeds; otherwise, it returns FALSE.</returns>
        [DllImport("hid.dll")]
        internal static extern Boolean HidD_GetInputReport(IntPtr hidDeviceObject, out Byte reportBuffer,
            Int32 reportBufferLength);

        /// <summary>
        /// The HidD_GetManufacturerString routine returns a top-level collection's embedded
        /// string that identifies the manufacturer.
        /// </summary>
        /// <param name="hidDeviceObject">Specifies an open handle to a top-level collection.</param>
        /// <param name="reportBuffer">Pointer to a caller-allocated buffer that the routine
        /// uses to return the collection's manufacturer string. The routine returns a
        /// NULL-terminated wide character string in a human-readable format.</param>
        /// <param name="reportBufferLength">Specifies the length, in bytes, of a
        /// caller-allocated buffer provided at Buffer. If the buffer is not large
        /// enough to return the entire NULL-terminated embedded string, the routine
        /// returns nothing in the buffer.</param>
        /// <returns>HidD_HidD_GetManufacturerString returns TRUE if it returns the
        /// entire NULL-terminated embedded string. Otherwise, the routine returns FALSE.</returns>
        [DllImport("hid.dll", CharSet = CharSet.Unicode)]
        internal static extern Boolean HidD_GetManufacturerString(IntPtr hidDeviceObject, out Byte reportBuffer,
            Int32 reportBufferLength);

        /// <summary>
        /// The HidD_GetPreparsedData routine returns a top-level collection's preparsed data.
        /// </summary>
        /// <param name="hidDeviceObject">Specifies an open handle to a top-level collection.</param>
        /// <param name="preparsedData">Pointer to the address of a routine-allocated buffer
        /// that contains a collection's preparsed data in a _HIDP_PREPARSED_DATA structure.</param>
        /// <returns>HidD_GetPreparsedData returns TRUE if it succeeds; otherwise, it returns FALSE.</returns>
        [DllImport("hid.dll")]
        internal static extern Boolean HidD_GetPreparsedData(IntPtr hidDeviceObject, out IntPtr preparsedData);

        /// <summary>
        /// The HidD_GetProductString routine returns the embedded string of a top-level
        /// collection that identifies the manufacturer's product.
        /// </summary>
        /// <param name="hidDeviceObject">Specifies an open handle to a top-level collection.</param>
        /// <param name="lpReportBuffer">Pointer to a caller-allocated buffer that the
        /// routine uses to return the requested product string. The routine returns
        /// a NULL-terminated wide character string.</param>
        /// <param name="reportBufferLength">Specifies the length, in bytes, of a caller-allocated
        /// buffer provided at Buffer. If the buffer is not large enough to return the entire
        /// NULL-terminated embedded string, the routine returns nothing in the buffer.</param>
        /// <returns>HidD_GetProductString returns TRUE if it successfully returns the entire
        /// NULL-terminated embedded string. Otherwise, the routine returns FALSE.</returns>
        [DllImport("hid.dll", CharSet = CharSet.Unicode)]
        internal static extern Boolean HidD_GetProductString(IntPtr hidDeviceObject, out Byte lpReportBuffer,
            Int32 reportBufferLength);

        /// <summary>
        /// The HidD_GetSerialNumberString routine returns the embedded string of a top-level
        /// collection that identifies the serial number of the collection's physical device.
        /// </summary>
        /// <param name="hidDeviceObject">Specifies an open handle to a top-level collection.</param>
        /// <param name="lpReportBuffer">Pointer to a caller-allocated buffer that the routine
        /// uses to return the requested serial number string. The routine returns a
        /// NULL-terminated wide character string.</param>
        /// <param name="reportBufferLength">Specifies the length, in bytes, of a caller-allocated
        /// buffer provided at Buffer. If the buffer is not large enough to return the entire
        /// NULL-terminated embedded string, the routine returns nothing in the buffer.</param>
        /// <returns>HidD_GetSerialNumberString returns TRUE if it successfully returns the
        /// entire NULL-terminated embedded string. Otherwise, the routine returns FALSE.</returns>
        [DllImport("hid.dll", CharSet = CharSet.Unicode)]
        internal static extern Boolean HidD_GetSerialNumberString(IntPtr hidDeviceObject, out Byte[] lpReportBuffer,
            Int32 reportBufferLength);

        /// <summary>
        /// The HidD_SetFeature routine sends a feature report to a top-level collection.
        /// </summary>
        /// <param name="hidDeviceObject">Specifies an open handle to a top-level collection.</param>
        /// <param name="lpReportBuffer">Pointer to a caller-allocated feature report buffer
        /// that the caller uses to specify a HID report ID.</param>
        /// <param name="reportBufferLength">Specifies the size, in bytes, of the report buffer.
        /// The report buffer must be large enough to hold the feature report - excluding its
        /// report ID, if report IDs are used - plus one additional byte that specifies a
        /// nonzero report ID or zero.</param>
        /// <returns>If HidD_SetFeature succeeds, it returns TRUE; otherwise, it returns FALSE.</returns>
        [DllImport("hid.dll")]
        internal static extern Boolean HidD_SetFeature(IntPtr hidDeviceObject, Byte[] lpReportBuffer,
            Int32 reportBufferLength);

        /// <summary>
        /// The HidD_SetOutputReport routine sends an output report to a top-level collection.
        /// </summary>
        /// <param name="hidDeviceObject">Specifies an open handle to a top-level collection.</param>
        /// <param name="lpReportBuffer">Pointer to a caller-allocated output report buffer
        /// that the caller uses to specify a report ID.</param>
        /// <param name="reportBufferLength">Specifies the size, in bytes, of the report
        /// buffer. The report buffer must be large enough to hold the output report -
        /// excluding its report ID, if report IDs are used - plus one additional byte
        /// that specifies a nonzero report ID or zero.</param>
        /// <returns>If HidD_SetOutputReport succeeds, it returns TRUE; otherwise,
        /// ]it returns FALSE.</returns>
        [DllImport("hid.dll")]
        internal static extern Boolean HidD_SetOutputReport(IntPtr hidDeviceObject, Byte[] lpReportBuffer,
            Int32 reportBufferLength);

        /// <summary>
        /// The HidP_GetCaps routine returns a top-level collection's HIDP_CAPS structure.
        /// </summary>
        /// <param name="preparsedData">Pointer to a top-level collection's preparsed data.</param>
        /// <param name="capabilities">Pointer to a caller-allocated buffer that the routine
        /// uses to return a collection's HIDP_CAPS structure.</param>
        /// <returns>HIDP_STATUS_SUCCESS if the routine successfully returned the collection capability
        /// information. HIDP_STATUS_INVALID_PREPARSED_DATA if the specified preparsed data is invalid.</returns>
        [DllImport("hid.dll")]
        internal static extern Int32 HidP_GetCaps(IntPtr preparsedData, out HidpCaps capabilities);

        /// <summary>
        /// The SetupDiDestroyDeviceInfoList function deletes a device information set and
        /// frees all associated memory.
        /// </summary>
        /// <param name="deviceInfoSet">A handle to the device information set to delete.</param>
        /// <returns>The function returns TRUE if it is successful. Otherwise, it returns
        /// FALSE and the logged error can be retrieved with a call to GetLastError.</returns>
        [DllImport("setupapi.dll")]
        internal static extern Int32 SetupDiDestroyDeviceInfoList(IntPtr deviceInfoSet);

        /// <summary>
        /// The SetupDiEnumDeviceInfo function returns a SP_DEVINFO_DATA structure that
        /// specifies a device information element in a device information set.
        /// </summary>
        /// <param name="deviceInfoSet">A handle to the device information set for
        /// which to return an SP_DEVINFO_DATA structure that represents a device
        /// information element.</param>
        /// <param name="memberIndex">A zero-based index of the device information
        /// element to retrieve.</param>
        /// <param name="deviceInfoData">A pointer to an SP_DEVINFO_DATA structure
        /// to receive information about an enumerated device information element.
        /// The caller must set DeviceInfoData.cbSize to sizeof(SP_DEVINFO_DATA).</param>
        /// <returns>The function returns TRUE if it is successful. Otherwise, it
        /// returns FALSE and the logged error can be retrieved with a call to GetLastError.</returns>
        [DllImport("setupapi.dll")]
        internal static extern Boolean SetupDiEnumDeviceInfo(IntPtr deviceInfoSet, Int32 memberIndex,
            ref SpDevinfoData deviceInfoData);

        /// <summary>
        /// The SetupDiEnumDeviceInterfaces function enumerates the device interfaces that
        /// are contained in a device information set.
        /// </summary>
        /// <param name="deviceInfoSet">A pointer to a device information set that contains
        /// the device interfaces for which to return information. This handle is typically
        /// returned by SetupDiGetClassDevs.</param>
        /// <param name="deviceInfoData">A pointer to an SP_DEVINFO_DATA structure that
        /// specifies a device information element in DeviceInfoSet. This parameter is
        /// optional and can be NULL. If this parameter is specified, SetupDiEnumDeviceInterfaces
        /// constrains the enumeration to the interfaces that are supported by the specified
        /// device. If this parameter is NULL, repeated calls to SetupDiEnumDeviceInterfaces
        /// return information about the interfaces that are associated with all the device
        /// information elements in DeviceInfoSet. This pointer is typically returned by
        /// SetupDiEnumDeviceInfo.</param>
        /// <param name="interfaceClassGuid">A pointer to a GUID that specifies the device
        /// interface class for the requested interface.</param>
        /// <param name="memberIndex">A zero-based index into the list of interfaces in the
        /// device information set. The caller should call this function first with MemberIndex
        /// set to zero to obtain the first interface. Then, repeatedly increment MemberIndex
        /// and retrieve an interface until this function fails and GetLastError returns
        /// ERROR_NO_MORE_ITEMS. If DeviceInfoData specifies a particular device, the
        /// MemberIndex is relative to only the interfaces exposed by that device.</param>
        /// <param name="deviceInterfaceData">A pointer to a caller-allocated buffer that
        /// contains, on successful return, a completed SP_DEVICE_INTERFACE_DATA structure
        /// that identifies an interface that meets the search parameters. The caller must
        /// set DeviceInterfaceData.cbSize to sizeof(SP_DEVICE_INTERFACE_DATA) before calling
        /// this function.</param>
        /// <returns>SetupDiEnumDeviceInterfaces returns TRUE if the function completed without
        /// error. If the function completed with an error, FALSE is returned and the error code
        /// for the failure can be retrieved by calling GetLastError.</returns>
        [DllImport("setupapi.dll")]
        internal static extern Boolean SetupDiEnumDeviceInterfaces(IntPtr deviceInfoSet,
            ref SpDevinfoData deviceInfoData, ref Guid interfaceClassGuid, Int32 memberIndex,
            out SpDeviceInterfaceData deviceInterfaceData);

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
    }
}