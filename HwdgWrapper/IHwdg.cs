using System;
using System.Threading.Tasks;

namespace HwdgWrapper
{
    /// <summary>
    /// Represents Hwdg API. See par.4 "Hardware watchdog V1 reference manual" for more details.
    /// </summary>
    public interface IHwdg
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
        Response SetRebootTimeout(Int32 ms);

        /// <summary>
        /// Set response timeout.
        /// </summary>
        /// <param name="ms">Timeout, ms. Range 5000-320000 ms.</param>
        /// <returns>Returns <see cref="Response.SetResponseTimeoutOk"/> when operation succeeded,
        /// otherwise returns <see cref="Response.Busy"/></returns>
        /// <remarks>Even if input value goes beyond 5000-320000 ms range this value stays
        /// within specified range. Ex: input value 999000 becomes 320000 before decoding proceed.
        /// See par.4.2 "Hardware watchdog V1 reference manual" for more details.</remarks>
        Response SetResponseTimeout(Int32 ms);

        /// <summary>
        /// Set soft reset attempts count.
        /// </summary>
        /// <param name="count">Attempts count (1-8).</param>
        /// <returns>Returns <see cref="Response.SetSoftResetAttemptsOk"/> when operation succeeded,
        /// otherwise returns <see cref="Response.Busy"/></returns>
        /// <remarks>Even if input value goes beyond 1-8 range this value stays
        /// within specified range. Ex: input value 0 becomes 1 before decoding proceed.
        /// See par.4.3 "Hardware watchdog V1 reference manual" for more details.</remarks>
        Response SetSoftResetAttempts(Byte count);

        /// <summary>
        /// Set hard reset attempts count.
        /// </summary>
        /// <param name="count">Attempts count (1-8).</param>
        /// <returns>Returns <see cref="Response.SetHardResetAttemptsOk"/> when operation succeeded,
        /// otherwise returns <see cref="Response.Busy"/></returns>
        /// <remarks>Even if input value goes beyond 1-8 range this value stays
        /// within specified range. Ex: input value 99 becomes 8 before decoding proceed.
        /// See par.4.4 "Hardware watchdog V1 reference manual" for more details.</remarks>
        Response SetHardResetAttempts(Byte count);

        /// <summary>
        /// Enable hard reset.
        /// </summary>
        /// <returns>Returns <see cref="Response.EnableHardResetOk"/> when operation succeeded,
        /// otherwise returns <see cref="Response.Busy"/></returns>
        /// <remarks>See par.4.5 "Hardware watchdog V1 reference manual" for more details.</remarks>
        Response EnableHardReset();

        /// <summary>
        /// Disable hard reset.
        /// </summary>
        /// <returns>Returns <see cref="Response.DisableHardResetOk"/> when operation succeeded,
        /// otherwise returns <see cref="Response.Busy"/></returns>
        /// <remarks>See par.4.6 "Hardware watchdog V1 reference manual" for more details.</remarks>
        Response DisableHardReset();

        /// <summary>
        /// Start hardware watchdog monitoring.
        /// </summary>
        /// <returns>Returns <see cref="Response.StartOk"/> when operation succeeded,
        /// otherwise returns <see cref="Response.Busy"/></returns>
        /// <remarks>See par.4.7 "Hardware watchdog V1 reference manual" for more details.</remarks>
        Response Start();

        /// <summary>
        /// Stop hardware watchdog monitoring.
        /// </summary>
        /// <returns>Returns <see cref="Response.StopOk"/> when operation succeeded,
        /// otherwise returns <see cref="Response.Busy"/></returns>
        /// <remarks>See par.4.8 "Hardware watchdog V1 reference manual" for more details.</remarks>
        Response Stop();

        /// <summary>
        /// Get current hwdg status.
        /// </summary>
        /// <returns>Returns <see cref="Status"/></returns>
        /// <remarks>See par.4.10 "Hardware watchdog V1 reference manual" for more details.</remarks>
        Status GetStatus();

        /// <summary>
        /// Set reboot timeout asynchronously.
        /// </summary>
        /// <param name="ms">Timeout, ms. Range 10000-645000 ms.</param>
        /// <returns>Returns <see cref="Response.SetRebootTimeoutOk"/> when operation succeeded,
        /// otherwise returns <see cref="Response.Busy"/></returns>
        /// <remarks>Even if input value goes beyond 10000-645000 ms range this value stays
        /// within specified range. Ex: input value 1 becomes 10000 before decoding proceed.
        /// See par.4.1 "Hardware watchdog V1 reference manual" for more details.</remarks>
        Task<Response> SetRebootTimeoutAsync(Int32 ms);

        /// <summary>
        /// Set response timeout asynchronously.
        /// </summary>
        /// <param name="ms">Timeout, ms. Range 5000-320000 ms.</param>
        /// <returns>Returns <see cref="Response.SetResponseTimeoutOk"/> when operation succeeded,
        /// otherwise returns <see cref="Response.Busy"/></returns>
        /// <remarks>Even if input value goes beyond 5000-320000 ms range this value stays
        /// within specified range. Ex: input value 999000 becomes 320000 before decoding proceed.
        /// See par.4.2 "Hardware watchdog V1 reference manual" for more details.</remarks>
        Task<Response> SetResponseTimeoutAsync(Int32 ms);

        /// <summary>
        /// Set soft reset attempts count asynchronously.
        /// </summary>
        /// <param name="count">Attempts count (1-8).</param>
        /// <returns>Returns <see cref="Response.SetSoftResetAttemptsOk"/> when operation succeeded,
        /// otherwise returns <see cref="Response.Busy"/></returns>
        /// <remarks>Even if input value goes beyond 1-8 range this value stays
        /// within specified range. Ex: input value 0 becomes 1 before decoding proceed.
        /// See par.4.3 "Hardware watchdog V1 reference manual" for more details.</remarks>
        Task<Response> SetSoftResetAttemptsAsync(Byte count);

        /// <summary>
        /// Set hard reset attempts count asynchronously.
        /// </summary>
        /// <param name="count">Attempts count (1-8).</param>
        /// <returns>Returns <see cref="Response.SetHardResetAttemptsOk"/> when operation succeeded,
        /// otherwise returns <see cref="Response.Busy"/></returns>
        /// <remarks>Even if input value goes beyond 1-8 range this value stays
        /// within specified range. Ex: input value 99 becomes 8 before decoding proceed.
        /// See par.4.4 "Hardware watchdog V1 reference manual" for more details.</remarks>
        Task<Response> SetHardResetAttemptsAsync(Byte count);

        /// <summary>
        /// Enable hard reset asynchronously.
        /// </summary>
        /// <returns>Returns <see cref="Response.EnableHardResetOk"/> when operation succeeded,
        /// otherwise returns <see cref="Response.Busy"/></returns>
        /// <remarks>See par.4.5 "Hardware watchdog V1 reference manual" for more details.</remarks>
        Task<Response> EnableHardResetAsync();

        /// <summary>
        /// Disable hard reset asynchronously.
        /// </summary>
        /// <returns>Returns <see cref="Response.DisableHardResetOk"/> when operation succeeded,
        /// otherwise returns <see cref="Response.Busy"/></returns>
        /// <remarks>See par.4.6 "Hardware watchdog V1 reference manual" for more details.</remarks>
        Task<Response> DisableHardResetAsync();

        /// <summary>
        /// Send 'Start' command asynchronously.
        /// </summary>
        /// <returns>Returns <see cref="Response.StartOk"/> when operation succeeded,
        /// otherwise returns <see cref="Response.Busy"/></returns>
        /// <remarks>See par.4.7 "Hardware watchdog V1 reference manual" for more details.</remarks>
        Task<Response> StartAsync();

        /// <summary>
        /// Send 'Stop' command asynchronously.
        /// </summary>
        /// <returns>Returns <see cref="Response.StopOk"/> when operation succeeded,
        /// otherwise returns <see cref="Response.Busy"/></returns>
        /// <remarks>See par.4.8 "Hardware watchdog V1 reference manual" for more details.</remarks>
        Task<Response> StopAsync();

        /// <summary>
        /// Get current hwdg status asynchronously.
        /// </summary>
        /// <returns>Returns <see cref="Status"/></returns>
        /// <remarks>See par.4.10 "Hardware watchdog V1 reference manual" for more details.</remarks>
        Task<Status> GetStatusAsync();

        /// <summary>
        /// Occurs when hwdg disconnects form host.
        /// </summary>
        event Action Disconnected;

        /// <summary>
        /// Occurs when hwdg connects to host.
        /// </summary>
        event HwdgResult Connected;

        /// <summary>
        /// Occurs when hwdg state changes.
        /// </summary>
        event HwdgResult Updated;
    }
}
