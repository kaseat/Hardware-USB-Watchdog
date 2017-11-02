using System;

namespace HwdgWrapper
{
    /// <summary>
    /// Represents
    /// </summary>
    public enum Response : Byte
    {
        Error = 0x50,
        NotFound = 0x51,
        ErrorSendCmd = 0x52,
        ErrorGetStatus = 0x23,

        Busy = 0x20,
        EnableEventsOk = 0x21,
        DisableEventsOk = 0x22,
        TestHardResetOk = 0x23,
        TestSoftResetOk = 0x24,
        StartOk = 0x25,
        StopOk = 0x26,
        PingOk = 0x27,
        EnableHardResetOk = 0x28,
        DisableHardResetOk = 0x29,
        EnableLedOk = 0x2A,
        DisableLedOk = 0x2B,
        SetRebootTimeoutOk = 0x2C,
        SetResponseTimeoutOk = 0x2D,
        SetSoftResetAttemptsOk = 0x2E,
        SetHardResetAttemptsOk = 0x2F,

        SaveCurrentSettingsOk = 0x40,
        LoadDefaultSettingsAtStartupOk = 0x41,
        ApplyUserSettingsAtStartupOk = 0x42,

        PwrPulseOnStartupEnableOk = 0x43,
        PwrPulseOnStartupDisableOk = 0x44,
        RstPulseOnStartupEnableOk = 0x45,
        RstPulseOnStartupDisableOk = 0x46,

        SaveSettingsError = 0x47,
        PowerPulseOk = 0x48,

        UnknownCommand = 0x4F,

        FirstResetOccurred = 0x30,
        SoftResetOccurred = 0x31,
        HardResetOccurred = 0x32,
        MovedToIdle = 0x33,
        WatchdogOk = 0x34,
        SoftwareVersion = 0x55,
    }
}
