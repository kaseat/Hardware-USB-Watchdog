using System;
using System.Reflection;
using System.Windows;
using Caliburn.Micro;
using HwdgWrapper;
using Microsoft.Win32;

namespace HwdgGui.ViewModels
{
    public class ShellViewModel : PropertyChangedBase
    {
        private readonly RegistryKey key = Registry.CurrentUser.OpenSubKey(@"SOFTWARE\Microsoft\Windows\CurrentVersion\Run", true);
        private readonly SerialHwdg hwdg = new SerialHwdg(new SerialWrapper());
        String name;
        Boolean autoStart;
        Int32 hrTimeout;

        public ShellViewModel()
        {
            AutoStart = IsAutostart();
            hwdg.Disconnected += Hwdg_Disconnected;
            hwdg.Connected += Hwdg_Connected;
            var sta = hwdg.GetStatus();
            if (sta == null) return;
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
            MessageBox.Show("Connected!");
            RebootTimeout = status.RebootTimeout/1000;
        }

        private void Hwdg_Disconnected()
        {
            MessageBox.Show("Disconnected!");
            Name = "disconn";
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
            hwdg.SetRebootTimeout(timeout);
        }
    }
}