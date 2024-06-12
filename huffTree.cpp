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

    encode(root->left, str + L"0", huffcode);
    encode(root->right, str + L"1", huffcode);
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


void buildHuffTree(wifstream &arq){

    if(!arq){
        return;
    }
    
    wchar_t ch;
    unordered_map<wchar_t,  int> freq;
    while(arq.get(ch)){
        freq[ch]++;
    }

    int teste = 0; //contador somente para fins de teste 
    wcout << L"Frequencia de caracteres: \n";
    for(auto ch : freq){ 
        if(ch.first == L' ') wcout << L"SPACE: " << ch.second << endl;
        else if(ch.first == L'\n') wcout << L"\\n: " << ch.second << endl;
        else wcout << ch.first << L": " << ch.second << endl;
        teste += ch.second; //esta inconsistente, ver pq
    }
    wcout << teste << endl << endl;

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

    wcout << L"Codigos binarios da arvore criada: \n";
    for(auto pair : huffCode){
        if(pair.first == L' ') wcout << L"SPACE: " << pair.second << endl;
        else if(pair.first == L'\n') wcout << L"\\n: " << pair.second << endl;
        else wcout << pair.first << L": " << pair.second << endl;
    }
}
 

int main(){
    wifstream arq(L"../clarissa.txt",ios::binary);
    arq.imbue(locale(arq.getloc(), new codecvt_utf8<wchar_t>)); // Set locale to handle UTF-8
    
    if(!arq.is_open()){
        wcout << L"Erro em abrir o arquivo!\n";
        return 1;
    }

    buildHuffTree(arq);
    arq.close();

    return 0;
}