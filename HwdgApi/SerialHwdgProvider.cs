using System;
using System.IO.Ports;

namespace HwdgApi
{
    public class SerialHwdgProvider : IHwdgProvider
    {
        private readonly SerialPort serial;
        public SerialHwdgProvider(SerialPort serial)
        {
            this.serial = serial;
        }
        public Response SetRebootTimeout(Int32 ms)
        {
            throw new NotImplementedException();
        }

        public Response SetResponseTimeout(Int32 ms)
        {
            throw new NotImplementedException();
        }

        public Response SetSoftResetAttempts(Byte count)
        {
            throw new NotImplementedException();
        }

        public Response SetHardResetAttempts(Byte count)
        {
            throw new NotImplementedException();
        }

        public Response EnableHardReset()
        {
            throw new NotImplementedException();
        }

        public Response DisableHardReset()
        {
            throw new NotImplementedException();
        }

        public Response Start()
        {
            throw new NotImplementedException();
        }

        public Response Stop()
        {
            throw new NotImplementedException();
        }

        public Response Ping()
        {
            throw new NotImplementedException();
        }

        public Status GetStatus()
        {
            throw new NotImplementedException();
        }
    }
}