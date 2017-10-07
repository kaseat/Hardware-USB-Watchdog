using System;
using System.IO.Ports;
using System.Threading.Tasks;
using System.Timers;
using Timer = System.Timers.Timer;

namespace HwdgWrapper
{
    public class SerialWrapper : IWrapper, IDisposable
    {
        private readonly Object threadLock = new Object();
        private String lastSuccessedPortName;
        private readonly Timer timer = new Timer(500);

        public SerialWrapper()
        {
            GetStatus();
            timer.Elapsed += OnElpse;
            timer.Start();
        }

        public event Action HwdgConnected = delegate { };
        public event Action HwdgDisconnected = delegate { };
        private event Action TransmissionComplete = delegate { };

        public void Dispose()
        {
            timer.Elapsed -= OnElpse;
            timer.Dispose();
            GC.SuppressFinalize(this);
        }

        public Status GetStatus()
        {
            var result = SearchAndGetResponse() ?? SearchAndGetResponse();
            TransmissionComplete?.Invoke();
            return result;
        }

        public Response SendCommand(Byte cmd)
        {
            var result = SearchAndSendCommand(cmd);
            if (result == Response.ErrorSendCmd)
                result = SearchAndSendCommand(cmd);
            TransmissionComplete?.Invoke();
            return result;
        }

        public async Task<Status> GetStatusAsync()
        {
            var tcs = new TaskCompletionSource<Object>();
            void Handler() => tcs.TrySetResult(null);
            try
            {
                TransmissionComplete += Handler;
                var result = GetStatus();
                await tcs.Task;
                return result;
            }
            finally
            {
                TransmissionComplete -= Handler;
            }
        }

        public async Task<Response> SendCommandAsync(Byte cmd)
        {
            var tcs = new TaskCompletionSource<Object>();
            void Handler() => tcs.TrySetResult(null);
            try
            {
                TransmissionComplete += Handler;
                var result = SendCommand(cmd);
                await tcs.Task;
                return result;
            }
            finally
            {
                TransmissionComplete -= Handler;
            }
        }

        private static void WriteToPort(SerialPort port, Byte data)
        {
            port.ReadTimeout = 30;
            port.WriteTimeout = 30;
            port.Open();
            port.BaseStream.WriteByte(data);
        }

        /// <summary>
        /// Get Hwdg status.
        /// </summary>
        /// <param name="portName">Serial port name.</param>
        /// <returns>Returns HWDG status if operation succeedeed, otherwise returns null.</returns>
        private Status GetStatus(String portName)
        {
            var port = new SerialPort(portName, 9600);
            try
            {
                WriteToPort(port, 0x00);
                var b = new Byte[4];
                b[0] = (Byte)port.ReadByte();
                b[1] = (Byte)port.ReadByte();
                b[2] = (Byte)port.ReadByte();
                b[3] = (Byte)port.ReadByte();
                UpdateConnectStatus(port.PortName);
                return new Status(b);
            }
            catch (Exception)
            {
                UpdateDisconnectStatus();
                return null;
            }
            finally
            {
                port.Close();
                port.Dispose();
            }
        }

        private void OnElpse(Object sender, ElapsedEventArgs e)
        {
            GetStatus();
        }

        private Status SearchAndGetResponse()
        {
            lock (threadLock)
            {
                Status result = null;
                if (lastSuccessedPortName == null)
                {
                    foreach (var portName in SerialPort.GetPortNames())
                    {
                        result = GetStatus(portName);
                        if (result != null)
                            return result;
                    }
                }
                else
                {
                    result = GetStatus(lastSuccessedPortName);
                }
                return result;
            }
        }

        private Response SearchAndSendCommand(Byte cmd)
        {
            lock (threadLock)
            {
                var result = Response.ErrorSendCmd;
                if (lastSuccessedPortName == null)
                {
                    foreach (var portName in SerialPort.GetPortNames())
                    {
                        result = SendCommand(portName, cmd);
                        if (result != Response.ErrorSendCmd)
                            return result;
                    }
                }
                else
                {
                    result = SendCommand(lastSuccessedPortName, cmd);
                }
                return result;
            }
        }

        /// <summary>
        /// Send Command to HWDG.
        /// </summary>
        /// <param name="portName">Serial port name.</param>
        /// <param name="cmd">Command to be sent.</param>
        /// <returns>Retuns Response status.</returns>
        private Response SendCommand(String portName, Byte cmd)
        {
            var port = new SerialPort(portName, 9600);
            try
            {
                WriteToPort(port, cmd);
                UpdateConnectStatus(port.PortName);
                return (Response)port.ReadByte();
            }
            catch (Exception)
            {
                UpdateDisconnectStatus();
                return Response.ErrorSendCmd;
            }
            finally
            {
                port.Close();
                port.Dispose();
            }
        }

        private void UpdateConnectStatus(String portName)
        {
            if (lastSuccessedPortName == portName) return;
            lastSuccessedPortName = portName;
            HwdgConnected?.Invoke();
        }

        private void UpdateDisconnectStatus()
        {
            if (lastSuccessedPortName == null) return;
            lastSuccessedPortName = null;
            HwdgDisconnected?.Invoke();
        }
    }
}