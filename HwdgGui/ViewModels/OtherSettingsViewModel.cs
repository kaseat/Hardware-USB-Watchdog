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
    public sealed class OtherSettingsViewModel : BaseHwdgVm
    {
        /// <summary>
        /// ctor
        /// </summary>
        /// <param name="hwdg">IHwdg instance.</param>
        /// <param name="settings">ISettingsProvider instance.</param>
        public OtherSettingsViewModel(IHwdg hwdg, ISettingsProvider settings) : base(hwdg, settings)
        {
        }

        protected override void OnStatusUpdate()
        {
            // If hwdg status is null that means hwdg disconnected.
            // We must disable all controls.
            if (HwStatus == null)
            {
                HwdgSettingsVisible = false;
            }
            else
            {
                // Update view controls.
                HwdgSettingsVisible = true;
                RstPulse = (HwStatus.State & WatchdogState.RstPulseEnabled) != 0;
                PwrPulse = (HwStatus.State & WatchdogState.PwrPulseEnabled) != 0;
            }
        }

        /// <summary>
        /// Determines if hwdg settings are editable at the moment.
        /// </summary>
        [UsedImplicitly]
        public Boolean HwdgSettingsVisible { get; set; }


        [UsedImplicitly]
        public void ResetTest() => Hwdg.TestSoftReset();

        /// <summary>
        /// Led on binding.
        /// </summary>
        [UsedImplicitly]
        public Boolean RstPulse { get; set; }

        /// <summary>
        /// Executes when WPF Led checks.
        /// </summary>
        [UsedImplicitly]
        public async void RstPulseChecked() => await Hwdg.RstPulseOnStartupEnableAsync();

        /// <summary>
        /// Executes when WPF Led unchecks.
        /// </summary>
        [UsedImplicitly]
        public async void RstPulseUnchecked() => await Hwdg.RstPulseOnStartupDisableAsync();

        /// <summary>
        /// Led on binding.
        /// </summary>
        [UsedImplicitly]
        public Boolean PwrPulse { get; set; }

        /// <summary>
        /// Executes when WPF Led checks.
        /// </summary>
        [UsedImplicitly]
        public async void PwrPulseChecked() => await Hwdg.PwrPulseOnStartupEnableAsync();

        /// <summary>
        /// Executes when WPF Led unchecks.
        /// </summary>
        [UsedImplicitly]
        public async void PwrPulseUnchecked() => await Hwdg.PwrPulseOnStartupDisableAsync();

        /// <summary>
        /// Executes when WPF ResetToFactory button pressed.
        /// </summary>
        [UsedImplicitly]
        public async void ResetToFactory() => await Hwdg.RestoreFactoryAsync();
    }
}