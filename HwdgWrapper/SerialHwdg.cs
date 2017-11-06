using System;
using System.Threading.Tasks;
using System.Timers;

namespace HwdgWrapper
{
    public class SerialHwdg : IHwdg, IDisposable
    {
        private Boolean disposed;
        private readonly Timer timer = new Timer(3000);
        private readonly IWrapper wrapper;

        public SerialHwdg(IWrapper wrapper)
        {
            this.wrapper = wrapper ?? throw new ArgumentNullException(nameof(wrapper));
            this.wrapper.HwdgConnected += OnConnected;
            this.wrapper.HwdgDisconnected += OnDisconnected;
            this.wrapper.HwdgUpdated += OnUpdated;
            timer.Elapsed += OnElapse;
        }

        private void OnDisconnected() => Disconnected?.Invoke();

        private void OnConnected(Status status) => Connected?.Invoke(status);
        private void OnUpdated(Status status) => Updated?.Invoke(status);

        private void OnElapse(Object sender, ElapsedEventArgs e)
        {
            wrapper.SendCommand(0xFB);
        }

        private Byte ConvertRebootTimeout(Int32 ms)
        {
            if (disposed) throw new ObjectDisposedException(nameof(SerialHwdg));
            if (ms > 600000) ms = 600000;
            if (ms < 10000) ms = 10000;
            var trbi = (ms - 10000) / 5000;
            return (Byte) (trbi | 0x80);
        }

        private Byte ConvertResponseTimeout(Int32 ms)
        {
            if (disposed) throw new ObjectDisposedException(nameof(SerialHwdg));
            if (ms > 300000) ms = 300000;
            if (ms < 5000) ms = 5000;
            var trsi = ms / 5000 - 1;
            return (Byte) (trsi | 0x40);
        }

        private Byte ConvertSoftResetAttempts(Byte count)
        {
            if (disposed) throw new ObjectDisposedException(nameof(SerialHwdg));
            if (count > 8) count = 8;
            if (count < 1) count = 1;
            var nsi = count - 1;
            return (Byte) (nsi | 0x10);
        }

        private Byte ConvertHardResetAttempts(Byte count)
        {
            if (disposed) throw new ObjectDisposedException(nameof(SerialHwdg));
            if (count > 8) count = 8;
            if (count < 1) count = 1;
            var nhi = count - 1;
            return (Byte) (nhi | 0x18);
        }

        public void RestoreStatus(Status status)
        {
            if ((GetStatus()?.RawData & 0x80000000) == 0)
            {
                SetResponseTimeout(status.ResponseTimeout);
                SetRebootTimeout(status.RebootTimeout);
            }
            else
            {
                SetResponseTimeout(status.ResponseTimeout > 60000 ? 60000 : status.ResponseTimeout);
            }


            SetHardResetAttempts(status.HardResetAttempts);
            SetSoftResetAttempts(status.SoftResetAttempts);
            if ((status.State & WatchdogState.HardRersetEnabled) != 0)
            {
                EnableHardReset();
            }
            else
            {
                DisableHardReset();
            }
        }

        public void TestSoftReset()
        {
            wrapper.SendCommand(0x7F);
        }

        public void TestHardReset()
        {
            wrapper.SendCommand(0x7E);
        }

        public Response SetRebootTimeout(Int32 ms) => wrapper.SendCommand(ConvertRebootTimeout(ms));

        public Response SetResponseTimeout(Int32 ms) => wrapper.SendCommand(ConvertResponseTimeout(ms));

        public Response SetSoftResetAttempts(Byte count) => wrapper.SendCommand(ConvertSoftResetAttempts(count));

        public Response SetHardResetAttempts(Byte count) => wrapper.SendCommand(ConvertHardResetAttempts(count));

        public Response EnableHardReset()
        {
            if (disposed) throw new ObjectDisposedException(nameof(SerialHwdg));
            return wrapper.SendCommand(0xFC);
        }

        public Response DisableHardReset()
        {
            if (disposed) throw new ObjectDisposedException(nameof(SerialHwdg));
            return wrapper.SendCommand(0xFD);
        }

        public Response Start()
        {
            if (disposed) throw new ObjectDisposedException(nameof(SerialHwdg));
            var status = wrapper.SendCommand(0xF9);
            timer.Enabled = status == Response.StartOk;
            return status;
        }

        public Response Stop()
        {
            if (disposed) throw new ObjectDisposedException(nameof(SerialHwdg));
            var status = wrapper.SendCommand(0xFA);
            if (status == Response.StopOk)
            {
                timer.Enabled = false;
            }
            return status;
        }

        public Status GetStatus()
        {
            if (disposed) throw new ObjectDisposedException(nameof(SerialHwdg));
            return wrapper.GetStatus();
        }

        public async Task<Response> SetRebootTimeoutAsync(Int32 ms) =>
            await wrapper.SendCommandAsync(ConvertRebootTimeout(ms));

        public async Task<Response> SetResponseTimeoutAsync(Int32 ms) =>
            await wrapper.SendCommandAsync(ConvertResponseTimeout(ms));

        public async Task<Response> SetSoftResetAttemptsAsync(Byte count) =>
            await wrapper.SendCommandAsync(ConvertSoftResetAttempts(count));

        public async Task<Response> SetHardResetAttemptsAsync(Byte count) =>
            await wrapper.SendCommandAsync(ConvertHardResetAttempts(count));

        public async Task<Response> EnableHardResetAsync()
        {
            if (disposed) throw new ObjectDisposedException(nameof(SerialHwdg));
            return await wrapper.SendCommandAsync(0xFC);
        }

        public async Task<Response> DisableHardResetAsync()
        {
            if (disposed) throw new ObjectDisposedException(nameof(SerialHwdg));
            return await wrapper.SendCommandAsync(0xFD);
        }

        public async Task<Response> StartAsync()
        {
            if (disposed) throw new ObjectDisposedException(nameof(SerialHwdg));
            var status = await wrapper.SendCommandAsync(0xF9);
            timer.Enabled = status == Response.StartOk;
            return status;
        }

        public async Task<Response> StopAsync()
        {
            if (disposed) throw new ObjectDisposedException(nameof(SerialHwdg));
            var status = await wrapper.SendCommandAsync(0xFA);
            if (status == Response.StopOk)
            {
                timer.Enabled = false;
            }
            return status;
        }

        public async Task<Status> GetStatusAsync()
        {
            if (disposed) throw new ObjectDisposedException(nameof(SerialHwdg));
            return await wrapper.GetStatusAsync();
        }

        public event Action Disconnected;
        public event HwdgResult Connected;
        public event HwdgResult Updated;

        public void Dispose()
        {
            if (disposed) return;
            disposed = true;
            timer.Elapsed -= OnElapse;
            wrapper.HwdgConnected -= OnConnected;
            wrapper.HwdgDisconnected -= OnDisconnected;
            wrapper.HwdgUpdated -= OnUpdated;
            timer.Dispose();
            GC.SuppressFinalize(this);
        }
    }
}