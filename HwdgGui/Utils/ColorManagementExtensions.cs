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