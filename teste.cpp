#include <iostream>
#include <unordered_map>
#include <queue>
#include <string>
#include <vector> 
#include <fstream>
#include <locale> // Adicionando a biblioteca locale para suporte Unicode

using namespace std;

struct node {
    wstring ch; // Usando wstring para suportar caracteres Unicode
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
node *create_node(const wstring& c, int frequency, node *left, node *right){
    node *p = new node;
    p->ch = c;
    p->freq = frequency;
    p->left = left;
    p->right = right;
    return p;
}

void encode(node *root, const wstring& str, unordered_map<wstring, string> &huffcode){
    if(!root) return;
    if(!root->left && !root->right){
        huffcode[root->ch] = string(str.begin(), str.end());
    }
    encode(root->left, str + L"0", huffcode);
    encode(root->right, str + L"1", huffcode);
}

void decode(node *root, int &top_index, const string& str){
    if(!root) return;
    if(!root->left && !root->right){
        wcout << root->ch;
        return;
    }
    top_index++;
    if(str[top_index] == '0') decode(root->left, top_index, str);
    else if(str[top_index] == '1') decode(root->right, top_index, str);
}

void buildHuffTree(const wstring& text){
    unordered_map<wstring, int> freq;
    for(wchar_t ch : text){
        freq[wstring(1, ch)]++;
    }

    wcout << L"Frequência de caracteres: \n";
    int teste = 0; //contador somente para fins de teste 
    for (const auto& pair : freq) {
        if (pair.first == L" ") wcout << L"SPACE: " << pair.second << endl;
        else if (pair.first == L"\n") wcout << L"\\n: " << pair.second << endl;
        else wcout << pair.first << L": " << pair.second << endl;
        teste += pair.second;//esta inconsistente, ver pq
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
        int soma = left->freq + right->freq;
        pq.push(create_node(L"", soma, left, right));
    }

    node *root = pq.top();
    unordered_map<wstring, string> huffCode;
    encode(root, L"", huffCode);

    wcout << L"Codigos binarios da arvore criada: \n";
    for(auto pair : huffCode){
        wcout << pair.first << L": " << wstring(pair.second.begin(), pair.second.end()) << endl;
    }

    wcout << L"\nString original: " << text << endl;

    string str = "";
    for(wchar_t ch : text){
        str += huffCode[wstring(1, ch)];
    }
    wcout << L"String criptografada: " << wstring(str.begin(), str.end()) << endl;

    int top_index = -1;
    wcout << L"\nString descriptofrada: ";
    while(top_index < (int)str.size() - 2){
        decode(root, top_index, str);
    }
    wcout << endl;
}

int main(){
    wstring text = L"Este é um texto de teste com caracteres Unicode. Aqui estão alguns exemplos de caracteres especiais: ♥, ☺, €, ☀, こんにちは, привет, مرحبا. Espero que este texto ajude a testar o suporte completo a Unicode em seu programa."; // Texto com caracteres Unicode
    setlocale(LC_ALL, ""); // Configurando a localização para suporte completo a Unicode
    buildHuffTree(text);
    return 0;
}
