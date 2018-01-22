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
using System.Threading;
using System.Threading.Tasks;

namespace HwdgWrapper
{
    /// <summary>
    /// Represents Hwdg API. See par.4 "Hardware watchdog V1 reference manual" for more details.
    /// </summary>
    public interface IHwdg
    {
        /// <summary>
        /// Check specific URL availability.
        /// </summary>
        /// <param name="url">URL we check.</param>
        void EnableUrlCheck(String url);

        /// <summary>
        /// Disable URL check.
        /// </summary>
        void DisableUrlCheck();

        /// <summary>
        /// Last known HWDG status.
        /// </summary>
        Status LastStatus { get; }

        /// <summary>
        /// Save current HWDG state into internal memory.
        /// </summary>
        /// <returns>Returns operation status.</returns>
        Response SaveCurrentState();

        /// <summary>
        /// Enable built in LED.
        /// </summary>
        /// <returns>Returns operation status.</returns>
        Response EnableLed();

        /// <summary>
        /// Disable built in LED.
        /// </summary>
        /// <returns>Returns operation status.</returns>
        Response DisableLed();

        /// <summary>
        /// Enable reset pulse on startup.
        /// </summary>
        /// <returns>Returns operation status.</returns>
        Response RstPulseOnStartupEnable();

        /// <summary>
        /// Disable reset pulse on startup.
        /// </summary>
        /// <returns>Returns operation status.</returns>
        Response RstPulseOnStartupDisable();

        /// <summary>
        /// Enable power pulse on startup.
        /// </summary>
        /// <returns>Returns operation status.</returns>
        Response PwrPulseOnStartupEnable();

        /// <summary>
        /// Disable power pulse on startup.
        /// </summary>
        /// <returns>Returns operation status.</returns>
        Response PwrPulseOnStartupDisable();

        /// <summary>
        /// Restore factory chip settings.
        /// </summary>
        void RestoreFactory();

        /// <summary>
        /// Execute sequence to test soft reset ability.
        /// </summary>
        void TestSoftReset();

        /// <summary>
        /// Execute sequence to test soft reset ability.
        /// </summary>
        void TestHardReset();

        /// <summary>
        /// Set reboot timeout.
        /// </summary>
        /// <param name="ms">Timeout, ms. Range 10000-600000 ms.</param>
        /// <returns>Returns <see cref="Response.SetRebootTimeoutOk"/> when operation succeeded,
        /// otherwise returns <see cref="Response.Busy"/></returns>
        /// <remarks>Even if input value goes beyond 10000-600000 ms range this value stays
        /// within specified range. Ex: input value 1 becomes 10000 before decoding proceed.
        /// See par.4.1 "Hardware watchdog V1 reference manual" for more details.</remarks>
        Response SetRebootTimeout(Int32 ms);

        /// <summary>
        /// Set response timeout.
        /// </summary>
        /// <param name="ms">Timeout, ms. Range 5000-300000 ms.</param>
        /// <returns>Returns <see cref="Response.SetResponseTimeoutOk"/> when operation succeeded,
        /// otherwise returns <see cref="Response.Busy"/></returns>
        /// <remarks>Even if input value goes beyond 5000-300000 ms range this value stays
        /// within specified range. Ex: input value 999000 becomes 300000 before decoding proceed.
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
        /// <returns>Returns <see cref="Status"/> or null if there is no status available.</returns>
        /// <remarks>See par.4.10 "Hardware watchdog V1 reference manual" for more details.</remarks>
        Status GetStatus();

        /// <summary>
        /// Save current HWDG state into internal memory asynchronously.
        /// </summary>
        /// <param name="ct">Cancellation token (optional).</param>
        /// <returns>Returns operation status.</returns>
        Task<Response> SaveCurrentStateAsync(CancellationToken ct = default(CancellationToken));

        /// <summary>
        /// Enable built in LED asynchronously.
        /// </summary>
        /// <param name="ct">Cancellation token (optional).</param>
        /// <returns>Returns operation status.</returns>
        Task<Response> EnableLedAsync(CancellationToken ct = default(CancellationToken));

        /// <summary>
        /// Disable built in LED asynchronously.
        /// </summary>
        /// <param name="ct">Cancellation token (optional).</param>
        /// <returns>Returns operation status.</returns>
        Task<Response> DisableLedAsync(CancellationToken ct = default(CancellationToken));

        /// <summary>
        /// Enable reset pulse on startup asynchronously.
        /// </summary>
        /// <param name="ct">Cancellation token (optional).</param>
        /// <returns>Returns operation status.</returns>
        Task<Response> RstPulseOnStartupEnableAsync(CancellationToken ct = default(CancellationToken));

        /// <summary>
        /// Disable reset pulse on startup asynchronously.
        /// </summary>
        /// <param name="ct">Cancellation token (optional).</param>
        /// <returns>Returns operation status.</returns>
        Task<Response> RstPulseOnStartupDisableAsync(CancellationToken ct = default(CancellationToken));

        /// <summary>
        /// Enable power pulse on startup asynchronously.
        /// </summary>
        /// <param name="ct">Cancellation token (optional).</param>
        /// <returns>Returns operation status.</returns>
        Task<Response> PwrPulseOnStartupEnableAsync(CancellationToken ct = default(CancellationToken));

        /// <summary>
        /// Disable power pulse on startup asynchronously.
        /// </summary>
        /// <param name="ct">Cancellation token (optional).</param>
        /// <returns>Returns operation status.</returns>
        Task<Response> PwrPulseOnStartupDisableAsync(CancellationToken ct = default(CancellationToken));

        /// <summary>
        /// Restore factory chip settings asynchronously.
        /// </summary>
        /// <param name="ct">Cancellation token (optional).</param>
        Task RestoreFactoryAsync(CancellationToken ct = default(CancellationToken));

        /// <summary>
        /// Set reboot timeout asynchronously.
        /// </summary>
        /// <param name="ms">Timeout, ms. Range 10000-600000 ms.</param>
        /// <param name="ct">Cancellation token (optional).</param>
        /// <returns>Returns <see cref="Response.SetRebootTimeoutOk"/> when operation succeeded,
        /// otherwise returns <see cref="Response.Busy"/></returns>
        /// <remarks>Even if input value goes beyond 10000-600000 ms range this value stays
        /// within specified range. Ex: input value 1 becomes 10000 before decoding proceed.
        /// See par.4.1 "Hardware watchdog V1 reference manual" for more details.</remarks>
        Task<Response> SetRebootTimeoutAsync(Int32 ms, CancellationToken ct = default(CancellationToken));

        /// <summary>
        /// Set response timeout asynchronously.
        /// </summary>
        /// <param name="ms">Timeout, ms. Range 5000-300000 ms.</param>
        /// <param name="ct">Cancellation token (optional).</param>
        /// <returns>Returns <see cref="Response.SetResponseTimeoutOk"/> when operation succeeded,
        /// otherwise returns <see cref="Response.Busy"/></returns>
        /// <remarks>Even if input value goes beyond 5000-300000 ms range this value stays
        /// within specified range. Ex: input value 999000 becomes 300000 before decoding proceed.
        /// See par.4.2 "Hardware watchdog V1 reference manual" for more details.</remarks>
        Task<Response> SetResponseTimeoutAsync(Int32 ms, CancellationToken ct = default(CancellationToken));

        /// <summary>
        /// Set soft reset attempts count asynchronously.
        /// </summary>
        /// <param name="count">Attempts count (1-8).</param>
        /// <param name="ct">Cancellation token (optional).</param>
        /// <returns>Returns <see cref="Response.SetSoftResetAttemptsOk"/> when operation succeeded,
        /// otherwise returns <see cref="Response.Busy"/></returns>
        /// <remarks>Even if input value goes beyond 1-8 range this value stays
        /// within specified range. Ex: input value 0 becomes 1 before decoding proceed.
        /// See par.4.3 "Hardware watchdog V1 reference manual" for more details.</remarks>
        Task<Response> SetSoftResetAttemptsAsync(Byte count, CancellationToken ct = default(CancellationToken));

        /// <summary>
        /// Set hard reset attempts count asynchronously.
        /// </summary>
        /// <param name="count">Attempts count (1-8).</param>
        /// <param name="ct">Cancellation token (optional).</param>
        /// <returns>Returns <see cref="Response.SetHardResetAttemptsOk"/> when operation succeeded,
        /// otherwise returns <see cref="Response.Busy"/></returns>
        /// <remarks>Even if input value goes beyond 1-8 range this value stays
        /// within specified range. Ex: input value 99 becomes 8 before decoding proceed.
        /// See par.4.4 "Hardware watchdog V1 reference manual" for more details.</remarks>
        Task<Response> SetHardResetAttemptsAsync(Byte count, CancellationToken ct = default(CancellationToken));

        /// <summary>
        /// Enable hard reset asynchronously.
        /// </summary>
        /// <param name="ct">Cancellation token (optional).</param>
        /// <returns>Returns <see cref="Response.EnableHardResetOk"/> when operation succeeded,
        /// otherwise returns <see cref="Response.Busy"/></returns>
        /// <remarks>See par.4.5 "Hardware watchdog V1 reference manual" for more details.</remarks>
        Task<Response> EnableHardResetAsync(CancellationToken ct = default(CancellationToken));

        /// <summary>
        /// Disable hard reset asynchronously.
        /// </summary>
        /// <param name="ct">Cancellation token (optional).</param>
        /// <returns>Returns <see cref="Response.DisableHardResetOk"/> when operation succeeded,
        /// otherwise returns <see cref="Response.Busy"/></returns>
        /// <remarks>See par.4.6 "Hardware watchdog V1 reference manual" for more details.</remarks>
        Task<Response> DisableHardResetAsync(CancellationToken ct = default(CancellationToken));

        /// <summary>
        /// Send 'Start' command asynchronously.
        /// </summary>
        /// <param name="ct">Cancellation token (optional).</param>
        /// <returns>Returns <see cref="Response.StartOk"/> when operation succeeded,
        /// otherwise returns <see cref="Response.Busy"/></returns>
        /// <remarks>See par.4.7 "Hardware watchdog V1 reference manual" for more details.</remarks>
        Task<Response> StartAsync(CancellationToken ct = default(CancellationToken));

        /// <summary>
        /// Send 'Stop' command asynchronously.
        /// </summary>
        /// <param name="ct">Cancellation token (optional).</param>
        /// <returns>Returns <see cref="Response.StopOk"/> when operation succeeded,
        /// otherwise returns <see cref="Response.Busy"/></returns>
        /// <remarks>See par.4.8 "Hardware watchdog V1 reference manual" for more details.</remarks>
        Task<Response> StopAsync(CancellationToken ct = default(CancellationToken));

        /// <summary>
        /// Get current hwdg status asynchronously.
        /// </summary>
        /// <param name="ct">Cancellation token (optional).</param>
        /// <returns>Returns <see cref="Status"/></returns>
        /// <remarks>See par.4.10 "Hardware watchdog V1 reference manual" for more details.</remarks>
        Task<Status> GetStatusAsync(CancellationToken ct = default(CancellationToken));

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