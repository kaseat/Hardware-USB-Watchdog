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
using System.Linq;
using System.Reflection;

namespace HwdgGui.Utils
{
    /// <summary>
    /// Wpf mvvm System.Assembly extensions
    /// </summary>
    public static class WpfAssemblyExtentions
    {
        /// <summary>
        /// Find all ViewModels in assembly.
        /// </summary>
        /// <param name="assembly">Сборка для поиска ViewModel-ей</param>
        /// <returns>Возвращает массив найденных типов ViewModel-ей</returns>
        public static Type[] GetViewModels(this Assembly assembly)
            => assembly.GetTypes().Where(type => !String.IsNullOrWhiteSpace(type.Namespace) &&
                                                 type.Namespace.EndsWith("ViewModels") &&
                                                 type.Name.EndsWith("ViewModel") &&
                                                 type.GetInterface(nameof(System.ComponentModel
                                                     .INotifyPropertyChanged)) != null).ToArray();

        /// <summary>
        /// Find all Views in assembly.
        /// </summary>
        /// <param name="assembly">Сборка для поиска View-х</param>
        /// <returns>Возвращает массив найденных типов View-х</returns>
        public static Type[] GetViews(this Assembly assembly)
            => assembly.GetTypes().Where(type => !String.IsNullOrWhiteSpace(type.Namespace) &&
                                                 type.Namespace.EndsWith("Views") &&
                                                 type.Name.EndsWith("View")).ToArray();
    }
}