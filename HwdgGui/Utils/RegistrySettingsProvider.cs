using System;
using System.Reflection;
using HwdgWrapper;
using Microsoft.Win32;

namespace HwdgGui.Utils
{

    /// <summary>
    /// Represents application settings manager.
    /// </summary>
    public class RegistrySettingsProvider : ISettingsProvider, IDisposable
    {
        private const String AutorunPath = @"SOFTWARE\Microsoft\Windows\CurrentVersion\Run";
        private const String SettingsPath = @"SOFTWARE\Hwdg";
        private const String AutoMonitor = "AutoMonitor";
        private const String HwStatus = "HwStatus";
        private readonly RegistryKey autorunKey;
        private readonly RegistryKey settingsKey;

        public RegistrySettingsProvider()
        {
            autorunKey = Registry.CurrentUser.OpenSubKey(AutorunPath, true);
            settingsKey = Registry.CurrentUser.OpenSubKey(SettingsPath, true);
            if (settingsKey == null)
            {
                settingsKey = Registry.CurrentUser.CreateSubKey(SettingsPath, true);
                settingsKey.SetValue(AutoMonitor, 0, RegistryValueKind.DWord);
                settingsKey.SetValue(HwStatus, 0, RegistryValueKind.DWord);
            }
            var am = settingsKey.GetValue(AutoMonitor, null);
            if (am == null) settingsKey.SetValue(AutoMonitor, 0);

            var hs = settingsKey.GetValue(HwStatus, null);
            if (hs == null) settingsKey.SetValue(HwStatus, 0);
        }

        /// <summary>
        /// Determines if hwdg client autostart entry exists in registry.
        /// </summary>
        /// <returns>Returns true if hwdg client is allowed to start
        /// with Windows. Otherwise returns false.</returns>
        private Boolean ReadAutostartEntry()
        {
            var curAssembly = Assembly.GetExecutingAssembly();
            var startVal = (String) autorunKey.GetValue(curAssembly.GetName().Name, String.Empty);
            return startVal == curAssembly.Location;
        }

        /// <summary>
        /// Write autotun info to registry.
        /// </summary>
        /// <param name="val">Value to write.</param>
        /// <remarks>We create a registry key if we need to add hwdg client autorun.
        /// If we need to remove our client from autorun we just remove registry key.</remarks>
        private void WriteAutostartEntry(Boolean val)
        {
            var curAssembly = Assembly.GetExecutingAssembly();
            if (val)
            {
                autorunKey?.SetValue(curAssembly.GetName().Name, curAssembly.Location);
            }
            else
            {
                autorunKey?.DeleteValue(curAssembly.GetName().Name, false);
            }
        }

        /// <inheritdoc />
        public Boolean Autorun
        {
            get => ReadAutostartEntry();
            set => WriteAutostartEntry(value);
        }

        /// <inheritdoc />
        public Boolean Automonitor
        {
            get => Convert.ToBoolean(settingsKey.GetValue(AutoMonitor, 0));
            set => settingsKey.SetValue(AutoMonitor, value, RegistryValueKind.DWord);
        }

        public Status HwdgStatus
        {
            get => new Status((Int32) settingsKey.GetValue(HwStatus, 0));
            set => settingsKey.SetValue(HwStatus, value.RawData, RegistryValueKind.DWord);
        }

        /// <inheritdoc />
        public void Dispose()
        {
            autorunKey.Dispose();
            settingsKey.Dispose();
            GC.SuppressFinalize(this);
        }
    }
}