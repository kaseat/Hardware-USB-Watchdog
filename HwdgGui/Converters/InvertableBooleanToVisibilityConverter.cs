using System;
using System.Globalization;
using System.Windows;
using System.Windows.Data;

namespace HwdgGui.Converters
{
    [ValueConversion(typeof(Boolean), typeof(Visibility))]
    public class InvertableBooleanToVisibilityConverter : IValueConverter
    {
        enum Parameters
        {
            Normal, Inverted
        }

        public Object Convert(Object value, Type targetType,
            Object parameter, CultureInfo culture)
        {
            var boolValue = (Boolean)value;
            var direction = (Parameters)Enum.Parse(typeof(Parameters), (String)parameter);

            if (direction == Parameters.Inverted)
                return !boolValue ? Visibility.Visible : Visibility.Collapsed;

            return boolValue ? Visibility.Visible : Visibility.Collapsed;
        }

        public Object ConvertBack(Object value, Type targetType,
            Object parameter, CultureInfo culture)
        {
            return null;
        }
    }
}