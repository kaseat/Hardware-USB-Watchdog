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
using System.Windows.Threading;
using Caliburn.Micro;
using HwdgGui.Annotations;
using HwdgWrapper;

namespace HwdgGui.ViewModels
{
    public class TestViewModel : PropertyChangedBase
    {
        private const Byte InitialCounter = 6;
        private readonly IHwdg hwdg;
        private readonly DispatcherTimer timer = new DispatcherTimer();

        public TestViewModel(IHwdg hwdg)
        {
            this.hwdg = hwdg;
            CanResetTest = hwdg.LastStatus != null;
            hwdg.Connected += OnConnected;
            hwdg.Disconnected += OnDisconnected;
            HardResetCountdownVisibility = false;

            HardResetCountdown = InitialCounter;
            timer.Interval = TimeSpan.FromMilliseconds(1000);
            timer.Tick += OnTick;
        }

        private void OnTick(Object sender, EventArgs e)
        {
            if (--HardResetCountdown >= 0) return;
            if (hwdg.GetStatus() != null)
                CanResetTest = true;
            HardResetCountdownVisibility = false;
            timer.Stop();
        }

        private void OnDisconnected() => CanResetTest = false;

        private void OnConnected(Status status) => CanResetTest = true;

        /// <summary>
        /// Determines can "test" buttons be pressed.
        /// </summary>
        [UsedImplicitly]
        public Boolean CanResetTest { get; set; }

        /// <summary>
        /// Countdown text.
        /// </summary>
        [UsedImplicitly]
        public Int32 HardResetCountdown { get; set; }

        /// <summary>
        /// Determines coundown is visible.
        /// </summary>
        [UsedImplicitly]
        public Boolean HardResetCountdownVisibility { get; set; }

        /// <summary>
        /// Executes when SoftResetTest button pressed.
        /// </summary>
        [UsedImplicitly]
        public void SoftResetTest() => hwdg.TestSoftReset();

        /// <summary>
        /// Executes when HardResetTest button pressed.
        /// </summary>
        [UsedImplicitly]
        public void HardResetTest()
        {
            HardResetCountdown = InitialCounter;
            HardResetCountdownVisibility = true;
            CanResetTest = false;
            timer.Start();
            hwdg.TestHardReset();
        }
    }
}