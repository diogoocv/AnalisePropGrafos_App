#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iterator>

using namespace std;

// Cores que representam o estado dos vértices durante uma busca
#define BRANCO 0     // Vértice não descoberto
#define CINZA 1      // Vértice descoberto
#define PRETO 2      // Vértice fechado

// Cores utilizadas em problemas de coloração de grafos
#define AZUL 3
#define VERMELHO 4  

// Representa uma aresta em uma lista de adjacência
struct aresta{
    int id;             // Código identificador única da aresta
    int vertice;        // Vértice a quem a aresta se liga 
    int peso;           // Peso da aresta
};

// Tipo do grafo 
enum TipoGrafo{
    direcionado,
    nao_direcionado
};

// Lê os dados do grafo passados pelo usuário e retorna sua lista de adjacência
vector<aresta>* lerGrafo(int n, int m, enum TipoGrafo tipo, vector<aresta>* LA) {
    int id, u, v, w;            // Identificador da aresta, dois vértices conectados por ela e seu peso
    int* auxId = new int[n];    // Vetor auxiliar para verificar id's repetidos

    for(int i = 0; i < m; i++) {
        cin >> id >> u >> v >> w;

        // Evitando a leitura de arestas inválidas (vértices menores que 0 ou maiores ou iguais a n)
        if(u < 0 || u >= n || v < 0 || v >= n) {
            continue;
        }

        // Evitando a leitura de arestas com id já existente
        for(int i = 0; i < n; i++) {
            for(auto i: LA[u]) {
                if(i.id == id) {
                    continue;
                }
            }
        }

        // Evitando a leitura de arestas repetidas
        for(auto i: LA[u]) {
            if(i.vertice == v) {
                continue;
            }
        }

        // Adicionando as arestas à lista de adjacência
        LA[u].push_back({id, v, w});
        if(tipo == nao_direcionado) {
            LA[v].push_back({id, u, w});
        }  
    }

    return LA;  // Retornando a lista de adjacência lida
}

// Busca em Largura utilizada para verificar a conexidade do grafo
int* bfsConexidade(int n, vector<aresta>* LA) {              
    int* cor = new int[n];  // Vetor de cores dos vértices
    queue<int> fila;        // Fila de vértices visitados durante a exploração do grafo
    int s = 0;              // Vértice inicial da busca
    
    // Inicializando todos os vértices com a cor branca
    for(int i = 0; i < n; i++) {
        cor[i] = BRANCO;    
    }

    fila.push(s);           // Adicionando o vértice de origem à fila para iniciar a busca
    cor[s] = CINZA;          // Vértice de origem descoberto

    while(!fila.empty()) {
        int u = fila.front();   // Auxiliar u recebe o 1° vértice da fila
        cor[u] = PRETO;         // Fechando o vértice u
        fila.pop();             // Retirando o vértice u da fila

        // Percorrendo a lista de adjacência de u e procurando por vértices brancos
        for(auto v: LA[u]) {
            if(cor[v.vertice] == BRANCO) {
                cor[v.vertice] = CINZA;   // Vértice v descoberto
                fila.push(v.vertice);     // Vértice v adicionado à fila
            }
        }
    }

    return cor;   
}

// Verifica a conexidade do grafo
// Retorna 1 se for conexo (nao_direcionado) ou fracamente conexo (direcionado) ou 0 se for desconexo
int conexidadeGrafo(int n, enum TipoGrafo tipo, vector<aresta>* LA) {
    int* cor = NULL;    // Vetor de cores dos vértices

    // Se o grafo for direcionado, para cada aresta em (u,v) é adicionada uma aresta (v,u), caso não exista
    if(tipo == direcionado) {
        vector<aresta>* LA_Aux = new vector<aresta>[n];   // Lista de adjacência auxiliar para verificar conexidade fraca
        
        for(int u = 0; u < n; u++) {
            for(auto v: LA[u]) {
                LA_Aux[u].push_back(v);   // Adicionando a aresta (u,v) na LA_Aux
                // Verificando se já existe a aresta (v,u) na LA
                for(auto i: LA[v.vertice]) {
                    if(i.vertice == u) {
                        continue;
                    }
                }
                LA_Aux[v.vertice].push_back({v.id, u, v.peso});     // Criando a aresta (v,u) na LA_Aux
            }
        }
        cor = bfsConexidade(n, LA_Aux);    // Passando a LA_Aux como parâmetro para a BFS em caso de grafos direcionados
    }

    if(tipo == nao_direcionado) {
        cor = bfsConexidade(n, LA);        // Passando a LA como parâmetro para a BFS em caso de grafos nao_direcionados
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


// Verifica se o vértice é bipartido e retorna 1 caso seja verdadeiro ou 0 caso contrário
int bipartido(int n, vector<aresta>* LA) {
    int* corBFS = new int[n];       // Vetor de cores dos vértices indicando seu estado durante a BFS
    int* coloracao = new int[n];    // Vetor de cores dos vértices indicando sua partição através de sua coloração   
    int auxColoracao = AZUL;        // Variável auxiliar para colorir o grafo
    queue<int> fila;                // Fila de vértices explorados durante a BFS
    int s = 0;                      // Vértice inicial da busca

    // Inicializando todos os vértices como brancos
    for(int i = 0; i < n; i++) {
        corBFS[i] = BRANCO;
        coloracao[i] = BRANCO;
    }

    // Utilizando uma BFS para verificar se o grafo é bipartido
    fila.push(s);                   // Adicionando o vértice inicial à fila
    corBFS[s] = CINZA;              // Vértice s descoberto
    coloracao[s] = auxColoracao;    // Vértice s colorido

    while(!fila.empty()) {
        int u = fila.front();   // Auxiliar u recebe o 1° vértice da fila
        corBFS[u] = PRETO;      // Fechando o vértice u
        fila.pop();             // Retirando o vértice u da fila

        // Alterando a cor de auxColoracao
        if(coloracao[u] == VERMELHO) {
            auxColoracao = AZUL;
        } else {
            auxColoracao = VERMELHO;
        }

        // Percorrendo a LA de u e procurando por vértices brancos
        for(auto v: LA[u]) {
            if(corBFS[v.vertice] == BRANCO) {
                corBFS[v.vertice] = CINZA;                  // Vértice v descoberto
                coloracao[v.vertice] = auxColoracao;        // Colorindo o vértice v 
                fila.push(v.vertice);                       // Adicionando v à fila
            } else {
                // Verificando se v tem a mesma cor de u
                if(coloracao[v.vertice] == coloracao[u]) {
                    return 0;
                }
            }
        }
    }

    // Verificando se existem vértices não coloridos, indicando mais de duas partições
    for(int i = 0; i < n; i ++) {
        if(coloracao[s] == BRANCO) {
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
    vector<aresta>* LA = new vector<aresta>[n]; 

    // Lendo as arestas do grafo
    LA = lerGrafo(n, m, tipo, LA);

    // Verificando a conexidade do grafo
    int conexidade = conexidadeGrafo(n, tipo, LA);
    cout << conexidade << endl;

    // Verificando se um grafo é bipartido
    int ehBipartido = bipartido(n, LA);
    cout << ehBipartido << endl;

    return 0;
}