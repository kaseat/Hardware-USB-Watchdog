using System;
using System.Windows.Threading;
using Caliburn.Micro;
using HwdgGui.Utils;
using HwdgWrapper;

namespace HwdgGui.ViewModels
{
    public partial class StatusViewModel : PropertyChangedBase, IDisposable
    {
        private readonly IHwdg hwdg;
        private readonly Dispatcher uiDisp;
        private Status status;

        /// <summary>
        /// ctor
        /// </summary>
        public StatusViewModel(IHwdg hwdg)
        {
            this.hwdg = hwdg;
            uiDisp = Dispatcher.CurrentDispatcher;
            RunButtonText = "Запустить монитринг";
            CanRunButton = true;
            hwdg.Disconnected += OnDisconnected;
            hwdg.Connected += OnConnected;
            status = hwdg.GetStatus();
            if (status != null && (status.State & WatchdogState.WaitingForReboot) != 0)
            {
                hwdg.Stop();
                hwdg.GetStatus();
            }
            uiDisp.SetAccentColor(status == null ? AccentColor.Disconnected : AccentColor.Connected);
        }

        /// <summary>
        /// Hwdg connected event handler.
        /// </summary>
        /// <param name="st">Hwdg status.</param>
        private void OnConnected(Status st)
        {
            status = st;
            CanRunButton = true;
            RunButtonText = "Запустить монитринг";
            uiDisp.SetAccentColor(AccentColor.Connected);
        }

        /// <summary>
        /// Hwdg disconnected event handler.
        /// </summary>
        private void OnDisconnected()
        {
            status = null;
            CanRunButton = false;
            RunButtonText = "Железный пёс не подключен";
            uiDisp.SetAccentColor(AccentColor.Disconnected);
        }

        /// <summary>
        /// Reboot timeout binding.
        /// </summary>
        public Int32 RebootTimeout
        {
            get => status?.RebootTimeout / 1000 ?? 50;
            set => status.RebootTimeout = value * 1000;
        }

        /// <summary>
        /// Executes when WPF RebootTimeout slider value changes.
        /// </summary>
        /// <param name="timeout">Slider value.</param>
        public void OnRebootTimeoutChanged(Int32 timeout)
        {
            var rts = hwdg.SetRebootTimeout(timeout * 1000);
        }

        public Boolean CanEditSettings => status != null && (status.State & WatchdogState.IsRunning) != 0;


        /// <inheritdoc />
        public void Dispose()
        {
            hwdg.Disconnected -= OnDisconnected;
            hwdg.Connected -= OnConnected;
            settings.Dispose();
            GC.SuppressFinalize(this);
        }
    }
}