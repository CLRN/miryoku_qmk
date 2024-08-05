* connect left half and TRSS
* do that `qmk flash -c -kb crkbd -km manna-harbour_miryoku -e MIRYOKU_ALPHAS=QWERTY`
* then reset left

if this doesn't work, reflash with QMK Toolbox and default:
* Detach keyboard from PC (USB cable)
* Detach keyboard splits (TRSS cable)
* Open QMK Toolbox
* Click Open local file
* Select firmware_name.hex file
* Check Auto-flash option
* Attach PC to left split only
* Double press keyboard reset button
* Wait until completion and device disconnected message is displayed
* Detach left split from PC
* Attach PC to right split only
* Double press keyboard reset button
* Wait until completion and device disconnected message is displayed
* Detach right split from PC
* Attach PC to left split only (USB cable)
* Attach keyboard splits (TRSS cable)
