#include <iostream>
#include <fstream>
#include <map>
#include <unordered_map>
#include <queue>
#include<bitset>
#include<string>


using namespace std;






class Huff {                                            //Creating data structure  
public:
	char ch;
	int frequency;
	Huff* l, * r;


	Huff(char ch, int frequency, Huff* l, Huff* r) {      //Getting new node, construction

		this->ch = ch;
		this->frequency = frequency;
		this->l = l;
		this->r = r;


	}


};







struct LowestFreqComp {                                                         //This operator for making queue descending by looking they frequencies

	bool operator()(Huff* Lhuff, Huff* Rhuff) {

		if (Lhuff->frequency > Rhuff->frequency) {                                        // Queue will take lowest freq node to left side every time because our path weight is 0 in left
			return true;
		}
		else {
			return false;
		}
	}
};







void HuffamanPathFinder(Huff* root, string str, char textElem, ofstream& out) {   //Function finding char(Given text) from created huffman tree 
																				//When function traverse tree recursively it stores the code to string variable



	if (root == nullptr)                                                       //if reaches the end of the file ,return from file
		return;


	if (root->l == nullptr && root->r == nullptr)                       //Detecting the leaf node when function reaches 
	{																		  //because leaf nodes include our char

		if (textElem == root->ch)
		{
			out << str;														 //writing into huffman.txt

		}

	}

	HuffamanPathFinder(root->l, str + "0", textElem, out);       //recursive function that traverse whole tree
	HuffamanPathFinder(root->r, str + "1", textElem, out);
}






string Uncompressed(char ch) {

	string BinaryText;
	BinaryText = bitset<8>(ch).to_string();      //converting char to binary(8bit)


	return BinaryText;
}






void Compressed(Huff* root, string MyText) {

	ofstream out("huffman.txt");


	for (int i = 0; i < MyText.size(); i++)                                 //Starting from first char of the given text, finding to char that 
	{                                                                     //we encoded in binarytree by calling HuffmanPathFinder function 


		if (MyText[i] == ' ') {                                            //detecting spaces 

			HuffamanPathFinder(root, " ", ';', out);
		}
		if (MyText[i] == '\n') {                                           //detecting enter 

			HuffamanPathFinder(root, " ", '!', out);
		}





		HuffamanPathFinder(root, " ", MyText[i], out);
	}
	out.close();

}







void QueueCreate(priority_queue< Huff*, vector<Huff*>, LowestFreqComp >& MyPQ, multimap <int, char>& ChFreq) {

	multimap <int, char> ::iterator i;


	for (i = ChFreq.begin(); i != ChFreq.end(); i++)
	{


		MyPQ.push(new Huff(i->second, i->first, nullptr, nullptr));     // taking every element on map and sending into queue as a leaf node


	}



}





void HuffmanTree(multimap <int, char>& ChFreq, string MyText) {

	priority_queue< Huff*, vector<Huff*>, LowestFreqComp > MyPQ;    //declaring priority queue to store and managing node with each other with comp struct

	QueueCreate(MyPQ, ChFreq);



	do
	{
		Huff* left = MyPQ.top();                 // taking lowest frequencies in queue and remove it 
		MyPQ.pop();



		Huff* right = MyPQ.top();               // taking next lowest frequencies in queue and remove it 
		MyPQ.pop();

		int sumFreq = left->frequency + right->frequency;     //adding them and creating as a new node and sending it back to queue


		MyPQ.push(new Huff('\0', sumFreq, left, right));


	} while (MyPQ.size() != 1);                   // do this until there is one  node in queue



	Huff* rootHuff = MyPQ.top();                          //take completed node and sending to encode function 

	Compressed(rootHuff, MyText);




}






void DisplayMap(multimap<int, char>& ChFreq) {

	multimap<int, char>::reverse_iterator it;                      //Declaring reverse iterator because we want to display map in descending order

	for (it = ChFreq.rbegin(); it != ChFreq.rend(); it++)          //traverse map
	{
		cout << it->first << " \t|| " << it->second << endl;       //printing
	}


}






void DisplayUnCompressed() {

	string AsciiText;
	ifstream in;
	in.open("ascii.txt");

	cout << "\n\n ----Decoded Version---" << endl;
	while (in >> AsciiText) {                         //reading from ascii.txt

		cout << AsciiText + " ";
	}
	cout << endl;
	in.close();
}




void DisplayCompressed() {

	string HuffmanCoddedText;
	ifstream in;
	in.open("huffman.txt");

	cout << "\n\n ----Encoded Version---" << endl;
	while (in >> HuffmanCoddedText) {              //reading from huffman.txt

		cout << HuffmanCoddedText + " ";
	}
	cout << endl;
	in.close();
}




int main() {

	fstream in;
	ofstream out("ascii.txt");

	char ch;
	string MyText;
	map <char, int> ChMap;   //store character with their frequencies in map structre









	in.open("Input.txt");        // Opening given file



	if (!in) {
		cout << "File couldn't found" << endl;
	}
	else {

		in >> std::noskipws;     //we don't want our program skip spaces

		while (in >> ch) {       //inputting given text to program as a character


			out << Uncompressed(ch) << " ";     //converting every char to binary(8bit) and writing to ascii file

			MyText.push_back(ch);               //Saving given text to string MyText because we'll use later to decode


			if (ch == 32) {
				ch = ';';                        // This part for special char that not seeing on printing  ; symbol represent spaces
			}																						//     ! symbol represent enter
			if (ch == '\n') {
				ch = '!';
			}

			ChMap[ch]++;                        //Creating map to store char frequency


		}
		out.close();
		in.close();
	}





	cout << "-------------------------------------------------------------------- " << endl;      // printing UI
	cout << "Given Text :" << endl << MyText;
	cout << "\n\n\n||Frequency List||" << endl;





	map<char, int> ::iterator it;
	multimap<int, char> ChFreq;                                       //We declare multimap because we should order the map by frequency

	for (it = ChMap.begin(); it != ChMap.end(); it++)                 //Traverse map
	{
		ChFreq.insert(pair<int, char>(it->second, it->first));        //assigning map to multimap
	}


	DisplayMap(ChFreq);                                               // Displaying Ordered Multimap 
	DisplayUnCompressed();											  // Displaying Binary(8bit) version of Text  


	HuffmanTree(ChFreq, MyText);										//Creating Huffman tree

	DisplayCompressed();											   // Displaying Huffman version of Text  





	return 0;
}




