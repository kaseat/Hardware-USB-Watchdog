using System;
using System.Threading;
using NLog;

namespace HwdgWrapper
{
    public partial class SerialWrapper
    {
        private readonly Logger logger = LogManager.GetCurrentClassLogger();

        public event HwdgResult HwdgConnected = delegate { };
        public event Action HwdgDisconnected = delegate { };
        public event HwdgResult HwdgUpdated = delegate { };

        /// <summary>
        /// Executes each HwdgConnected callback method in separate thread.
        /// </summary>
        /// <param name="status">Current hwdg status.</param>
        private void OnConnected(Status status)
        {
            foreach (var itemDelegate in HwdgConnected.GetInvocationList())
            {
                ThreadPool.QueueUserWorkItem(Callback);
                void Callback(Object state)
                {
                    var threadId = Thread.CurrentThread.ManagedThreadId;
                    try
                    {
                        logger.Trace($"Begin invoke OnConnected delegate in {threadId} thread.");
                        ((HwdgResult) itemDelegate).Invoke(status);
                        logger.Trace($"End invoke OnConnected delegate in {threadId} thread.");
                    }
                    catch (Exception e)
                    {
                        logger.Error($"OnConnected delegate in {threadId} thread thrown exception: {e}.");
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
            foreach (var itemDelegate in HwdgDisconnected.GetInvocationList())
            {
                ThreadPool.QueueUserWorkItem(Callback);
                void Callback(Object state)
                {
                    var threadId = Thread.CurrentThread.ManagedThreadId;
                    try
                    {
                        logger.Trace($"Begin invoke OnDisconnected delegate in {threadId} thread.");
                        ((Action) itemDelegate).Invoke();
                        logger.Trace($"End invoke OnDisconnected delegate in {threadId} thread.");
                    }
                    catch (Exception e)
                    {
                        logger.Error($"OnDisconnected delegate in {threadId} thread thrown exception: {e}.");
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
            foreach (var itemDelegate in HwdgUpdated.GetInvocationList())
            {
                ThreadPool.QueueUserWorkItem(Callback);
                void Callback(Object state)
                {
                    var threadId = Thread.CurrentThread.ManagedThreadId;
                    try
                    {
                        logger.Trace($"Begin invoke OnUpdated delegate in {threadId} thread.");
                        ((HwdgResult)itemDelegate).Invoke(status);
                        logger.Trace($"End invoke OnUpdated delegate in {threadId} thread.");
                    }
                    catch (Exception e)
                    {
                        logger.Error($"OnUpdated delegate in {threadId} thread thrown exception: {e}.");
                        throw;
                    }
                }
            }
        }
    }
}