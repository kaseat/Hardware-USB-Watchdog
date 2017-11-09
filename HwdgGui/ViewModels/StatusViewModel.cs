using System;
using HwdgGui.Annotations;
using HwdgGui.Utils;
using HwdgWrapper;

namespace HwdgGui.ViewModels
{
    public sealed class StatusViewModel : BaseHwdgVm
    {
        /// <summary>
        /// ctor
        /// </summary>
        /// <param name="hwdg">IHwdg instance.</param>
        /// <param name="settings">ISettingsProvider instance.</param>
        public StatusViewModel(IHwdg hwdg, ISettingsProvider settings) : base(hwdg, settings)
        {
        }

        protected override void OnStatusUpdate()
        {
            // If hwdg status is null that means hwdg disconnected.
            // We must disable all controls and change accent color.
            if (HwStatus == null)
            {
                UiDisp.SetAccentColor(AccentColor.Disconnected);
                UpdateControlsOnDisconnect();
            }
            else
            {
                // If hwdg is running all settings must be disabled as
                // we cant change them during monitoring execution.
                if ((HwStatus.State & WatchdogState.IsRunning) != 0)
                {
                    UiDisp.SetAccentColor(AccentColor.Running);
                    UpdateControlsOnRunning();
                }

                // If hwdg is not run we can edit settings.
                if ((HwStatus.State & WatchdogState.IsRunning) == 0)
                {
                    UiDisp.SetAccentColor(AccentColor.Connected);
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
            HwStatus = await Hwdg.GetStatusAsync();

            // If there is no HWDG move to 'disconnected' mode.
            if (HwStatus == null)
            {
                UpdateControlsOnDisconnect();
            }
            else
            {
                // If HWDG connected stop monitoring and update view.
                if ((HwStatus.State & WatchdogState.IsRunning) != 0)
                {
                    // Save current status for futher comparison.
                    Settings.HwdgStatus = HwStatus;
                    UpdateControlsOnRunning();
                    await Hwdg.StopAsync();
                    processing = false;
                    return;
                }

                // If HWDG is online, save current settings and start monitoring.
                if ((HwStatus.State & WatchdogState.IsRunning) == 0)
                {
                    UpdateControlsOnConnected();
                    // If settings changed since last button press, update HWDG settings.
                    if (!Settings.HwdgStatus.EqualsState(HwStatus))
                        await Hwdg.SaveCurrentStateAsync();
                    await Hwdg.StartAsync();
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