using System;
using System.Collections.Generic;
using System.Text;

namespace HwdgApi
{
    /// <summary>
    /// Representd hwdg view model.
    /// </summary>
    public class Hwdg
    {
        private Boolean confingBeenUpdated;
        private Status status;
        private readonly IHwdgProvider hwdg;

        /// <summary>
        /// Initialize hwdg view model with IHwdgProvider instance.
        /// </summary>
        /// <param name="hwdg">hwdg interface provider.</param>
        public Hwdg(IHwdgProvider hwdg)
        {
            this.hwdg = hwdg;
            status = hwdg.GetStatus();
        }

        /// <summary>
        /// Getss or sets hwdg response timeout.
        /// </summary>
        public Int32 ResponseTimeout
        {
            get => confingBeenUpdated ? (status = hwdg.GetStatus()).ResponseTimeout : status.ResponseTimeout;
            set => confingBeenUpdated = hwdg.SetResponseTimeout(value) == Response.SetResponseTimeoutOk;
        }

        public Int32 RebootTimeout
        {
            get => confingBeenUpdated ? (status = hwdg.GetStatus()).RebootTimeout : status.RebootTimeout;
            set => confingBeenUpdated = hwdg.SetRebootTimeout(value) == Response.SetRebootTimeoutOk;
        }

        public Byte SoftResetAttempts
        {
            get => confingBeenUpdated ? (status = hwdg.GetStatus()).SoftResetAttempts : status.SoftResetAttempts;
            set => confingBeenUpdated = hwdg.SetSoftResetAttempts(value) == Response.SetSoftResetAttemptsOk;
        }

        public Byte HardResetAttempts
        {
            get => confingBeenUpdated ? (status = hwdg.GetStatus()).HardResetAttempts : status.HardResetAttempts;
            set => confingBeenUpdated = status.HardResetAttempts == value &&
                                        hwdg.SetHardResetAttempts(value) == Response.SetHardResetAttemptsOk;
        }

        public Boolean MonitoringEnabled
        {
            get => confingBeenUpdated
                ? (UpdateStatus().State & WatchdogState.IsRunning) != 0
                : (status.State & WatchdogState.IsRunning) != 0;
            set => confingBeenUpdated = (status.State & WatchdogState.IsRunning) == 0 == value && (value
                                            ? hwdg.Start() == Response.StartOk
                                            : hwdg.Stop() == Response.StopOk);
        }

        public Boolean HardResetEnabled
        {
            get => confingBeenUpdated
                ? (UpdateStatus().State & WatchdogState.HardRersetEnabled) != 0
                : (status.State & WatchdogState.HardRersetEnabled) != 0;
            set => confingBeenUpdated = (status.State & WatchdogState.HardRersetEnabled) == 0 == value && (value
                                            ? hwdg.EnableHardReset() == Response.EnableHardResetOk
                                            : hwdg.DisableHardReset() == Response.DisableHardResetOk);
        }

        private Status UpdateStatus()
        {
            confingBeenUpdated = false;
            return status = hwdg.GetStatus();
        }
    }
}
