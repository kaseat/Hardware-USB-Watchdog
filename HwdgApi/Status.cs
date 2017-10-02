using System;
using System.Collections.Generic;

namespace HwdgApi
{
    /// <summary>
    /// Represents watchdog status.
    /// </summary>
    public struct Status
    {
        /// <summary>
        /// Initialize watchdog with recieved data.
        /// </summary>
        /// <param name="data">Data recived from watchdog.</param>
        public Status(IReadOnlyList<Byte> data)
        {
            //todo: remove magic numbers!
            RebootTimeout = 10000 + (data[0] & 0x7F) * 5000;
            ResponseTimeout = (((data[1] & 0x3F) >> 2) + 1) * 5000;
            State = (WatchdogState) ((data[1] & 0x03) | ((data[2] & 2) << 1));
            HardResetAttempts = (Byte) ((data[2] >> 5) + 1);
            SoftResetAttempts = (Byte) (((data[2] >> 2) & 0x07) + 1);
        }

        /// <summary>
        /// Get inner watchdog state.
        /// </summary>
        public WatchdogState State { get; }
        /// <summary>
        /// Get watchdog response timeout.
        /// </summary>
        public Int32 ResponseTimeout { get; }
        /// <summary>
        /// Get watchdog reboot tomeout.
        /// </summary>
        public Int32 RebootTimeout { get; }
        /// <summary>
        /// Get watchdog hard reset attempts.
        /// </summary>
        public Byte HardResetAttempts { get; }
        /// <summary>
        /// Get watchdog soft reset attempts.
        /// </summary>
        public Byte SoftResetAttempts { get; }
    }

    /// <summary>
    /// Represents watchdog inner state.
    /// </summary>
    [Flags]
    public enum WatchdogState : Byte
    {
        /// <summary>
        /// Watcdog is running.
        /// </summary>
        IsRunning = 1 << 0,
        /// <summary>
        /// Is hard reset enabled.
        /// </summary>
        HardRersetEnabled = 1 << 1,
        /// <summary>
        /// Is hwdg waiting for system reboot.
        /// </summary>
        WaitingForReboot = 1 << 2,
    }
}
