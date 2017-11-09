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
            // Initialize timer that checks hwdg availability once per onElapseTimeout.
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
                Trace.Write($"Finish SearchAndSendCommand result: {result} ");
                Trace.WriteLine($"at {Thread.CurrentThread.ManagedThreadId} thread");

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
                if (!isUpdated) return result;
                isUpdated = false;
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

            // If last transmission was unsuccessful or this is a first
            // transmission we need to find serial port HWDG connected to.
            if (lastSuccessedPortName == null)
            {
                var result = Response.ErrorSendCmd;
                Trace.WriteLine("Trying to find a port with WDG connected...");
                foreach (var portName in SerialPort.GetPortNames())
                {
                    Trace.WriteLine($"Trying to sent cmd at {portName}");
                    result = SendCommand(portName, cmd);
                    Trace.WriteLine($"Cmd result {result}");
                    if (result == Response.SendCommandNoHwdgResponse ||
                        result == Response.SendCommandUnknownError ||
                        result == Response.SendCommandPortBusy) continue;

                    Trace.Write($" HWDG found at {portName} with status '{result}'");
                    Trace.WriteLine($" at {Thread.CurrentThread.ManagedThreadId} thread");
                    return result;
                }

                Trace.Write($"HWDG not found on any port. Exit with status '{result}'");
                Trace.WriteLine($" at {Thread.CurrentThread.ManagedThreadId} thread");
                return result;
            }
            // In case we already know port name just send the command.
            else
            {
                Trace.Write($"Trying to sent cmd at {lastSuccessedPortName}");
                var result = SendCommand(lastSuccessedPortName, cmd);
                Trace.WriteLine($"Cmd result {result}");
                return result;
            }
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
                    Trace.WriteLine($"Getting status from {portName} at {Thread.CurrentThread.ManagedThreadId} thread");
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

                    // Verify checksum.
                    var checksum = b.CalcCrc7(statusLength - 1);
                    if (checksum != b[statusLength - 1])
                    {
                        Trace.Write("Checksum verification error.");
                        Trace.WriteLine($"Calculated: {checksum}. Received: {b[statusLength - 1]}");
                        TransmissionFailed();
                        return null;
                    }
                    Trace.WriteLine($"Checksum verification ok. at {Thread.CurrentThread.ManagedThreadId} thread");

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
                    Trace.Write($"Get status fail at {Thread.CurrentThread.ManagedThreadId} thread! ");
                    Trace.WriteLine($"Reason: {ex.Message}");
                    TransmissionFailed();
                    return null;
                }
            }
        }

        private Response SendCommand(String portName, Byte cmd)
        {
            const Int32 readCmdResponseTimeout = 130;
            const Int32 cmdResponseLength = 1;

            // Trying to open port and send the command.
            using (var port = new SerialPort(portName, Baudrate))
            {
                try
                {
                    Trace.Write($"Sending command from {portName} ");
                    Trace.WriteLine($"at {Thread.CurrentThread.ManagedThreadId} thread");
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
                        Trace.Write($"No HWDG found on {portName} port ");
                        Trace.WriteLine($"at {Thread.CurrentThread.ManagedThreadId} thread");
                        TransmissionFailed();
                        return Response.SendCommandNoHwdgResponse;
                    }

                    var rsp = (Response) port.ReadByte();

                    // If there is no exceptions during writing and reading that
                    // means HWDG is present on current port and responses. Update
                    // last successful connection port name if necessary.
                    Trace.WriteLine($"Reading {rsp} OK! at {Thread.CurrentThread.ManagedThreadId} thread");
                    TransmissionOk(portName);
                    return rsp;
                }

                // If any other exception occurred that means transmission has failed.
                // So we need to set last succeeded port name to NULL.
                // Set lastSuccessedPortName to NULL if necessary.
                catch (Exception ex)
                {
                    Trace.Write($"Send command fail! Hr:{ex.HResult:X2} Reason: {ex.Message}. ");
                    Trace.WriteLine($"at {Thread.CurrentThread.ManagedThreadId} thread");

                    TransmissionFailed();
                    if ((UInt32) ex.HResult == 0x80070005)
                    {
                        Trace.WriteLine($"Port busy at {Thread.CurrentThread.ManagedThreadId} thread");
                        return Response.SendCommandPortBusy;
                    }
                    Trace.WriteLine("Other err");
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
            Trace.Write($"lastSuccessedPortName set to {portName} ");
            Trace.WriteLine($"at {Thread.CurrentThread.ManagedThreadId} thread");
            isUpdated = true;
        }

        /// <summary>
        /// This procedure calls in case of transmission error.
        /// </summary>
        private void TransmissionFailed()
        {
            if (lastSuccessedPortName == null) return;
            lastSuccessedPortName = null;
            Trace.WriteLine($"lastSuccessedPortName set to NULL at {Thread.CurrentThread.ManagedThreadId} thread");
            isUpdated = true;
        }

        private static void WriteToPort(SerialPort port, Byte data)
        {
            port.ReadTimeout = 30;
            port.WriteTimeout = 30;
            Trace.Write($"Opening {port.PortName} port ");
            Trace.WriteLine($"at {Thread.CurrentThread.ManagedThreadId} thread");
            port.Open();
            Trace.Write($"Writing {data:X2} to the {port.PortName} port ");
            Trace.WriteLine($"at {Thread.CurrentThread.ManagedThreadId} thread");
            port.BaseStream.WriteByte(data);
            Trace.WriteLine($"Write {data:X2} OK! at {Thread.CurrentThread.ManagedThreadId} thread");
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
                        Trace.Write("Begin invoke OnConnected delegate ");
                        Trace.WriteLine($"{itemDelegate.Method.Name} in {threadId} thread.");
                        ((HwdgResult) itemDelegate).Invoke(status);
                        Trace.Write("End invoke OnConnected delegate ");
                        Trace.WriteLine($"{itemDelegate.Method.Name} in {threadId} thread.");
                    }
                    catch (Exception e)
                    {
                        Trace.Write($"OnConnected delegate {itemDelegate.Method.Name} ");
                        Trace.WriteLine($"in {threadId} thread thrown exception: {e}.");
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
                        Trace.Write("Begin invoke OnDisconnected delegate ");
                        Trace.WriteLine($"{itemDelegate.Method.Name} in {threadId} thread.");
                        ((Action) itemDelegate).Invoke();
                        Trace.Write("End invoke OnDisconnected delegate ");
                        Trace.WriteLine($"{itemDelegate.Method.Name} in {threadId} thread.");
                    }
                    catch (Exception e)
                    {
                        Trace.Write($"OnDisconnected delegate {itemDelegate.Method.Name} ");
                        Trace.WriteLine($"in {threadId} thread thrown exception: {e}.");
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
                        Trace.Write("Begin invoke OnUpdated delegate ");
                        Trace.WriteLine($"{itemDelegate.Method.Name} in {threadId} thread.");
                        ((HwdgResult) itemDelegate).Invoke(status);
                        Trace.Write("End invoke OnUpdated delegate ");
                        Trace.WriteLine($"{itemDelegate.Method.Name} in {threadId} thread.");
                    }
                    catch (Exception e)
                    {
                        Trace.Write($"OnUpdated delegate {itemDelegate.Method.Name} ");
                        Trace.WriteLine($"in {threadId} thread thrown exception: {e}.");
                        throw;
                    }
                }
            }
        }
    }
}