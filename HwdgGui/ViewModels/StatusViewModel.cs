using System;
using System.Windows.Threading;
using Caliburn.Micro;
using HwdgGui.Annotations;
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
            hwdg.Updated += OnUpdated;

            // Get last known status.
            HwStatus = hwdg.GetStatus();
        }

        private void OnDisconnected() => HwStatus = null;
        private void OnConnected(Status st)
        {
            // If auto monitoring enabled, start it immediately.
            // We'll update status in upcoming 'OnUpdated' event.
            if (settings.Automonitor)
            {
                hwdg.Stop();
                hwdg.Start();
            }

            // Otherwise just update current status.
            else
            {
                HwStatus = status;
            }
        }
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
            // We must disable all controls and change accent color.
            if (HwStatus == null)
            {
                uiDisp.SetAccentColor(AccentColor.Disconnected);
                UpdateControlsOnDisconnect();
            }
            else
            {
                // If hwdg is running all settings must be disabled as
                // we cant change them during monitoring execution.
                if ((HwStatus.State & WatchdogState.IsRunning) != 0)
                {
                    uiDisp.SetAccentColor(AccentColor.Running);
                    UpdateControlsOnRunning();
                }

                // If hwdg is not run we can edit settings.
                if ((HwStatus.State & WatchdogState.IsRunning) == 0)
                {
                    uiDisp.SetAccentColor(AccentColor.Connected);
                    UpdateControlsOnConnected();
                }
            }
        }

        /// <summary>
        /// Main button text. Value and action depends on hwdg state.
        /// </summary>
        [UsedImplicitly]
        public String RunButtonText { get; set; }

        /// <summary>
        /// Executes when RunMonitor click button occurs.
        /// </summary>
        [UsedImplicitly]
        public async void RunButtonAsync()
        {
            // Ignore any user actions when button handling is pending.
            if (processing) return;
            processing = true;

            // Get current HWDG status.
            status = await hwdg.GetStatusAsync();
            if (status == null)
            {
                UpdateControlsOnDisconnect();
            }
            else
            {
                if ((status.State & WatchdogState.IsRunning) != 0)
                {
                    UpdateControlsOnRunning();
                    await hwdg.StopAsync();
                    processing = false;
                    return;
                }
                if ((status.State & WatchdogState.IsRunning) == 0)
                {
                    UpdateControlsOnConnected();
                    await hwdg.StartAsync();
                    processing = false;
                    return;
                }
            }

            // Release processing key.
            processing = false;
        }

        private Boolean processing;

        private void UpdateControlsOnConnected()
        {
            RunButtonText = "СТАРТ";
            CanRunButton = true;
            ErrVis = false;
            ConnVis = true;
            RunVis = false;
        }

        private void UpdateControlsOnRunning()
        {
            RunButtonText = "СТОП";
            CanRunButton = true;
            ErrVis = false;
            ConnVis = false;
            RunVis = true;
        }

        private void UpdateControlsOnDisconnect()
        {
            RunButtonText = "ОШИБКА";
            CanRunButton = false;
            ErrVis = true;
            ConnVis = false;
            RunVis = false;
        }

        /// <summary>
        /// Visibility of "Error" string.
        /// </summary>
        [UsedImplicitly]
        public Boolean ErrVis { get; set; }

        /// <summary>
        /// Visibility of "Connected" string.
        /// </summary>
        [UsedImplicitly]
        public Boolean ConnVis { get; set; }

        /// <summary>
        /// Visibility of "IsRunning" string.
        /// </summary>
        [UsedImplicitly]
        public Boolean RunVis { get; set; }

        /// <summary>
        /// Enable / disable RunMonitor button.
        /// </summary>
        [UsedImplicitly]
        public Boolean CanRunButton { get; set; }
    }
}