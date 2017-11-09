using System.Diagnostics;
using System.Threading;
using HwdgWrapper;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace HwdgWrapperTests
{
    [TestClass]
    public class SerialWrapperTests
    {
        private readonly Stopwatch sw = new Stopwatch();
        [TestMethod]
        public void VerifyEnableHardResetSendsCorrectCommand()
        {
            sw.Start();
            Trace.WriteLine($"Enter test method at {Thread.CurrentThread.ManagedThreadId} thread ({sw.Elapsed.TotalMilliseconds})");
            var t = new Thread(WatiAsync);
            Trace.WriteLine($"Run WatiAsync at {Thread.CurrentThread.ManagedThreadId} thread ({sw.Elapsed.TotalMilliseconds})");
            t.Start();
            Trace.WriteLine($"Test method sleep at {Thread.CurrentThread.ManagedThreadId} thread ({sw.Elapsed.TotalMilliseconds})");
            Thread.Sleep(2000);
            Trace.WriteLine($"Exit test method at {Thread.CurrentThread.ManagedThreadId} thread ({sw.Elapsed.TotalMilliseconds})");
        }

        private async void WatiAsync()
        {
            Trace.WriteLine($"Enter WatiAsync at {Thread.CurrentThread.ManagedThreadId} thread ({sw.Elapsed.TotalMilliseconds})");
            using (var wrapper = new SerialWrapper())
            {
                wrapper.HwdgConnected += Wrapper_HwdgConnected;
                Trace.WriteLine($"Run GetStatusAsync at {Thread.CurrentThread.ManagedThreadId} thread ({sw.Elapsed.TotalMilliseconds})");
                await wrapper.GetStatusAsync();
                Trace.WriteLine($">>> Run SendCommand at {Thread.CurrentThread.ManagedThreadId} thread ({sw.Elapsed.TotalMilliseconds})");

                var res= wrapper.SendCommand(0xF8);
                Trace.WriteLine($"Exit SendCommand with {res} at {Thread.CurrentThread.ManagedThreadId} thread ({sw.Elapsed.TotalMilliseconds})");
                Thread.Sleep(1500);
                wrapper.HwdgConnected -= Wrapper_HwdgConnected;
            }
            Trace.WriteLine($"Exit WatiAsync at {Thread.CurrentThread.ManagedThreadId} thread ({sw.Elapsed.TotalMilliseconds})");
        }

        private void Wrapper_HwdgConnected(Status status)
        {
            Trace.WriteLine($"Enter Wrapper_HwdgConnected at {Thread.CurrentThread.ManagedThreadId} thread ({sw.Elapsed.TotalMilliseconds})");
            Thread.Sleep(1200);
            Trace.WriteLine($"Exit Wrapper_HwdgConnected at {Thread.CurrentThread.ManagedThreadId} thread ({sw.Elapsed.TotalMilliseconds})");
        }
    }
}
