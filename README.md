# disconnall
Do you ever just want to cause chaos with everyone around you? Do you hate everyone specificially in your area? Do you have too much time? Well this is the program for you. Introducing **disconnall**! The program that just disconnects everyone from the WiFi! It uses intelligent AI (if statements) to find the right router thats connected to the right client, to be extra efficient at disconnecting. Now you can have your personal internet in peace (as long as it's through ethernet).

# installation
Get aircrack-ng, compile this program and you're done. 
How you get aircrack-ng you ask? I only tried this in a debian based distro (ubuntu, i know, i know), so I just did a simple `sudo apt-get install aircrack-ng`. 
Windows? There are precompiled binaries that you can use, but I have no idea how to! 

# usage
in linux: `sudo ./disconnall {write file} {wifi adapter} [rounds] [deauth packets]` (braces {} are required, brackets [] are not).
You can find your wifi adapter with `iwconfig` or `ifconfig`.
Before you run it, do these 2 commands: 
`sudo iwconfig {wifi adapter} mode monitor` (make sure your wifi is off before) and 
`sudo rfkill unblock all` (if you need to). 
Then run disconnall away and have fun with the results.

in windows? idfk, the program does support windows but i have no idea how to use aircrack-ng in windows so good luck

after airodump-ng has collected enough data that you are pleased with, just press `ctrl + c` to go on to the next step and start disconnecting!
