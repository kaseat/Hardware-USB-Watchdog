using System;
using System.IO.Ports;
using System.Threading;
using System.Threading.Tasks;

namespace HwdgWrapper
{
    public partial class SerialWrapper : IWrapper, IDisposable
    {
        private readonly Object threadLock = new Object();
        private readonly Timer timer;
        private String lastSuccessedPortName;
        private Boolean isUpdated;
        private Status lastStatus;

        public SerialWrapper()
        {
            timer = new Timer(OnElapse, null, 0, 1000);
            lastStatus = GetStatus();
        }

        private void OnElapse(Object stste) => GetStatus();

        public Status GetStatus()
        {
            // We're working with single resource (Serial port) so we
            // need GetStatus operation to be atomic.
            lock (threadLock)
            {
                var threadId = Thread.CurrentThread.ManagedThreadId;
                logger.Trace($"Begin get status at {threadId} thread...");
                var result = SearchAndGetStatus();

                // Last result present but not equal previous
                if (lastStatus != null && result != null && !lastStatus.Equals(result))
                    OnUpdated(lastStatus = result);

                // Last status is NULL but now connection ok
                if (lastStatus == null && result != null) OnConnected(lastStatus = result);

                // Last status was not NULL but now connection lost
                if (lastStatus != null && result == null)
                {
                    lastStatus = null;
                    OnDisconnected();
                }
                return result;
            }
        }

        public Response SendCommand(Byte cmd)
        {
            // We're working with single resource (Serial port) so we
            // must use one SerialPort instance at the moment.
            lock (threadLock)
            {
                var threadId = Thread.CurrentThread.ManagedThreadId;
                logger.Trace($"Begin send command at {threadId} thread...");
                var result = SearchAndSendCommand(cmd);
                if (result == Response.ErrorSendCmd) result = SearchAndSendCommand(cmd);
                if (!isUpdated) return result;
                isUpdated = false;
                if (lastSuccessedPortName == null) OnDisconnected();
                else
                {
                    var status = GetStatus(lastSuccessedPortName);

                    // Last result present but not equal previous
                    if (lastStatus != null && status != null && !lastStatus.Equals(status))
                        OnUpdated(lastStatus = status);

                    // Last status is NULL but now connection ok
                    if (lastStatus == null && status != null) OnConnected(lastStatus = status);

                    // Last status was not NULL but now connection lost
                    if (lastStatus != null && status == null)
                    {
                        OnUpdated(lastStatus = null);
                        OnDisconnected();
                    }
                }
                return result;
            }
        }

        public async Task<Status> GetStatusAsync()
        {
            var threadId = Thread.CurrentThread.ManagedThreadId;
            // ReSharper disable once InconsistentlySynchronizedField
            logger.Trace($"Begin get status async at {threadId} thread...");
            return await Task.Run(() => GetStatus());
        }

        public async Task<Response> SendCommandAsync(Byte cmd)
        {
            var threadId = Thread.CurrentThread.ManagedThreadId;
            // ReSharper disable once InconsistentlySynchronizedField
            logger.Trace($"Begin send command async at {threadId} thread...");
            return await Task.Run(() => SendCommand(cmd));
        }

        private Status SearchAndGetStatus()
        {
            Status result = null;
            // If last transmission was unsuccessful or this is a first
            // transmission we need to find serial port HWDG connected to.
            if (lastSuccessedPortName == null)
            {
                logger.Trace("Trying to find a port with WDG connected...");
                foreach (var portName in SerialPort.GetPortNames())
                {
                    result = GetStatus(portName);
                    if (result != null)
                        return result;
                }
            }
            // In case we already know port name just send the command.
            else
            {
                result = GetStatus(lastSuccessedPortName);
            }
            return result;
        }

        private Response SearchAndSendCommand(Byte cmd)
        {
            var result = Response.ErrorSendCmd;

            // If last transmission was unsuccessful or this is a first
            // transmission we need to find serial port HWDG connected to.
            if (lastSuccessedPortName == null)
            {
                logger.Trace("Trying to find a port with WDG connected...");
                foreach (var portName in SerialPort.GetPortNames())
                {
                    result = SendCommand(portName, cmd);
                    if (result != Response.ErrorSendCmd)
                        return result;
                }
            }
            // In case we already know port name just send the command.
            else
            {
                result = SendCommand(lastSuccessedPortName, cmd);
            }
            return result;
        }

        private Status GetStatus(String portName)
        {
            // Trying to open port and get status.
            using (var port = new SerialPort(portName, 9600))
            {
                try
                {
                    logger.Trace($"Getting status from {portName}...");
                    WriteToPort(port, 0x01);

                    // As GetStatus command response 4 bytes long
                    // we must read them all consistently.
                    logger.Trace($"Reading response from {portName}...");
                    var b = new Byte[4];
                    b[0] = (Byte) port.ReadByte();
                    b[1] = (Byte) port.ReadByte();
                    b[2] = (Byte) port.ReadByte();
                    b[3] = (Byte) port.ReadByte();

                    // If there is no exceptions during writing and reading that
                    // means HWDG is present on current port and responses. Update
                    // last successful connection port name if necessary.
                    logger.Trace($"Reading response {b[0]}{b[1]}{b[2]}{b[3]} OK!");
                    TransmissionOk(port.PortName);
                    logger.Trace("Get status OK!");
                    return new Status(b);
                }

                // If any exception occurred that means transmission has failed.
                // So we need to set last succeeded port name to NULL.
                // Set lastSuccessedPortName to NULL if necessary.
                catch (Exception ex)
                {
                    logger.Info($"Get status fail! Reason: {ex.Message}");
                    TransmissionFailed();
                    return null;
                }
            }
        }

        private Response SendCommand(String portName, Byte cmd)
        {
            // Trying to open port and send the command.
            using (var port = new SerialPort(portName, 9600))
            {
                try
                {
                    WriteToPort(port, cmd);
                    logger.Trace($"Reading from {portName}...");
                    var rsp = (Response) port.ReadByte();

                    // If there is no exceptions during writing and reading that
                    // means HWDG is present on current port and responses. Update
                    // last successful connection port name if necessary.
                    logger.Trace($"Reading {rsp} OK!");
                    TransmissionOk(portName);
                    return rsp;
                }

                // If any exception occurred that means transmission has failed.
                // So we need to set last succeeded port name to NULL.
                // Set lastSuccessedPortName to NULL if necessary.
                catch (Exception ex)
                {
                    logger.Info($"Send command fail! Reason: {ex.Message}");
                    TransmissionFailed();
                    return Response.ErrorSendCmd;
                }
            }
        }

        /// <summary>
        /// This procedure calls in case of successful transmission.
        /// </summary>
        /// <param name="portName">Successful operation serial port name.</param>
        private void TransmissionOk(String portName)
        {
            if (lastSuccessedPortName == portName) return;
            lastSuccessedPortName = portName;
            logger.Trace($"lastSuccessedPortName set to {portName}");
            isUpdated = true;
        }

        /// <summary>
        /// This procedure calls in case of transmission error.
        /// </summary>
        private void TransmissionFailed()
        {
            if (lastSuccessedPortName == null) return;
            lastSuccessedPortName = null;
            logger.Trace("lastSuccessedPortName set to NULL");
            isUpdated = true;
        }

        private void WriteToPort(SerialPort port, Byte data)
        {
            port.ReadTimeout = 30;
            port.WriteTimeout = 30;
            logger.Trace($"Opening {port.PortName} port...");
            port.Open();
            logger.Trace($"Writing {data} to the {port.PortName} port...");
            port.BaseStream.WriteByte(data);
            logger.Trace($"Write {data} OK!");
        }

        public void Dispose()
        {
            timer.Dispose();
            GC.SuppressFinalize(this);
            logger.Trace("Serial Wrapper disposed");
        }
    }
}