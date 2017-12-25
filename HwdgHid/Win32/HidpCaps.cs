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
    /// <summary>
    /// The HIDP_CAPS structure contains information
    /// about a top-level collection's capability.
    /// </summary>
    /// <remarks>See https://msdn.microsoft.com/en-us/library/windows/hardware/ff539697.aspx
    /// for more info</remarks>
    [StructLayout(LayoutKind.Sequential)]
    internal struct HidpCaps
    {
        /// <summary>
        /// Specifies a top-level collection's usage ID.
        /// </summary>
        internal readonly UInt16 Usage;

        /// <summary>
        /// Specifies the top-level collection's usage page.
        /// </summary>
        internal readonly UInt16 UsagePage;

        /// <summary>
        /// Specifies the maximum size, in bytes, of all the input reports
        /// (including the report ID, if report IDs are used, which is
        /// prepended to the report data).
        /// </summary>
        internal readonly UInt16 InputReportByteLength;

        /// <summary>
        /// Specifies the maximum size, in bytes, of all the output reports
        /// (including the report ID, if report IDs are used, which is
        /// prepended to the report data).
        /// </summary>
        internal readonly UInt16 OutputReportByteLength;

        /// <summary>
        /// Specifies the maximum length, in bytes, of all the feature reports
        /// (including the report ID, if report IDs are used, which is
        /// prepended to the report data).
        /// </summary>
        internal readonly UInt16 FeatureReportByteLength;

        /// <summary>
        /// Reserved for internal system use.
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 17)] private readonly UInt16[] Reserved;

        /// <summary>
        /// Specifies the number of HIDP_LINK_COLLECTION_NODE structures that are
        /// returned for this top-level collection by HidP_GetLinkCollectionNodes.
        /// </summary>
        internal readonly UInt16 NumberLinkCollectionNodes;

        /// <summary>
        /// Specifies the number of input HIDP_BUTTON_CAPS structures
        /// that HidP_GetButtonCaps returns.
        /// </summary>
        internal readonly UInt16 NumberInputButtonCaps;

        /// <summary>
        /// Specifies the number of input HIDP_VALUE_CAPS structures
        /// that HidP_GetValueCaps returns.
        /// </summary>
        internal readonly UInt16 NumberInputValueCaps;

        /// <summary>
        /// Specifies the number of data indices assigned
        /// to buttons and values in all input reports.
        /// </summary>
        internal readonly UInt16 NumberInputDataIndices;

        /// <summary>
        /// Specifies the number of output HIDP_BUTTON_CAPS
        /// structures that HidP_GetButtonCaps returns.
        /// </summary>
        internal readonly UInt16 NumberOutputButtonCaps;

        /// <summary>
        /// Specifies the number of output HIDP_VALUE_CAPS
        /// structures that HidP_GetValueCaps returns.
        /// </summary>
        internal readonly UInt16 NumberOutputValueCaps;

        /// <summary>
        /// Specifies the number of data indices assigned
        /// to buttons and values in all output reports.
        /// </summary>
        internal readonly UInt16 NumberOutputDataIndices;

        /// <summary>
        /// Specifies the total number of feature HIDP_BUTTONS_CAPS
        /// structures that HidP_GetButtonCaps returns.
        /// </summary>
        internal readonly UInt16 NumberFeatureButtonCaps;

        /// <summary>
        /// Specifies the total number of feature HIDP_VALUE_CAPS
        /// structures that HidP_GetValueCaps returns.
        /// </summary>
        internal readonly UInt16 NumberFeatureValueCaps;

        /// <summary>
        /// Specifies the number of data indices assigned
        /// to buttons and values in all feature reports.
        /// </summary>
        internal readonly UInt16 NumberFeatureDataIndices;
    }
}