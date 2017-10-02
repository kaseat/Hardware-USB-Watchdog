using System;
using System.Threading.Tasks;

namespace HwdgApi
{
    /// <summary>
    /// Represents Hwdg API provider. See par.4 "Hardware watchdog V1 reference manual" for more details.
    /// </summary>
    public interface IHwdgProvider
    {
        /// <summary>
        /// Set reboot timeout.
        /// </summary>
        /// <param name="ms">Timeout, ms. Range 10000-645000 ms.</param>
        /// <returns>Returns <see cref="Response.SetRebootTimeoutOk"/> when operation succeeded,
        /// otherwise returns <see cref="Response.Busy"/></returns>
        /// <remarks>Even if input value goes beyond 10000-645000 ms range this value stays
        /// within specified range. Ex: input value 1 becomes 10000 before decoding proceed.
        /// See par.4.1 "Hardware watchdog V1 reference manual" for more details.</remarks>
        Task<Response> SetRebootTimeout(Int32 ms);
        /// <summary>
        /// Set response timeout.
        /// </summary>
        /// <param name="ms">Timeout, ms. Range 5000-320000 ms.</param>
        /// <returns>Returns <see cref="Response.SetResponseTimeoutOk"/> when operation succeeded,
        /// otherwise returns <see cref="Response.Busy"/></returns>
        /// <remarks>Even if input value goes beyond 5000-320000 ms range this value stays
        /// within specified range. Ex: input value 999000 becomes 320000 before decoding proceed.
        /// See par.4.2 "Hardware watchdog V1 reference manual" for more details.</remarks>
        Task<Response> SetResponseTimeout(Int32 ms);
        /// <summary>
        /// Set soft reset attempts count.
        /// </summary>
        /// <param name="count">Attempts count (1-8).</param>
        /// <returns>Returns <see cref="Response.SetSoftResetAttemptsOk"/> when operation succeeded,
        /// otherwise returns <see cref="Response.Busy"/></returns>
        /// <remarks>Even if input value goes beyond 1-8 range this value stays
        /// within specified range. Ex: input value 0 becomes 1 before decoding proceed.
        /// See par.4.3 "Hardware watchdog V1 reference manual" for more details.</remarks>
        Task<Response> SetSoftResetAttempts(Byte count);
        /// <summary>
        /// Set hard reset attempts count.
        /// </summary>
        /// <param name="count">Attempts count (1-8).</param>
        /// <returns>Returns <see cref="Response.SetHardResetAttemptsOk"/> when operation succeeded,
        /// otherwise returns <see cref="Response.Busy"/></returns>
        /// <remarks>Even if input value goes beyond 1-8 range this value stays
        /// within specified range. Ex: input value 99 becomes 8 before decoding proceed.
        /// See par.4.4 "Hardware watchdog V1 reference manual" for more details.</remarks>
        Task<Response> SetHardResetAttempts(Byte count);
        /// <summary>
        /// Enable hard reset.
        /// </summary>
        /// <returns>Returns <see cref="Response.EnableHardResetOk"/> when operation succeeded,
        /// otherwise returns <see cref="Response.Busy"/></returns>
        /// <remarks>See par.4.5 "Hardware watchdog V1 reference manual" for more details.</remarks>
        Task<Response> EnableHardReset();
        /// <summary>
        /// Disable hard reset.
        /// </summary>
        /// <returns>Returns <see cref="Response.DisableHardResetOk"/> when operation succeeded,
        /// otherwise returns <see cref="Response.Busy"/></returns>
        /// <remarks>See par.4.6 "Hardware watchdog V1 reference manual" for more details.</remarks>
        Task<Response> DisableHardReset();
        /// <summary>
        /// Start hardware watchdog monitoring.
        /// </summary>
        /// <returns>Returns <see cref="Response.StartOk"/> when operation succeeded,
        /// otherwise returns <see cref="Response.Busy"/></returns>
        /// <remarks>See par.4.7 "Hardware watchdog V1 reference manual" for more details.</remarks>
        Task<Response> Start();
        /// <summary>
        /// Stop hardware watchdog monitoring.
        /// </summary>
        /// <returns>Returns <see cref="Response.StopOk"/> when operation succeeded,
        /// otherwise returns <see cref="Response.Busy"/></returns>
        /// <remarks>See par.4.8 "Hardware watchdog V1 reference manual" for more details.</remarks>
        Task<Response> Stop();
        /// <summary>
        /// Send Ping.
        /// </summary>
        /// <returns>Returns <see cref="Response.PingOk"/> when operation succeeded,
        /// otherwise returns <see cref="Response.Busy"/></returns>
        /// <remarks>See par.4.9 "Hardware watchdog V1 reference manual" for more details.</remarks>
        Task<Response> Ping();
        /// <summary>
        /// Get current hwdg status.
        /// </summary>
        /// <returns>Returns <see cref="Status"/></returns>
        /// <remarks>See par.4.10 "Hardware watchdog V1 reference manual" for more details.</remarks>
        Task<Status> GetStatus();
    }
}
