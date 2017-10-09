using Caliburn.Micro;
using System;
using System.Windows;
using HwdgGui.ViewModels;

namespace HwdgGui
{
    class Bootstrapper : BootstrapperBase
    {
        public Bootstrapper() => Initialize();

        /// <inheritdoc />
        protected override void OnStartup(Object sender, StartupEventArgs e) => DisplayRootViewFor<ShellViewModel>();
    }
}