#include <iostream>
#include <unordered_map>
#include <queue>
#include <string>
#include <vector> 

using namespace std;

struct node{
    char ch;
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
node *create_node(char c, int frequency, node *left, node *right){
    node *p = new node;
    p->ch = c;
    p->freq = frequency;
    p->left = left;
    p->right = right;

    return p;
}

void encode(node *root, string str, unordered_map<char, string> &huffcode){
    if(!root) return;

    if(!root->left && !root->right){
        huffcode[root->ch] = str;
    }

    encode(root->left, str + "0", huffcode);
    encode(root->right, str + "1", huffcode);
}

void decode(node *root, int &top_index, string str){
    if(!root) return;

    if(!root->left && !root->right){
        cout << root->ch;
        return;
    }
    top_index++;

    if(str[top_index] == '0') decode(root->left, top_index, str);
    else if(str[top_index] == '1') decode(root->right, top_index, str);
}


void buildHuffTree(string text){

    unordered_map<char, int> freq;
    for(char ch : text){
        freq[ch]++;
    }

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
        pq.push(create_node('\0', soma, left, right));
    }

    node *root = pq.top();
    unordered_map<char, string> huffCode;
    encode(root, "", huffCode);

    cout << "Codigos binarios da arvore criada: \n";
    for(auto pair : huffCode){
        cout << pair.first << ": " << pair.second << endl;
    }

    cout << "\nString original: " << text << endl;

    string str = "";
    for(char ch : text){
        str += huffCode[ch];
    }
    cout << "String criptografada: " << str << endl;

    int top_index = -1;
    cout << "String decriptografada: ";
    while(top_index < (int)str.size() - 2){
        decode(root, top_index, str);
    }

}


int main(){
    string text = "SHOW DE BOLA MARCIO ÉOQ GRAXAAAAA VÉÉÉÉÉIAAA SHOW DE BOLA\n\n\n\nshow de bola marcio éééoqqqq graxxa véééia shw de bola capotemo o golzão verméio\n\n";

    buildHuffTree(text);

    return 0;
}