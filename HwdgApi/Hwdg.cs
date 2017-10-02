using System;

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
            status = hwdg.GetStatus().Result;
        }

        /// <summary>
        /// Getss or sets hwdg response timeout.
        /// </summary>
        public Int32 ResponseTimeout
        {
            get => confingBeenUpdated ? (status = hwdg.GetStatus().Result).ResponseTimeout : status.ResponseTimeout;
            set => confingBeenUpdated = hwdg.SetResponseTimeout(value).Result == Response.SetResponseTimeoutOk;
        }

        public Int32 RebootTimeout
        {
            get => confingBeenUpdated ? (status = hwdg.GetStatus().Result).RebootTimeout : status.RebootTimeout;
            set => confingBeenUpdated = hwdg.SetRebootTimeout(value).Result == Response.SetRebootTimeoutOk;
        }

        public Byte SoftResetAttempts
        {
            get => confingBeenUpdated ? (status = hwdg.GetStatus().Result).SoftResetAttempts : status.SoftResetAttempts;
            set => confingBeenUpdated = hwdg.SetSoftResetAttempts(value).Result == Response.SetSoftResetAttemptsOk;
        }

        public Byte HardResetAttempts
        {
            get => confingBeenUpdated ? (status = hwdg.GetStatus().Result).HardResetAttempts : status.HardResetAttempts;
            set => confingBeenUpdated = status.HardResetAttempts == value &&
                                        hwdg.SetHardResetAttempts(value).Result == Response.SetHardResetAttemptsOk;
        }

        public Boolean MonitoringEnabled
        {
            get => confingBeenUpdated
                ? (UpdateStatus().State & WatchdogState.IsRunning) != 0
                : (status.State & WatchdogState.IsRunning) != 0;
            set => confingBeenUpdated = (status.State & WatchdogState.IsRunning) == 0 == value && (value
                                            ? hwdg.Start().Result == Response.StartOk
                                            : hwdg.Stop().Result == Response.StopOk);
        }

        public Boolean HardResetEnabled
        {
            get => confingBeenUpdated
                ? (UpdateStatus().State & WatchdogState.HardRersetEnabled) != 0
                : (status.State & WatchdogState.HardRersetEnabled) != 0;
            set => confingBeenUpdated = (status.State & WatchdogState.HardRersetEnabled) == 0 == value && (value
                                            ? hwdg.EnableHardReset().Result == Response.EnableHardResetOk
                                            : hwdg.DisableHardReset().Result == Response.DisableHardResetOk);
        }

        private Status UpdateStatus()
        {
            confingBeenUpdated = false;
            return status = hwdg.GetStatus().Result;
        }
    }
}
