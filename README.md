# Nexmon for ARC
The nexmon framework adapted for the ARC architecture, tested with Ubuntu 16.04.03

## Getting Started
* Installing dependencies (mostly for the ARC toolchain):

```bash
sudo apt-get install texinfo byacc flex libncurses5-dev zlib1g-dev libexpat1-dev texlive build-essential git wget bison gawk libgmp3-dev
```
* Run `make` in the root directory, this will download and compile the ARC toolchain. **This needs to be done only once!**
* Download the original FW file from [here](https://git.kernel.org/pub/scm/linux/kernel/git/firmware/linux-firmware.git/plain/wil6210.fw) and place it in this directory: `firmwares/wil6210/4-1-0_55/`
* Setup the build environment for Nexmon:

```bash
source setup_env.sh
```
* Go to the hello world example in the patches directory and execte `make`. This will build a patched firmware which prints a string at the initialization time of the fw and uc code. 

```bash
cd patches/wil6210/4-1-0_55/hello_world
make
```
* Copy the resulting `wil6210.fw` to your router (the default place in the filesystem is `/lib/firmware/wil6210.fw`)
* Restart the interface:

```bash
ifconfig wlan2 down && ifconfig wlan2 up
```
* You should be able to get the following reults by dumping the `console_dump_fw` and `console_dump_uc` in the debugfs:

```bash
root@TALON1:~# cat /sys/kernel/debug/ieee80211/phy2/wil6210/console_dump_fw 
FW: INITIALIZED
root@TALON1:~# cat /sys/kernel/debug/ieee80211/phy2/wil6210/console_dump_uc 
UC: INITIALIZED
```

## DISCLAIMER
This software might damage your hardware and may void your hardware’s warranty. You use our tools at your risk and responsibility.

You acknowledge that you are solely responsible for how you use the software, & for complying with all relevant laws in your area. 
You also acknowledge that neither of the developers of nexmon-arc nor any of its affiliates & associates may be held liable, 
responsible or accountable for any type of damage, litigation or other legal action, which may arise either from your legal or 
illegal use of nexmon-arc, or any other software. We do not tolerate the use of our software for any illegal purpose. 
By using our software in any way, you acknowledge & approve to use it exclusively in a lawful manner.


## Related projects
* [LEDE](https://lede-project.org): The “Linux Embedded Development Environment” is a Linux operating system based on OpenWrt.
* [Nexmon](https://nexmon.org): The C-based Firmware Patching Framework for Broadcom/Cypress WiFi Chips.

## Our papers using Talon Tools
* Daniel Steinmetzer, Daniel Wegemer, Matthias Schulz, Joerg Widmer, Matthias Hollick. 
  **Compressive Millimeter-Wave Sector Selection in Off-the-Shelf IEEE 802.11ad Devices**.
  Accepted for publication in *Proceedings of the 13th International Conference on emerging Networking EXperiments and Technologies (CoNEXT 2017)*, December 2017, Seoul/Incheon, South Korea.
* Daniel Steinmetzer, Adrian Loch, Amanda García-García, Joerg Widmer, Matthias Hollick. 
  **Mitigating Lateral Interference: Adaptive Beam Switching for Robust Millimeter-Wave Networks**.
  *1st ACM Workshop on Millimeter Wave Networks and Sensing Systems (mmNets 2017)*, October 2017, Snowbird, Utah, USA.

  [Get references as bibtex file](talon-tools.bib)

## Reference our project
Any use of the Software which results in an academic publication or other publication which includes a bibliography must include citations to the Talon Tools project and probably one of our papers depending on the code you use. Find all references in our [bibtex file](talon-tools.bib). The project should be cited as follows:

```
@electronic{talon-tools:project,
	author = {Steinmetzer, Daniel and Wegemer, Daniel and Hollick, Matthias},
	title = {Talon Tools: The Framework for Practical IEEE 802.11ad Research},
	url = {https://seemoo.de/talon-tools/},
	year = {2017}
}
```

## Give us Feedback
We want to learn how people use our platform and what aspects we might improve. Please report any issues or comments using the bug-tracker and do not hesitate to approach us via e-mail.

## Statistics

### What kind of statistics do you collect?

Everytime you run a Nexmon firmware build, we collect the following information:
* A unique identifier based on a random number (e.g., 5O31UY9Z5IEX3O9KL680V5IHNASIE1SB)
* The name, release, machine and processor of your build system (`uname -srmp`, e.g., `Linux 4.2.0-42-generic x86_64 x86_64`)
* Git internal path to the built project (e.g., `patches/bcm4339/6_37_34_43/nexmon/`)
* Git version (e.g., `2.2.1-55-g3684a80c-dirty`)
* Git repository URL (e.g., `git@github.com:seemoo-lab/wisec2017_nexmon_jammer.git`)

### Why do you collect statistics?

Nexmon is mainly intended as a research project that we share with the community so that others can benefit from our tools.
We want to collect statistics to figure out how often Nexmon is used in general and which platform and firmware version is the most popular.
We also intent to share our findings in the future.

### How do I disable the collection of statistics?

If you have privacy concerns, we also offer to opt-out of the statistic collections. To this end, you simply have to create a `DISABLE_STATISTICS` file in your Nexmon root directory.

## Contact
* [Daniel Steinmetzer](https://seemoo.tu-darmstadt.de/dsteinmetzer) <<dsteinmetzer@seemoo.tu-darmstadt.de>>
* Daniel Wegemer <<dwegemer@seemoo.tu-darmstadt.de>>

## Powered By
<a href="https://www.seemoo.tu-darmstadt.de">![SEEMOO logo](logos/seemoo.png)</a> &nbsp;
<a href="https://www.nicer.tu-darmstadt.de">![NICER logo](logos/nicer.png)</a> &nbsp;
<a href="https://www.crossing.tu-darmstadt.de">![CROSSING logo](logos/crossing.jpg)</a>&nbsp;
<a href="https://www.crisp-da.de">![CRSIP logo](logos/crisp.jpg)</a>&nbsp;
<a href="http://www.maki.tu-darmstadt.de/">![MAKI logo](logos/maki.png)</a> &nbsp;
<a href="https://www.cysec.tu-darmstadt.de">![CYSEC logo](logos/cysec.jpg)</a>&nbsp;
<a href="https://www.tu-darmstadt.de/index.en.jsp">![TU Darmstadt logo](logos/tudarmstadt.png)</a>&nbsp;
