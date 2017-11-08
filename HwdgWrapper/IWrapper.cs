using System;
using System.Threading;
using System.Threading.Tasks;

namespace HwdgWrapper
{

    public delegate void HwdgResult(Status status);

    /// <summary>
    /// Represents hwdg send/get status wrapper.
    /// </summary>
    public interface IWrapper
    {
        /// <summary>
        /// Occurs when hwdg gets connected.
        /// </summary>
        event HwdgResult HwdgConnected;

        /// <summary>
        /// Occurs when hwdg disconnects.
        /// </summary>
        event Action HwdgDisconnected;

        /// <summary>
        /// Occurs when hwdg state changes.
        /// </summary>
        event HwdgResult HwdgUpdated;

        /// <summary>
        /// Gets hwdg status.
        /// </summary>
        /// <returns>Returns hwdg status.</returns>
        Status GetStatus();

        /// <summary>
        /// Send command to hwdg.
        /// </summary>
        /// <param name="cmd">Command to be sent.</param>
        /// <returns>Returns hwdg command response.</returns>
        Response SendCommand(Byte cmd);

        /// <summary>
        /// Gets hwdg status asynchronously.
        /// </summary>
        /// <param name="ct">Cancellation token (optional).</param>
        /// <returns>Returns hwdg status.</returns>
        Task<Status> GetStatusAsync(CancellationToken ct = default(CancellationToken));

        /// <summary>
        /// Send command to hwdg asynchronously.
        /// </summary>
        /// <param name="cmd">Command to be sent.</param>
        /// <param name="ct">Cancellation token (optional).</param>
        /// <returns>Returns hwdg command response.</returns>
        Task<Response> SendCommandAsync(Byte cmd, CancellationToken ct = default(CancellationToken));
    }
}