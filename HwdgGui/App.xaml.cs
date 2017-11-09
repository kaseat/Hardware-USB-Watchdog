using System;
using System.Threading;
using System.Windows;

namespace HwdgGui
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
    }

    public static class Startup
    {
        static readonly Mutex Mutex = new Mutex(true, "{EC3DE107-E996-4FBE-9889-C15A23B10242}");
        [STAThread]
        public static void Main()
        {
            if (Mutex.WaitOne(TimeSpan.Zero, true))
            {
                var application = new App();
                application.InitializeComponent();
                application.Run();
                Mutex.ReleaseMutex();
            }
            else
            {
                MessageBox.Show("Приложение уже запущено!");
            }
        }
    }
}
