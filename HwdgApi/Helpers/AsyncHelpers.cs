using System;
using System.IO.Ports;
using System.Threading;
using System.Threading.Tasks;

namespace HwdgApi.Helpers
{
    public static class AsyncHelpers
    {
        /// <summary>
        /// Gets hwdg status async.
        /// </summary>
        /// <param name="port">Serial port instance.</param>
        /// <param name="timeout">Response timeout, ms.</param>
        /// <returns>Returns hwdg status.</returns>
        public static async Task<Status> GetHwdgStatusAsync(this SerialPort port, Int32 timeout = 100)
        {
            var tcs = new TaskCompletionSource<Object>();
            new CancellationTokenSource(timeout).Token.Register(() => tcs.TrySetCanceled(), false);
            void Handler(Object s, SerialDataReceivedEventArgs e) => tcs.TrySetResult(null);
            try
            {
                port.DataReceived += Handler;
                port.DiscardInBuffer();
                port.Write(new[] {(Byte) 0x00}, 0, 1);
                await tcs.Task;
                await tcs.Task;
                await tcs.Task;
                await tcs.Task;
                var b = new Byte[4];
                port.Read(b, 0, 4);
                return new Status(b);
            }
            finally
            {
                port.DataReceived -= Handler;
            }
        }

        /// <summary>
        /// Send command to hwdg async.
        /// </summary>
        /// <param name="port">Serial port instance.</param>
        /// <param name="data">command to be  sent.</param>
        /// <param name="timeout">Response timeout, ms.</param>
        /// <returns>Returns hwdg response.</returns>
        public static async Task<Response> SendHwdgCommandAsync(this SerialPort port, Byte data, Int32 timeout = 100)
        {
            var tcs = new TaskCompletionSource<Object>();
            new CancellationTokenSource(timeout).Token.Register(() => tcs.TrySetCanceled(), false);
            void Handler(Object s, SerialDataReceivedEventArgs e) => tcs.TrySetResult(null);
            try
            {
                port.DataReceived += Handler;
                port.DiscardInBuffer();
                port.Write(new[] { data }, 0, 1);
                await tcs.Task;
                return (Response)port.ReadByte();
            }
            finally
            {
                port.DataReceived -= Handler;
            }
        }
    }
}
