#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <map>

using namespace std;

/*===============================define and object==================================*/
class BTB
{
public:
	int BC[8];
	int mistaken;
	BTB()
		:mistaken(0)
	{
		for (int i = 0; i < 8; i++)
		{
			BC[i] = 0;
		}
	}
	void addBC(int number)
	{
		if (BC[number] != 3)
			BC[number]++;
	}
	void subBC(int number)
	{
		if (BC[number] != 0)
			BC[number]--;
	}
};

class instruction
{
public:
	int address;//0~
	int type;//addi = 1 or beq = 2
	int entry;//0 or 1 or ...

	int rd1, rd2, imi;
	string label;
	instruction(int address, string typeString, int entry)
		:address(address), entry(entry)
	{
		if (typeString == "addi")
			type = 1;
		else if (typeString == "beq")
			type = 2;
	}
};

#define Historybit 3
int Entry = 2;//default
vector<BTB> BTBs;//depend on your entry
vector<instruction> instructions;//depend on your code
vector<string> historys;
map<string, int> jump;
vector<int> registers;
string arr[4] = { "SN","WN","WT","ST" };

/*===============================program==================================*/
int changeHistory(string bit)
{
	int number = 0;
	number += (bit[0] - '0') * 4;
	number += (bit[1] - '0') * 2;
	number += (bit[2] - '0') * 1;
	return number;
}
void update(int address, bool taken)
{
	if (taken)
		BTBs[address % Entry].addBC(changeHistory(historys[address % Entry]));
	else
		BTBs[address % Entry].subBC(changeHistory(historys[address % Entry]));

	historys[address % Entry][0] = historys[address % Entry][1];
	historys[address % Entry][1] = historys[address % Entry][2];
	if (taken)
		historys[address % Entry][2] = '1';
	else
		historys[address % Entry][2] = '0';

}
void outcome(int address, bool taken)
{
	cout << endl;
	cout << "Entry: " << address % Entry << "        ";
	if (instructions[address].type == 1)
		cout << "addi r";
	else
		cout << "beq r";
	cout << instructions[address].rd1 << " r" << instructions[address].rd2 << " ";

	if (instructions[address].type == 1)
		cout << instructions[address].imi;
	else
		cout << instructions[address].label;
	cout << endl;

	for (int i = 0; i < Entry; i++)
	{
		cout << historys[i];
		for (int j = 0; j < 8; j++)
		{
			cout << " " << arr[BTBs[i].BC[j]];
		}
		cout << endl;
	}

	cout << "predict : ";
	if (BTBs[address % Entry].BC[changeHistory(historys[address % Entry])] < 2)
	{
		cout << "N   ";
		cout << "actual : ";
		if (taken)
		{
			cout << "T ";
			BTBs[address % Entry].mistaken++;
			update(address, true);
		}
		else
		{
			cout << "N ";
			update(address, false);
		}
	}
	else
	{
		cout << "T   ";
		cout << "actual : ";
		if (taken)
		{
			cout << "T ";
			update(address, true);
		}
		else
		{
			cout << "N ";
			BTBs[address % Entry].mistaken++;
			update(address, false);
		}

	}
	cout << "     mistaken : " << BTBs[address % Entry].mistaken << endl;

}

void checkEntry()
{
	cout << "Please input entry number:";
	cin >> Entry;
	cin.get();
	if (Entry <= 0)
	{
		cout << "Your entry number had wrong." << endl;
		exit(1);
	}
	int testEntry = Entry;
	while (testEntry != 1)
	{
		if (testEntry % 2 != 0)
		{
			cout << "Your entry number had wrong." << endl;
			exit(1);
		}
		testEntry = testEntry / 2;
	}
}
void inputString()
{
	int address = 0;
	string str;
	cout << "Plese input your String:(end with EOF or ctrl+z)" << endl;
	while (getline(cin, str))
	{
		int pos = str.find_first_of(':');
		if (pos != -1)//LabelCase
		{
			string label = str.substr(0, pos);
			jump[label] = address;
			continue;
		}
		int pos1 = str.find_first_of(' ');
		int pos2 = str.find_first_of(',', pos1 + 1);
		int pos3 = str.find_first_of(',', pos2 + 1);

		string typeString = str.substr(0, pos1);
		int rd1 = stoi(str.substr(pos1 + 2, pos2 - pos1 - 3));
		int rd2 = stoi(str.substr(pos2 + 2, pos3 - pos2 - 3));

		instruction inst(address, typeString, address % Entry);
		inst.rd1 = rd1;
		inst.rd2 = rd2;
		if (typeString == "addi")
		{
			int imi = stoi(str.substr(pos3 + 1, -1));
			inst.imi = imi;
		}
		else if (typeString == "beq")
		{
			string label = str.substr(pos3 + 1, -1);
			inst.label = label;
		}
		instructions.push_back(inst);
		address++;
	}
}
void run()
{
	BTBs.resize(Entry);
	historys.resize(Entry);
	for (int i = 0; i < Entry; i++)
		historys[i] = "000";

	registers.resize(9);
	for (int i = 0; i < 9; i++)
		registers[i] = 0;

	for (int i = 0; i < instructions.size(); i++)
	{

		if (instructions[i].type == 1)//run addi
		{
			outcome(i, false);
			registers[instructions[i].rd1] = registers[instructions[i].rd2] + instructions[i].imi;
		}
		else if (instructions[i].type == 2)//run beq and jump
		{
			if (registers[instructions[i].rd1] == registers[instructions[i].rd2])
			{
				outcome(i, true);
				i = jump[instructions[i].label] - 1;
			}
			else
			{
				outcome(i, false);
			}
		}
	}
}
int main()
{
	checkEntry();
	inputString();
	run();

	return 0;
}

