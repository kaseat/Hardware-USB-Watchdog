using System;
using System.Collections.Generic;

namespace HwdgWrapper
{
    /// <summary>
    /// Represents watchdog status.
    /// </summary>
    public class Status
    {
        /// <summary>
        /// Initialize watchdog with recieved data.
        /// </summary>
        /// <param name="data">Data recived from watchdog.</param>
        public Status(IReadOnlyList<Byte> data)
        {
            //todo: repalce magic numbers!
            RebootTimeout = 10000 + (data[0] & 0x7F) * 5000;
            ResponseTimeout = (((data[1] & 0x3F) >> 2) + 1) * 5000;
            State = (WatchdogState)((data[1] & 0x03) | ((data[2] & 2) << 1));
            HardResetAttempts = (Byte)((data[2] >> 5) + 1);
            SoftResetAttempts = (Byte)(((data[2] >> 2) & 0x07) + 1);
        }

        /// <summary>
        /// Get inner watchdog state.
        /// </summary>
        public WatchdogState State { get; set; }
        /// <summary>
        /// Get watchdog response timeout.
        /// </summary>
        public Int32 ResponseTimeout { get; set; }
        /// <summary>
        /// Get watchdog reboot tomeout.
        /// </summary>
        public Int32 RebootTimeout { get; set; }
        /// <summary>
        /// Get watchdog hard reset attempts.
        /// </summary>
        public Byte HardResetAttempts { get; set; }
        /// <summary>
        /// Get watchdog soft reset attempts.
        /// </summary>
        public Byte SoftResetAttempts { get; set; }

        public override Boolean Equals(Object obj)
        {
            return obj is Status status &&
                   State == status.State &&
                   ResponseTimeout == status.ResponseTimeout &&
                   RebootTimeout == status.RebootTimeout &&
                   HardResetAttempts == status.HardResetAttempts &&
                   SoftResetAttempts == status.SoftResetAttempts;
        }
        
        public override Int32 GetHashCode()
        {
            var hashCode = -1572781591;
            hashCode = hashCode * -1521134295 + State.GetHashCode();
            hashCode = hashCode * -1521134295 + ResponseTimeout.GetHashCode();
            hashCode = hashCode * -1521134295 + RebootTimeout.GetHashCode();
            hashCode = hashCode * -1521134295 + HardResetAttempts.GetHashCode();
            hashCode = hashCode * -1521134295 + SoftResetAttempts.GetHashCode();
            return hashCode;
        }
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