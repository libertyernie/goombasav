using System;
using System.Diagnostics;
using System.Windows.Forms;

namespace goombasav_cs {
	public partial class AboutForm : Form {
		public AboutForm() {
			InitializeComponent();
		}

		private void button1_Click(object sender, EventArgs e) {
			Process.Start("cmd", "/C start http://www.gnu.org/licenses/gpl-3.0.html");
		}

		private void button2_Click(object sender, EventArgs e) {
			Process.Start("cmd", "/C start https://github.com/libertyernie/goombasav");
		}
	}
}
