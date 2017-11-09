using System;
using Caliburn.Micro;
using HwdgGui.Annotations;
using HwdgGui.Utils;
using HwdgWrapper;
using PropertyChanged;

namespace HwdgGui.ViewModels
{
    public class MainSettingsViewModel : PropertyChangedBase
    {
        private readonly IHwdg hwdg;
        private readonly ISettingsProvider settings;

        public MainSettingsViewModel(IHwdg hwdg, ISettingsProvider settings)
        {
            // Inject dependency
            this.hwdg = hwdg;
            this.settings = settings;

            // Subscribe on hwdg events to handle HWDG
            // connecting and disconnecting situations.
            // We have only one instance of this view per app,
            // so no need to worry about memory leak.
            hwdg.Disconnected += OnDisconnected;
            hwdg.Connected += OnConnected;
            hwdg.Updated += OnUpdated;

            // Get last known status.
            HwStatus = hwdg.GetStatus();
        }

        /// <summary>
        /// Hwdg connected event handler.
        /// </summary>
        /// <param name="st">Hwdg status.</param>
        private void OnConnected(Status st)
        {
            // If auto monitoring enabled, we'll update status
            // later in upcoming 'OnUpdated' event.
            if (!settings.Automonitor)
            {
                HwStatus = st;
            }
        }

        /// <summary>
        /// Hwdg disconnected event handler.
        /// </summary>
        private void OnDisconnected() => HwStatus = null;

        /// <summary>
        /// Hwdg status updated event handler.
        /// </summary>
        /// <param name="sta"></param>
        private void OnUpdated(Status sta) => HwStatus = sta;

        private Status status;
        [DoNotNotify]
        private Status HwStatus
        {
            get => status;
            set
            {
                status = value;
                OnStatusUpdate();
            }
        }

        private void OnStatusUpdate()
        {
            // If hwdg status is null that means hwdg disconnected.
            // We must disable all controls.
            if (HwStatus == null)
            {
                CanEditSettings = false;
                HwdgConnected = false;
            }
            else
            {
                // If hwdg is running all settings must be disabled as
                // we cant change them during monitoring execution.
                if ((HwStatus.State & WatchdogState.IsRunning) != 0)
                {
                    CanEditSettings = false;
                }

                // If hwdg is not run we can edit settings.
                if ((HwStatus.State & WatchdogState.IsRunning) == 0)
                {
                    CanEditSettings = true;
                }

                // Update view controls.
                HwdgConnected = true;
                HardReset = (status.State & WatchdogState.HardRersetEnabled) != 0;
                Led = (status.State & WatchdogState.LedDisabled) == 0;
                HardResetCount = status.HardResetAttempts;
                SoftResetCount = status.SoftResetAttempts;
                RebootTimeout = status.RebootTimeout / 1000;
                ResponseTimeout = status.ResponseTimeout / 1000;
            }
        }


        /// <summary>
        /// HwdgConnected binding.
        /// </summary>
        [UsedImplicitly]
        public Boolean HwdgConnected { get; set; }

        /// <summary>
        /// Hard reset binding.
        /// </summary>
        [UsedImplicitly]
        public Boolean HardReset { get; set; }

        /// <summary>
        /// Executes when WPF SoftResetCount slider value changes.
        /// </summary>
        [UsedImplicitly]
        public async void OnHwdgHrCheckedAsync() => await hwdg.EnableHardResetAsync();

        /// <summary>
        /// Executes when WPF SoftResetCount slider value changes.
        /// </summary>
        [UsedImplicitly]
        public async void OnHwdgHrUncheckedAsync() => await hwdg.DisableHardResetAsync();

        /// <summary>
        /// Reboot timeout binding.
        /// </summary>
        [UsedImplicitly]
        public Int32 RebootTimeout { get; set; }

        /// <summary>
        /// Soft reset attempts binding.
        /// </summary>
        [UsedImplicitly]
        public Int32 SoftResetCount { get; set; }

        /// <summary>
        /// Hard reset attempts binding.
        /// </summary>
        [UsedImplicitly]
        public Int32 HardResetCount { get; set; }

        /// <summary>
        /// Response timeout binding.
        /// </summary>
        [UsedImplicitly]
        public Int32 ResponseTimeout { get; set; }

        /// <summary>
        /// Executes when WPF RebootTimeout slider value changes.
        /// </summary>
        /// <param name="timeout">Slider value.</param>
        [UsedImplicitly]
        public void OnRebootTimeoutChanged(Int32 timeout) => hwdg.SetRebootTimeout(timeout * 1000);

        /// <summary>
        /// Executes when WPF RebootTimeout slider value changes.
        /// </summary>
        /// <param name="timeout">Slider value.</param>
        [UsedImplicitly]
        public void OnResponseTimeoutChanged(Int32 timeout) => hwdg.SetResponseTimeout(timeout * 1000);

        /// <summary>
        /// Executes when WPF SoftResetCount slider value changes.
        /// </summary>
        /// <param name="val">Slider value.</param>
        [UsedImplicitly]
        public void OnSoftResetCountChanged(Byte val) => hwdg.SetSoftResetAttempts(val);

        /// <summary>
        /// Executes when WPF HardResetCount slider value changes.
        /// </summary>
        /// <param name="val">Slider value.</param>
        [UsedImplicitly]
        public void OnHardResetCountChanged(Byte val) => hwdg.SetHardResetAttempts(val);

        /// <summary>
        /// Determines if hwdg settings are editable at the moment.
        /// </summary>
        [UsedImplicitly]
        public Boolean CanEditSettings { get; set; }

        /// <summary>
        /// Led on binding.
        /// </summary>
        [UsedImplicitly]
        public Boolean Led { get; set; }

        /// <summary>
        /// Executes when WPF Led checks.
        /// </summary>
        [UsedImplicitly]
        public async void LedOnAsync() => await hwdg.EnableLedAsync();

        /// <summary>
        /// Executes when WPF Led unchecks.
        /// </summary>
        [UsedImplicitly]
        public async void LedOffAsync() => await hwdg.DisableLedAsync();
    }
}