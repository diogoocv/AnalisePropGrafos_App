/*
 *  GCC 218 - Algoritmos em Grafos
 *  Trabalho Prático
 * 
 *  Diogo Oliveira Carvalho - 202120533
 *  Lucas Meira
 *  Bruno
 * 
*/

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iterator>
#include <stack>

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
int* bfsConexidade(int s, int* cor, vector<aresta>* LA) {              
    queue<int> fila;        // Fila de vértices visitados durante a exploração do grafo

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

// Verifica, através de uma BFS, a conexidade do grafo
// Retorna 1 se for conexo (nao_direcionado) ou fracamente conexo (direcionado) ou 0 se for desconexo
int conexidadeGrafo(int n, enum TipoGrafo tipo, vector<aresta>* LA) {
    int* cor = new int[n];  // Vetor de cores dos vértices
    int s = 0;              // Vértice de origem da BFS

    // Inicializando todos os vértices com a cor branca
    for(int i = 0; i < n; i++) {
        cor[i] = BRANCO;    
    }

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
        cor = bfsConexidade(s, cor, LA_Aux);    // Passando a LA_Aux como parâmetro para a BFS em caso de grafos direcionados
    }

    if(tipo == nao_direcionado) {
        cor = bfsConexidade(s, cor, LA);        // Passando a LA como parâmetro para a BFS em caso de grafos nao_direcionados
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


// Verifica, através de uma BFS, se um grafo nao_direcionado é bipartido e retorna 1 caso seja verdadeiro ou 0 caso contrário
// Se o grafo não for nao_direcionado, retorna 0
int bipartido(int n, enum TipoGrafo tipo, vector<aresta>* LA) {
    int* corBFS = new int[n];       // Vetor de cores dos vértices indicando seu estado durante a BFS
    int* coloracao = new int[n];    // Vetor de cores dos vértices indicando sua partição através de sua coloração   
    int auxColoracao = AZUL;        // Variável auxiliar para colorir o grafo
    queue<int> fila;                // Fila de vértices explorados durante a BFS
    int s = 0;                      // Vértice inicial da busca


    if(tipo != nao_direcionado) {
        return 0;
    }

    // Inicializando todos os vértices como brancos
    for(int i = 0; i < n; i++) {
        corBFS[i] = BRANCO;
        coloracao[i] = BRANCO;
    }

    // Utilizando uma BFS e coloração para verificar se o grafo é bipartido
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

    // Verificando se existe um vértice não colorido, indicando mais de duas partições
    for(int i = 0; i < n; i ++) {
        if(coloracao[i] == BRANCO) {
            return 0;
        }
    }

    return 1;
}

// Verifica se um grafo é euleriano, retornando 1 caso afirmativo ou 0 caso contrário
int euleriano(int n, enum TipoGrafo tipo, vector<aresta>* LA) {
    
    // Verificando se o grafo é desconexo
    if(conexidadeGrafo(n, tipo, LA) == 0) {
        return 0;
    }
    
    // Percorrendo a LA
    for(int u = 0; u < n; u++) {
        int grau = 0;
        
        grau = LA[u].size();    // Calculando o grau do vértice u

        // Calculando o grau de entrada de u para vértices direcionados
        if(tipo == direcionado) {
            int grauEntrada = 0;

            for(int j = 0; j < n; j++) {
                for(auto k : LA[j]) {
                    if(k.vertice == u) {
                        grauEntrada++;
                    }
                }
            }

            // Verificando se o vértice possui grau de entrada != grau saída (grafo direcionado não é euleriano)
            if(grau != grauEntrada) {
                return 0;
            }
        } else {
            // Verificando se o grau do vértice não é par (grafo nao_direcionado não é euleriano)
            if(grau % 2 != 0) {
                return 0;
            }
        }
    }

    return 1;
}

// Busca em profundidade recursiva para verificar se um grafo possui ciclo
// Retorna 1 caso verdadeiro ou 0 caso contrário
int dfsCiclo(int n, int u, int* cor, int* pai, vector<aresta>* LA) {
    cor[u] = CINZA;     // Vértice u descoberto

    // Percorrendo a vizinhança de u
    for(auto v: LA[u]) {
        if(cor[v.vertice] == BRANCO) {
            pai[v.vertice] = u;         // Pai de v é u
            // Chamando a dfs para v
            if(dfsCiclo(n, v.vertice, cor, pai, LA) == 1) {
                return 1;
            }  
        } else {
            // Se for encontrado um vértice cinza diferente do pai de u, o grafo possui ciclo
            if(cor[v.vertice] == CINZA and v.vertice != pai[u]) {
                return 1;
            }
        }
    }

    cor[u] = PRETO;      // Vértice fechado

    return 0;
}

// Verifica, através de uma DFS, se o grafo possui ciclo, retornando 1 caso verdadeiro ou 0 caso contrário
int ciclo(int n, vector<aresta>* LA) {
    int* cor = new int[n];  // Vetor de cores dos vértices indicando seu estado durante a DFS
    int* pai = new int[n];  // Armazena o pai de cada vértice
    stack<int> pilha;       // Pilha de vértices visitados durante a exploração do grafo

    // Inicializando as estruturas auxiliares
    for(int i = 0; i < n; i++) {
        cor[i] = BRANCO;
        pai[i] = -1;
    }

    // Executando a DFS para todos os vértices brancos
    for(int u = 0; u < n; u++) {
        if(cor[u] == BRANCO) {
            // Se a dfsCiclo retornar 1, existe um ciclo no grafo
            if(dfsCiclo(n, u, cor, pai, LA) == 1) {
                return 1;
            }
        }
    }

    return 0;
}

// Calcula a quantidade de componentes conexas no grafo através de uma BFS (apenas grafos nao_direcionados)
// Retorna a quantidade de componentes conexas ou -1 caso o grafo não seja nao_direcionado
int qtdCompConexas(int n, enum TipoGrafo tipo, vector<aresta>* LA) {
    int contComponentes = 0;    // Contador para verificar a quantidade de componentes no grafo
    int* cor = new int[n];      // Vetor de cores dos vértices
    int s = 0;                  // Vértice de origem da BFS

    // Verificando se o grafo não é nao_direcionado  
    if(tipo != nao_direcionado) {
        return -1;
    } 

    // Inicializando todos os vértices com a cor branca
    for(int i = 0; i < n; i++) {
        cor[i] = BRANCO;    
    }

    // Executando uma BFS até que todos os vértices sejam encontrados
    bool verticeNaoVisitado = true;
    while(verticeNaoVisitado) {
        cor = bfsConexidade(s, cor, LA);        // Rodando uma BFS a partir do 1° vértice não visitado (ordem lexicográfica)
        contComponentes++;                      // Incrementando a quantidade de componentes conexas

        // Percorrendo o vetor de cores e verificando se existem vértices brancos
        for(int u = 0; u < n; u++) {
            if(cor[u] == BRANCO) {
                s = u;
                verticeNaoVisitado = true;
                u = n;
            } else {
                verticeNaoVisitado = false;
            }
        }
    }

    return contComponentes;
}

// DFS para o tempo de fechamento dos vértices durante a busca para encontrar a quantidade de componentes fortemente conexas
void dfsCompFortConexas(int s, int &t, int* cor, int* f, vector<aresta>* LA) {
    stack<int> pilha;   // Pilha de vértices descobertos durante a busca

    cor[s] = CINZA;     // Vértice s descoberto
    pilha.push(s);      // Empilhando s

    // Executando a DFS na LA
    while(!pilha.empty()) {
        bool vBrancoEncontrado = false;     // Auxiliar para armazenar se um vértice branco foi encontrado na LA[u]
        int u = pilha.top();                // u recebe o vértice do topo da pilha 
        
        // Percorrendo a LA de u
        for(auto v: LA[u]) {
            if(cor[v.vertice] == BRANCO) {
                t++;                        // Incrementando o tempo da busca
                cor[v.vertice] = CINZA;     // Vértice v descoberto
                pilha.push(v.vertice);      // Empilhando v 
                vBrancoEncontrado = true; 
            }
        }

        // Verificando se não foram encontrados vizinhos não visitados na LA de u
        if(!vBrancoEncontrado) {
            t++;                // Incrementando o tempo da busca
            cor[u] = PRETO;     // Vértice u fechado
            f[u] = t;           // Tempo de fechamento de u
            pilha.pop();        // Retirando u da pilha
        }   
    }
}

// Calcula a quantidade de componentes fortemente conexas no grafo através de uma DFS (apenas grafos direcionados)
// Retorna a quantidade de componentes fortemente conexas ou -1 caso o grafo não seja direcionado
int qtdCompFortConexas(int n, enum TipoGrafo tipo, vector<aresta>* LA) {
    vector<aresta>* LA_Transposta = new vector<aresta>[n];      // Grafo transposto do grafo passado (LA)
    int* cor = new int[n];      // Vetor de cores dos vértices
    int* f = new int[n];        // Armazena o tempo de fechamento dos vértices durante a DFS   
    int s = 0;                  // Vértice de origem da busca
    int t = 0;                  // Auxiliar que armazena o tempo da busca
    int contComp = 0;               // Contador de componentes

    // Verificando se o grafo não é direcionado
    if(tipo != direcionado) {
        return -1;
    }

    // Calculando o grafo transposto
    for(int u = 0; u < n; u++) {
        for(auto v: LA[u]) {
            LA_Transposta[v.vertice].push_back({v.id, u, v.peso});
        }
    } 

    // Inicializando todos os vértices como brancos
    for(int i = 0; i < n; i++) {
        cor[i] = BRANCO;
    }

    // Rodando a DFS na LA para descobrir os tempos de fechamento
    for(int u = 0; u < n; u++) {
        if(cor[u] == BRANCO) {
            s = u;
            dfsCompFortConexas(s, t, cor, f, LA);
        }
    }
    
    // Re-inicializando todos os vértices como brancos
    for(int i = 0; i < n; i++) {
        cor[i] = BRANCO;
    }


    // Executando a DFS NA LA_Transposta por ordem decrescente de fechamento
    bool vBrancoEncontrado = true;
    while(vBrancoEncontrado) {
        vBrancoEncontrado = false;
        int maiorT = -1;            // Armazena o maior tempo de fechamento entra os vértices brancos

        // Procurando pelo vértice branco com maior tempo de fechamento
        for(int u = 0; u < n; u++) {
            if(cor[u] == BRANCO) {
            vBrancoEncontrado = true;
                if(f[u] > maiorT) {
                    maiorT = f[u];      // Atualizando o maior tempo de fechamento
                    s = u;              // u será o vértice de origem da DFS
                }
            }
        }

        // Executando a DFS a partir de s
        if(vBrancoEncontrado) {
            dfsCompFortConexas(s, t, cor, f, LA_Transposta);
            contComp++;
        }
    }

    return contComp;
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

    // Retornando se um grafo é conexo
    int conexidade = conexidadeGrafo(n, tipo, LA);
    cout << conexidade << endl;

    // Retornando se um grafo é bipartido
    int ehBipartido = bipartido(n, tipo, LA);
    cout << ehBipartido << endl;

    // Retornando se um grafo é euleriano
    int ehEuleriano = euleriano(n, tipo, LA);
    cout << ehEuleriano << endl;

    // Retornando se um grafo possui ciclo
    int possuiCiclo = ciclo(n, LA);
    cout << possuiCiclo << endl;

    // Retornando a quantidade de componentes conexas de um grafo
    int compConexas = qtdCompConexas(n, tipo, LA);
    cout << compConexas << endl;

    // Retornando a quantidade de componentes fortemente conexas de um grafo
    int compFortConexas = qtdCompFortConexas(n, tipo, LA);
    cout << compFortConexas;

    return 0;
}