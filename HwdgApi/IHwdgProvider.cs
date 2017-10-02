using System;
namespace HwdgApi
{
    public interface IHwdgProvider
    {
        Response SetRebootTimeout(Int32 ms);
        Response SetResponseTimeout(Int32 ms);
        Response SetSoftResetAttempts(Byte count);
        Response SetHardResetAttempts(Byte count);
        Response EnableHardReset();
        Response DisableHardReset();
        Response Start();
        Response Stop();
        Response Ping();
        Status GetStatus();
    }
}
