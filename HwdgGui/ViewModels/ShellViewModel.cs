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
                if(settings.CheckUrl) hwdg.EnableUrlCheck(settings.Url);
                else hwdg.DisableUrlCheck();
                hwdg.Start();
            }

            hwdg.Connected += OnConnected;
        }

        private void OnConnected(Status status)
        {
            hwdg.Stop();
            hwdg.DisableUrlCheck();
            if (settings.Automonitor)
            {
                if (settings.CheckUrl)
                    hwdg.EnableUrlCheck(settings.Url);
                hwdg.Start();
            }
        }

        /// <summary>
        /// Stop monitoring at window exit.
        /// </summary>
        public void OnClose()
        {
            hwdg.Stop();
            hwdg.DisableUrlCheck();
        }
    }
}