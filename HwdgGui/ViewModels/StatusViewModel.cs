using System;
using System.Windows.Threading;
using Caliburn.Micro;
using HwdgGui.Utils;
using HwdgWrapper;
using PropertyChanged;

namespace HwdgGui.ViewModels
{
    public class StatusViewModel : PropertyChangedBase
    {
        private readonly IHwdg hwdg;
        private readonly Dispatcher uiDisp;
        private readonly ISettingsProvider settings;

        /// <summary>
        /// ctor
        /// </summary>
        public StatusViewModel(IHwdg hwdg, ISettingsProvider settings)
        {
            // Inject dependency
            this.hwdg = hwdg;
            this.settings = settings;

            // Get and store main thread dispatcher.
            // This is necessary for further window color changes.
            uiDisp = Dispatcher.CurrentDispatcher;

            // Subscribe on hwdg events to handle HWDG
            // connecting and disconnecting situations.
            // We have only one instance of this view per app,
            // so no need to worry about memory leak.
            hwdg.Disconnected += OnDisconnected;
            hwdg.Connected += OnConnected;

            hwdg.Stop();
            if (AutoMonitor)
            {
                hwdg.RestoreStatus(settings.HwdgStatus);
                hwdg.Start();
            }
            HwStatus = hwdg.GetStatus();
        }

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
            // We must disable all controls and change accent color.
            if (HwStatus == null)
            {
                CanRunButton = false;
                CanEditSettings = false;
                RunButtonText = "Железный пёс не подключен";
                uiDisp.SetAccentColor(AccentColor.Disconnected);
            }
            else
            {
                // If hwdg is running all settings must be disabled as
                // we cant change them during monitoring execution.
                if ((HwStatus.State & WatchdogState.IsRunning) != 0)
                {
                    CanRunButton = true;
                    CanEditSettings = false;
                    RunButtonText = "Остановить монитроинг";
                    uiDisp.SetAccentColor(AccentColor.Running);
                }
                // If hwdg is not run we can edit settings.
                if ((HwStatus.State & WatchdogState.IsRunning) == 0)
                {
                    CanRunButton = true;
                    CanEditSettings = true;
                    RunButtonText = "Запустить монитринг";
                    uiDisp.SetAccentColor(AccentColor.Connected);
                }
                // Update view controls.
                settings.HwdgStatus = HwStatus;
                HardReset = (status.State & WatchdogState.HardRersetEnabled) != 0;
                HardResetCount = status.HardResetAttempts;
                SoftResetCount = status.SoftResetAttempts;
                RebootTimeout = status.RebootTimeout / 1000;
                ResponseTimeout = status.ResponseTimeout / 1000;
            }
        }

        /// <summary>
        /// Hwdg connected event handler.
        /// </summary>
        /// <param name="st">Hwdg status.</param>
        private void OnConnected(Status st)
        {
            if (AutoMonitor)
            {
                hwdg.RestoreStatus(settings.HwdgStatus);
                hwdg.Start();
                HwStatus = hwdg.GetStatus();
            }
            else
            {
                HwStatus = st;
            }
        }

        /// <summary>
        /// Hwdg disconnected event handler.
        /// </summary>
        private void OnDisconnected()
        {
            HwStatus = null;
        }

        /// <summary>
        /// Hard reset binding.
        /// </summary>
        public Boolean HardReset
        {
            get => (HwStatus.State & WatchdogState.HardRersetEnabled) != 0;
            set
            {
                if (value)
                {
                    hwdg.EnableHardReset();
                }
                else
                {
                    hwdg.DisableHardReset();
                }
                status = hwdg.GetStatus();
            }
        }

        /// <summary>
        /// Reboot timeout binding.
        /// </summary>
        public Int32 RebootTimeout { get; set; }

        /// <summary>
        /// Soft reset attempts binding.
        /// </summary>
        public Int32 SoftResetCount { get; set; }

        /// <summary>
        /// Hard reset attempts binding.
        /// </summary>
        public Int32 HardResetCount { get; set; }

        /// <summary>
        /// Response timeout binding.
        /// </summary>
        public Int32 ResponseTimeout { get; set; }

        /// <summary>
        /// Executes when WPF RebootTimeout slider value changes.
        /// </summary>
        /// <param name="timeout">Slider value.</param>
        public void OnRebootTimeoutChanged(Int32 timeout)
        {
            hwdg.SetRebootTimeout(timeout * 1000);
            HwStatus = hwdg.GetStatus();
        }

        /// <summary>
        /// Executes when WPF RebootTimeout slider value changes.
        /// </summary>
        /// <param name="timeout">Slider value.</param>
        public void OnResponseTimeoutChanged(Int32 timeout)
        {
            hwdg.SetResponseTimeout(timeout * 1000);
            HwStatus = hwdg.GetStatus();
        }

        /// <summary>
        /// Executes when WPF SoftResetCount slider value changes.
        /// </summary>
        /// <param name="val">Slider value.</param>
        public void OnSoftResetCountChanged(Byte val)
        {
            hwdg.SetSoftResetAttempts(val);
            HwStatus = hwdg.GetStatus();
        }

        /// <summary>
        /// Executes when WPF HardResetCount slider value changes.
        /// </summary>
        /// <param name="val">Slider value.</param>
        public void OnHardResetCountChanged(Byte val)
        {
            hwdg.SetHardResetAttempts(val);
            HwStatus = hwdg.GetStatus();
        }

        /// <summary>
        /// Determines if hwdg settings are editable at the moment.
        /// </summary>
        public Boolean CanEditSettings { get; set; }

        /// <summary>
        /// Hwdg client autorun state.
        /// </summary>
        public Boolean Autorun
        {
            get => settings.Autorun;
            set => settings.Autorun = value;
        }

        /// <summary>
        /// Hwdg client autorun state.
        /// </summary>
        public Boolean AutoMonitor
        {
            get => settings.Automonitor;
            set => settings.Automonitor = value;
        }

        /// <summary>
        /// Main button text. Value and action depends on hwdg state.
        /// </summary>
        public String RunButtonText { get; set; }

        /// <summary>
        /// Executes when RunMonitor click button occurs.
        /// </summary>
        public async void RunButtonAsync()
        {
            CanRunButton = false;
            status = await hwdg.GetStatusAsync();

            if ((status.State & WatchdogState.IsRunning) != 0)
            {
                uiDisp.SetAccentColor(AccentColor.Connected);
                RunButtonText = "Запустить монитроинг";
                CanEditSettings = true;
                await hwdg.StopAsync();
            }
            else
            {
                RunButtonText = "Остановить монитроинг";
                uiDisp.SetAccentColor(AccentColor.Running);
                CanEditSettings = false;
                await hwdg.StartAsync();
            }
            CanRunButton = true;
        }

        /// <summary>
        /// Enable / disable RunMonitor button.
        /// </summary>
        public Boolean CanRunButton { get; set; }
    }
}