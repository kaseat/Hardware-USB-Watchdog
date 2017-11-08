using System;
using System.Diagnostics;
using System.IO.Ports;
using System.Threading;
using System.Threading.Tasks;

namespace HwdgWrapper
{
    public class SerialWrapper : IWrapper, IDisposable
    {
        private const Int32 Baudrate = 9600;
        private readonly Object threadLock = new Object();
        private readonly Timer timer;
        private String lastSuccessedPortName;
        private Boolean isUpdated;
        private Status lastStatus;

        public SerialWrapper()
        {
            Trace.WriteLine($"SerialWrapper ctor at thread {Thread.CurrentThread.ManagedThreadId}");
            const Int32 onElapseTimeout = 1000;
            // Initialize timer that checks hwdg availability once per second.
            timer = new Timer(OnElapse, null, onElapseTimeout, onElapseTimeout);
        }

        private void OnElapse(Object stste)
        {
            Trace.WriteLine($"OnElapse at thread {Thread.CurrentThread.ManagedThreadId}");
            GetStatus();
        }

        public Status GetStatus()
        {
            // We're working with single resource (Serial port) so we
            // need GetStatus operation to be atomic.
            lock (threadLock)
            {
                Trace.WriteLine($"Enter GetStatus at {Thread.CurrentThread.ManagedThreadId} thread");
                var result = SearchAndGetStatus();

                // Last result is present but not equal previous one
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
                Trace.WriteLine($"Exit GetStatus with {result} at {Thread.CurrentThread.ManagedThreadId} thread");
                return result;
            }
        }

        public Response SendCommand(Byte cmd)
        {
            // We're working with single resource (Serial port) so we
            // must use one SerialPort instance at the moment.
            lock (threadLock)
            {
                Trace.WriteLine($"Enter SendCommand at {Thread.CurrentThread.ManagedThreadId} thread");
                var result = SearchAndSendCommand(cmd);
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
                        lastStatus = null;
                        OnDisconnected();
                    }
                }
                Trace.WriteLine($"Exit SendCommand with {result} at {Thread.CurrentThread.ManagedThreadId} thread");
                return result;
            }
        }

        public async Task<Status> GetStatusAsync(CancellationToken ct = default(CancellationToken))
        {
            Trace.WriteLine($"Begin get status async at {Thread.CurrentThread.ManagedThreadId} thread...");
            return await Task.Run(() => GetStatus(), ct);
        }

        public async Task<Response> SendCommandAsync(Byte cmd, CancellationToken ct = default(CancellationToken))
        {
            Trace.WriteLine($"Begin send command async at {Thread.CurrentThread.ManagedThreadId} thread...");
            return await Task.Run(() => SendCommand(cmd), ct);
        }

        /// <summary>
        /// Trying to find serial port HWDG connected to and get status.
        /// </summary>
        /// <returns></returns>
        private Status SearchAndGetStatus()
        {
            Status result = null;
            // If last transmission was unsuccessful or this is a first
            // transmission we need to find serial port HWDG connected to.
            if (lastSuccessedPortName == null)
            {
                Trace.WriteLine("Trying to find a port with WDG connected...");
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
                Trace.WriteLine("Trying to find a port with WDG connected...");
                foreach (var portName in SerialPort.GetPortNames())
                {
                    result = SendCommand(portName, cmd);
                    if (result != Response.SendCommandNoHwdgResponse || result != Response.SendCommandUnknownError)
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
            const Int32 readStatusTimeout = 80;
            const Int32 statusLength = 5;
            const Int32 inputBufferOffset = 0;
            const Byte getStatusCommand = 0x01;

            // Trying to open port and get status.
            using (var port = new SerialPort(portName, Baudrate))
            {
                try
                {
                    Trace.WriteLine($"Getting status from {portName}");
                    WriteToPort(port, getStatusCommand);

                    // Wait until we get all bytes of response
                    using (var source = new CancellationTokenSource(readStatusTimeout))
                    {
                        while (port.BytesToRead < statusLength && !source.IsCancellationRequested)
                        {
                        }
                    }

                    // Check that we received all bytes.
                    if (port.BytesToRead != statusLength)
                    {
                        Trace.WriteLine($"No HWDG found on {portName} port");
                        TransmissionFailed();
                        return null;
                    }

                    //Read received data.
                    var b = new Byte[statusLength];
                    port.Read(b, inputBufferOffset, statusLength);

                    // If there is no exceptions during writing and reading that
                    // means HWDG is present on current port and responses. Update
                    // last successful connection port name if necessary.
                    Trace.WriteLine($"Reading response {b[0]:X2} {b[1]:X2} {b[2]:X2} {b[3]:X2} {b[4]:X2} OK!");
                    TransmissionOk(port.PortName);
                    return new Status(b);
                }

                // If any exception occurred that means transmission has failed.
                // So we need to set last succeeded port name to NULL.
                // Set lastSuccessedPortName to NULL if necessary.
                catch (Exception ex)
                {
                    Trace.WriteLine($"Get status fail! Reason: {ex.Message}");
                    TransmissionFailed();
                    return null;
                }
            }
        }

        private Response SendCommand(String portName, Byte cmd)
        {
            const Int32 readCmdResponseTimeout = 30;
            const Int32 cmdResponseLength = 1;

            // Trying to open port and send the command.
            using (var port = new SerialPort(portName, Baudrate))
            {
                try
                {
                    Trace.WriteLine($"Sending command from {portName}");
                    WriteToPort(port, cmd);

                    // Wait until we get all bytes of response
                    using (var source = new CancellationTokenSource(readCmdResponseTimeout))
                    {
                        while (port.BytesToRead < cmdResponseLength && !source.IsCancellationRequested)
                        {
                        }
                    }

                    // Check that we received all bytes.
                    if (port.BytesToRead != cmdResponseLength)
                    {
                        Trace.WriteLine($"No HWDG found on {portName} port");
                        TransmissionFailed();
                        return Response.SendCommandNoHwdgResponse;
                    }

                    var rsp = (Response) port.ReadByte();

                    // If there is no exceptions during writing and reading that
                    // means HWDG is present on current port and responses. Update
                    // last successful connection port name if necessary.
                    Trace.WriteLine($"Reading {rsp} OK!");
                    TransmissionOk(portName);
                    return rsp;
                }

                // If any exception occurred that means transmission has failed.
                // So we need to set last succeeded port name to NULL.
                // Set lastSuccessedPortName to NULL if necessary.
                catch (Exception ex)
                {
                    Trace.WriteLine($"Send command fail! Reason: {ex.Message}");
                    TransmissionFailed();
                    return Response.SendCommandUnknownError;
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
            Trace.WriteLine($"lastSuccessedPortName set to {portName}");
            isUpdated = true;
        }

        /// <summary>
        /// This procedure calls in case of transmission error.
        /// </summary>
        private void TransmissionFailed()
        {
            if (lastSuccessedPortName == null) return;
            lastSuccessedPortName = null;
            Trace.WriteLine("lastSuccessedPortName set to NULL");
            isUpdated = true;
        }

        private static void WriteToPort(SerialPort port, Byte data)
        {
            port.ReadTimeout = 30;
            port.WriteTimeout = 30;
            Trace.WriteLine($"Opening {port.PortName} port...");
            port.Open();
            Trace.WriteLine($"Writing {data:X2} to the {port.PortName} port...");
            port.BaseStream.WriteByte(data);
            Trace.WriteLine($"Write {data:X2} OK!");
        }

        public void Dispose()
        {
            timer.Dispose();
            GC.SuppressFinalize(this);
            Trace.WriteLine($"SerialWrapper disposed at thread {Thread.CurrentThread.ManagedThreadId}");
        }

        public event HwdgResult HwdgConnected;
        public event Action HwdgDisconnected;
        public event HwdgResult HwdgUpdated;

        /// <summary>
        /// Executes each HwdgConnected callback method in separate thread.
        /// </summary>
        /// <param name="status">Current hwdg status.</param>
        private void OnConnected(Status status)
        {
            if (HwdgConnected == null) return;
            foreach (var itemDelegate in HwdgConnected.GetInvocationList())
            {
                ThreadPool.QueueUserWorkItem(Callback);

                void Callback(Object state)
                {
                    var threadId = Thread.CurrentThread.ManagedThreadId;
                    try
                    {
                        Trace.WriteLine(
                            $"Begin invoke OnConnected delegate {itemDelegate.Method.Name} in {threadId} thread.");
                        ((HwdgResult) itemDelegate).Invoke(status);
                        Trace.WriteLine(
                            $"End invoke OnConnected delegate {itemDelegate.Method.Name} in {threadId} thread.");
                    }
                    catch (Exception e)
                    {
                        Trace.WriteLine(
                            $"OnConnected delegate {itemDelegate.Method.Name} in {threadId} thread thrown exception: {e}.");
                        throw;
                    }
                }
            }
        }

        /// <summary>
        /// Executes each HwdgDisconnected callback method in separate thread.
        /// </summary>
        private void OnDisconnected()
        {
            if (HwdgDisconnected == null) return;
            foreach (var itemDelegate in HwdgDisconnected.GetInvocationList())
            {
                ThreadPool.QueueUserWorkItem(Callback);

                void Callback(Object state)
                {
                    var threadId = Thread.CurrentThread.ManagedThreadId;
                    try
                    {
                        Trace.WriteLine(
                            $"Begin invoke OnDisconnected delegate {itemDelegate.Method.Name} in {threadId} thread.");
                        ((Action) itemDelegate).Invoke();
                        Trace.WriteLine(
                            $"End invoke OnDisconnected delegate {itemDelegate.Method.Name} in {threadId} thread.");
                    }
                    catch (Exception e)
                    {
                        Trace.WriteLine(
                            $"OnDisconnected delegate {itemDelegate.Method.Name} in {threadId} thread thrown exception: {e}.");
                        throw;
                    }
                }
            }
        }

        /// <summary>
        /// Executes each HwdgUpdated callback method in separate thread.
        /// </summary>
        /// <param name="status">Current hwdg status.</param>
        private void OnUpdated(Status status)
        {
            if (HwdgUpdated == null) return;
            foreach (var itemDelegate in HwdgUpdated.GetInvocationList())
            {
                ThreadPool.QueueUserWorkItem(Callback);

                void Callback(Object state)
                {
                    var threadId = Thread.CurrentThread.ManagedThreadId;
                    try
                    {
                        Trace.WriteLine(
                            $"Begin invoke OnUpdated delegate {itemDelegate.Method.Name} in {threadId} thread.");
                        ((HwdgResult) itemDelegate).Invoke(status);
                        Trace.WriteLine(
                            $"End invoke OnUpdated delegate {itemDelegate.Method.Name} in {threadId} thread.");
                    }
                    catch (Exception e)
                    {
                        Trace.WriteLine(
                            $"OnUpdated delegate {itemDelegate.Method.Name} in {threadId} thread thrown exception: {e}.");
                        throw;
                    }
                }
            }
        }
    }
}