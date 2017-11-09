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
using System.Collections.Generic;
using System.Reflection;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using Caliburn.Micro;
using Autofac;
using FirstFloor.ModernUI;
using FirstFloor.ModernUI.Windows;
using HwdgGui.Utils;
using HwdgGui.ViewModels;
using HwdgWrapper;

namespace HwdgGui
{
    sealed class Bootstrapper : BootstrapperBase, IContentLoader
    {
        private readonly IContainer container;

        public Bootstrapper()
        {
            var builder = new ContainerBuilder();
            var assembly = Assembly.GetExecutingAssembly();
            builder.RegisterTypes(assembly.GetViewModels());
            builder.RegisterTypes(assembly.GetViews());
            builder.Register<IHwdg>(x => new SerialHwdg(new SerialWrapper())).SingleInstance();
            builder.Register<ISettingsProvider>(x => new RegistrySettingsProvider()).SingleInstance();
            builder.Register<IWindowManager>(x => new WindowManager()).InstancePerLifetimeScope();
            builder.Register<IEventAggregator>(x => new EventAggregator()).InstancePerLifetimeScope();
            container = builder.Build();
            Initialize();
        }

        /// <inheritdoc />
        protected override void BuildUp(Object instance) => container.InjectProperties(instance);

        /// <inheritdoc />
        protected override IEnumerable<Object> GetAllInstances(Type service) =>
            container.Resolve(typeof(IEnumerable<>).MakeGenericType(service)) as IEnumerable<Object>;

        /// <inheritdoc />
        protected override Object GetInstance(Type service, String key)
        {
            if (String.IsNullOrWhiteSpace(key))
                if (container.TryResolve(service, out var ob)) return ob;
                else if (container.TryResolveNamed(key, service, out var obj)) return obj;
            throw new InvalidOperationException($"Could not locate any instances of service {key ?? service.Name}.");
        }

        /// <inheritdoc />
        protected override void OnStartup(Object sender, StartupEventArgs e) => DisplayRootViewFor<ShellViewModel>();

        /// <inheritdoc />
        public Task<Object> LoadContentAsync(Uri uri, CancellationToken cancellationToken)
        {
            if (!Application.Current.Dispatcher.CheckAccess()) throw new InvalidOperationException();
            return Task.Factory.StartNew(() => LoadContent(uri), cancellationToken, TaskCreationOptions.None,
                TaskScheduler.FromCurrentSynchronizationContext());
        }

        /// <summary>
        /// Loads content from specified uri.
        /// </summary>
        /// <param name="uri">Content uri.</param>
        /// <returns>Returns loaded content.</returns>
        private static Object LoadContent(Uri uri)
        {
            if (ModernUIHelper.IsInDesignMode) return null;
            var content = Application.LoadComponent(uri);
            var vm = ViewModelLocator.LocateForView(content);
            if (vm == null) return content;
            if (content is DependencyObject) ViewModelBinder.Bind(vm, content as DependencyObject, null);
            return content;
        }
    }
}