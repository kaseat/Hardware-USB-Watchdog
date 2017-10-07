using System;
using System.Threading.Tasks;

namespace HwdgWrapper
{
    /// <summary>
    /// Represents hwdg send/get status wrapper.
    /// </summary>
    public interface IWrapper
    {
        /// <summary>
        /// Occurs when hwdg gets connected.
        /// </summary>
        event Action HwdgConnected;

        /// <summary>
        /// Occurs when hwdg disconnects.
        /// </summary>
        event Action HwdgDisconnected;

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
        /// <returns>Returns hwdg status.</returns>
        Task<Status> GetStatusAsync();

        /// <summary>
        /// Send command to hwdg asynchronously.
        /// </summary>
        /// <param name="cmd">Command to be sent.</param>
        /// <returns>Returns hwdg command response.</returns>
        Task<Response> SendCommandAsync(Byte cmd);
    }
}