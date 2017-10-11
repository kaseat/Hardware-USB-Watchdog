using System;
using HwdgGui.Utils;

namespace HwdgGui.ViewModels
{
    /// <summary>
    /// Autorun control view logic.
    /// </summary>
    public partial class StatusViewModel
    {
        private readonly RegistrySettingsProvider settings = new RegistrySettingsProvider();

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
    }
}