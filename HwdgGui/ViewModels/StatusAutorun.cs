using System;
using System.Reflection;
using System.Windows;
using Microsoft.Win32;

namespace HwdgGui.ViewModels
{
    /// <summary>
    /// Autorun control view logic.
    /// </summary>
    public partial class StatusViewModel
    {
        private const String KeyPath = @"SOFTWARE\Microsoft\Windows\CurrentVersion\Run";
        private readonly RegistryKey key = Registry.CurrentUser.OpenSubKey(KeyPath, true);
        private Boolean autorun;

        /// <summary>
        /// Determines if hwdg client autostart entry exists in register.
        /// </summary>
        /// <returns>Returns true if hwdg client is allowed to start
        /// with Windows. Otherwise returns false.</returns>
        private Boolean ReadAutostartEntry()
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

        /// <summary>
        /// Write autotun info to tegister.
        /// </summary>
        /// <param name="val">Value to write.</param>
        /// <remarks>We create a register key if we need to add hwdg client autorun.
        /// If we need to remove our client from autorun we just remove register key.</remarks>
        public void WriteAutostartEntry(Boolean val)
        {
            try
            {
                var curAssembly = Assembly.GetExecutingAssembly();
                if (val)
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

        /// <summary>
        /// Hwdg client autorun state.
        /// </summary>
        public Boolean Autorun
        {
            get => autorun;
            set
            {
                autorun = value;
                WriteAutostartEntry(value);
            }
        }
    }
}