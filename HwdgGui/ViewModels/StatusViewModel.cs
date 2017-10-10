using System;
using System.Windows.Media;
using System.Windows.Threading;
using Caliburn.Micro;
using FirstFloor.ModernUI.Presentation;
using HwdgWrapper;

namespace HwdgGui.ViewModels
{
    public partial class StatusViewModel : PropertyChangedBase, IDisposable
    {

        private readonly IHwdg hwdg = new SerialHwdg(new SerialWrapper());
        private readonly Dispatcher uiDispatcher;
        private Status status;

        /// <summary>
        /// Updates main window accent color.
        /// </summary>
        /// <param name="color">Accent color.</param>
        private void SetAccentColor(Color color) =>
            uiDispatcher.Invoke(() => { AppearanceManager.Current.AccentColor = color; });

        /// <summary>
        /// ctor
        /// </summary>
        public StatusViewModel()
        {
            uiDispatcher = Dispatcher.CurrentDispatcher;
            Autorun = ReadAutostartEntry();
            hwdg.Disconnected += OnDisconnected;
            hwdg.Connected += OnConnected;
            status = hwdg.GetStatus();
            if ((status.State & WatchdogState.WaitingForReboot) != 0)
            {
                hwdg.Stop();
                hwdg.GetStatus();
            }
            SetAccentColor(status == null ? Color.FromRgb(229, 20, 0) : Color.FromRgb(96, 169, 23));
            NotifyOfPropertyChange(() => CanRunMonitor);
        }

        /// <summary>
        /// Hwdg connected event handler.
        /// </summary>
        /// <param name="st">Hwdg status.</param>
        private void OnConnected(Status st)
        {
            status = st;
            NotifyOfPropertyChange(() => CanRunMonitor);
            SetAccentColor(Color.FromRgb(96, 169, 23));
        }

        /// <summary>
        /// Hwdg disconnected event handler.
        /// </summary>
        private void OnDisconnected()
        {
            status = null;
            NotifyOfPropertyChange(() => CanRunMonitor);
            SetAccentColor(Color.FromRgb(229, 20, 0));
        }

        /// <summary>
        /// Reboot timeout binding.
        /// </summary>
        public Int32 RebootTimeout
        {
            get => status.RebootTimeout / 1000;
            set
            {
                status.RebootTimeout = value * 1000;
                NotifyOfPropertyChange(() => RebootTimeout);
            }
        }



        /// <summary>
        /// Executes when WPF RebootTimeout slider value changes.
        /// </summary>
        /// <param name="timeout">Slider value.</param>
        public void OnRebootTimeoutChanged(Int32 timeout)
        {
            var rts = hwdg.SetRebootTimeout(timeout * 1000);
        }

        public Boolean CanEditSettings => (status.State & WatchdogState.IsRunning) != 0;


        /// <inheritdoc />
        public void Dispose()
        {
            hwdg.Disconnected -= OnDisconnected;
            hwdg.Connected -= OnConnected;
            key.Dispose();
            GC.SuppressFinalize(this);
        }
    }
}