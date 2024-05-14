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

    for(auto pair : huffCode){
        cout << pair.first << ": " << pair.second << endl;
    }

}


int main(){
    string text = "Big Bob Bites Banana\n";

    buildHuffTree(text);

    return 0;
}