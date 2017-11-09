using System;
using Caliburn.Micro;
using HwdgGui.Annotations;
using HwdgGui.Utils;
using HwdgWrapper;
using PropertyChanged;

namespace HwdgGui.ViewModels
{
    public class OtherSettingsViewModel : PropertyChangedBase
    {
        private readonly IHwdg hwdg;
        private readonly ISettingsProvider settings;

        public OtherSettingsViewModel(IHwdg hwdg, ISettingsProvider settings)
        {
            this.settings = settings;
            this.hwdg = hwdg;
            hwdg.Connected += OnConnected;
            hwdg.Disconnected += OnDisconnected;
            hwdg.Updated += OnUpdated;

            HwStatus = hwdg.LastStatus;
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

        /// <summary>
        /// Determines if hwdg settings are editable at the moment.
        /// </summary>
        [UsedImplicitly]
        public Boolean HwdgSettingsVisible { get; set; }
        private void OnStatusUpdate()
        {
            // If hwdg status is null that means hwdg disconnected.
            // We must disable all controls.
            if (HwStatus == null)
            {
                HwdgSettingsVisible = false;
            }
            else
            {
                HwdgSettingsVisible = true;
                // Update view controls.
                settings.HwdgStatus = HwStatus;
                RstPulse = (HwStatus.State & WatchdogState.RstPulseEnabled) != 0;
                PwrPulse = (HwStatus.State & WatchdogState.PwrPulseEnabled) != 0;
            }
        }

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

        /// <summary>
        /// Hwdg client autorun state.
        /// </summary>
        [UsedImplicitly]
        public Boolean Autorun
        {
            get => settings.Autorun;
            set => settings.Autorun = value;
        }
        /// <summary>
        /// Hwdg client autorun state.
        /// </summary>
        [UsedImplicitly]
        public Boolean AutoMonitor
        {
            get => settings.Automonitor;
            set => settings.Automonitor = value;
        }

        [UsedImplicitly]
        public void ResetTest() => hwdg.TestSoftReset();

        /// <summary>
        /// Led on binding.
        /// </summary>
        [UsedImplicitly]
        public Boolean RstPulse { get; set; }

        /// <summary>
        /// Executes when WPF Led checks.
        /// </summary>
        [UsedImplicitly]
        public async void RstPulseChecked() => await hwdg.RstPulseOnStartupEnableAsync();

        /// <summary>
        /// Executes when WPF Led unchecks.
        /// </summary>
        [UsedImplicitly]
        public async void RstPulseUnchecked() => await hwdg.RstPulseOnStartupDisableAsync();

        /// <summary>
        /// Led on binding.
        /// </summary>
        [UsedImplicitly]
        public Boolean PwrPulse { get; set; }

        /// <summary>
        /// Executes when WPF Led checks.
        /// </summary>
        [UsedImplicitly]
        public async void PwrPulseChecked() => await hwdg.PwrPulseOnStartupEnableAsync();

        /// <summary>
        /// Executes when WPF Led unchecks.
        /// </summary>
        [UsedImplicitly]
        public async void PwrPulseUnchecked() => await hwdg.PwrPulseOnStartupDisableAsync();
    }
}