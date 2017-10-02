using System;
using System.IO.Ports;
using System.Threading.Tasks;
using HwdgApi.Helpers;

namespace HwdgApi
{
    public class SerialHwdgProvider : IHwdgProvider
    {
        private readonly SerialPort serial;
        public SerialHwdgProvider(SerialPort serial)
        {
            this.serial = serial;
        }
        public async Task<Response> SetRebootTimeout(Int32 ms)
        {
            if (ms > 645000) ms = 645000;
            if (ms < 10000) ms = 10000;
            var trbi = (ms - 10000) / 5000;
            var cmd = (Byte)(trbi | 0x80);
            return await serial.SendHwdgCommandAsync(cmd);
        }

        public async Task<Response> SetResponseTimeout(Int32 ms)
        {
            if (ms > 320000) ms = 320000;
            if (ms < 5000) ms = 5000;
            var trsi = ms / 5000 - 1;
            var cmd = (Byte)(trsi | 0x40);
            return await serial.SendHwdgCommandAsync(cmd);
        }

        public async Task<Response> SetSoftResetAttempts(Byte count)
        {
            if (count > 8) count = 8;
            if (count < 1) count = 1;
            var nsi = count  - 1;
            var cmd = (Byte)(nsi | 0x10);
            return await serial.SendHwdgCommandAsync(cmd);
        }

        public async Task<Response> SetHardResetAttempts(Byte count)
        {
            if (count > 8) count = 8;
            if (count < 1) count = 1;
            var nhi = count - 1;
            var cmd = (Byte)(nhi | 0x18);
            return await serial.SendHwdgCommandAsync(cmd);
        }

        public async Task<Response> EnableHardReset()
        {
            return await serial.SendHwdgCommandAsync(0x03);
        }

        public async Task<Response> DisableHardReset()
        {
            return await serial.SendHwdgCommandAsync(0x04);
        }

        public async Task<Response> Start()
        {
            return await serial.SendHwdgCommandAsync(0x01);
        }

        public async Task<Response> Stop()
        {
            return await serial.SendHwdgCommandAsync(0x02);
        }

        public async Task<Response> Ping()
        {
            return await serial.SendHwdgCommandAsync(0x05);
        }

        public async Task<Status> GetStatus()
        {
            return await serial.GetHwdgStatusAsync();
        }
    }
}