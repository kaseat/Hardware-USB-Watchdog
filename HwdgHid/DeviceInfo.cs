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

namespace HwdgHid
{
    public class DeviceInfo
    {
        internal IntPtr Handle { get; set; }
        public String Path { get; set; }
        public DeviceCapabilities Capabilities { get; set; }
        public Int32 VendorId { get; set; }
        public Int32 ProductId { get; set; }
        public Int32 Version { get; set; }
        public String ProductName { get; set; }
        public String Manufacturer { get; set; }

        public override String ToString()
            => $"VID 0x{VendorId:X4}    PID 0x{ProductId:X4}    {ProductName}";
    }
}