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

using System.Windows.Threading;
using Caliburn.Micro;
using HwdgGui.Utils;
using HwdgWrapper;
using PropertyChanged;

namespace HwdgGui.ViewModels
{
    public class BaseHwdgVm : PropertyChangedBase
    {
        protected readonly IHwdg Hwdg;
        protected readonly Dispatcher UiDisp;
        protected readonly ISettingsProvider Settings;

        /// <inheritdoc />
        protected BaseHwdgVm(IHwdg hwdg, ISettingsProvider settings)
        {
            // Inject dependency
            Hwdg = hwdg;
            Settings = settings;

            // Get and store main thread dispatcher.
            // This is necessary for further window color changes.
            UiDisp = Dispatcher.CurrentDispatcher;

            // Subscribe on hwdg events to handle HWDG
            // connecting and disconnecting situations.
            // We have only one instance of this view per app,
            // so no need to worry about memory leak.
            hwdg.Disconnected += OnDisconnected;
            hwdg.Connected += OnConnected;
            hwdg.Updated += OnUpdated;

            // Get last known status.
            HwStatus = hwdg.GetStatus();
        }

        /// <summary>
        /// Hwdg connected event handler.
        /// </summary>
        /// <param name="st">Hwdg status.</param>
        private void OnConnected(Status st)
        {
            // If auto monitoring enabled, we'll update status
            // later in upcoming 'OnUpdated' event.
            if (!Settings.Automonitor)
            {
                HwStatus = st;
            }
        }

        /// <summary>
        /// Hwdg disconnected event handler.
        /// </summary>
        private void OnDisconnected() => HwStatus = null;

        /// <summary>
        /// Hwdg status updated event handler.
        /// </summary>
        /// <param name="sta"></param>
        private void OnUpdated(Status sta) => HwStatus = sta;

        private Status status;

        [DoNotNotify]
        protected Status HwStatus
        {
            get => status;
            set
            {
                status = value;
                OnStatusUpdate();
            }
        }

        protected virtual void OnStatusUpdate()
        {
        }
    }
}