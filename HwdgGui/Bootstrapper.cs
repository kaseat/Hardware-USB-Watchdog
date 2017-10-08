using System;
using System.Windows;
using Caliburn.Micro;
using HwdgGui.ViewModels;

namespace HwdgGui
{
    public class Bootstrapper : BootstrapperBase
    {
        public Bootstrapper()
        {
            Initialize();
        }

        protected override void OnStartup(Object sender, StartupEventArgs e)
        {
            DisplayRootViewFor<ShellViewModel>();
        }
    }
}