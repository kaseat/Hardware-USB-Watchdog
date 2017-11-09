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
using HwdgGui.Annotations;
using HwdgGui.Utils;
using HwdgWrapper;

namespace HwdgGui.ViewModels
{
    public class MainSettingsViewModel : BaseHwdgVm
    {
        public MainSettingsViewModel(IHwdg hwdg, ISettingsProvider settings):base(hwdg,settings)
        {
        }

        protected override void OnStatusUpdate()
        {
            // If hwdg status is null that means hwdg disconnected.
            // We must disable all controls.
            if (HwStatus == null)
            {
                CanEditSettings = false;
                HwdgConnected = false;
            }
            else
            {
                // If hwdg is running all settings must be disabled as
                // we cant change them during monitoring execution.
                if ((HwStatus.State & WatchdogState.IsRunning) != 0)
                {
                    CanEditSettings = false;
                }

                // If hwdg is not run we can edit settings.
                if ((HwStatus.State & WatchdogState.IsRunning) == 0)
                {
                    CanEditSettings = true;
                }

                // Update view controls.
                HwdgConnected = true;
                HardReset = (HwStatus.State & WatchdogState.HardRersetEnabled) != 0;
                Led = (HwStatus.State & WatchdogState.LedDisabled) == 0;
                HardResetCount = HwStatus.HardResetAttempts;
                SoftResetCount = HwStatus.SoftResetAttempts;
                RebootTimeout = HwStatus.RebootTimeout / 1000;
                ResponseTimeout = HwStatus.ResponseTimeout / 1000;
            }
        }


        /// <summary>
        /// HwdgConnected binding.
        /// </summary>
        [UsedImplicitly]
        public Boolean HwdgConnected { get; set; }

        /// <summary>
        /// Hard reset binding.
        /// </summary>
        [UsedImplicitly]
        public Boolean HardReset { get; set; }

        /// <summary>
        /// Executes when WPF SoftResetCount slider value changes.
        /// </summary>
        [UsedImplicitly]
        public async void OnHwdgHrCheckedAsync() => await Hwdg.EnableHardResetAsync();

        /// <summary>
        /// Executes when WPF SoftResetCount slider value changes.
        /// </summary>
        [UsedImplicitly]
        public async void OnHwdgHrUncheckedAsync() => await Hwdg.DisableHardResetAsync();

        /// <summary>
        /// Reboot timeout binding.
        /// </summary>
        [UsedImplicitly]
        public Int32 RebootTimeout { get; set; }

        /// <summary>
        /// Soft reset attempts binding.
        /// </summary>
        [UsedImplicitly]
        public Int32 SoftResetCount { get; set; }

        /// <summary>
        /// Hard reset attempts binding.
        /// </summary>
        [UsedImplicitly]
        public Int32 HardResetCount { get; set; }

        /// <summary>
        /// Response timeout binding.
        /// </summary>
        [UsedImplicitly]
        public Int32 ResponseTimeout { get; set; }

        /// <summary>
        /// Executes when WPF RebootTimeout slider value changes.
        /// </summary>
        /// <param name="timeout">Slider value.</param>
        [UsedImplicitly]
        public void OnRebootTimeoutChanged(Int32 timeout) => Hwdg.SetRebootTimeout(timeout * 1000);

        /// <summary>
        /// Executes when WPF RebootTimeout slider value changes.
        /// </summary>
        /// <param name="timeout">Slider value.</param>
        [UsedImplicitly]
        public void OnResponseTimeoutChanged(Int32 timeout) => Hwdg.SetResponseTimeout(timeout * 1000);

        /// <summary>
        /// Executes when WPF SoftResetCount slider value changes.
        /// </summary>
        /// <param name="val">Slider value.</param>
        [UsedImplicitly]
        public void OnSoftResetCountChanged(Byte val) => Hwdg.SetSoftResetAttempts(val);

        /// <summary>
        /// Executes when WPF HardResetCount slider value changes.
        /// </summary>
        /// <param name="val">Slider value.</param>
        [UsedImplicitly]
        public void OnHardResetCountChanged(Byte val) => Hwdg.SetHardResetAttempts(val);

        /// <summary>
        /// Determines if hwdg settings are editable at the moment.
        /// </summary>
        [UsedImplicitly]
        public Boolean CanEditSettings { get; set; }

        /// <summary>
        /// Led on binding.
        /// </summary>
        [UsedImplicitly]
        public Boolean Led { get; set; }

        /// <summary>
        /// Executes when WPF Led checks.
        /// </summary>
        [UsedImplicitly]
        public async void LedOnAsync() => await Hwdg.EnableLedAsync();

        /// <summary>
        /// Executes when WPF Led unchecks.
        /// </summary>
        [UsedImplicitly]
        public async void LedOffAsync() => await Hwdg.DisableLedAsync();
    }
}