using System;
using System.Diagnostics;
using System.Threading;
using System.Threading.Tasks;
using Timer = System.Timers.Timer;

namespace HwdgWrapper
{
    public class SerialHwdg : IHwdg, IDisposable
    {
        private Boolean disposed;
        private readonly IWrapper wrapper;
        private const Int32 OnElapseTimeout = 4000;
        private readonly Timer timer = new Timer(OnElapseTimeout);

        public SerialHwdg(IWrapper wrapper)
        {
            Trace.WriteLine($"SerialHwdg ctor at {Thread.CurrentThread.ManagedThreadId} thread");
            this.wrapper = wrapper ?? throw new ArgumentNullException(nameof(wrapper));

            // Apply user settings at chip startup
            wrapper.SendCommand(0x3B);

            // Subscribe on HWDG events
            this.wrapper.HwdgConnected += OnConnected;
            this.wrapper.HwdgDisconnected += OnDisconnected;
            this.wrapper.HwdgUpdated += OnUpdated;
            timer.Elapsed += OnElapse;
        }

        private void OnDisconnected() => Disconnected?.Invoke();
        private void OnConnected(Status status) => Connected?.Invoke(status);
        private void OnUpdated(Status status) => Updated?.Invoke(status);
        private void OnElapse(Object sender, System.Timers.ElapsedEventArgs e) => wrapper.SendCommand(0xFB);

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

        public Status LastStatus { get; private set; }

        public Response SaveCurrentState() => wrapper.SendCommand(0x39);

        public Response EnableLed() => wrapper.SendCommand(0xFE);

        public Response DisableLed() => wrapper.SendCommand(0xFF);

        public Response RstPulseOnStartupEnable() => wrapper.SendCommand(0x3E);

        public Response RstPulseOnStartupDisable() => wrapper.SendCommand(0x3F);

        public Response PwrPulseOnStartupEnable() => wrapper.SendCommand(0x3C);

        public Response PwrPulseOnStartupDisable() => wrapper.SendCommand(0x3D);

        public void RestoreFactory() => wrapper.SendCommand(0xF7);

        public void TestSoftReset() => wrapper.SendCommand(0x7F);

        public void TestHardReset() => wrapper.SendCommand(0x7E);

        public Response SetRebootTimeout(Int32 ms) => wrapper.SendCommand(ConvertRebootTimeout(ms));

        public Response SetResponseTimeout(Int32 ms) => wrapper.SendCommand(ConvertResponseTimeout(ms));

        public Response SetSoftResetAttempts(Byte count) => wrapper.SendCommand(ConvertSoftResetAttempts(count));

        public Response SetHardResetAttempts(Byte count) => wrapper.SendCommand(ConvertHardResetAttempts(count));

        public Response EnableHardReset() => wrapper.SendCommand(0xFC);

        public Response DisableHardReset() => wrapper.SendCommand(0xFD);

        public Response Start()
        {
            timer.Start();
            return wrapper.SendCommand(0xF9);
        }

        public Response Stop()
        {
            timer.Stop();
            return wrapper.SendCommand(0xFA);
        }

        public Status GetStatus()
        {
            Trace.WriteLine($"> GetStatus at {Thread.CurrentThread.ManagedThreadId} thread");
            return LastStatus = wrapper.GetStatus();
        }

        public Task<Response> SaveCurrentStateAsync(CancellationToken ct = default(CancellationToken))
            => wrapper.SendCommandAsync(0x39, ct);

        public Task<Response> EnableLedAsync(CancellationToken ct = default(CancellationToken))
            => wrapper.SendCommandAsync(0xFE, ct);

        public Task<Response> DisableLedAsync(CancellationToken ct = default(CancellationToken))
            => wrapper.SendCommandAsync(0xFF, ct);

        public Task<Response> RstPulseOnStartupEnableAsync(CancellationToken ct = default(CancellationToken))
            => wrapper.SendCommandAsync(0x3E, ct);

        public Task<Response> RstPulseOnStartupDisableAsync(CancellationToken ct = default(CancellationToken))
            => wrapper.SendCommandAsync(0x3F, ct);

        public Task<Response> PwrPulseOnStartupEnableAsync(CancellationToken ct = default(CancellationToken))
            => wrapper.SendCommandAsync(0x3C, ct);

        public Task<Response> PwrPulseOnStartupDisableAsync(CancellationToken ct = default(CancellationToken))
            => wrapper.SendCommandAsync(0x3D, ct);

        public void RestoreFactoryAsync(CancellationToken ct = default(CancellationToken))
            => wrapper.SendCommandAsync(0xF7, ct);

        public async Task<Response>
            SetRebootTimeoutAsync(Int32 ms, CancellationToken ct = default(CancellationToken)) =>
            await wrapper.SendCommandAsync(ConvertRebootTimeout(ms), ct);

        public async Task<Response>
            SetResponseTimeoutAsync(Int32 ms, CancellationToken ct = default(CancellationToken)) =>
            await wrapper.SendCommandAsync(ConvertResponseTimeout(ms), ct);

        public async Task<Response> SetSoftResetAttemptsAsync(Byte count,
            CancellationToken ct = default(CancellationToken)) =>
            await wrapper.SendCommandAsync(ConvertSoftResetAttempts(count), ct);

        public async Task<Response> SetHardResetAttemptsAsync(Byte count,
            CancellationToken ct = default(CancellationToken)) =>
            await wrapper.SendCommandAsync(ConvertHardResetAttempts(count), ct);

        public async Task<Response> EnableHardResetAsync(CancellationToken ct = default(CancellationToken)) =>
            await wrapper.SendCommandAsync(0xFC, ct);

        public async Task<Response> DisableHardResetAsync(CancellationToken ct = default(CancellationToken)) =>
            await wrapper.SendCommandAsync(0xFD, ct);

        public async Task<Response> StartAsync(CancellationToken ct = default(CancellationToken))
        {
            timer.Start();
            return await wrapper.SendCommandAsync(0xF9, ct);
        }

        public async Task<Response> StopAsync(CancellationToken ct = default(CancellationToken))
        {
            timer.Stop();
            return await wrapper.SendCommandAsync(0xFA, ct);
        }

        public async Task<Status> GetStatusAsync(CancellationToken ct = default(CancellationToken)) =>
            LastStatus = await wrapper.GetStatusAsync(ct);

        public event Action Disconnected;
        public event HwdgResult Connected;
        public event HwdgResult Updated;

        public void Dispose()
        {
            if (disposed) return;
            disposed = true;
            wrapper.HwdgConnected -= OnConnected;
            wrapper.HwdgDisconnected -= OnDisconnected;
            wrapper.HwdgUpdated -= OnUpdated;
            timer.Elapsed -= OnElapse;
            timer.Dispose();
            GC.SuppressFinalize(this);
        }
    }
}