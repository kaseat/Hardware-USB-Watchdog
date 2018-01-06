// Copyright 2018 Oleg Petrochenko
// 
// This file is part of HwdgWrapper.
// 
// HwdgWrapper is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or any
// later version.
// 
// HwdgWrapper is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with HwdgWrapper. If not, see <http://www.gnu.org/licenses/>.

using System;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using HwdgHid;

namespace HwdgWrapper
{
    public class HidWrapper:IWrapper
    {
        private readonly IHidDevice device;
        public HidWrapper(IHidDevice device)
        {
            this.device = device;
        }

        public event HwdgResult HwdgConnected;
        public event Action HwdgDisconnected;
        public event HwdgResult HwdgUpdated;
        public Status GetStatus()
        {
            throw new NotImplementedException();
        }

        public Response SendCommand(Byte cmd)
        {
            var report = new Report
            {
                ReportId = 1,
                Data = new[] {cmd}
            };

            device.SendReport(report);
            var result = device.GetReport(1);
            return (Response) result.Data.ToArray().Last();
        }

        public Task<Status> GetStatusAsync(CancellationToken ct = default(CancellationToken))
        {
            throw new NotImplementedException();
        }

        public Task<Response> SendCommandAsync(Byte cmd, CancellationToken ct = default(CancellationToken))
        {
            throw new NotImplementedException();
        }
    }
}