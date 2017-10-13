using System;
using Caliburn.Micro;
using HwdgGui.Utils;
using HwdgWrapper;

namespace HwdgGui.ViewModels
{
    public class SettingsViewModel: PropertyChangedBase
    {
        private readonly IHwdg hwdg;
        private readonly ISettingsProvider settings;

        public SettingsViewModel(IHwdg hwdg, ISettingsProvider settings)
        {
            this.settings = settings;
            this.hwdg = hwdg;
            CanResetTest = hwdg.GetStatus() != null;
            hwdg.Connected += OnConnected;
            hwdg.Disconnected += OnDisconnected;
        }

        private void OnDisconnected() => CanResetTest = false;

        private void OnConnected(Status status) => CanResetTest = true;

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
        public void ResetTest()
        {
            hwdg.TestReset();
        }
        public Boolean CanResetTest { get; set; }
    }
}