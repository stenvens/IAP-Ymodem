using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Windows.Forms; //需要加入窗体命名空间

namespace Ymodem_tool
{
    /// <summary>
    /// 重写Combobox类 (为了处理下拉选项过长显示不全) 需要在Form1.Designer.cs下对相关控件进行类替换
    /// </summary>
    public class CustomCombox : ComboBox
    {
        protected override void OnDropDown(EventArgs e)
        {
            base.OnDropDown(e);
            AdjustComboBoxDropDownListWidth();
        }

        private void AdjustComboBoxDropDownListWidth()
        {
            int vertScrollBarWidth = (this.Items.Count > this.MaxDropDownItems) ? SystemInformation.VerticalScrollBarWidth : 0;

            int maxWidth = this.DropDownWidth;
            foreach (var layouts in this.Items)
            {
                int measureTextWidth = TextRenderer.MeasureText(layouts.ToString(), this.Font).Width;
                maxWidth = maxWidth < measureTextWidth ? measureTextWidth : maxWidth;
            }

            this.DropDownWidth = maxWidth + vertScrollBarWidth;
        }
    }
}
