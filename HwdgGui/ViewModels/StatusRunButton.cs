using System;
using System.Windows.Media;
using HwdgWrapper;

namespace HwdgGui.ViewModels
{
    public partial class StatusViewModel
    {
        private String btnText = "Запустить монитринг";

        /// <summary>
        /// Main button text. Value and action depends on hwdg state.
        /// </summary>
        public String BtnText
        {
            get => btnText;
            set
            {
                btnText = value;
                NotifyOfPropertyChange(() => BtnText);
            }
        }

        /// <summary>
        /// Executes when RunMonitor click button occurs.
        /// </summary>
        public async void RunMonitor()
        {
            status = await hwdg.GetStatusAsync();
            NotifyOfPropertyChange(() => CanEditSettings);
            if ((status.State & WatchdogState.IsRunning) != 0)
            {
                SetAccentColor(Color.FromRgb(96, 169, 23));
                BtnText = "Запустить монитроинг";
                await hwdg.StopAsync();
            }
            else
            {
                BtnText = "Остановить монитроинг";
                SetAccentColor(Color.FromRgb(35, 143, 255));
                await hwdg.StartAsync();
            }
        }
        public Boolean CanRunMonitor => status != null;
    }
}