#include <iostream>
#include <unordered_map>
#include <queue>
#include <string>
#include <vector> 
#include <fstream>
#include <codecvt>
#include <locale>

using namespace std;

struct node{
    wchar_t ch;
    int freq;
    node *left;
    node *right;
};

//struct usada na fila de prioridade(priority_queue)
struct comp {
    bool operator()(node* l, node* r) {
        return l->freq > r->freq;
    }
};


//funcao para alocar um novo nodo na memoria
node *create_node(wchar_t c, int frequency, node *left, node *right){
    node *p = new node;
    p->ch = c;
    p->freq = frequency;
    p->left = left;
    p->right = right;

    return p;
}

void encode(node *root, wstring str, unordered_map<wchar_t, wstring> &huffcode){
    if(!root) return;

    if(!root->left && !root->right){
        huffcode[root->ch] = str;
    }

    encode(root->left, str + L'0', huffcode);
    encode(root->right, str + L'1', huffcode);
}

void decode(node *root, int &top_index, wstring str){
    if(!root) return;

    if(!root->left && !root->right){
        cout << root->ch;
        return;
    }
    top_index++;

    if(str[top_index] == L'0') decode(root->left, top_index, str);
    else if(str[top_index] == L'1') decode(root->right, top_index, str);
}




void buildHuffTree(wifstream &arq, wofstream &saida){

    if(!arq){
        return;
    }
    
    wchar_t ch;
    unordered_map<wchar_t,  int> freq;
    while(arq.get(ch)){
        freq[ch]++;
    }
    
    int teste = 0; //contador somente para fins de teste 
    saida << L"Frequência de caracteres: \n";
    for(auto ch : freq){ 
        if(ch.first == L' ') saida << L"SPACE: " << ch.second << endl;
        else if(ch.first == L'\n') saida << L"\\n: " << ch.second << endl;
        else saida << ch.first << L": " << ch.second << endl;
        teste += ch.second; //esta inconsistente, ver pq
    }
    saida << teste << endl << endl;

    //priority_queue é uma estrutura muito similar a queue
    //no entanto ela sempre armazena os valores de tal modo
        // que o maior deles sempre fica mais a frente
    priority_queue<node*, vector<node*> , comp> pq; 

    //cria um nodo folha para cada caracter
        // e adiciona na fila de prioridade
    for(auto pair : freq){
        pq.push(create_node(pair.first, pair.second, nullptr, nullptr));
    }

    while(pq.size() != 1){
        //remove os dois elementos com a maior prioridade da fila
        node *left = pq.top();
        pq.pop();
        node *right = pq.top();
        pq.pop();

        //cria um nodo interno na arvore com esses 2 nodos acima como filhos
        // e a frequencia sendo igual a soma das frequencias dos filhos.
        //E adiciona o novo nodo a fila de prioridade;  
        int soma =left->freq + right->freq;
        pq.push(create_node(L'\0', soma, left, right));
    }

    node *root = pq.top();
    unordered_map<wchar_t, wstring> huffCode;
    encode(root, L"", huffCode);

    saida << L"Codigos binários da árvore criada: \n";
    for(auto pair : huffCode){
        if(pair.first == L' ') saida << L"SPACE: " << pair.second << endl;
        else if(pair.first == L'\n') saida << L"\\n: " << pair.second << endl;
        else saida << pair.first << L": " << pair.second << endl;
    }

    arq.clear();
    arq.seekg(0, ios::end);
    int original_size = arq.tellg();

    int compressed_size = 0;
    for (auto pair : freq) {
        compressed_size += pair.second * huffCode[pair.first].length();
    }
    compressed_size = (compressed_size + 7) / 8; 

    double compression_ratio = ((double)(original_size - compressed_size) / original_size) * 100;

    saida << L"\n\n\nTamanho original: " << original_size << L" bytes" << endl;
    saida << L"Tamanho compactado: " << compressed_size << L" bytes" << endl;
    saida << L"Redução: " << compression_ratio << L"%" << endl;

}
 

int main(){

    wifstream arq(L"../gpl3.txt");
    arq.imbue(locale(arq.getloc(), new codecvt_utf8<wchar_t>)); 

    wofstream saida("../saida.txt");
    saida.imbue(locale(saida.getloc(), new codecvt_utf8<wchar_t>));
    
    if(!arq.is_open()){
        wcout << L"Erro em abrir o arquivo!\n";
        return 1;
    }

    if (!saida.is_open()) {
        wcout << L"Erro em criar o arquivo de saída!\n";
        return 1;
    }

    buildHuffTree(arq, saida);
    arq.close();
    saida.close();

    return 0;
}