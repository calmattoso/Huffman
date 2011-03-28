#include <iostream>
#include <fstream>
#include <string>
#include <map>

#include <cassert>

using namespace std;

class Node{
 public:
	char character;
	int frequency;
	Node *right, *left;
	
	Node(char character, int frequency){
		this->character = character;
		this->frequency = frequency;
		this->right = NULL;
		this->left = NULL;
	}
	
	bool operator<(const Node &rhs) const{
		if(this->frequency != rhs.frequency) return (this->frequency < rhs.frequency);
		return (this->character < rhs.character);
	}	
	
	bool operator>(const Node &rhs) const{
		if(this->frequency != rhs.frequency) return (this->frequency > rhs.frequency);
		return (this->character > rhs.character);
	}
	
	bool operator>=(const Node &rhs) const{
		if(this->frequency != rhs.frequency) return (this->frequency >= rhs.frequency);
		return (this->character >= rhs.character);
	}
};

/* Template � usado para que seja poss�vel usar qualquer tipo com a �rvore: int, float, Node, etc */
template <class T>
class Heap{
	T *A[1000]; // Vetor que armazena at� 1000 ponteiros do tipo abstrato T
	int N; 		// Determina o tamanho atual da Heap
	
	/* void up - utilizada para reajustar a Heap ap�s a introdu��o de um novo elemento */
	void up(int u){
		while (u > 0){
			int parent = (u-1)/2;
			if (*(A[parent]) > *(A[u])) {
					swap(A[parent], A[u]);
					u = parent;
					continue;
			}
			break;
		}	
	}

	/* void down - utilizada para se reajustar a Heap ap�s a remo��o de um elemento */
	void down(int u){
		while (1){
			int left_child = 2 * u + 1;
			int right_child = 2 * u  + 2;
			int smallest = -1;
			if (left_child < N) smallest = left_child;
			if (right_child < N && *(A[right_child]) < *(A[left_child])) smallest = right_child;
			if (smallest == -1 || *(A[smallest]) >= *(A[u]) ) break;
			swap(A[smallest], A[u]);
			u = smallest;
		}

	}

 public:
	Heap():N(0){}
	
	/* void insert - insere um novo elemento na Heap */
	void insert(Node *x){
		A[N] = x; 
		up(N);
		++N;
	}
	
	/* void top - retorna o elemento presente no topo da Heap */
	T* top(){
		assert(N > 0);	
		return A[0];
	}

	/* void pop - remove o elemento que est� no topo da Heap */
	void pop(){
		swap(A[0], A[--N]);
		down(0);
	}

	/* bool empty - retorna true caso a Heap esteja vazia, e false caso contr�rio */
	bool empty(){
		return N == 0;
	}
	
	/* int size - retorna o tamanho atual da Heap */
	int size(){
		return N;
	}
};

class BinaryTree{
 private:
	Node *root;
	
	/* void displayTree - mostra o n�vel, caractere e frequ�ncia de cada n� �rvore de Huffman, para valida��o desta */
	void displayTree(Node *cur, int level, string direction=""){
		if(cur == NULL) return;
		cout << "Level " << level << "(" << direction << "): " << cur->character << "::" << cur->frequency << endl;
		displayTree(cur->left, level+1, "left");
		displayTree(cur->right, level+1, "right");
	}
	
	/* void huffmanCodesGenerator() - gera uma tabela que associa cada caractere da �rvore a seu respectivo c�digo */
	void huffmanCodesGenerator(map<char, string> &huffmanCodes, Node *cur, string code){
		/* Caso a �rvore tenha apenas um n� */
		if((cur->left == NULL) && (cur->right == NULL)){
			huffmanCodes[cur->character] = code;
			
			return;
		}	
		
		huffmanCodesGenerator(huffmanCodes, cur->left, (code + "0"));
		huffmanCodesGenerator(huffmanCodes, cur->right, (code + "1"));
	}
	
 public: 
	BinaryTree(){
		root = NULL;
	}
	
	/* void constructTree - respons�vel pela constru��o da �rvore de Huffman */
	void constructTree(const map<char,int> &frequenciesCounter){
		Heap<Node> auxHeap;
		map<char,int>::const_iterator it;
		
		for(it = frequenciesCounter.begin(); it != frequenciesCounter.end(); it++){
			Node *temp = new Node(it->first, it->second);
			// temp.character recebe o caractere atual (it->first)
			// temp.frequency recebe a frequ�ncia atual (it->second)
			
			auxHeap.insert(temp); // Joga temp em auxHeap
		}
		
		while(auxHeap.size() > 1){ // Enquanto houver pelo menos 2 Nodes na Heap
			Node *firstNode = auxHeap.top(); // Pega o primeiro n� da futura �rvore de Huffman
			auxHeap.pop(); // Retira o n� da �rvore
			Node *secondNode = auxHeap.top(); // Pega o segundo n� da futura �rvore de Huffman
			auxHeap.pop(); // Retira o segundo n� da �rvore
			
			int totalFrequency = (firstNode->frequency + secondNode->frequency); // Obt�m a soma das frequ�ncias de cada n�
			

			Node *fatherNode = new Node(0, totalFrequency); // Cria um n�, pai dos dois n�s obtidos anteriormente
			fatherNode->left = firstNode;
			fatherNode->right = secondNode;
			
			auxHeap.insert(fatherNode); // Joga o novo n� na Heap
		}
		root = auxHeap.top(); // root passa a apontar para a raiz da �rvore de Huffman
		auxHeap.pop();	// remove o �ltimo n� salvo na Heap
	}
	
	/* void showTree - utilizada para chamar a displayTree, que torna poss�vel validar a �rvore de Huffman */
	void showTree(){
		displayTree(root, 0);
	}
	
	/* void generateHuffmanCodes() - gera uma tabela que associa cada caractere da �rvore a seu respectivo c�digo */
	void generateHuffmanCodes(map<char, string> &huffmanCodes){
		// Caso a raiz da �rvore seja uma folha
		if((root->left == NULL) && (root->right == NULL)){
			huffmanCodes[root->character] = "0";
		}
		else huffmanCodesGenerator(huffmanCodes, root, "");
	}
	
	/* Node* GetRoot() - retorna o endere�o da raiz da �rvore */
	Node* GetRoot(){
		return root;
	}
	
	/* Node* GoDownInTree - desce na �rvore a partir do node cur baseado na dire��o definida (right ou left) */
	Node* GoDownInTree(Node *cur, string direction){
		if(direction == "right"){
			return cur->right;
		}
		else {
			return cur->left;
		}
	}
	
	/* bool IsLeaf() - se o node cur for folha, retorna true. Caso contr�rio, retorna false */
	bool IsLeaf(Node *cur){
		if((cur->left == NULL) && (cur->right == NULL)){
			return true;
		}
		return false;
	}
	
	/* char GetCharacter() - retorna o caractere salvo no node de endere�o cur */
	char GetCharacter(Node *cur){
		return (cur->character);
	}
};

class CompressionAndDecompression{
 private:
	ifstream inputFile; // Utilizado para manipula��o do arquivo de entrada
	ofstream outputFile; // Utilizado para manipula��o do arquivo de sa�da 
	
 public:
	/* CompressionAndDecompression (construtor) - abre o arquivo seguindo os crit�rios definidos nos par�metros */
	CompressionAndDecompression(string fileName, string inputPurpose, string outputPurpose){
		// Read
		if(inputPurpose == "readTXT"){ // Leitura de um arquivo de texto
			inputFile.open(fileName.c_str(), ios::in);
		}
		else if(inputPurpose == "readBIN"){ // Leitura de um arquivo bin�rio
			inputFile.open(fileName.c_str(), (ios::in | ios::binary));
		}
		
		// Write
		if(outputPurpose == "writeTXT"){ // Escrita em um arquivo de texto
			fileName = (fileName.substr(0, (fileName.size()-3)) + "txt"); // Troca a extans�o .cdv de fileName por .txt
			outputFile.open(fileName.c_str(), ios::out);
		}
		else if(outputPurpose == "writeBIN"){ // Escrita em um arquivo bin�rio
			fileName = (fileName.substr(0, (fileName.size()-3)) + "cdv"); // Troca a extans�o .txt de fileName por .cdv	
			outputFile.open(fileName.c_str(), (ios::out | ios::binary));
		}
		
		assert(inputFile.is_open());
		assert(outputFile.is_open());
	}
	
	/* void Read - Fun��o que ler� o arquivo de texto, contando a frequ�ncia de cada caractere do arquivo.  */
	void Read(map<char,int> &frequenciesCounter){
		if(inputFile.is_open()){
			char character;
			
			inputFile.seekg(0, ios::beg); // Certifica que o arquivo de entrada est� no come�o
				
			inputFile >> noskipws;			
			while(inputFile>>character){ // Abaixo ocorre a contagem da frequ�ncia de cada caractere
				if(frequenciesCounter.find(character) != frequenciesCounter.end())
					frequenciesCounter[character]++;
				else
					frequenciesCounter[character]=1;
			}
			
			inputFile.clear();
		}	
	}
	
	/* void GenerateHeader - escreve o cabe�alho no arquivo de sa�da */
	void GenerateHeader(map<char,int> &frequenciesCounter){
		int frequency;
		
		for(int i = 0; i < 256; i++){ // Salva a quantidade de vezes que cada caractere da tabela ASCII apareceu no arquivo de entrada
           	if (frequenciesCounter.find((char)i) != frequenciesCounter.end())
	            frequency = frequenciesCounter[(char)i];
           	else
				frequency = 0;
				
			outputFile.write((char *)&frequency, sizeof(int));
		}
	}
	
	/* size_t getOutputFileLength() - obt�m e retorna o tamanho do arquivo de entrada */
	size_t getInputFileLength(){
		size_t fileSize;
		
		inputFile.clear(); // Limpa todos as flags do arquivo
		
		inputFile.seekg(0, ios::end); // Vai para o final do arquivo de entrada
		fileSize = inputFile.tellg(); // Obt�m a posi��o atual do get pointer no arquivo de entrada (posi��o que indica o tamanho, em bytes, do arquivo)
		
		inputFile.clear();		
		inputFile.seekg(0, ios::beg); // Volta o get pointer ao come�o do arquivo de entrada
		
		return (fileSize); // Retorna o tamanho, em bytes, do arquivo de entrada
	}
	
	/* size_t getOutputFileLength() - obt�m e retorna o tamanho do arquivo de sa�da */
	size_t getOutputFileLength(){
		size_t fileSize;
			
		outputFile.clear(); // Limpa todas as flags do arquivo de sa�da
		
		outputFile.seekp(0, ios::end); // Move o put pointer do arquivo de sa�da para seu final
		fileSize = outputFile.tellp(); // Obt�m a posi��o atual do put pointer no arquivo de sa�da (posi��o que indica o tamanho, em bytes, do arquivo)
		
		outputFile.clear();		
		outputFile.seekp(0, ios::beg); // Retorna o put pointer ao come�o do arquivo de sa�da
		
		return (fileSize); // Retorna o tamanho, em bytes, do arquivo de sa�da
	}

	/* void setCharacterBits() - seta os bits de character de acordo com a string passada como segundo argumento */
	void setCharacterBits(char &character, string code){
		int pos = 7;
		
		for(int i = 0; (i < 8) && (i < code.size()); i++, pos--){
			if(code[i] == '1'){
				character = (character | (1 << pos));				
			}	
		}
	}	
	
	/* void Compress - gera o arquivo compactado */
	void Compress(map<char,string> huffmanCodes){
		char inputCharacter, outputCharacter;
		string code;	// Salva um conjunto de c�digos dos caracteres do arquivo de entrada
		size_t inputFileLength; // Armazena a quantidade de caracteres do arquivo
		map<char,string>::const_iterator it;
		
		inputFileLength = getInputFileLength(); // Obt�m o tamanho do arquivo de entrada
		outputFile.write((char *)&inputFileLength, sizeof(size_t)); // Escreve no arquivo compactado a quantidade de caracteres a serem lidos
		
		inputFile.seekg(0, ios::beg); // Certifica que o arquivo de entrada est� no come�o
		
		outputCharacter = 0; // Seta o caractere de sa�da como 0
		code = ""; 
		
		inputFile >> noskipws; // Torna poss�vel a leitura de whitespaces
		while(inputFile >> inputCharacter){
			it = huffmanCodes.find(inputCharacter); // Obt�m o c�digo do caractere de entrada
			
			code += it->second; // Code passa a salvar o c�digo do caractere de entrada
						
			while(code.size() >= 8){
				setCharacterBits(outputCharacter, code.substr(0, 8)); // Seta os bits de outputCharacter de acordo com os 8 primeiros caracteres de code

				code = code.substr(8, code.size());
				
				outputFile.write(&outputCharacter, sizeof(char));
				
				outputCharacter = 0;
			}
		}

		// Caso tenha sobrado algo em code, escreve-o no arquivo compactado
		if(code.size()){
			setCharacterBits(outputCharacter, code.substr(0, 8));
			
			outputFile.write(&outputCharacter, sizeof(char));
			
			outputCharacter = (char)0;
		}
	}

	/* void GetHeader() - obt�m o cabe�alho do arquivo compactado */
	void GetHeader(map<char,int> &frequenciesCounter){
		int frequency;
		
		for(int i = 0; i < 256; i++){
			inputFile.read((char *)&frequency, sizeof(int));
			
			if (frequency != 0)
				frequenciesCounter[((char)i)] = frequency;
		}
	}

	/* void Decompress() - descompacta o arquivo .cdv */
	void Decompress(BinaryTree &huffmanTree){
		size_t fileLength; // Vari�vel que guarda a quantidade de caracteres a serem obtidos
		int bitPosition; // Indica qual a posi��o atual do caractere a ser vista
		char inputCharacter, outputCharacter; 
		Node *curNode; // Salva um n� da �rvore
		
		//cout << "Decompress results:" << endl;
		
		inputFile.read((char *)&fileLength, sizeof(size_t)); // Obt�m o tamanho do arquivo
		// DEBBUGING: Verificar se foi obtido corretamente o tamanho do arquivo
		// cout << "fileLength: " << fileLength << endl;
		
		curNode = huffmanTree.GetRoot(); // Salva em curNode o endere�o da raiz da �rvore de Huffman
		
		// Caso base: raiz como folha
		if(huffmanTree.IsLeaf(curNode)){
			outputCharacter = huffmanTree.GetCharacter(curNode);			
			while(fileLength > 0){	// Escreve o caractere da raiz fileLength vezes		
				outputFile << outputCharacter;
				fileLength--;
			}
			
			return;
		}
		
		bitPosition = 7;
		while(fileLength > 0){ // L� do arquivo de entrada enquanto ainda houver caracteres a serem obtidos
			// cout << "File Length now equals " << fileLength << endl;
			if(bitPosition == 7)
				inputFile.read(&inputCharacter, sizeof(char));
			// cout << inputCharacter;
			
			while(bitPosition >= 0){
				if((inputCharacter & (1 << bitPosition)) != 0){ // Ir para a direita
					curNode = huffmanTree.GoDownInTree(curNode, "right");
				}
				else { // Ir para a esquerda
					curNode = huffmanTree.GoDownInTree(curNode, "left");
				}
				
				if(huffmanTree.IsLeaf(curNode)){
					outputCharacter = huffmanTree.GetCharacter(curNode);

					if(outputCharacter == '\n') fileLength--; // Como o '\n' ocupa 2 bytes, caso o char obtido seja '\n', deve-se descontar mais 1 
					// DEBBUGING: Checar se o arquivo obtido � o esperado
					// cout << outputCharacter;
					outputFile << outputCharacter;
					
					curNode = huffmanTree.GetRoot(); // Recupera o endere�o da raiz
					fileLength--; 
					if(fileLength <= 0) break; // Se n�o houver mais caracteres a serem lidos, sai do loop
				}
				
				bitPosition--; // Indica que mais uma posi��o do c�digo bin�rio do char foi vista
			}
			
			bitPosition = 7; // Faz com que o char volte a ser analisado a partir de seu bit de �ndice 7
		}
		// cout << "Out of decompress while" << endl;
	}

	/* double Diagnosis() - retorna a taxa de compress�o */
	double Diagnosis(){
		size_t inputFileSize, outputFileSize;
		double difference;
		
		inputFileSize = getInputFileLength();
		outputFileSize = getOutputFileLength();
		
		difference = ((1 - ((double)outputFileSize/(double)inputFileSize)) * 100);
		
		return difference;
	}
	
	/* void Finish() - fecha os arquivos de entrada e de sa�da */
	void Finish(){
		inputFile.close();
		outputFile.close();
	}
	
	/* bool isInputFileEmpty() - retorna true se o arquivo tiver vazio, false caso contr�rio */
	bool isInputFileEmpty(){
		if(getInputFileLength() == 0) return true;
		return false;
	}
};

/* bool Compression - fun��o encarregada de comprimir o arquivo de texto */
bool Compression(string fileName){
	CompressionAndDecompression interface(fileName, "readTXT", "writeBIN"); // objeto para manipula��o dos arquivos de entrada (.txt) e sa�da (.bin)	
	BinaryTree huffmanTree; 				// �rvore de huffman para obten��o de c�digos de cada caractere
	map<char, int> frequenciesCounter; 		// contador das frequ�ncias de cada caractere - utilizado por interface
	map<char, string> huffmanCodes; 	// salva cada caractere e seu respectivo c�digo
	
	if(interface.isInputFileEmpty() == true) return false;
	interface.Read(frequenciesCounter); 	// Conta as frequ�ncias de cada caractere e as salva em frequenciesCounter 
	
	huffmanTree.constructTree(frequenciesCounter); // Gera uma �rvore de Huffman a partir da frequencia de cada caractere

	huffmanTree.generateHuffmanCodes(huffmanCodes);
	
	interface.GenerateHeader(frequenciesCounter);
	
	interface.Compress(huffmanCodes);
	
	cout << "Compression succesful" << endl;
	cout << "Diagnosis: " << endl;
	cout << "Input File Size : " << interface.getInputFileLength() << endl;
	cout << "Output File Size: " << interface.getOutputFileLength() << endl;
	cout << "% Difference    : " << interface.Diagnosis() << "%" << endl;
	
	interface.Finish();
}

/* bool Decompression - fun��o na qual ocorre todo o processo de descompress�o do arquivo .cdv */
bool Decompression(string fileName){
	CompressionAndDecompression interface(fileName, "readBIN", "writeTXT"); // objeto para manipula��o dos arquivos de entrada (.bin) e sa�da (.txt)	
	BinaryTree huffmanTree; // �rvore de huffman para obten��o de c�digos de cada caractere
	map<char, int> frequenciesCounter; // contador das frequ�ncias de cada caractere - utilizado por interface
	map<char, string> huffmanCodes; // salva cada caractere e seu respectivo c�digo

	if(interface.isInputFileEmpty() == true) return false;
	
	interface.GetHeader(frequenciesCounter);
	
	huffmanTree.constructTree(frequenciesCounter);

	interface.Decompress(huffmanTree);
	
	interface.Finish();
	
	cout << "Decompression succesful." << endl;
}

int main(int argc, const char *argv[]){
	if(argc > 1){ // Caso tenham sido passados argumentos
		string fileName = (string) argv[1]; // Pega o nome do arquivo passado como argumento ao programa
		string fileExtension = fileName.substr(fileName.size() - 3, fileName.size()); // Obt�m a extens�o do arquivo
		
		if (fileExtension == "txt") // caso o arquivo seja um arquivo de texto
			Compression(fileName); // chama Compression para gerar um arquivo comprimido
		else if(fileExtension == "cdv") // caso o arquivo seja um arquivo comprimido
			Decompression(fileName); // chama Decompression para recuperar o arquivo de texto original
		else
			cout << "Extensao (" << fileExtension << ") invalida." << endl; // Se a extens�o n�o for v�lida, alerta-se o usu�rio
	}
	else cout << "Sem arquivo de entrada." << endl; // Caso n�o seja especificado o arquivo de entrada, emite-se um alerta

	return 0;
}
