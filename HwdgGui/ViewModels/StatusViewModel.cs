using System;
using System.Reflection;
using System.Windows;
using System.Windows.Media;
using System.Windows.Threading;
using Caliburn.Micro;
using FirstFloor.ModernUI.Presentation;
using HwdgWrapper;
using Microsoft.Win32;

namespace HwdgGui.ViewModels
{
    public class StatusViewModel : PropertyChangedBase, IDisposable
    {
        private readonly Dispatcher uiDispatcher;
        private readonly RegistryKey key = Registry.CurrentUser.OpenSubKey(@"SOFTWARE\Microsoft\Windows\CurrentVersion\Run", true);
        private readonly SerialHwdg hwdg = new SerialHwdg(new SerialWrapper());
        private String name;
        private Boolean autoStart;
        private Int32 hrTimeout;


        public StatusViewModel()
        {
            uiDispatcher = Dispatcher.CurrentDispatcher;
            AutoStart = IsAutostart();
            hwdg.Disconnected += Hwdg_Disconnected;
            hwdg.Connected += Hwdg_Connected;
            var sta = hwdg.GetStatus();
            if (sta == null)
            {
                AppearanceManager.Current.AccentColor = Color.FromRgb(0xe5, 0x14, 0x00);
                return;
            }
            AppearanceManager.Current.AccentColor = Color.FromRgb(0x60, 0xa9, 0x17);

            RebootTimeout = sta.RebootTimeout / 1000;
        }

        private Boolean IsAutostart()
        {
            var startVal = String.Empty;
            var curAssembly = Assembly.GetExecutingAssembly();
            try
            {
                startVal = (String)key.GetValue(curAssembly.GetName().Name, "");
            }
            catch (Exception)
            {
                MessageBox.Show("Ошибка чтения реестра");
            }
            return startVal == curAssembly.Location;
        }
        public void AutoStartChanged(Boolean val)
        {
            try
            {

                var curAssembly = Assembly.GetExecutingAssembly();
                if (!val)
                {
                    key?.SetValue(curAssembly.GetName().Name, curAssembly.Location);
                }
                else
                {
                    key?.DeleteValue(curAssembly.GetName().Name, false);
                }
            }
            catch (Exception)
            {
                MessageBox.Show("Ошибка изменения реестра");
            }
        }

        public Boolean AutoStart
        {
            get => autoStart;
            set
            {
                autoStart = value;
                NotifyOfPropertyChange(() => AutoStart);
            }
        }

        private void Hwdg_Connected(Status status)
        {
            RebootTimeout = status.RebootTimeout / 1000;
            uiDispatcher.Invoke(() => { AppearanceManager.Current.AccentColor = Color.FromRgb(0x60, 0xa9, 0x17); });
        }

        private void Hwdg_Disconnected()
        {
            uiDispatcher.Invoke(() => { AppearanceManager.Current.AccentColor = Color.FromRgb(0xe5, 0x14, 0x00); });
        }

        public String Name
        {
            get => name;
            set
            {
                name = value;
                NotifyOfPropertyChange(() => Name);
                NotifyOfPropertyChange(() => CanSayHello);
            }
        }

        public Boolean CanSayHello => !String.IsNullOrWhiteSpace(Name);

        public void SayHello() => MessageBox.Show($"Hello { hwdg.Start()}!");

        public Int32 RebootTimeout
        {
            get => hrTimeout;
            set
            {
                hrTimeout = value;
                NotifyOfPropertyChange(() => RebootTimeout);
            }
        }

        public void UpdateRebootTimeout(Int32 timeout)
        {
            var rts = hwdg.SetRebootTimeout(timeout * 1000);
        }

        public void Dispose()
        {
            hwdg.Dispose();
            key.Dispose();
            GC.SuppressFinalize(this);
        }
    }
}