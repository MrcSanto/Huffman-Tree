#include <iostream>
#include <fstream>
#include <codecvt>
#include <locale>
#include "huffTree.cpp"

using namespace std;

int main(){

    wifstream arq(L"../clarissa.txt");
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

    cout << "Arquivos gerados na pasta : saida\n";

    return 0;
}