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
using System.Collections.Generic;

namespace HwdgHid.Win32
{
    public class HidDevice : IHidDevice
    {
        public Boolean IsOpen { get; }
        public Boolean IsConnected { get; }
        public String DevicePath { get; }

        public void OpenDevice()
        {
            throw new NotImplementedException();
        }

        public void CloseDevice()
        {
            throw new NotImplementedException();
        }

        public DeviceInfo Info { get; }

        public IEnumerable<Byte> Read()
        {
            throw new NotImplementedException();
        }

        public Boolean Write(IEnumerable<Byte> data)
        {
            throw new NotImplementedException();
        }
    }
}