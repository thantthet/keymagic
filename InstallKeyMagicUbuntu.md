## deb package Released [Click HERE](http://code.google.com/p/keymagic/downloads/list). Try the following only if you cannot install this deb package ##

# Introduction #

About how to install KeyMagic on Ubuntu. You need to have iBus installed on your system.


## Prerequisite ##
  * Run Terminal and enter the following commands:
```
sudo apt-get install libibus-dev autopoint automake
sudo mkdir -p /usr/share/keymagic
mkdir -p ~/.keymagic
```

## Installing ##
  * Download linux source archive from http://code.google.com/p/keymagic/downloads/list
  * Run Terminal and enter the following commands (assuming you have downloaded file into ~/Downloads folder):
```
cd ~/Downloads/
tar -xvf ibus-keymagic-1.0.tar
cd ibus-keymagic
./autogen.sh --prefix=/usr
make
sudo make install
```

## Installing Keyboard Layout ##
  * Download Keyboard Layout (example file: [Download](http://dl.dropbox.com/u/176693/Zawgyi%20L%20-%20Unicode.km2))
  * Copy downloaded file
```
cp "~/Downloads/Zawgyi L - Unicode.km2" "~/.keymagic/Zawgyi L - Unicode.km2"
```
_You can either install into '/usr/shared/keymagic/' for all users_

### Other available keyboard layouts ###
  * [myWin](http://dl.dropbox.com/u/176693/myWin.km2)
  * [Parabaik](http://dl.dropbox.com/u/176693/Parabaik.km2)
  * [Myanmar3](http://dl.dropbox.com/u/176693/Myanmar3.km2)

## Activate Installed Keyboard ##
  * Restart iBus
    * Click on iBus icon at the panel
    * Click Restart
  * Click again on iBus icon and click Preferences
  * Choose 'Input Method' tab
  * Select 'English->Zawgyi L - Unicode (KeyMagic)' from the combo box
  * Click Add
  * Click Close

**IMPORTANT TIPS**:

If you run into error something like  `./autogen.sh: 1: package_name: not found`

Try `sudo apt-get install package_name`

For Example:

Says, you got an error `./autogen.sh: 1: autopoint: not found`

Try `sudo apt-get install autopoint`

This works for most cases.