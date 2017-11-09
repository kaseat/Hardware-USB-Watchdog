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
using System.Windows.Media;
using System.Windows.Threading;
using FirstFloor.ModernUI.Presentation;

namespace HwdgGui.Utils
{
    public static class ColorManagementExtensions
    {
        /// <summary>
        /// Updates main window accent color.
        /// </summary>
        /// <param name="dsp">Dispatcher.</param>
        /// <param name="color">Accent color.</param>
        private static void SetAccentColor(this Dispatcher dsp, Color color) =>
            dsp.Invoke(() => { AppearanceManager.Current.AccentColor = color; });

        /// <summary>
        /// Updates main window accent color.
        /// </summary>
        /// <param name="dsp">Dispatcher.</param>
        /// <param name="color">Accent color.</param>
        public static void SetAccentColor(this Dispatcher dsp, AccentColor color)
        {
            switch (color)
            {
                case AccentColor.Connected:
                    dsp.SetAccentColor(Color.FromRgb(96, 169, 23));
                    break;
                case AccentColor.Disconnected:
                    dsp.SetAccentColor(Color.FromRgb(229, 20, 0));
                    break;
                case AccentColor.Running:
                    dsp.SetAccentColor(Color.FromRgb(35, 143, 255));
                    break;
                default:
                    throw new ArgumentOutOfRangeException(nameof(color), color, null);
            }
        }
    }
}