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
using HwdgHid.Win32;

namespace HwdgHid
{
    public class DeviceCapabilities
    {
        internal DeviceCapabilities(HidpCaps capabilities)
        {
            InputReportByteLength = capabilities.InputReportByteLength;
            OutputReportByteLength = capabilities.OutputReportByteLength;
            FeatureReportByteLength = capabilities.FeatureReportByteLength;
            NumberLinkCollectionNodes = capabilities.NumberLinkCollectionNodes;
            NumberInputButtonCaps = capabilities.NumberInputButtonCaps;
            NumberInputValueCaps = capabilities.NumberInputValueCaps;
            NumberInputDataIndices = capabilities.NumberInputDataIndices;
            NumberOutputButtonCaps = capabilities.NumberOutputButtonCaps;
            NumberOutputValueCaps = capabilities.NumberOutputValueCaps;
            NumberOutputDataIndices = capabilities.NumberOutputDataIndices;
            NumberFeatureButtonCaps = capabilities.NumberFeatureButtonCaps;
            NumberFeatureValueCaps = capabilities.NumberFeatureValueCaps;
            NumberFeatureDataIndices = capabilities.NumberFeatureDataIndices;
        }

        public UInt16 InputReportByteLength { get; }
        public UInt16 OutputReportByteLength { get; }
        public UInt16 FeatureReportByteLength { get; }
        public UInt16 NumberLinkCollectionNodes { get; }
        public UInt16 NumberInputButtonCaps { get; }
        public UInt16 NumberInputValueCaps { get; }
        public UInt16 NumberInputDataIndices { get; }
        public UInt16 NumberOutputButtonCaps { get; }
        public UInt16 NumberOutputValueCaps { get; }
        public UInt16 NumberOutputDataIndices { get; }
        public UInt16 NumberFeatureButtonCaps { get; }
        public UInt16 NumberFeatureValueCaps { get; }
        public UInt16 NumberFeatureDataIndices { get; }
    }
}