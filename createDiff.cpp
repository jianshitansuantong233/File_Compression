#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include <algorithm>
#include <iterator>
#include <cassert>
#include<vector>
#include<functional>
using namespace std;
bool re(string old, string diff, ostream& fnew) {//a recursive function used to apply the diff file
	if (diff.size() == 0) {
		return true;
	}
	if (diff[0] == '\r' || diff[0] == '\n') {
		re(old, diff.substr(1), fnew);
	}
	if (diff[0] != 'A'&&diff[0] != 'C') {
		cerr << 1;
		return false;
	}
	else if (diff[0] == 'A') {
		string len = diff.substr(1, diff.find(":") - 1);//assume the file is in correct format
		for (int i = 0; i != len.size(); i++) {//len is the length of what should be added, in string form
			if (!isdigit(len[i])) {
				cerr << 2;
				return false;
			}
		}
		if (stoi(len) + 1 + diff.find(":") > diff.size()) {//if the length of added item is out of bounds

			cerr << 6;
			return false;
		}
		string g = diff.substr(diff.find(":") + 1, stoi(len));
		//cout << stoi(len)<<endl;
		for (int i = 0; i != stoi(len); i++) {
			fnew << g[i];
		}
		//fnew << diff.substr(diff.find(":") + 1, stoi(len));
		return re(old, diff.substr(diff.find(":") + stoi(len) + 1), fnew);
	}
	else {
		string len = diff.substr(1, diff.find(",") - 1);//the length of what should be copied, in string format
		for (int i = 0; i != len.size(); i++) {
			if (!isdigit(len[i])) {
				cerr << 3;
				return false;
			}
		}
		int p1 = stoi(len);
		int p = 0;
		int st = 0;
		int j = 0;
		for (j = diff.find(",") + 1; j != diff.size(); j++) {//once we find the start position, break;
			if (!isdigit(diff[j])) {
				p = j;
				st = j;
				break;
			}
		}
		if (p == diff.find(",") + 1) {//if we cannot find the start position
			cerr << 4;
			return false;
		}
		else if (p == 0) {//if the start position is at the end of diff file
			p = diff.size() - 1;
		}
		p = stoi(diff.substr(diff.find(",") + 1, p - diff.find(",")));
		if (p1 + p > old.size()) {//if what should be copied is longer than the original file
			cerr << old.size() << " ";
			cerr << p + p1;
			cerr << 5;
			return false;
		}
		else {
			fnew << old.substr(p, p1);
		}
		if (j == diff.size()) {
			return true;
		}
		return re(old, diff.substr(st), fnew);//recursively apply the diff file
	}
}
bool applyDiff(istream& fold, istream& fdiff, ostream& fnew) {
	if (!fold || !fdiff) {
		cerr << "Cannot open" << endl;
		return false;
	}
	if (!fnew) {
		cerr << "cannot create" << endl;
		return false;
	}
	string old;
	string diff;
	char temp;
	while (fold.get( temp)) {
		old += temp;
	}
	temp = ' ';
	while (fdiff.get( temp)) {
		diff += temp;
	}
	if (diff.size() == 0) {
		fnew << old;
		return true;
	}
	return re(old, diff, fnew);
}







const int L_OF_STRING = 16;
const int LENGTH = 51;
struct Node {
	vector<int> start_pos;
	string da;
};
class table {
public:
	table() {
		for (int i = 0; i != LENGTH; i++) {
			tt[i] = new vector<Node*>;
		}
	}
	void insert(string s,int a) {//insert a string into the hash table
		while (true) {
			if (s.size() <= L_OF_STRING) {
				hash<string> h;
				unsigned int va = h(s);//hash the string
				int counter = 0;
				for (; counter != tt[va % LENGTH]->size(); counter++) {
					if (tt[va % LENGTH]->at(counter)->da == s) {// if the string is already in the table
						tt[va % LENGTH]->at(counter)->start_pos.push_back(a);
						break;
					}
				}
				if (counter == tt[va%LENGTH]->size()) {// if the string cannot be found in the tablw
					Node* p = new Node;
					tt[va % LENGTH]->push_back(p);
					p->da = s;
					p->start_pos.push_back(a);
				}		
				break;
			}else {//the part below is exactly the same as the above part
				hash<string> h;
				string temp = s.substr(0, L_OF_STRING);
				unsigned int va = h(temp);
				int pp = va % LENGTH;
				if (tt[pp]->size() == 0) {
					Node* p = new Node;
					tt[pp]->push_back(p);
					p->da = temp;
					p->start_pos.push_back(a);
					s = s.substr(1);
					a++;
				}else {
					int counter = 0;
					for (;counter != tt[pp]->size(); counter++) {
						if (tt[pp]->at(counter)->da == temp) {
							tt[pp]->at(counter)->start_pos.push_back(a);
							s = s.substr(1);
							a++;
							break;
						}
					}
					if (counter == tt[pp]->size()) {
						Node* p = new Node;
						tt[pp]->push_back(p);	
						p->da = temp;
						p->start_pos.push_back(a);
					}
					s=s.substr(1);
					a++;
				}
			}
		}
	}
	Node* find(string s) {//return the Node where the string is stored
		hash<string> h;
		unsigned int va = h(s);
		for (int i = 0; i != tt[va % LENGTH]->size(); i++) {
			if (tt[va % LENGTH]->at(i)->da == s) {
				return tt[va % LENGTH]->at(i);
			}
		}
		return nullptr;
	}
	~table() {
		for (int i = 0; i != LENGTH; i++) {
			if (tt[i]->size() != 0) {
				for (int j = 0; j != tt[i]->size(); j++) {
					delete tt[i]->at(j);
				}
			}
			delete tt[i];
		}
	}
private:
	vector<Node*>* tt[LENGTH];
};
inline
string fff(string fn,int start,string ff) {//return the substring of fn after start, which ff also has from position 0
	int counter = 0;
	for (int i = start; i != fn.size(); i++) {
		
		if (counter == ff.size()) {
			return ff;
		}
		if (ff[counter] != fn[i]) {
			return ff.substr(0, counter);
		}
		else {
			counter++;
		}
	}
	return ff.substr(0,counter);
}
inline
string ffff(string fn, int start, string ff) {//return the substring of fn before start, which ff also has from the end
	int counter =ff.size()-1;
	for (int i = start-1; i > 0; i--) {
		if (counter == -1) {
			return ff;
		}
		if (ff[counter] != fn[i]) {
			return ff.substr(counter + 1);
		}
		else {
			counter--;
		}
	}
	return ff.substr(0, ff.size()-1-counter);
}

void createDiff(istream& fold, istream& fnew, ostream& fdiff) {
	if (!fold || !fnew) {
		cerr << "Cannot open" << endl;
		return;
	}
	if (!fdiff) {
		cerr << "cannot create" << endl;
	}
	string old;
	string fn;
	char temp;
	while (fold.get(temp)) {
		old += temp;
	}
	temp = ' ';
	while (fnew.get( temp)) {
		fn += temp;
	}
	table t;
	
	t.insert(old,0);
	if (fn.size() < L_OF_STRING) {
		Node* h=t.find(fn);
		if (h == nullptr) {
			fdiff << "A" << fn.size()<<":"<<fn;
		}
		else {
			fdiff << "C" << fn.size() << "," << h->start_pos.front();
		}
	}else {
		int* c = new int[2];
		int* p = new int[2];
		vector<char> re;
		while (fn.size()>=L_OF_STRING) {
			Node* g=t.find(fn.substr(0, L_OF_STRING));
			if (g == nullptr) {//if this part of string cannot be found, move to position one, store the first character
				re.push_back(fn[0]);//and do again
				fn = fn.substr(1);
			}else {//if this part of string can be found
				string te;
				for (int i = 0; i != re.size(); i++) {
					te += re[i];
				}
				while (!re.empty()) {
					re.pop_back();
				}//te is what cannot be found in the old file
				fn = fn.substr(L_OF_STRING);
				c[0] = 0;
				p[0] = 0;
				c[1] = 0;
				p[1] = -100;
				for (int i = 0; i != g->start_pos.size(); i++) {
					c[0] = fff(old, g->start_pos[i]+L_OF_STRING, fn).size();
					c[1] = g->start_pos[i];
					if (c[0] > p[0]) {
						p[0] = c[0];
						p[1] = c[1];
					}
				}//at this point, p[0] is the length of matching string after the matching pair, p[1]is the start position
				if (p[1] == -100) {
					if (te.size() != 0) {
						for (int i = 0; i != g->start_pos.size(); i++) {
							c[0] = ffff(old, g->start_pos[i], te).size();
							c[1] = g->start_pos[i];
							if (c[0] > p[0]) {
								p[0] = c[0];
								p[1] = c[1];
							}
						}
						if (p[1] == -100) {//at this point, p[0] is the length of matching string before the matching pair
							//p[1] is the end position
							fdiff << "A" << te.size() << ":" << te;
							fdiff << "C" << L_OF_STRING << "," << g->start_pos[0];
						}else {
							if (p[0] == te.size()) {
								fdiff << "C" << L_OF_STRING + te.size() << "," << p[1] - te.size();
							}else {
								fdiff << "A" << te.size()-p[0] << ":" << te.substr(0, te.size()-p[0]);
								fdiff << "C" << L_OF_STRING + p[0] << "," << p[1] - p[0];
							}
						}
					}else {//if there are no non-found string
						
						fdiff << "C" << L_OF_STRING << "," << g->start_pos[0];
					}
				}else {
					if (te.size() != 0) {
						int a = ffff(old, p[1], te).size();
						if (a == te.size()) {
							fdiff << "C" << p[0] + L_OF_STRING + te.size() << "," << p[1] - te.size();
						}
						else {
							fdiff << "A" << te.size() - a << ":" << te.substr(0, te.size() - a);
							fdiff << "C" << L_OF_STRING + p[0] + a << "," << p[1]  - a;
						}
					}
					else {//
						fdiff << "C" << p[0]+L_OF_STRING << "," << p[1];
					}
					fn = fn.substr(p[0]);
				}
				
			}	
		}
		string te;
		if (re.size() != 0) {
			for (int i = 0; i != re.size(); i++) {
				te += re[i];
			}
			fdiff << "A" << re.size()+fn.size() << ":" << te+fn;
		}
		else if (fn.size() != 0) {
			fdiff << "A" << fn.size() << ":" << fn;
		}
		delete[] c;
		delete[] p;
	}
}


bool runtest(string oldName, string newName, string diffName, string newName2)
{
	if (diffName == oldName || diffName == newName ||
		newName2 == oldName || newName2 == diffName ||
		newName2 == newName)
	{
		cerr << "Files used for output must have names distinct from other files" << endl;
		return false;
	}
	ifstream oldFile(oldName, ios::binary);
	if (!oldFile)
	{
		cerr << "Cannot open " << oldName << endl;
		return false;
	}
	ifstream newFile(newName, ios::binary);
	if (!newFile)
	{
		cerr << "Cannot open " << newName << endl;
		return false;
	}
	ofstream diffFile(diffName, ios::binary);
	if (!diffFile)
	{
		cerr << "Cannot create " << diffName << endl;
		return false;
	}
	createDiff(oldFile, newFile, diffFile);
	diffFile.close();

	oldFile.clear();   // clear the end of file condition
	oldFile.seekg(0);  // reset back to beginning of the file
	ifstream diffFile2(diffName, ios::binary);
	if (!diffFile2)
	{
		cerr << "Cannot read the " << diffName << " that was just created!" << endl;
		return false;
	}
	ofstream newFile2(newName2, ios::binary);
	if (!newFile2)
	{
		cerr << "Cannot create " << newName2 << endl;
		return false;
	}
	assert(applyDiff(oldFile, diffFile2, newFile2));
	newFile2.close();

	newFile.clear();
	newFile.seekg(0);
	ifstream newFile3(newName2, ios::binary);
	if (!newFile)
	{
		cerr << "Cannot open " << newName2 << endl;
		return false;
	}
	if (!equal(istreambuf_iterator<char>(newFile), istreambuf_iterator<char>(),
		istreambuf_iterator<char>(newFile3), istreambuf_iterator<char>()))
	{

		cerr << newName2 << " is not identical to " << newName
			<< "; test FAILED" << endl;
			return false;
	}
	return true;
}

int main()
{
	assert(runtest("strange1.txt", "strange2.txt", "1.txt", "sm.txt"));
	cerr << "Test PASSED" << endl;
}