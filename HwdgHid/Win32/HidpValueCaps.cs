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
    /// The HIDP_VALUE_CAPS structure contains information that
    /// describes the capability of a set of HID control values
    /// (either a single usage or a usage range).
    /// </summary>
    /// <remarks>See https://msdn.microsoft.com/en-us/library/windows/hardware/ff539832.aspx
    /// for more info</remarks>
    [StructLayout(LayoutKind.Sequential)]
    internal struct HidpValueCaps
    {
        /// <summary>
        /// Specifies the usage page of the usage or usage range.
        /// </summary>
        internal readonly UInt16 UsagePage;

        /// <summary>
        /// Specifies the report ID of the HID report that
        /// contains the usage or usage range.
        /// </summary>
        internal readonly Byte ReportID;

        /// <summary>
        /// Indicates, if TRUE, that the usage is member of
        /// a set of aliased usages. Otherwise, if IsAlias
        /// is FALSE, the value has only one usage.
        /// </summary>
        internal readonly Int32 IsAlias;

        /// <summary>
        /// Contains the data fields (one or two bytes) associated
        /// with an input, output, or feature main item.
        /// </summary>
        internal readonly UInt16 BitField;

        /// <summary>
        /// Specifies the index of the link collection in a top-level
        /// collection's link collection array that contains the usage
        /// or usage range. If LinkCollection is zero, the usage or usage
        /// range is contained in the top-level collection.
        /// </summary>
        internal readonly UInt16 LinkCollection;

        /// <summary>
        /// Specifies the usage of the link collection that contains the
        /// usage or usage range. If LinkCollection is zero, LinkUsage
        /// specifies the usage of the top-level collection.
        /// </summary>
        internal readonly UInt16 LinkUsage;

        /// <summary>
        /// Specifies the usage page of the link collection that contains
        /// the usage or usage range. If LinkCollection is zero, LinkUsagePage
        /// specifies the usage page of the top-level collection.
        /// </summary>
        internal readonly UInt16 LinkUsagePage;

        /// <summary>
        /// Specifies, if TRUE, that the structure describes a
        /// usage range. Otherwise, if IsRange is FALSE, the
        /// structure describes a single usage.
        /// </summary>
        internal readonly Int32 IsRange;

        /// <summary>
        /// Specifies, if TRUE, that the usage or usage range has a
        /// set of string descriptors. Otherwise, if IsStringRange
        /// is FALSE, the usage or usage range has zero or one
        /// string descriptor.
        /// </summary>
        internal readonly Int32 IsStringRange;

        /// <summary>
        /// Specifies, if TRUE, that the usage or usage range has a set
        /// of designators. Otherwise, if IsDesignatorRange is FALSE,
        /// the usage or usage range has zero or one designator.
        /// </summary>
        internal readonly Int32 IsDesignatorRange;

        /// <summary>
        /// Specifies, if TRUE, that the usage or usage range provides
        /// absolute data. Otherwise, if IsAbsolute is FALSE, the value
        /// is the change in state from the previous value.
        /// </summary>
        internal readonly Int32 IsAbsolute;

        /// <summary>
        /// Specifies, if TRUE, that the usage supports a NULL value,
        /// which indicates that the data is not valid and should be
        /// ignored. Otherwise, if HasNull is FALSE, the usage does
        /// not have a NULL value.
        /// </summary>
        internal readonly Int32 HasNull;

        /// <summary>
        /// Reserved for internal system use.
        /// </summary>
        internal readonly Byte Reserved;

        /// <summary>
        /// Specifies the size, in bits, of a usage's data field
        /// in a report. If ReportCount is greater than one,
        /// each usage has a separate data field of this size.
        /// </summary>
        internal readonly UInt16 BitSize;

        /// <summary>
        /// Specifies the number of usages that this structure describes.
        /// </summary>
        internal readonly UInt16 ReportCount;

        /// <summary>
        /// Reserved for internal system use.
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 5)] private readonly UInt16[] Reserved2;

        /// <summary>
        /// Specifies the usage's exponent, as described
        /// by the USB HID standard.
        /// </summary>
        internal readonly UInt64 UnitsExp;

        /// <summary>
        /// Specifies the usage's units, as described by
        /// the USB HID Standard.
        /// </summary>
        internal readonly UInt64 Units;

        /// <summary>
        /// Specifies a usage's signed lower bound.
        /// </summary>
        internal readonly Int64 LogicalMin;

        /// <summary>
        /// Specifies a usage's signed upper bound.
        /// </summary>
        internal readonly Int64 LogicalMax;

        /// <summary>
        /// Specifies a usage's signed lower bound after
        /// scaling is applied to the logical minimum value.
        /// </summary>
        internal readonly Int64 PhysicalMin;

        /// <summary>
        /// Specifies a usage's signed upper bound after
        /// scaling is applied to the logical maximum value.
        /// </summary>
        internal readonly Int64 PhysicalMax;

        /// <summary>
        /// If IsRange is TRUE: Indicates the inclusive lower
        /// bound of usage range whose inclusive upper bound
        /// is specified by Range.UsageMax.
        /// If IsRange is FALSE: Indicates a usage ID.
        /// </summary>
        internal readonly UInt16 UsageMin;

        /// <summary>
        /// If IsRange is TRUE: Indicates the inclusive upper
        /// bound of a usage range whose inclusive lower bound
        /// is indicated by Range.UsageMin.
        /// If IsRange is FALSE: Reserved for internal system use.
        /// </summary>
        internal readonly UInt16 UsageMax;

        /// <summary>
        /// If IsRange is TRUE: Indicates the inclusive lower
        /// bound of a range of string descriptors (specified
        /// by string minimum and string maximum items) whose
        /// inclusive upper bound is indicated by Range.StringMax.
        /// If IsRange is FALSE: Indicates a string descriptor
        /// ID for the usage specified by NotRange.Usage.
        /// </summary>
        internal readonly UInt16 StringMin;

        /// <summary>
        /// If IsRange is TRUE: Indicates the inclusive upper
        /// bound of a range of string descriptors (specified
        /// by string minimum and string maximum items) whose
        /// inclusive lower bound is indicated by Range.StringMin.
        /// If IsRange is FALSE: Reserved for internal system use.
        /// </summary>
        internal readonly UInt16 StringMax;

        /// <summary>
        /// If IsRange is TRUE: Indicates the inclusive lower
        /// bound of a range of designators (specified by designator
        /// minimum and designator maximum items) whose inclusive
        /// lower bound is indicated by Range.DesignatorMax.
        /// If IsRange is FALSE: Indicates a designator ID for
        /// the usage specified by NotRange.Usage.
        /// </summary>
        internal readonly UInt16 DesignatorMin;

        /// <summary>
        /// If IsRange is TRUE: Indicates the inclusive upper
        /// bound of a range of designators (specified by designator
        /// minimum and designator maximum items) whose inclusive
        /// lower bound is indicated by Range.DesignatorMin.
        /// If IsRange is FALSE: Reserved for internal system use.
        /// </summary>
        internal readonly UInt16 DesignatorMax;

        /// <summary>
        /// If IsRange is TRUE: Indicates the inclusive lower
        /// bound of a sequential range of data indices that
        /// correspond, one-to-one and in the same order, to
        /// the usages specified by the usage range
        /// Range.UsageMin to Range.UsageMax.
        /// If IsRange is FALSE: Indicates the data index of
        /// the usage specified by NotRange.Usage.
        /// </summary>
        internal readonly UInt16 DataIndexMin;

        /// <summary>
        /// If IsRange is TRUE: Indicates the inclusive upper
        /// bound of a sequential range of data indices that
        /// correspond, one-to-one and in the same order, to
        /// the usages specified by the usage range
        /// Range.UsageMin to Range.UsageMax.
        /// If IsRange is FALSE: Reserved for internal system use.
        /// </summary>
        internal readonly UInt16 DataIndexMax;
    }
}