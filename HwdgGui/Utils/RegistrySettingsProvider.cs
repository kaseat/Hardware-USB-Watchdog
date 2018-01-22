// Copyright 2017 Oleg Petrochenko
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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
        private const String PingUrl = "PingUrl";
        private const String UrlString = "UrlString";
        private const String AutoRun = "AutoRun";
        private const String HwStatus = "HwStatus";
        private readonly RegistryKey autorunKey;
        private readonly RegistryKey settingsKey;

        public RegistrySettingsProvider()
        {
            autorunKey = Registry.CurrentUser.OpenSubKey(AutorunPath, true);
            settingsKey = Registry.CurrentUser.OpenSubKey(SettingsPath, true);
            if (settingsKey == null)
            {
                settingsKey = Registry.CurrentUser.CreateSubKey(SettingsPath);
                settingsKey?.SetValue(AutoMonitor, 1, RegistryValueKind.DWord);
                settingsKey?.SetValue(AutoRun, 1, RegistryValueKind.DWord);
                settingsKey?.SetValue(PingUrl, 0, RegistryValueKind.DWord);
                settingsKey?.SetValue(UrlString, "https://google.com/", RegistryValueKind.String);
                settingsKey?.SetValue(HwStatus, 0, RegistryValueKind.DWord);
            }
            var am = settingsKey?.GetValue(AutoMonitor, null);
            if (am == null) settingsKey?.SetValue(AutoMonitor, 1);

            var ar = settingsKey?.GetValue(AutoRun, null);
            if (ar == null) settingsKey?.SetValue(AutoRun, 1);

            var pu = settingsKey?.GetValue(PingUrl, null);
            if (pu == null) settingsKey?.SetValue(PingUrl, 0);

            var us = settingsKey?.GetValue(UrlString, null);
            if (us == null) settingsKey?.SetValue(UrlString, "https://google.com/");

            var hs = settingsKey?.GetValue(HwStatus, null);
            if (hs == null) settingsKey?.SetValue(HwStatus, 0);

            WriteAutostartEntry(Autorun);
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
            get => Convert.ToBoolean(settingsKey.GetValue(AutoRun, 0));
            set
            {
                settingsKey.SetValue(AutoRun, value, RegistryValueKind.DWord);
                WriteAutostartEntry(value);
            }
        }

        /// <inheritdoc />
        public Boolean Automonitor
        {
            get => Convert.ToBoolean(settingsKey.GetValue(AutoMonitor, 0));
            set => settingsKey.SetValue(AutoMonitor, value, RegistryValueKind.DWord);
        }

        /// <inheritdoc />
        public Boolean CheckUrl
        {
            get => Convert.ToBoolean(settingsKey.GetValue(PingUrl, 0));
            set => settingsKey.SetValue(PingUrl, value, RegistryValueKind.DWord);
        }

        /// <inheritdoc />
        public String Url
        {
            get => settingsKey.GetValue(UrlString, "https://google.com/").ToString();
            set => settingsKey.SetValue(UrlString, value, RegistryValueKind.String);
        }

        /// <inheritdoc />
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