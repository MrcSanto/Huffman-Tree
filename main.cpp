#include <iostream>
#include <fstream>
#include <codecvt>
#include <locale>
#include "huffTree.cpp"

using namespace std;

int main(){
    string path;
    cout << "Digite o nome do arquivo para ser comprimido: ";
    cin >> path;

    
//ideia

//implementar uma funcao no arquivo de funcoes que lista todos os arquivos txt em /Docs
// 1--biblia.txt
// 2--clarissa.txt
// 3--...
//e fazer o usuario escolher o arquivo pelo numero.
//alem de ficar mais facil fica mais bonito

    wifstream arq("../Docs/" + path);
    arq.imbue(locale(arq.getloc(), new codecvt_utf8<wchar_t>)); 

    wofstream saida("../saida/saida.txt");
    saida.imbue(locale(saida.getloc(), new codecvt_utf8<wchar_t>));
    
    if(!arq.is_open()){
        cout << "Erro em abrir o arquivo!\n";
        return 1;
    }

    if (!saida.is_open()) {
        cout << "Erro em criar o arquivo de saida!\n";
        return 1;
    }

    buildHuffTree(arq, saida);
    arq.close();
    saida.close();

    cout << "Arquivo em saida/.\n";

    return 0;
}