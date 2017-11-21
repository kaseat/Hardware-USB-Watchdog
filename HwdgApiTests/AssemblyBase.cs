// Copyright 2017 Oleg Petrochenko
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

using System;
using System.IO.Ports;
using System.Threading;
using HwdgWrapper;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace HwdgApiTests
{
    [TestClass]
    public static class AssemblyBase
    {
        private const Byte ChipResetCommand = 0x00;
        private const Byte IsAliveCommand = 0xF8;

        public static SerialWrapper Wrapper;

        [AssemblyInitialize]
        public static void AssemblyInitialize(TestContext context)
        {
            Console.WriteLine(context.TestDir);
            Wrapper = new SerialWrapper();
        }

        [AssemblyCleanup]
        public static void AssemblyCleanup()
        {
            Wrapper.FactoryReset();
            Wrapper.Dispose();
        }

        /// <summary>
        /// Make factory reset and wait until HWDG comes online.
        /// </summary>
        /// <param name="wrapper">IWrapper implementation instance.</param>
        /// <param name="timeout">Operation timeout.</param>
        public static void FactoryResetAndWaitForReady(this IWrapper wrapper, Int32 timeout = 1000)
        {
            wrapper.FactoryReset();
            using (var source = new CancellationTokenSource(timeout))
            {
                while (wrapper.IsAlive() != Response.SoftwareVersion)
                {
                }

                if (source.IsCancellationRequested) Assert.Fail("HWDG does not response after FactoryReset");
            }
        }

        /// <summary>
        /// Make chip reset and wait until HWDG comes online.
        /// </summary>
        /// <param name="wrapper">IWrapper implementation instance.</param>
        /// <param name="timeout">Operation timeout.</param>
        public static void ChipResetAndWaitForReady(this IWrapper wrapper, Int32 timeout = 1000)
        {
            wrapper.ChipReset();
            using (var source = new CancellationTokenSource(timeout))
            {
                while (wrapper.IsAlive() != Response.SoftwareVersion)
                {
                }

                if (source.IsCancellationRequested) Assert.Fail("HWDG does not response after ChipReset");
            }
        }

        /// <summary>
        /// Wait for specified flag in HWDG response.
        /// </summary>
        /// <param name="wrapper">IWrapper implementation instance.</param>
        /// <param name="timeout">Operation timeout.</param>
        /// <param name="expectedFlag">Flag we expect.</param>
        public static void WaitForFlag(this IWrapper wrapper, WatchdogState expectedFlag, Int32 timeout = 5500)
        {
            using (var source = new CancellationTokenSource(timeout))
            {
                while (!wrapper.GetStatus().State.HasFlag(expectedFlag) &&
                       !source.IsCancellationRequested)
                {
                }

                if (source.IsCancellationRequested) Assert.Fail("HWDG WaitForFlag timeout");
            }
        }

        /// <summary>
        /// Reset chip and wait until it comes online.
        /// </summary>
        /// <param name="port">Serial port HWDG connected to.</param>
        /// <param name="timeout">Operation timeout.</param>
        /// <remarks>Apply this method when you cannot use IWrapper.</remarks>
        public static void ChipResetAndWaitForReady(this SerialPort port, Int32 timeout = Timeout.Infinite)
        {
            // Send ChipReset command.
            port.BaseStream.WriteByte(ChipResetCommand);

            // We must wait until chip comes online.
            // As some of our requests to HWDG will fade away we must send new requests periodically.
            // We can achieve this by setting short read timeout, sending "IsAlive" command and waiting
            // for TimeoutException occurred. After TimeoutException thrown  we'll start over until
            // operation timeout elapses. If proper answer receieved that means HWDG is online, so just
            // break the loop.
            var actualReadTimeout = port.ReadTimeout;
            port.ReadTimeout = 100;
            using (var source = new CancellationTokenSource(timeout))
            {
                while (!source.IsCancellationRequested)
                {
                    try
                    {
                        port.BaseStream.WriteByte(IsAliveCommand);
                        if ((Response) port.ReadByte() == Response.SoftwareVersion)
                        {
                            break;
                        }
                    }
                    catch (Exception e)
                    {
                        Console.WriteLine(e);
                    }
                }

                port.ReadTimeout = actualReadTimeout;
                if (source.IsCancellationRequested) Assert.Fail("Waiting for response after reset timeout");
            }
        }

        /// <summary>
        /// Find a port HWDG connected to.
        /// </summary>
        /// <returns>Returns SerialPort instance in case HWDG found. Otherwise returns null.</returns>
        /// <remarks>
        /// Apply this method when you cannot use IWrapper.
        /// Note that you must dispose SerialPort instance after usage.
        /// </remarks>
        public static SerialPort GetHwdgPort()
        {
            // Just loop through all serial ports and try to send "IsAlive" command.
            // If proper response received that means HWDG is online on current port.
            foreach (var portName in SerialPort.GetPortNames())
            {
                var p = new SerialPort(portName);
                try
                {
                    p.Open();
                    p.ReadTimeout = 100;
                    p.WriteTimeout = 100;
                    p.BaseStream.WriteByte(0xF8);
                    if ((Response) p.ReadByte() == Response.SoftwareVersion)
                    {
                        p.ReadTimeout = 1100;
                        return p;
                    }
                }
                catch (Exception e)
                {
                    Console.WriteLine(e);
                    p.Dispose();
                }
            }
            return null;
        }

        /// <summary>
        /// Wait for specified event.
        /// </summary>
        /// <param name="port">Serial port HWDG connected to.</param>
        /// <param name="ev">Event we expect.</param>
        /// <param name="timeout">Operation timeout.</param>
        public static void WaitForEvent(this SerialPort port, Response ev, Int32 timeout = Timeout.Infinite)
        {
            using (var source = new CancellationTokenSource(timeout))
            {
                try
                {
                    while ((Response) port.ReadByte() != ev && !source.IsCancellationRequested)
                    {
                    }
                }
                catch (Exception e)
                {
                    Console.WriteLine(e);
                    Assert.Fail("Read byte timeout");
                }

                if (source.IsCancellationRequested) Assert.Fail($"Waiting for event {ev} timeout");
            }
        }
    }
}