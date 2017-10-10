using System;
using HwdgWrapper;

namespace HwdgGui.ViewModels
{
    public partial class StatusViewModel
    {
        /// <summary>
        /// Main button text. Value and action depends on hwdg state.
        /// </summary>
        public String RunButtonText { get; set; }

        /// <summary>
        /// Executes when RunMonitor click button occurs.
        /// </summary>
        public async void RunButtonAsync()
        {
            status = await hwdg.GetStatusAsync();
            if ((status.State & WatchdogState.IsRunning) != 0)
            {
                SetAccentColor(AccentColor.Connected);
                RunButtonText = "Запустить монитроинг";
                await hwdg.StopAsync();
            }
            else
            {
                RunButtonText = "Остановить монитроинг";
                SetAccentColor(AccentColor.Running);
                await hwdg.StartAsync();
            }
        }

        /// <summary>
        /// Enable / disable RunMonitor button.
        /// </summary>
        public Boolean CanRunButton { get; set; }
    }
}