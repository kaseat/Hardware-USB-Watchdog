using Caliburn.Micro;
using HwdgGui.Utils;
using HwdgWrapper;

namespace HwdgGui.ViewModels
{
    public class ShellViewModel : PropertyChangedBase
    {
        private readonly IHwdg hwdg;
        private readonly ISettingsProvider settings;

        /// <summary>
        /// ctor
        /// </summary>
        /// <param name="hwdg">IHwdg instance.</param>
        /// <param name="settings">ISettingsProvider instance.</param>
        public ShellViewModel(IHwdg hwdg, ISettingsProvider settings)
        {
            // Inject dependency.
            this.hwdg = hwdg;
            this.settings = settings;

            // Firstly we should stop any processes on HWDG
            // as signal that OS loaded properly.
            hwdg.Stop();

            // If auto monitoring enabled, start it immediately.
            if (settings.Automonitor)
            {
                hwdg.Start();
            }
        }
        /// <summary>
        /// Stop monitoring at window exit.
        /// </summary>
        public void OnClose()
        {
            hwdg.Stop();
        }
    }
}