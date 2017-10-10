using System;
using System.Windows.Media;
using FirstFloor.ModernUI.Presentation;

namespace HwdgGui.ViewModels
{
    public partial class StatusViewModel
    {
        /// <summary>
        /// Represents accent color style.
        /// </summary>
        private enum AccentColor : Byte
        {
            /// <summary>
            /// Connected color appearance.
            /// </summary>
            Connected,
            /// <summary>
            /// Disconnected color appearance.
            /// </summary>
            Disconnected,
            /// <summary>
            /// Running color appearance.
            /// </summary>
            Running
        }

        /// <summary>
        /// Updates main window accent color.
        /// </summary>
        /// <param name="color">Accent color.</param>
        private void SetAccentColor(Color color) =>
            uiDispatcher.Invoke(() => { AppearanceManager.Current.AccentColor = color; });

        /// <summary>
        /// Updates main window accent color.
        /// </summary>
        /// <param name="color">Accent color.</param>
        private void SetAccentColor(AccentColor color)
        {
            switch (color)
            {
                case AccentColor.Connected:
                    SetAccentColor(Color.FromRgb(96, 169, 23));
                    break;
                case AccentColor.Disconnected:
                    SetAccentColor(Color.FromRgb(229, 20, 0));
                    break;
                case AccentColor.Running:
                    SetAccentColor(Color.FromRgb(35, 143, 255));
                    break;
                default:
                    throw new ArgumentOutOfRangeException(nameof(color), color, null);
            }
        }
    }
}