using System.Windows;

namespace HwdgGui.Views
{
    /// <summary>
    /// Interaction logic for ShellView.xaml
    /// </summary>
    public partial class ShellView : Window
    {
        public ShellView()
        {
            InitializeComponent();
        }

        private void AutoStart_TargetUpdated(object sender, System.Windows.Data.DataTransferEventArgs e)
        {
            MessageBox.Show("sd");
        }
    }
}
