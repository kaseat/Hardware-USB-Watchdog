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