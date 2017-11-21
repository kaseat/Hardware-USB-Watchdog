using System;

namespace HwdgWrapper
{
    /// <summary>
    /// Represents hwdg settings installed by default
    /// </summary>
    public static class DefaultSettings
    {
        public const Byte HardResetAttempts = 3;
        public const Byte SoftResetAttempts = 3;
        public const Int32 ResponseTimeout = 90000;
        public const Int32 RebootTimeout = 150000;
        public const Boolean HardResetEnabled = false;
        public const Boolean LedDisabled = false;
        public const Boolean EventsEnabled = false;
        public const Boolean ApplySettingsAtStartup = false;
        public const Boolean RstPulseEnabled = false;
        public const Boolean PwrPulseEnabled = false;
    }
}