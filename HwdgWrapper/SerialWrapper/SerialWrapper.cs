using System;
using System.IO.Ports;
using System.Threading.Tasks;
using System.Timers;
using Timer = System.Timers.Timer;

namespace HwdgWrapper
{
    public partial class SerialWrapper : IWrapper, IDisposable
    {
        private readonly Object threadLock = new Object();
        private readonly Timer timer = new Timer(500);
        private String lastSuccessedPortName;
        private Boolean isUpdated;
        private Status lastStatus;

        public SerialWrapper()
        {
            lastStatus = GetStatus();
            timer.Elapsed += OnElapse;
            timer.AutoReset = false;
            timer.Start();
        }

        private event Action TransmissionComplete = delegate { };

        private void OnElapse(Object sender, ElapsedEventArgs e)
        {
            GetStatus();
            timer.Enabled = true;
        }

        public Status GetStatus()
        {
            lock (threadLock)
            {
                var result = SearchAndGetStatus() ?? SearchAndGetStatus();
                if (lastStatus != null && result != null && !lastStatus.Equals(result))
                    OnUpdated(lastStatus = result);
                if (lastStatus == null && result != null) OnUpdated(lastStatus = result);
                if (lastStatus != null && result == null) OnUpdated(lastStatus=null);
                TransmissionComplete?.Invoke();

                if (!isUpdated) return result;

                isUpdated = false;
                if (lastSuccessedPortName == null) OnDisconnected();
                else OnConnected(result);
                return result;
            }
        }

        public Response SendCommand(Byte cmd)
        {
            lock (threadLock)
            {
                var result = SearchAndSendCommand(cmd);
                if (result == Response.ErrorSendCmd) result = SearchAndSendCommand(cmd);
                TransmissionComplete?.Invoke();
                if (!isUpdated) return result;
                isUpdated = false;
                if (lastSuccessedPortName == null) OnDisconnected();
                else
                {
                    var status = GetStatus(lastSuccessedPortName);

                    if (lastStatus != null && status != null && !lastStatus.Equals(status))
                        OnUpdated(lastStatus = status);
                    if (lastStatus == null && status != null) OnUpdated(lastStatus = status);
                    if (lastStatus != null && status == null) OnUpdated(lastStatus);
                    OnConnected(status);
                }
                return result;
            }
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

        private Status SearchAndGetStatus()
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

        private Response SearchAndSendCommand(Byte cmd)
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

        private Status GetStatus(String portName)
        {
            var port = new SerialPort(portName, 9600);
            try
            {
                WriteToPort(port, 0x00);
                var b = new Byte[4];
                b[0] = (Byte) port.ReadByte();
                b[1] = (Byte) port.ReadByte();
                b[2] = (Byte) port.ReadByte();
                b[3] = (Byte) port.ReadByte();
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

        private Response SendCommand(String portName, Byte cmd)
        {
            var port = new SerialPort(portName, 9600);
            try
            {
                WriteToPort(port, cmd);
                UpdateConnectStatus(port.PortName);
                return (Response) port.ReadByte();
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
            isUpdated = true;
        }

        private void UpdateDisconnectStatus()
        {
            if (lastSuccessedPortName == null) return;
            lastSuccessedPortName = null;
            isUpdated = true;
        }

        private static void WriteToPort(SerialPort port, Byte data)
        {
            port.ReadTimeout = 30;
            port.WriteTimeout = 30;
            port.Open();
            port.BaseStream.WriteByte(data);
        }

        public void Dispose()
        {
            timer.Elapsed -= OnElapse;
            timer.Dispose();
            GC.SuppressFinalize(this);
        }
    }
}