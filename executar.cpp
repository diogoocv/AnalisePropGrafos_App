#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iterator>

using namespace std;

// Cores dos vértices durante uma busca
#define BRANCO 0    // Vértice não descoberto
#define CINZA 1      // Vértice descoberto
#define PRETO 2      // Vértice fechado

// Tipo do grafo 
enum TipoGrafo{
    direcionado,
    nao_direcionado
};

// Lê os dados do grafo passados pelo usuário e retorna sua lista de adjacência
vector<pair<int, int>>* lerGrafo(int m, enum TipoGrafo tipo, vector<pair<int, int>>* LA) {
    int id, u, v, w;

    for(int i = 0; i < m; i++) {
        cin >> id >> u >> v >> w;

        // Evitando a leitura de arestas repetidas
        for(auto i: LA[u]) {
            if(i.first == v) {
                continue;
            }
        }

        // Adicionando as arestas à lista de adjacência
        LA[u].push_back({v,w});
        if(tipo == nao_direcionado) {
            LA[v].push_back({u,w});
        }  
    }

    return LA;  // Retornando a lista de adjacência lida
}

// Busca em Largura
int* BFS(int n, int s, vector<pair<int, int>>* LA) {              
    int* cor = new int[n];  // Vetor de cores dos vértices
    queue<int> fila;        // Fila de vértices visitados durante a exploração do grafo
    
    // Inicializando todos os vértices com a cor branca
    for(int i = 0; i < n; i++) {
        cor[i] = BRANCO;    
    }

    fila.push(s);           // Adicionando o vértice de origem à fila para iniciar a busca
    cor[s] = CINZA;          // Vértice de origem descoberto

    while(!fila.empty()) {
        int u = fila.front();   // Auxiliar u recebe o 1° vértice da fila
        cor[s] = PRETO;         // Fechando o vértice u
        fila.pop();             // Retirando o vértice u da fila

        // Percorrendo a lista de adjacência de u e procurando por vértices brancos
        for(auto v: LA[u]) {
            if(cor[v.first] == BRANCO) {
                cor[v.first] = CINZA;   // Vértice v descoberto
                fila.push(v.first);     // Vértice v adicionado à fila
            }
        }
    }
    return cor;   
}

// Verifica a conexidade do grafo
// Retorna 1 se for conexo (nao_direcionado) ou fracamente conexo (direcionado) ou 0 se for desconexo
int conexidadeGrafo(int n, enum TipoGrafo tipo, vector<pair<int, int>>* LA) {
    int* cor = NULL;    // Vetor de cores dos vértices

    // Se o grafo for direcionado, para cada aresta em (u,v) é adicionada uma aresta (v,u), caso não exista
    if(tipo == direcionado) {
        vector<pair<int, int>>* LA_Aux = new vector<pair<int, int>>[n];   // Lista de adjacência auxiliar para verificar conexidade fraca
        
        for(int u = 0; u < n; u++) {
            for(auto v: LA[u]) {
                LA_Aux[u].push_back(v);   // Adicionando a aresta (u,v) na LA_Aux
                // Verificando se já existe a aresta (v,u) em LA e, caso não exista, ela é inserida na LA_Aux 
                for(auto i: LA[v.first]) {
                    if(i.first == u) {
                        continue;
                    }
                }
                LA_Aux[v.first].push_back({u, v.second});
            }
        }
        cor = BFS(n, 0, LA_Aux);    // Passando a LA_Aux como parâmetro para a BFS em caso de grafos direcionados
    }

    if(tipo == nao_direcionado) {
        cor = BFS(n, 0, LA);        // Passando a LA como parâmetro para a BFS em caso de grafos nao_direcionados
    }

    // Percorrendo a lista de cores dos vértices
    for(int i = 0; i < n; i++) {
        // Se algum vértice for branco, o grafo é desconexo
        if (cor[i] == BRANCO) {
            return 0;
        }
    }

    return 1;
}

int main() {
    int n = 0, m = 0;       // Número de vértices e arestas do grafo 
    enum TipoGrafo tipo;    // Tipo do grafo (direcionado ou nao_direcionado)
    string auxTipo;         // Auxiliar para ler do usuário o tipo do grafo

    cin >> n >> m;          // Lendo as arestas do grafo
    cin >> auxTipo;         // Lendo o tipo do grafo

    // Verificando se o grafo é direcionado ou nao_direcionado
    if(auxTipo == "direcionado") {
        tipo = direcionado;
    } else {
        if(auxTipo == "nao_direcionado") {
            tipo = nao_direcionado;
        } else {
            return -1;
        }
    }

    // Lista de adjacência do grafo
    vector<pair<int, int>>* LA = new vector<pair<int, int>>[n]; 

    // Lendo as arestas do grafo
    LA = lerGrafo(m, tipo, LA);

    // Verificando a conexidade do grafo
    int conexidade = conexidadeGrafo(n, tipo, LA);
    cout << conexidade << endl;


    return 0;
}