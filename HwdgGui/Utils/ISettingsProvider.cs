using System;
using HwdgWrapper;

namespace HwdgGui.Utils
{
    public interface ISettingsProvider
    {
        /// <summary>
        /// Can application start with Windows.
        /// </summary>
        Boolean Autorun { get; set; }

        /// <summary>
        /// Is monitoring sarts when application starts.
        /// </summary>
        Boolean Automonitor { get; set; }

        /// <summary>
        /// Save or restore hwdg status.
        /// </summary>
        Status HwdgStatus { get; set; }
    }
}