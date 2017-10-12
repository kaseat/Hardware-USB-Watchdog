using Caliburn.Micro;
using HwdgWrapper;

namespace HwdgGui.ViewModels
{
    public class ShellViewModel : PropertyChangedBase
    {
        private readonly IHwdg hwdg;

        public ShellViewModel(IHwdg hwdg)
        {
            this.hwdg = hwdg;
        }

        public void OnClose()
        {
            hwdg.Stop();
        }
    }
}