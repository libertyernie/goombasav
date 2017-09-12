using Goombasav;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace goombasav_cs {
	public partial class Form1 : Form {
		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main(string[] args) {
			Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);
			Application.Run(new Form1(args.Length > 0 ? args[0] : null));
		}

		const string TITLE = "Goomba Save Manager";

		private GoombaSRAM loaded_sram;
		private List<ExtractedROM> loaded_rom_contents;

		private string filePath;
		private bool dirty;

		public Form1(string filename) {
			InitializeComponent();

			// Update status of Save and Save As items whenever File menu is opened
			fileToolStripMenuItem.DropDownOpening += (o, e) => {
				saveToolStripMenuItem.Enabled = (filePath != null && dirty);
				saveAsToolStripMenuItem.Enabled = (filePath != null);
			};
			this.Closing += (o, e) => {
				e.Cancel = !okToClose();
			};
			listBox1.SelectedIndexChanged += listBox1_SelectedIndexChanged;

			filePath = null;

			this.DragEnter += Form1_DragEnter;
			this.DragDrop += Form1_DragDrop;
			this.AllowDrop = true;

			this.Text = TITLE;
			if (filename != null) {
				try {
					load(filename);
				} catch (FileNotFoundException) {
					MessageBox.Show("File not found: " + filename, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
				}
			}
		}

		private void openToolStripMenuItem_Click(object sender, EventArgs e) {
			OpenFileDialog d = new OpenFileDialog();
			d.Filter = "GBA save data (*.sav)|*.sav|GBA ROMs (*.gba)|*.gba|All files (*.*)|*.*";
			if (d.ShowDialog() == DialogResult.OK) {
				load(d.FileName);
			}
		}

		private void saveToolStripMenuItem_Click(object sender, EventArgs e) {
			save(filePath);
		}

		private void saveAsToolStripMenuItem_Click(object sender, EventArgs e) {
			SaveFileDialog d = new SaveFileDialog();
			d.Filter = "GBA save data (*.sav)|*.sav|GBA ROMs (*.gba)|*.gba|All files (*.*)|*.*";
			d.AddExtension = true;
			if (d.ShowDialog() == DialogResult.OK) {
				save(d.FileName);
			}
		}

		private void exitToolStripMenuItem_Click(object sender, EventArgs e) {
			this.Close();
		}

		private void aboutToolStripMenuItem_Click(object sender, EventArgs e) {
            using (AboutForm a = new AboutForm() {
                Icon = Icon
            }) {
                a.ShowDialog(this);
            }
		}

		private void btnReplace_Click(object sender, EventArgs e) {
			OpenFileDialog d = new OpenFileDialog();
			d.Title = btnReplace.Text;
			d.Filter = "Raw save data (*.sav, *.srm)|*.sav;*.srm|All files (*.*)|*.*";
			if (d.ShowDialog() == DialogResult.OK) {
				replace(d.FileName);
			}
		}

		private void btnExtract_Click(object sender, EventArgs e) {
			object h = listBox1.SelectedItem;
			if (h is Stateheader) {
				Stateheader sh = (Stateheader)h;
				byte[] data;
				try {
					data = loaded_sram.Extract(sh);
				} catch (GoombaException ex) {
					MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
					return;
				}
				SaveFileDialog d = new SaveFileDialog();
				d.Title = btnExtract.Text;
				d.Filter = "Raw save data (*.sav, *.srm)|*.sav;*.srm|All files (*.*)|*.*";
				d.AddExtension = true;
				if (d.ShowDialog() == DialogResult.OK) {
					File.WriteAllBytes(d.FileName, data);
				}
			} else if (h is ExtractedROM) {
				ExtractedROM r = (ExtractedROM)h;
				SaveFileDialog d = new SaveFileDialog();
				d.Title = btnExtract.Text;
				d.Filter =
					h is GameBoyROM ? "Game Boy ROMs (*.gb, *.gbc)|*.gb;*.gbc|All files (*.*)|*.*"
					: h is PocketNESROM ? "NES/Famicom ROMs (*.nes)|*.nes|All files (*.*)|*.*"
					: h is SMSAdvanceROM ? "Master System/Game Gear ROMs (*.sms, *.gg)|*.sms;*.gg|All files (*.*)|*.*"
					: "All files (*.*)|*.*";
				d.FileName = filePath == null || loaded_rom_contents.Count > 1
					? r.Name
					: Path.GetFileNameWithoutExtension(filePath);
				d.AddExtension = true;
				if (d.ShowDialog() == DialogResult.OK) {
					File.WriteAllBytes(d.FileName, r.Data);
				}
			} else {
				MessageBox.Show("Cannot export this type of data.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
			}
        }

        private void btnDelete_Click(object sender, EventArgs e) {
            Stateheader sh = listBox1.SelectedItem as Stateheader;
            if (sh == null) return;

            string msg = $"Are you sure you want to remove the save data for {sh.Title} from this file? You will need to run Goomba if you want to add new save data later.";
            if (MessageBox.Show(this, msg, Text, MessageBoxButtons.YesNo) == DialogResult.Yes) {
                try {
                    GoombaSRAM new_data = loaded_sram.CopyAndRemove(sh);
                    loaded_sram = new_data;
                    dirty = true;

                    int sel = listBox1.SelectedIndex;
                    headerScan();
                    if (listBox1.Items.Count > sel) listBox1.SelectedIndex = sel;
                } catch (GoombaException ex) {
                    MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }

        private void listBox1_SelectedIndexChanged(object sender, EventArgs e) {
			object o = listBox1.SelectedItem;
			if (o is ExtractedROM) {
				ExtractedROM r = (ExtractedROM)o;
				lblSizeVal.Text = r.Data.Length + " bytes";
				lblTypeVal.Text = "ROM image";
				flpConfigdata.Visible = flpStateheader.Visible = panel1.Visible = false;
				lblChecksumVal.Text = r.GetChecksum().ToString("X8");
				lblTitleVal.Text = r.Name;
				btnExtract.Enabled = true;
				btnReplace.Enabled = false;
                btnDelete.Enabled = false;
                return;
			}

			GoombaHeader h = (GoombaHeader)o;
			lblSizeVal.Text = h.Size + " bytes";
			lblTypeVal.Text = h.Type == GoombaHeader.STATESAVE ? "Savestate"
				: h.Type == GoombaHeader.SRAMSAVE ? "SRAM"
				: h.Type == GoombaHeader.CONFIGSAVE ? "Config"
				: "Unknown";
			if (h is Stateheader) {
				Stateheader sh = (Stateheader)h;
				flpConfigdata.Visible = false;
				flpStateheader.Visible = true;
				lblUncompressedSize.Text =
					sh.DataSize >= sh.Size
					? "Uncompressed size:"
					: "Compressed size:";
				lblUncompressedSizeVal.Text = sh.DataSize + " bytes";
				lblFramecountVal.Text = sh.Framecount.ToString();
				lblChecksumVal.Text = sh.ROMChecksum.ToString("X8");

				panel1.Visible = true;
				uint hash = sh.CompressedDataChecksum(3);
				lblHashVal.Text = hash.ToString("X6");
				hashBox.BackColor = Color.FromArgb((int)(hash | 0xFF000000));

				btnExtract.Enabled = btnReplace.Enabled = (sh.Type == GoombaHeader.SRAMSAVE);
			} else if (h is Configdata) {
				flpStateheader.Visible = false;

				Configdata cd = (Configdata)h;
				if (h is GoombaConfigdata) {
					flpConfigdata.Visible = true;
					GoombaConfigdata gcd = (GoombaConfigdata)h;
					lblBorderVal.Text = gcd.BorderColor.ToString();
					lblPaletteVal.Text = gcd.PaletteBank.ToString();
					MiscStrings strs = gcd.GetMiscStrings;
					lblSleepVal.Text = strs.SleepStr;
					lblAutostateVal.Text = strs.AutoloadStateStr;
					lblGammaVal.Text = strs.GammaStr;
				} else {
					flpConfigdata.Visible = false;
					lblBorderVal.Text = "";
					lblPaletteVal.Text = "";
					lblSleepVal.Text = "";
					lblAutostateVal.Text = "";
					lblGammaVal.Text = "";
				}
				lblChecksumVal.Text = cd.ROMChecksum.ToString("X8"); // The SRAM with this ROM checksum value is currently in 0xe000-0xffff

				panel1.Visible = false;
				btnExtract.Enabled = btnReplace.Enabled = false;
			} else {
				flpConfigdata.Visible = flpStateheader.Visible = panel1.Visible = false;
				btnExtract.Enabled = btnReplace.Enabled = false;
			}
			lblTitleVal.Text = h.Title;
		}

		private void Form1_DragEnter(object sender, DragEventArgs e) {
			e.Effect = DragDropEffects.None;
			if (e.Data.GetDataPresent(DataFormats.FileDrop)) {
				string[] arr = (string[])e.Data.GetData(DataFormats.FileDrop);
				if (arr.Length == 1) {
					string pathname = arr[0];
					if (new[] { ".sav", ".srm" }.Any(s => pathname.EndsWith(s, StringComparison.InvariantCultureIgnoreCase))) {
						e.Effect = DragDropEffects.Link;
					}
				}
			}
		}

		private void Form1_DragDrop(object sender, DragEventArgs e) {
			string[] arr = (string[])e.Data.GetData(DataFormats.FileDrop);
			int first = 0;
			using (FileStream stream = new FileStream(arr[0], FileMode.Open, FileAccess.Read)) {
				for (int i=0; i<4; i++) {
					first += stream.ReadByte() << 8 * i;
				}
			}
			if ((uint)first == GoombaHeader.STATEID) {
				// try open file
				load(arr[0]);
			} else {
				// try replace
				replace(arr[0]);
			}
		}

		private void replace(string filename) {
			byte[] gbc_data_arr = File.ReadAllBytes(filename);
			object h = listBox1.SelectedItem;
			if (h == null) {
				MessageBox.Show("No item is selected.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
				return;
			} else if (h is Stateheader) {
				try {
					GoombaSRAM new_data = loaded_sram.CopyAndReplace((Stateheader)h, gbc_data_arr);
					loaded_sram = new_data;
					dirty = true;

					int sel = listBox1.SelectedIndex;
					headerScan();
					if (listBox1.Items.Count > sel) listBox1.SelectedIndex = sel;
				} catch (GoombaException e) {
					MessageBox.Show(e.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
				}
			} else {
				MessageBox.Show("You cannot replace data for this type of header.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
			}
		}

		private void resetDescriptionPanel() {
			btnExtract.Enabled = false;
			btnReplace.Enabled = false;
			lblSizeVal.Text = "";
			lblTypeVal.Text = "";
			flpConfigdata.Visible = false;
			flpStateheader.Visible = true;
			lblUncompressedSize.Text = "Uncompressed size:";
			lblUncompressedSizeVal.Text = "";
			lblFramecountVal.Text = "";
			lblChecksumVal.Text = "";
			lblTitleVal.Text = "";
			panel1.Visible = false;
		}

		private bool okToClose() {
			if (filePath != null && dirty) {
				DialogResult dr = MessageBox.Show("Save changes to " + Path.GetFileName(filePath) + "?",
					TITLE, MessageBoxButtons.YesNoCancel);
				if (dr == DialogResult.Yes) {
					save(filePath);
				} else if (dr == DialogResult.Cancel) {
					return false;
				}
			}
			return true;
		}

		private void load(string filename) {
			if (!okToClose()) return;

			try {
				byte[] arr = File.ReadAllBytes(filename);

				var extractedRoms1 = GameBoyROM.Extract(arr);
				var extractedRoms2 = PocketNESROM.Extract(arr);
				var extractedRoms3 = SMSAdvanceROM.Extract(arr);
				if (extractedRoms1.Any() || extractedRoms2.Any() || extractedRoms3.Any()) {
					loaded_sram = null;
					loaded_rom_contents = new List<ExtractedROM>();
					loaded_rom_contents.AddRange(extractedRoms1);
					loaded_rom_contents.AddRange(extractedRoms2);
					loaded_rom_contents.AddRange(extractedRoms3);
				} else {
					if (arr.Length > GoombaSRAM.ExpectedSize) {
						MessageBox.Show("This file is more than " + GoombaSRAM.ExpectedSize +
							" bytes. If you overwrite the file, the last " + (arr.Length - GoombaSRAM.ExpectedSize) +
							" bytes will be discarded.", "Note", MessageBoxButtons.OK, MessageBoxIcon.Information);
					}
					loaded_sram = new GoombaSRAM(arr, true);
					loaded_rom_contents = null;
					dirty = false;
				}

				filePath = filename;
				this.Text = (filename == null)
					? TITLE
					: TITLE + " - " + Path.GetFileName(filename);

				headerScan();
			} catch (GoombaException e) {
				MessageBox.Show(e.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
				return;
			}
		}

		private void save(string path) {
			byte[] arr = loaded_sram.ToArray();
			File.WriteAllBytes(path, arr);

			filePath = path;
			dirty = false;
			this.Text = TITLE + " - " + Path.GetFileName(path);
		}

		private void headerScan() {
			listBox1.Items.Clear();
			resetDescriptionPanel();
			if (loaded_sram != null) {
				listBox1.Items.AddRange(loaded_sram.Headers.ToArray());
				if (loaded_sram.Headers.Count == 0) {
					MessageBox.Show("No headers were found in this file. It may not be valid SRAM data", "Note",
						MessageBoxButtons.OK,
						MessageBoxIcon.Information);
				}
			}
			if (loaded_rom_contents != null) {
				listBox1.Items.AddRange(loaded_rom_contents.ToArray());
				if (loaded_rom_contents.Count == 0) {
					MessageBox.Show("No Game Boy, NES, or Sega ROMs were not found in this file. It may not be a valid emulator ROM.", "Note",
						MessageBoxButtons.OK,
						MessageBoxIcon.Information);
				}
			}
		}
    }
}
