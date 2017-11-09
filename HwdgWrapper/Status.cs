using System;
using System.Collections.Generic;

namespace HwdgWrapper
{
    /// <summary>
    /// Represents watchdog status.
    /// </summary>
    public sealed class Status
    {
        /// <summary>
        /// Initialize watchdog with recieved data.
        /// </summary>
        /// <param name="data">Data recived from watchdog.</param>
        public Status(IReadOnlyList<Byte> data)
        {
            if (data.Count != 5) throw new ArgumentException("Malformed data array");

            //todo: repalce magic numbers!
            RebootTimeout = 10000 + (data[0] & 0x7F) * 5000;
            ResponseTimeout = (((data[1] & 0xFC) >> 2) + 1) * 5000;
            State = (WatchdogState) ((data[1] & 3) | ((data[2] & 1) << 2) | (data[3] << 3));
            SoftResetAttempts = (Byte) ((data[2] >> 5) + 1);
            HardResetAttempts = (Byte) (((data[2] >> 2) & 7) + 1);
            RawData = data[0] << 24 | data[1] << 16 | data[2] << 8 | data[3];
        }

        /// <summary>
        /// Initialize watchdog with data from registry.
        /// </summary>
        /// <param name="data">Data recived from registry.</param>
        public Status(Int32 data) : this(
            new[]
            {
                Convert.ToByte((data >> 24) & 0xFF),
                Convert.ToByte((data >> 16) & 0xFF),
                Convert.ToByte((data >> 8) & 0xFF),
                Convert.ToByte(data & 0xFF),
                Convert.ToByte(0xFF),
            })
        {
        }

        /// <summary>
        /// Raw data to restore this object.
        /// </summary>
        public Int32 RawData { get; }
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

        /// <inheritdoc />
        public override Boolean Equals(Object obj)
        {
            return obj is Status status &&
                   State == status.State &&
                   ResponseTimeout == status.ResponseTimeout &&
                   RebootTimeout == status.RebootTimeout &&
                   HardResetAttempts == status.HardResetAttempts &&
                   SoftResetAttempts == status.SoftResetAttempts;
        }

        public Boolean EqualsState(Object obj)
        {
            return obj is Status status &&
                   ((Int32)State & (1 << 2)) == ((Int32)status.State & (1 << 2)) &&
                   ResponseTimeout == status.ResponseTimeout &&
                   RebootTimeout == status.RebootTimeout &&
                   HardResetAttempts == status.HardResetAttempts &&
                   SoftResetAttempts == status.SoftResetAttempts;
        }

        /// <inheritdoc />
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

        /// <inheritdoc />
        public override String ToString() => RawData.ToString("X8");
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
        /// Is hwdg waiting for system reboot.
        /// </summary>
        WaitingForReboot = 1 << 1,

        /// <summary>
        /// Is hard reset enabled.
        /// </summary>
        HardRersetEnabled = 1 << 2,

        /// <summary>
        /// Is LED disabled.
        /// </summary>
        LedDisabled = 1 << 3,

        /// <summary>
        /// Are events enabled.
        /// </summary>
        EventsEnabled = 1 << 4,

        /// <summary>
        /// Does hwdg apply user settings at startup.
        /// </summary>
        LoadUserSettings = 1 << 5,

        /// <summary>
        /// Does RST pulse at startup enabled.
        /// </summary>
        RstPulseEnabled = 1 << 6,

        /// <summary>
        /// Does PWR pulse at startup enabled.
        /// </summary>
        PwrPulseEnabled = 1 << 7,
    }
}