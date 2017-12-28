// Copyright 2017 Oleg Petrochenko
//
// This file is part of HwdgHid.
//
// HwdgHid is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or any
// later version.
//
// HwdgHid is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with HwdgHid. If not, see <http://www.gnu.org/licenses/>.

using System;

namespace HwdgHid
{
    /// <summary>
    /// Represents USB HID compatible device.
    /// </summary>
    public interface IHidDevice
    {
        /// <summary>
        /// Device info
        /// </summary>
        DeviceInfo Info { get; }

        /// <summary>
        /// Send report to the device.
        /// </summary>
        /// <param name="report">Report to be sent.</param>
        /// <exception cref="NullReferenceException">Occurs when <paramref name="report"/> equals null.</exception>
        /// <exception cref="ArgumentOutOfRangeException">Occurs when report size and target device report length mismatch.</exception>
        /// <exception cref="NotSupportedException">Occurs when target device not support output reports.</exception>
        /// <exception cref="InvalidOperationException">Occurs when send report operation fails.</exception>
        void SendReport(Report report);

        /// <summary>
        /// Get device report.
        /// </summary>
        /// <param name="reportId">Report Id.</param>
        /// <returns>Returns report if read operation succeeded, otherwise returns null.</returns>
        Report GetReport(Byte reportId);

        /// <summary>
        /// Send feature report to the device.
        /// </summary>
        /// <param name="report">Report to be sent.</param>
        /// <exception cref="NullReferenceException">Occurs when <paramref name="report"/> equals null.</exception>
        /// <exception cref="ArgumentOutOfRangeException">Occurs when report size and target device report length mismatch.</exception>
        /// <exception cref="NotSupportedException">Occurs when target device not support feature reports.</exception>
        /// <exception cref="InvalidOperationException">Occurs when send feature report operation fails.</exception>
        void SendFeatureReport(Report report);

        /// <summary>
        /// Get device feature report.
        /// </summary>
        /// <param name="reportId">Report Id.</param>
        /// <returns>Returns feature report if read operation succeeded, otherwise returns null.</returns>
        Report GetFeatureReport(Byte reportId);
    }
}