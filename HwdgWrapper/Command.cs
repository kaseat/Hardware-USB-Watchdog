using System;

namespace HwdgWrapper
{
    /// <summary>
    /// Represents available commands for Hwdg
    /// </summary>
    public static class Cmd
    {
        public static Response DisableLed(this IWrapper wrp) => wrp.SendCommand(0xFF);
        public static Response EnableLed(this IWrapper wrp) => wrp.SendCommand(0xFE);
        public static Response DisableHardReset(this IWrapper wrp) => wrp.SendCommand(0xFD);
        public static Response EnableHardReset(this IWrapper wrp) => wrp.SendCommand(0xFC);
        public static Response Ping(this IWrapper wrp) => wrp.SendCommand(0xFB);
        public static Response Stop(this IWrapper wrp) => wrp.SendCommand(0xFA);
        public static Response Start(this IWrapper wrp) => wrp.SendCommand(0xF9);
        public static Response IsAlive(this IWrapper wrp) => wrp.SendCommand(0xF8);
        public static Response TestSoftReset(this IWrapper wrp) => wrp.SendCommand(0x7F);
        public static Response TestHardReset(this IWrapper wrp) => wrp.SendCommand(0x7E);
        public static Response DisableEvents(this IWrapper wrp) => wrp.SendCommand(0x03);
        public static Response EnableEvents(this IWrapper wrp) => wrp.SendCommand(0x02);
        public static Response GetStatus(this IWrapper wrp) => wrp.SendCommand(0x01);
        public static Response RstPulseOnStartupDisable(this IWrapper wrp) => wrp.SendCommand(0x3F);
        public static Response RstPulseOnStartupEnable(this IWrapper wrp) => wrp.SendCommand(0x3E);
        public static Response PwrPulseOnStartupDisable(this IWrapper wrp) => wrp.SendCommand(0x3D);
        public static Response PwrPulseOnStartupEnable(this IWrapper wrp) => wrp.SendCommand(0x3C);
        public static Response ApplyUserSettingsAtStartup(this IWrapper wrp) => wrp.SendCommand(0x3B);
        public static Response LoadDefaultSettingsAtStartup(this IWrapper wrp) => wrp.SendCommand(0x3A);
        public static Response SaveCurrentSettingsAsUsers(this IWrapper wrp) => wrp.SendCommand(0x39);
        public static void FactoryReset(this IWrapper wrp) => wrp.SendCommand(0xF7);
        public static void ChipReset(this IWrapper wrp) => wrp.SendCommand(0x00);

        public static Response SetHardResetAttempts(this IWrapper wrp, Byte count)
        {
            if (count > 8) count = 8;
            if (count < 1) count = 1;
            var nhi = count - 1;
            return wrp.SendCommand((Byte)(nhi | 0x18));
        }

        public static Response SetSoftResetAttempts(this IWrapper wrp, Byte count)
        {
            if (count > 8) count = 8;
            if (count < 1) count = 1;
            var nsi = count - 1;
            return wrp.SendCommand((Byte)(nsi | 0x10));
        }

        public static Response SetResponseTimeout(this IWrapper wrp, Int32 ms)
        {
            if (ms > 320000) ms = 320000;
            if (ms < 5000) ms = 5000;
            var trsi = ms / 5000 - 1;
            return wrp.SendCommand((Byte)(trsi | 0x40));
        }

        public static Response SetRebootTimeout(this IWrapper wrp, Int32 ms)
        {
            if (ms > 645000) ms = 645000;
            if (ms < 10000) ms = 10000;
            var trbi = (ms - 10000) / 5000;
            return wrp.SendCommand((Byte)(trbi | 0x80));
        }
    }
}