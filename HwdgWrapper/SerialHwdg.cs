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
            timer.Elapsed += OnElapse;
        }

        private void OnDisconnected() => Disconnected?.Invoke();

        private void OnConnected(Status status) => Connected?.Invoke(status);

        private void OnElapse(Object sender, ElapsedEventArgs e)
        {
            wrapper.SendCommand(0x05);
        }

        private Byte ConvertRebootTimeout(Int32 ms)
        {
            if (disposed) throw new ObjectDisposedException(nameof(SerialHwdg));
            if (ms > 645000) ms = 645000;
            if (ms < 10000) ms = 10000;
            var trbi = (ms - 10000) / 5000;
            return (Byte) (trbi | 0x80);
        }

        private Byte ConvertResponseTimeout(Int32 ms)
        {
            if (disposed) throw new ObjectDisposedException(nameof(SerialHwdg));
            if (ms > 320000) ms = 320000;
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

        public Response SetRebootTimeout(Int32 ms) => wrapper.SendCommand(ConvertRebootTimeout(ms));

        public Response SetResponseTimeout(Int32 ms) => wrapper.SendCommand(ConvertResponseTimeout(ms));

        public Response SetSoftResetAttempts(Byte count) => wrapper.SendCommand(ConvertSoftResetAttempts(count));

        public Response SetHardResetAttempts(Byte count) => wrapper.SendCommand(ConvertHardResetAttempts(count));

        public Response EnableHardReset()
        {
            if (disposed) throw new ObjectDisposedException(nameof(SerialHwdg));
            return wrapper.SendCommand(0x03);
        }

        public Response DisableHardReset()
        {
            if (disposed) throw new ObjectDisposedException(nameof(SerialHwdg));
            return wrapper.SendCommand(0x04);
        }

        public Response Start()
        {
            if (disposed) throw new ObjectDisposedException(nameof(SerialHwdg));
            var status = wrapper.SendCommand(0x01);
            timer.Enabled = status == Response.StartOk;
            return status;
        }

        public Response Stop()
        {
            if (disposed) throw new ObjectDisposedException(nameof(SerialHwdg));
            var status = wrapper.SendCommand(0x02);
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
            return await wrapper.SendCommandAsync(0x03);
        }

        public async Task<Response> DisableHardResetAsync()
        {
            if (disposed) throw new ObjectDisposedException(nameof(SerialHwdg));
            return await wrapper.SendCommandAsync(0x04);
        }

        public async Task<Response> StartAsync()
        {
            if (disposed) throw new ObjectDisposedException(nameof(SerialHwdg));
            var status = await wrapper.SendCommandAsync(0x01);
            timer.Enabled = status == Response.StartOk;
            return status;
        }

        public async Task<Response> StopAsync()
        {
            if (disposed) throw new ObjectDisposedException(nameof(SerialHwdg));
            var status = await wrapper.SendCommandAsync(0x02);
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
        public event HwdgConnected Connected;

        public void Dispose()
        {
            if (disposed) return;
            disposed = true;
            timer.Elapsed -= OnElapse;
            wrapper.HwdgConnected -= OnConnected;
            wrapper.HwdgDisconnected -= OnDisconnected;
            timer.Dispose();
            GC.SuppressFinalize(this);
        }
    }
}