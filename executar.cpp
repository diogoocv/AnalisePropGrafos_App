/*
 *  GCC 218 - Algoritmos em Grafos
 *  Trabalho Prático
 * 
 *  DIOGO OLIVEIRA CARVALHO - 202120533
 *  LUCAS SILVA MEIRA - 202120807
 *  BRUNO DE ALMEIDA DE PAULA
 * 
*/

#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <queue>
#include <stack>

using namespace std;

#define NAO_VISITADO -1     // Vértice não visitado

// Cores que representam o estado dos vértices durante uma busca
#define BRANCO 0     // Vértice não descoberto
#define CINZA 1      // Vértice descoberto
#define PRETO 2      // Vértice fechado

// Cores utilizadas em problemas de coloração de grafos
#define AZUL 3
#define VERMELHO 4  

#define INFINITO 10000000     // Definindo um valor muito grande para ser considerado "infinito"

// Representa uma aresta em uma lista de adjacência
struct aresta{
    int id;             // Código identificador única da aresta
    int u;              // Primeiro vértice a quem a aresta se liga
    int v;              // Segundo vértice a quem a aresta se liga 
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
        if(u < 0 or u >= n or v < 0 or v >= n) {
            continue;
        }

        // Evitando a leitura de arestas com id já existente
        for(int j = 0; j < n; j++) {
            for(auto k: LA[j]) {
                if(k.id == id) {
                    continue;
                }
            }
        }

        // Evitando a leitura de arestas com id inválido (menor que 0 ou maior que o número de arestas)
        if(id < 0 or id > m) {
            continue;
        }

        // Evitando a leitura de arestas repetidas
        for(auto i: LA[u]) {
            if(i.v == v) {
                continue;
            }
        }

        // Adicionando as arestas à lista de adjacência
        LA[u].push_back({id, u, v, w});         // Adicionando a aresta (u,v)
        if(tipo == nao_direcionado) {
            LA[v].push_back({id, v, u, w});     // Adicionando a aresta (v,u)
        }  
    }

    return LA;  // Retornando a lista de adjacência lida
}

// Imprime uma lista de inteiros separados por um espaço e com uma quebra de linha ao final da lista
// Imprime -1 caso não haja nenhum valor na lista
void imprimirListaInt(int qtdValores, int* Lista) {
    if(qtdValores > 0 and Lista != NULL) {
        cout << Lista[0];
        if(qtdValores > 1) {
            for(int i = 1; i < qtdValores; i++) {
                cout << " " << Lista[i];
            }
        }
    } else {
        cout << -1;
    }
    cout << endl;
}

// Imprime os valores presentes em uma fila de inteiros
// Imprime -1 caso a fila esteja vazia
void imprimirFilaInt(queue<int> fila) {
    if(!fila.empty()) {
        cout << fila.front();
        fila.pop();
        while(!fila.empty()) {
            cout << " " << fila.front();
            fila.pop();
        }
    } else {
        cout << -1;
    }
    cout << endl;
}

// Busca em Largura utilizada para verificar a conexidade do grafo
void bfsConexidade(int s, int* cor, const vector<aresta>* LA) {              
    queue<int> fila;        // Fila de vértices visitados durante a exploração do grafo

    fila.push(s);           // Adicionando o vértice de origem à fila para iniciar a busca
    cor[s] = CINZA;          // Vértice de origem descoberto

    while(!fila.empty()) {
        int u = fila.front();   // Auxiliar u recebe o 1° vértice da fila
        cor[u] = PRETO;         // Fechando o vértice u
        fila.pop();             // Retirando o vértice u da fila

        // Percorrendo a lista de adjacência de u e procurando por vértices brancos
        for(auto uv: LA[u]) {
            if(cor[uv.v] == BRANCO) {
                cor[uv.v] = CINZA;   // Vértice v descoberto
                fila.push(uv.v);     // Vértice v adicionado à fila
            }
        }
    }  
}

// Verifica, através de uma BFS, a conexidade do grafo
// Retorna 1 se for conexo (nao_direcionado) ou fracamente conexo (direcionado) ou 0 se for desconexo
int conexidadeGrafo(int n, enum TipoGrafo tipo, const vector<aresta>* LA) {
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
            for(auto uv: LA[u]) {
                LA_Aux[u].push_back(uv);   // Adicionando a aresta (u,v) na LA_Aux

                // Verificando se já existe a aresta (v,u) na LA
                bool vuExiste = false;
                for(auto i: LA[uv.v]) {
                    if(i.v == u) {
                        vuExiste = true;
                    }
                }

                if(!vuExiste) {
                    LA_Aux[uv.v].push_back({uv.id, uv.v, u, uv.peso});     // Criando a aresta (v,u) na LA_Aux
                }
                
            }
        }
        bfsConexidade(s, cor, LA_Aux);    // Passando a LA_Aux como parâmetro para a BFS em caso de grafos direcionados

        delete[] LA_Aux;
    }

    if(tipo == nao_direcionado) {
        bfsConexidade(s, cor, LA);        // Passando a LA como parâmetro para a BFS em caso de grafos nao_direcionados
    }

    // Percorrendo a lista de cores dos vértices
    for(int i = 0; i < n; i++) {
        // Se algum vértice for branco, o grafo é desconexo
        if (cor[i] == BRANCO) {
            return 0;
        }
    }

    delete[] cor;

    return 1;
}


// Verifica, através de uma BFS, se um grafo nao_direcionado é bipartido e retorna 1 caso seja verdadeiro ou 0 caso contrário
// Se o grafo não for nao_direcionado, retorna 0
int bipartido(int n, enum TipoGrafo tipo, const vector<aresta>* LA) {
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
        for(auto uv: LA[u]) {
            if(corBFS[uv.v] == BRANCO) {
                corBFS[uv.v] = CINZA;                  // Vértice v descoberto
                coloracao[uv.v] = auxColoracao;        // Colorindo o vértice v 
                fila.push(uv.v);                       // Adicionando v à fila
            } else {
                // Verificando se v tem a mesma cor de u
                if(coloracao[uv.v] == coloracao[u]) {
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

    delete[] corBFS;
    delete[] coloracao;

    return 1;
}

// Verifica se um grafo é euleriano, retornando 1 caso afirmativo ou 0 caso contrário
int euleriano(int n, enum TipoGrafo tipo, int conexidade, const vector<aresta>* LA) {
    
    // Verificando se o grafo é desconexo
    if(conexidade == 0) {
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
                    if(k.v == u) {
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
int dfsCiclo(int n, int u, int* cor, int* pai, const vector<aresta>* LA) {
    cor[u] = CINZA;     // Vértice u descoberto

    // Percorrendo a vizinhança de u
    for(auto uv: LA[u]) {
        if(cor[uv.v] == BRANCO) {
            pai[uv.v] = u;         // Pai de v é u
            // Chamando a dfs para v
            if(dfsCiclo(n, uv.v, cor, pai, LA) == 1) {
                return 1;
            }  
        } else {
            // Se for encontrado um vértice cinza diferente do pai de u, o grafo possui ciclo
            if(cor[uv.v] == CINZA and uv.v != pai[u]) {
                return 1;
            }
        }
    }

    cor[u] = PRETO;      // Vértice fechado

    return 0;
}

// Verifica, através de uma DFS, se o grafo possui ciclo, retornando 1 caso verdadeiro ou 0 caso contrário
int ciclo(int n, const vector<aresta>* LA) {
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

    delete[] cor;
    delete[] pai;
 
    return 0;
}

// Calcula a quantidade de componentes conexas no grafo através de uma BFS (apenas grafos nao_direcionados)
// Retorna a quantidade de componentes conexas ou -1 caso o grafo não seja nao_direcionado
int qtdCompConexas(int n, enum TipoGrafo tipo, const vector<aresta>* LA) {
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
        bfsConexidade(s, cor, LA);              // Rodando uma BFS a partir do 1° vértice não visitado (ordem lexicográfica)
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

    delete[] cor;

    return contComponentes;
}

// DFS para o tempo de fechamento dos vértices durante a busca para encontrar a quantidade de componentes fortemente conexas
void dfsCompFortConexas(int s, int& t, int* cor, int* f, const vector<aresta>* LA) {
    stack<int> pilha;   // Pilha de vértices descobertos durante a busca

    cor[s] = CINZA;     // Vértice s descoberto
    pilha.push(s);      // Empilhando s

    // Executando a DFS na LA
    while(!pilha.empty()) {
        bool vBrancoEncontrado = false;     // Auxiliar para armazenar se um vértice branco foi encontrado na LA[u]
        int u = pilha.top();                // u recebe o vértice do topo da pilha 
        
        // Percorrendo a LA de u
        for(auto uv: LA[u]) {
            if(cor[uv.v] == BRANCO) {
                t++;                   // Incrementando o tempo da busca
                cor[uv.v] = CINZA;     // Vértice v descoberto
                pilha.push(uv.v);      // Empilhando v 
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
int qtdCompFortConexas(int n, enum TipoGrafo tipo, const vector<aresta>* LA) {
    vector<aresta>* LA_Transposta = new vector<aresta>[n];      // Grafo transposto do grafo passado (LA)
    int* cor = new int[n];      // Vetor de cores dos vértices
    int* f = new int[n];        // Armazena o tempo de fechamento dos vértices durante a DFS   
    int s = 0;                  // Vértice de origem da busca
    int t = 0;                  // Auxiliar que armazena o tempo da busca
    int contComp = 0;           // Contador de componentes

    // Verificando se o grafo não é direcionado
    if(tipo != direcionado) {
        return -1;
    }

    // Calculando o grafo transposto
    for(int u = 0; u < n; u++) {
        for(auto uv: LA[u]) {
            LA_Transposta[uv.v].push_back({uv.id, uv.v, u, uv.peso});
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

    delete[] cor;
    delete[] f;

    return contComp;
}

// Algoritmo de Tarjan para detecção de arestas ponte e vértices de articulação
void tarjan(int u, int* d, int* low, int* pai, int& t, int& qtdFilhosRaiz, const int raiz, int& contArtic, int& contPontes, const vector<aresta>* LA, bool* articulacoes, bool* pontes) {
    d[u] = t;       // Tempo de descoberta de s = t
    low[u] = t;     // Low de s = t
    t++;            // Incrementando o tempo da busca

    // Percorrendo a lista de adjacência de u
    for(auto uv: LA[u]) {
        if(u == raiz) {
            qtdFilhosRaiz++;        // Incrementando a quantidade de filhos da raiz
        }

        if(d[uv.v] == NAO_VISITADO) {
            // Visitando vértice u
            pai[uv.v] = u;

            // Executando Tarjan para o vértice v (em profundidade)
            tarjan(uv.v, d, low, pai, t, qtdFilhosRaiz, raiz, contArtic, contPontes, LA, articulacoes, pontes);

            // Verificando se u é um vértice de articulação
            // Um vértice é articulação quando ele não é a raiz e seu tempo de descoberta é menor ou igual ao low de seu filho
            if(low[uv.v] >= d[u] and u != raiz) {
                // Verificando se u não foi marcado como articulação
                if(articulacoes[u] == false) {
                    articulacoes[u] = true;     // Adicionando o vértice u no vetor de articulações
                    contArtic++;                // Incrementando o contador de articulações
                }
            }

            // Verificando se (u,v) é uma aresta ponte
            if(low[uv.v] > d[u]) {
                pontes[uv.id] = true;       // Adicionando a aresta no vetor de pontes
                contPontes++;               // Incrementando o contador de pontes
            }

            low[u] = min(low[u], low[uv.v]);       // Atualizando o low de u caso o low de v seja menor que o dele
        } else {
            // Verificando se o vértice encontrado não é o pai de u
            if(uv.v != pai[u]) {
                low[u] = min(low[u], d[uv.v]);     // Atualizando o low de u caso seu low seja maior que o tempo de descoberta de v e v não seja seu pai
            }
        }
    }
}

// Verifica os vértices de articulação e arestas ponte do grafo (apenas nao_direcionado)
// Calcula-os através do algoritmo de Tarjan
void articulacoesEPontes(int n, int m, enum TipoGrafo tipo, int& contArtic, int& contPontes, int** verticesArticulacao, int** idArestasPonte, const vector<aresta>* LA) {    
    
    // Verificando se o grafo é nao_direcionado
    if(tipo == nao_direcionado) {
        int* low = new int[n];              // Vetor que armazena os menores tempos de descoberta de arestas que abracam um vertice
        int* d = new int[n];                // Vetor que armazena o tempo de descoberta do vértice
        int* pai = new int[n];              // Vetor que armazena o pai de cada vértice
        int t = 0;                          // Auxiliar que armazena o tempo da busca
        bool* articulacoes = new bool[n];   // Armazena os vértices de articulação do grafo
        bool* pontes = new bool[m];         // Armazena as arestas ponte do grafo

        contArtic = 0;                      // Auxiliar para contar o número de vértices de articulação
        contPontes = 0;                     // Auxiliar para contar o número de arestas ponte

        // Inicializando as estruturas necessárias para encontrar as articulações
        for(int i = 0; i < n; i++) {
            low[i] = NAO_VISITADO;
            d[i] = NAO_VISITADO;
            pai[i] = -1;
            articulacoes[i] = false;
        }

        for(int i = 0; i < m; i++) {
            pontes[i] = false;
        }

        // Executando o algoritmo de Tarjan até que todos os vértices sejam visitados
        for(int i = 0; i < n; i++) {
            if(d[i] == NAO_VISITADO) {
                int qtdFilhosRaiz = 0;              // Quantidade de filhos da raiz da busca
                int raiz = i;                       // Vértice raiz da busca
                tarjan(i, d, low, pai, t, qtdFilhosRaiz, raiz, contArtic, contPontes, LA, articulacoes, pontes);
                
                // Verificando se a raiz é um vértice de articulação
                // A raiz é um vértice de articulação quando possui 2 ou mais filhos
                if(qtdFilhosRaiz > 1) {
                    articulacoes[i] = true;
                    contArtic++;
                }
            }
        }

        // Armazenando no vetor passado por referência os vértices de articulação 
        if(contArtic > 0) {
            *verticesArticulacao = new int[contArtic-1];
            int auxCont = 0;
            for(int i = 0; i < n; i++) {
                if(articulacoes[i]) {
                    (*verticesArticulacao)[auxCont] = i;
                    auxCont++;
                }
            }
        }
        // Armazenando no vetor passado por referência os id's das arestas ponte
        if(contPontes > 0) {
            *idArestasPonte = new int[contPontes-1];
            int auxCont = 0;
            for(int i = 0; i < m; i++) {
                if(pontes[i]) {
                    (*idArestasPonte)[auxCont] = i;
                    auxCont++;
                }
            }
        }

        delete[] low;
        delete[] d;
        delete[] pai;
        delete[] articulacoes;
        delete[] pontes;
    }
}

// Função auxiliar para visitar os vértices em profundidade e adicionar à fila as arestas da árvore da busca
void dfsArvoreVisit(int u, int* cor, queue<int>& filaArvore, const vector<aresta>* LA) {
    cor[u] = CINZA;         // Visitando u

    // Percorrendo a vizinhança de u e procurando por vértices não visitados
    for(auto uv: LA[u]) {
        if(cor[uv.v] == BRANCO) {
            filaArvore.push(uv.id);                          // Adicionando à fila o id da aresta uv
            dfsArvoreVisit(uv.v, cor, filaArvore, LA);       // Chamando a DFS para o vértice v
        }
    }

    cor[u] = PRETO;     // Fechando u
}

// DFS para calcular a árvore em profundidade do grafo
// Retorna uma fila contendo o id das arestas presentes na árvore em profundidade partindo do vértice 0
queue<int> dfsArvore(int n, const vector<aresta>* LA) {
    int* cor = new int[n];      // Cor dos vértices durante a busca
    queue<int>  filaArvore;     // Fila que armazena o id das arestas pertencentes à arvore de profundidade
    int s = 0;                  // Vértice de origem da busca

    // Inicializando o vetor de cores
    for(int i = 0; i < n; i++) {
        cor[i] = BRANCO;
    }

    // Executando uma busca em profundidade a partir do vértice s
    dfsArvoreVisit(s, cor, filaArvore, LA);

    delete[] cor;

    return filaArvore;
}    

// BFS para calcular a árvore em largura do grafo
// Retorna uma fila contendo o id das arestas presentes na árvore em largura partindo do vértice 0
queue<int>bfsArvore(int n, const vector<aresta>* LA) {
    int* cor = new int[n];      // Cor dos vértices durante a busca
    queue<int> filaBfs;         // Fila que armazena os vértices visitados durante a busca
    queue<int> filaArvore;      // Fila que armazena o id das arestas pertencentes à arvore de largura
    int s = 0;                  // vértice de origem da busca

    // Inicializando o vetor de cores
    for(int i = 0; i < n; i++) {
        cor[i] = BRANCO;
    }

    cor[s] = CINZA;     // Visitando s
    filaBfs.push(s);    // Enfileirando s

    while(!filaBfs.empty()) {
        int u = filaBfs.front();

        // Percorrendo a vizinhança de u e procurando por vértices não visitados
        for(auto uv: LA[u]) {
            if(cor[uv.v] == BRANCO) {
                cor[uv.v] = CINZA;          // Visitando uv
                filaBfs.push(uv.v);         // Adicionando o vértice v à fila da busca
                filaArvore.push(uv.id);     // Adicionando o id de uv à árvore de largura
            }
        }

        cor[u] = PRETO;     // Fechando o vértice u
        filaBfs.pop();      // Retirando u da fila
    }

    delete[] cor;

    return filaArvore;
}

// Função auxiliar de agmPrim()
// Retorna a o vértice com a menor chave entre os vértices não pertencentes à árvore geradora mínima parcial 
int obtemMenorChave(int* chave, bool* pertence_agm, int n) {
    int menorChave = INFINITO;      // Armazena a menor chave encontrada
    int u = -1;                      // Vértice não pertencente à AGM com a menor chave

    // Verificando, para todos os vértices, se sua chave é menor que a menor chave encontrada até então
    for(int v = 0; v < n; v++) {
        if(!pertence_agm[v] and menorChave > chave[v]) {
            menorChave = chave[v];
            u = v;
        }
    }
    return u;
}

// Retorna uma lista com o id das arestas presentes na árvore geradora mínima
// Retorna -1 caso o grafo não seja nao_direcionado ou seja desconexo
int agmPrim(int n, enum TipoGrafo tipo, int conexidade, const vector<aresta>* LA) {
    
    // Verificando se o grafo não é nao_direcionado
    if(tipo != nao_direcionado) {
        return -1;
    }
    // Verificando se o grafo é desconexo
   if(conexidade == 0) {
        return -1;
    }

    int* chave = new int[n];                // Chave[v] = peso mínimo de uma aresta que conecta v a outro vértice na árvore 
    int* pai = new int[n];                  // Vetor de pais dos vértices
    bool* pertence_agm = new bool[n];       // Vetor que armazena true para os vértices já adicionados à arvore geradora mínima
    int r = 0;                              // Raiz da árvore  
    int verticesNaoExp = n;                 // Auxiliar que guarda a quantidade de vértices não explorados

    // Inicializando as estruturas auxiliares
    for(int i = 0; i < n; i++) {
        chave[i] = INFINITO;
        pertence_agm[i] = false;
        pai[i] = -1;
    }

    // Inicializando a raiz
    chave[r] = 0;   
    pai[r] = r;

    // Explorando todos os vértices, com prioridade para os que possuem menor chave entre os não visitados
    while(verticesNaoExp > 0) {
        int u = obtemMenorChave(chave, pertence_agm, n);    // Próximo vértice a ser explorado
        verticesNaoExp--;                               // Decrementando a quantidade de vértices não explorados
        pertence_agm[u] = true;                         // Vértice adicionado à AGM

        // Percorrendo a lista de adjacência de u para tentar atualizar as chaves de sua vizinhança 
        for(auto uv: LA[u]) {
            if(!pertence_agm[uv.v]) {
                // Verificando se o peso da aresta uv é maior que a chave de v
                if(uv.peso < chave[uv.v]) {
                    chave[uv.v] = uv.peso;          // Atualizando a chave de v 
                    pai[uv.v] = u;                  // Atualizando o pai de v
                }
            }
        }
    }

    // Somando o peso das arestas da AGM
    int valorArvore = 0;
    for(int i = 1; i < n; i++) {
        if(pai[i] != -1) {
            valorArvore += chave[i];
        }
    }

    delete[] chave;
    delete[] pai;
    delete[] pertence_agm;

    return valorArvore;
}

// Algoritmo de Kahn para calcular a ordenação topológica dos vértices priorizando a ordem lexicográfica
// Retorna uma fila com os vértices na ordem topológica
queue<int> ordenacaoTopologicaKahn(int n ,enum TipoGrafo tipo, int possuiCiclo, const vector<aresta>* LA) {
    queue<int> filaOrdem;       // Armazena os vértices em ordem topológica

    // Verificando se o grafo não é direcionado
    if(tipo != direcionado) {
        return filaOrdem;
    }

    // Verificando se o grafo possui ciclo
    if(possuiCiclo == 1) {
        return filaOrdem;
    }

    // inicializando o vetor de graus de entrada e o vetor de cores
    int* grauEntrada = new int[n];
    for(int i = 0; i < n; i++) {
        grauEntrada[i] = 0;
    }

    // Calculando grau de entrada dos vértices
    for(int u = 0; u < n; u++) {
        for(auto uv: LA[u]) {
            grauEntrada[uv.v]++;        // Incrementando o grau de entrada de v
        }
    }

    // Procurando por vértices com grau 0 para adicionar à fila de ordem topológica
    int contVert = 0;        // Auxiliar para contar o número de vértices adicionados à fila
    while(contVert < n) {
        for(int u = 0; u < n; u++) {
            if(grauEntrada[u] == 0) {
                filaOrdem.push(u);     // Adicionando u à fila
                // Decrementando o grau de entrada da vizinhança de u
                for(auto uv: LA[u]) {
                    grauEntrada[uv.v]--;
                }

                grauEntrada[u] = -1;    // Fechando o vértice u
                contVert++;             // Incrementando o contador de vértices na fila

                u = n;                  // Finalizando esta iteração do for
            }
        }
    }

    return filaOrdem;
}

// Algoritmo de Bellman Ford para calcular o caminho mínimo entre dois vértices
// Retorna o valor do caminho mínimo
int caminhoMinimoBellmanFord(int n, int s, int t, const vector<aresta>* LA) {
    // 's' é o vertice de origem e 't' o destino

    int* d = new int[n];        // Armazena a distância (soma dos pesos) da origem a cada vértice
    
    // Inicializando o vetor de distâncias
    for(int i = 0; i < n; i++) {
        d[i] = INFINITO;
    }

    d[s] = 0;        // Distância da origem à ela mesma é 0

    // Executando o algoritmo para relaxar as arestas n-1 vezes
    for (int i = 0; i < n-1; i++) {
        // Percorrendo a vizinhança de todos os vértices
        for(int u = 0; u < n; u++) {
            for(auto uv: LA[u]) {
                d[uv.v] = min(d[uv.v], d[u] + uv.peso);     // Tentando relaxar uv
            }
        }
    }

    // d[dest] representa a soma dos pesos do menor caminho da origem ao destino
    int distOrigDestino = d[t];

    delete[] d;

    // Não existe caminho de s até t
    if(distOrigDestino == INFINITO) {
        return -1;
    }

    return distOrigDestino;
}

// Verifica se há um caminho aumentante entre o vértice v e a origem e calcula seu fluxo
void caminhoAumentante(int s, int v, int& fluxo, int capacidade_fluxo, int* pai, int** MatFluxo) {
    // Caso base: vértice passado é a origem
    if(v == s) {
        fluxo = capacidade_fluxo;
    } else {
        if(pai[v] != -1) {
            // Verificando se há um caminho aumentante entre s e o pai[v]
            caminhoAumentante(s, pai[v], fluxo, min(capacidade_fluxo, MatFluxo[pai[v]] [v]), pai, MatFluxo);
            MatFluxo[pai[v]] [v] -= fluxo;       // Subtraindo o fluxo encontrado do arco que liga o pai de v ao próprio v
            MatFluxo[v] [pai[v]] += fluxo;       // Adicionando o fluxo ao arco que liga v ao seu pai para representar o grafo residual
        }
    }
}

// Busca em profundidade para calcular o vetor de pais dos vértices considerando apenas as arestas que possuem fluxo maior que 0
void bfsFluxoMaximo(int n, int s, int t, int* pai, int** MatFluxo, const vector<aresta>* LA) {
    bool* descoberto = new bool[n];     // Armazena se o vértice já foi 
    queue<int> fila;                    // Fila de vértices descobertos na busca

    // Incializando as estruturas auxiliares
    for(int i = 0; i < n; i++) {
        pai[i] = -1;
        descoberto[i] = false;
    }

    // Inicializando o vértice de origem
    descoberto[s] = true;
    fila.push(s);

    // Executando a busca
    while(!fila.empty()) {
        int u = fila.front();   // u recebe o primeiro vértice na fila
        fila.pop();             // retirando u da fila

        // Se u for o vértice de destino, a busca se encerra
        if(u == t) {
            break;
        }

        for(auto uv: LA[u]) {
            // Verificando se v não foi descoberto e a aresta uv possui fluxo
            if(MatFluxo[u][uv.v] > 0 and !descoberto[uv.v]) {
                descoberto[uv.v] = true;        // Visitando v
                fila.push(uv.v);                // Enfileirando v
                pai[uv.v] = u;                  // u é pai de v
            }
        }
    }

    delete[] descoberto;
}

// Algoritmo de Edmonds Karp (Aplicação do algoritmo de Ford Fulkerson) para calcular o fluxo máximo entre dois vértices
// Retorna o valor do fluxo máximo
int fluxoMaximoEdmondsKarp(int n, int s, int t, enum TipoGrafo tipo, const vector<aresta>* LA) {
    // 's' é o vértice de origem e 't' o destino

    // Verificando se o grafo não é direcionado
    if(tipo != direcionado) {
        return -1;
    }

    int** MatFluxo = new int*[n];       // Matriz que armazena o fluxo dos arcos
    int* pai = new int[n];              // Vetor de pais dos vértices
    int fluxoMaximo = 0;                // Armazena o fluxo máximo encontrado


    // Adicionando os pesos iniciais dos arcos à matriz de fluxo
    for(int u = 0; u < n; u++) {
        MatFluxo[u] = new int[n];
        for(auto uv: LA[u]) {
            MatFluxo[u][uv.v] = uv.peso;
        }
    }

    int fluxo = INFINITO;       // Auxiliar que armazena o fluxo encontrado em cada iteração
    // Executando o algoritmo enquanto houverem caminhos aumentantes
    while(fluxo > 0) {
        fluxo = 0;

        bfsFluxoMaximo(n, s, t, pai, MatFluxo, LA);         // Calculando os pais de cada vértice considerando apenas arestas com fluxo maior que 0
        caminhoAumentante(s, t, fluxo, INFINITO, pai, MatFluxo);           // Verificando se há caminho aumentante e calculando seu fluxo

        // Verificando se não há um caminho aumentante
        if (fluxo == 0) {
            break;
        }
        
        fluxoMaximo += fluxo;
    }

    delete[] pai;
    for(int i = 0; i < n; i++) {
        delete[] MatFluxo[i];
    }
    delete[] MatFluxo;


    return fluxoMaximo;
}

// Calcula o fecho transitivo de grafos direcionados através do algoritmo de Warshall
// Retorna uma lista com o fecho transitivo do vértice s
queue<int> fechoWarshall(int n, int s, enum TipoGrafo tipo, const vector<aresta>* LA) {
    queue<int> fechoDeS;            // Fila que armazena o fecho do vértice s 

    // Verificando se o grafo não é direcionado
    if(tipo != direcionado) {
        return fechoDeS;
    }

    int** matFecho = new int*[n];    // Matriz que armazena o fecho transitivo do grafo

    // Inicializando a matriz de fecho transitivo
    for(int u = 0; u < n; u++) {
        matFecho[u] = new int[n];
        
        // Inicializando as posições com 0
        for(int v = 0; v < n; v++) {
            matFecho[u][v] = 0;
        }
    }    
    
    // Adicionando as arestas iniciais do grafo à matriz
    for(int u = 0; u < n; u++) {
        for(auto uv: LA[u]) {
            matFecho[u][uv.v] = 1;
        }
        
    }

    // Calculando o fecho transitivo
    for(int u = 0; u < n; u++) {
        for(int v = 0; v < n; v++) {
            for(int w = 0; w < n; w++) {
                if(matFecho[v][u] == 1 and matFecho[u][w]) {
                    matFecho[v][w] = 1;     // Adicionando (v,w) ao fecho
                }
           }
        }
    }

    // Adicionando o fecho de s à fila
    for(int i = 1; i < n; i++) {
        if(matFecho[s][i] == 1) {
            fechoDeS.push(i);
        } 
    }

    for(int i = 0; i < n; i++) {
        delete[] matFecho;
    }
    delete[] matFecho;

    return fechoDeS;
}

// Verifica se um grafo é semi euleriano (possui uma trilha euleriana)
bool ehSemiEuleriano(int n, enum TipoGrafo tipo, int conexidade, const vector<aresta>* LA) {
    // Verificando se o grafo não é conexo
    if(conexidade != 1) {
        return false;
    }

    int grauImpar = 0;              // Quantidade de vértices de grau ímpar
    int grauEntradaMaior = 0;       // Quantidade de vértices com grau de entrada maior que o grau de saída
    int grauSaidaMaior = 0;         // Quantidade de vértices com grau de saída maior que o grau de entrada

    // Percorrendo a LA
    for(int u = 0; u < n; u++) {
        int grau = 0;
        
        grau = LA[u].size();    // Calculando o grau do vértice u

        // Calculando o grau de entrada de u para vértices direcionados
        if(tipo == direcionado) {
            int grauEntrada = 0;

            for(int j = 0; j < n; j++) {
                for(auto k : LA[j]) {
                    if(k.v == u) {
                        grauEntrada++;
                    }
                }
            }

            // Verificando se o vértice possui grau de entrada != grau saída (grafo direcionado)
            if(grau > grauEntrada) {
                grauSaidaMaior++;   
            } else {
                if(grauEntrada > grau) {
                    grauEntradaMaior ++;
                }
            }
        } else {
            // Verificando se o grau do vértice é ímpar
            if(grau % 2 != 0) {
                grauImpar++;        // Incrementando quantidade de vértices de gra ímpar        
            }
        }
    }

    if(tipo == direcionado) {
        // Grau de entrada e de saída dos vértices deve ser iguai ou existir exatamente um grau de entrada maior e um grau de saída maior
        if(grauEntradaMaior != 0 or grauSaidaMaior!= 0) {
            if(grauEntradaMaior != 1 and grauSaidaMaior != 1) {
                return false;
            }
        }
    } else {
        // Quantidade de vértices de grau ímpar deve ser 0 ou 2 para existir uma trilha euleriana
        if(grauImpar != 0 and grauImpar != 2) {
            return false;
        }
    }
    
    return true;
}

// Algoritmo de Tarjan para detectar arestas ponte
void tarjanRecursivo(enum TipoGrafo tipo, int u, int* d, int* low, int* pai, int& t, vector<pair<pair<int,int>,bool>>* ListAdjTrilha, vector<pair<int,int>>* ponte) {
    d[u] = t;       // Tempo de descoberta de s = t
    low[u] = t;     // Low de s = t
    t++;            // Incrementando o tempo da busca

    // Percorrendo a lista de adjacência de u
    for(auto uv: ListAdjTrilha[u]) {
        // Verificando se a aresta não foi visitada durante a trilha    
        if(uv.second == false) {
            if(d[uv.first.second] == NAO_VISITADO) {
                pai[uv.first.second] = u;      // Atualizando o pai de v

                uv.second = true;   // Visitando a aresta (u,v)
                // Visitando a aresta (v,u) se o grafo for nao_direcionado
                if(tipo == nao_direcionado) {
                    for(auto vu: ListAdjTrilha[uv.first.second]) {
                        if(vu.first.second == u) {
                            vu.second = true;
                        }
                    }
                }

                // Executando Tarjan para o vértice v (em profundidade)
                tarjanRecursivo(tipo, uv.first.second, d, low, pai, t, ListAdjTrilha, ponte);    

                // Verificando se (u,v) é uma aresta ponte
                if(low[uv.first.second] > d[u]) {
                    ponte[u].push_back({u, uv.first.second});       // Adicionando a aresta no vetor de pontes
                }

                low[u] = min(low[u], low[uv.first.second]);       // Atualizando o low de u caso o low de v seja menor que o dele
            } else {
                // Verificando se o vértice encontrado não é o pai de u
                if(uv.first.second != pai[u]) {
                    low[u] = min(low[u], d[uv.first.second]);     // Atualizando o low de u caso seu low seja maior que o tempo de descoberta de v e v não seja seu pai
                }
            }
        } 
    }
}

// Algoritmo de Tarjan para detectar arestas ponte
// Retorna uma lista com as arestas ponte
vector<pair<int,int>>* tarjanPonte(enum TipoGrafo tipo, int n, vector<pair<pair<int,int>,bool>>* ListAdjTrilha) {
    int* low = new int[n];                              // Vetor que armazena os menores tempos de descoberta de arestas que abracam um vertice
    int* d = new int[n];                                // Vetor que armazena o tempo de descoberta do vértice
    int* pai = new int[n];                              // Vetor que armazena o pai de cada vértice
    int t = 0;                                          // Auxiliar que armazena o tempo da busca
    vector<pair<int, int>>* ponte = new vector<pair<int,int>>[n];         // Armazena as arestas ponte do grafo

    // Inicializando as estruturas necessárias para encontrar as articulações
    for(int i = 0; i < n; i++) {
        low[i] = NAO_VISITADO;
        d[i] = NAO_VISITADO;
        pai[i] = -1;
    }

    // Executando o algoritmo de Tarjan até que todos os vértices sejam visitados
    for(int u = 0; u < n; u++) {
        if(d[u] == NAO_VISITADO) {
            tarjanRecursivo(tipo, u, d, low, pai, t, ListAdjTrilha, ponte);
        }
    }   

    delete[] low;
    delete[] d;
    delete[] pai;

    return ponte;
}

// Verifica se uma aresta é válida para ser visitada no momento na trilha euleriana
bool arestaValidaTrilhaEuler(enum TipoGrafo tipo, int u, int v, int n, vector<pair<pair<int,int>,bool>>* ListAdjTrilha) {
    int vizinhos = 0;

    for(auto uv: ListAdjTrilha[u]) {
        // Verificando se a aresta não foi visitada
        if(uv.second == false) {
            vizinhos++;     // Incrementando a quantidade de vizinhos de u
        }
    }

    // (u,v) é a única aresta não visitada saindo de u
    if(vizinhos == 1) {
        return true;
    }
    // Calculando as arestas ponte
    vector<pair<int, int>>* ponte = tarjanPonte(tipo, n, ListAdjTrilha);
    // Verificando se (u,v) é ponte
    for(auto uw: ponte[u]) {
        if(uw.second == v) {
            return false;
        }
    }
    for(auto vw: ponte[v]) {
        if(vw.second == u) {
            return false;
        }
    }

    return true;
}


/*
 *  ALGORITMO DE FLEURY PARA TRILHA EULERIANA ESTÁ RETORNANDO RESULTADO INCORRETO
 *  CORRIGIR ANTES DE UTILIZÁ-LO
*/
// Algoritmo de Fleury para calcular uma trilha euleriana
// Retorna uma lista com uma trilha euleriana ou -1 caso não exista
vector<int> trilhaEulerianaFleury(int n, int m, enum TipoGrafo tipo, int conexidade, const vector<aresta>* LA) {
    vector<int> trilhaEuleriana;        // Trilha euleriana
    int contArestasVisit = 0;           // Contador de arestas visitadas na trilha
    vector<pair<pair<int,int>,bool>>* ListAdjTrilha = new vector<pair<pair<int,int>,bool>>[n];        // Lista de adjacência da trilha euleriana

    // Verificando se o grafo possui uma trilha euleriana
    if(!ehSemiEuleriano(n, tipo, conexidade, LA)) {
        return trilhaEuleriana;
    }
    
    // Inicializando a lista de adjacência da trilha
    for(int u = 0; u < n; u ++) {
        for(auto uv: LA[u]) {
            ListAdjTrilha[u].push_back({{uv.u, uv.v}, false});
        }
    }
    

    int s = 0;      // Vértice de origem da trilha

    // Procurando por um vértice de grau ímpar para iniciar a busca
    for (int u = 1; u <= n; u++) {
        if (LA[u].size() % 2 != 0) {
            s = u;
            break;
        }
    }

    trilhaEuleriana.push_back(s);       // Adicionando o vértice inicial à trilha
    while(contArestasVisit < m) {    
        for(auto uv: ListAdjTrilha[s]) {     
            if(uv.second == false and arestaValidaTrilhaEuler(tipo, uv.first.first, uv.first.second, n, ListAdjTrilha) == true) {
                trilhaEuleriana.push_back(uv.first.second);
                uv.second = true;       // Aresta (u,v) visitada
                // visitando a aresta (v,u) caso o grafo seja nao_direcionado
                if(tipo == nao_direcionado) {
                    for(auto vu: ListAdjTrilha[uv.first.second]) {
                        if(vu.first.second == s) {
                            vu.second = true;
                        }
                    }
                }     
                contArestasVisit++;     // Incrementando o contador de arestas visitadas
                s = uv.first.second;    // Próximo vértice da trilha
                break;
            }
        }
    }

    delete[] ListAdjTrilha;

    return trilhaEuleriana;

}

int main() {
    int n = 0, m = 0;       // Número de vértices e arestas do grafo 
    enum TipoGrafo tipo;    // Tipo do grafo (direcionado ou nao_direcionado)
    string auxTipo;         // Auxiliar para ler do usuário o tipo do grafo
    vector<int> opcoes;     // Opções selecionadas pelo usuário

    // Auxiliares para ler as opções
    string stringOpcoes;
    int intOpcoes;

    // Lendo as opções
    getline(cin, stringOpcoes);     
    stringstream ss(stringOpcoes);
    while(ss >> intOpcoes) {
        opcoes.push_back(intOpcoes);
    }

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

    //Lendo as arestas do grafo
    LA = lerGrafo(n, m, tipo, LA);


    // Variáveis utilizadas nos algoritmos para analisar o grafo
    int conexidade = -1;
    int possuiCiclo = -1;
    int* verticesArticulacao = NULL;
    int* idArestasPonte = NULL;
    bool tarjanExecutado = false;
    int contArtic = 0;
    int contPontes = 0;

    // Executando as análises no grafo de acordo com as opções escolhidas
    for(auto op: opcoes) {
        switch(op) {
            case 0:
            {
                // (0) Retornando se um grafo é conexo
                if(conexidade == -1) {
                    conexidade = conexidadeGrafo(n, tipo, LA);
                }
                cout << conexidade << endl;
                break;
            }

            case 1:
            {
                // (1) Retornando se um grafo é bipartido
                int ehBipartido = bipartido(n, tipo, LA);
                cout << ehBipartido << endl;
                break;
            }

            case 2:
            {
                if(conexidade == -1) {
                    conexidade = conexidadeGrafo(n, tipo, LA);
                }
                // (2) Retornando se um grafo é euleriano
                int ehEuleriano = euleriano(n, tipo, conexidade, LA);
                cout << ehEuleriano << endl;
                break;
            }

            case 3: 
            {
                // (3) Retornando se um grafo possui ciclo
                if(possuiCiclo == -1) {
                    possuiCiclo = ciclo(n, LA);
                }
                cout << possuiCiclo << endl;
                break;
            }

            case 4:
            {
                // (4) Retornando a quantidade de componentes conexas de um grafo
                int compConexas = qtdCompConexas(n, tipo, LA);
                cout << compConexas << endl;
                break;
            }    

            case 5:
            {
                // (5) Retornando a quantidade de componentes fortemente conexas de um grafo
                int compFortConexas = qtdCompFortConexas(n, tipo, LA);
                cout << compFortConexas << endl;
                break;
            }

            case 6:
            {
                if(!tarjanExecutado) {
                    // Calculando vértices de articulação e arestas ponte
                    articulacoesEPontes(n, m, tipo, contArtic, contPontes, &verticesArticulacao, &idArestasPonte, LA);
                    tarjanExecutado = true;
                }    
                // (6) Retornando vértices de articulação
                imprimirListaInt(contArtic, verticesArticulacao);
                delete[] verticesArticulacao;
                break;
            }

            case 7:
            {
                if(!tarjanExecutado) {
                    articulacoesEPontes(n, m, tipo, contArtic, contPontes, &verticesArticulacao, &idArestasPonte, LA);
                    tarjanExecutado = true; 
                }
                // (7) Retornando o id das arestas ponte
                imprimirListaInt(contPontes, idArestasPonte);
                delete[] idArestasPonte;
                break;
            }

            case 8:
            {
                // (8) Retornando o id das arestas da árvore em profundindade
                queue<int> filaArvoreProfundidade = dfsArvore(n, LA);
                imprimirFilaInt(filaArvoreProfundidade);
                break;
            }

            case 9:
            {
                // (9) Retornando o id das arestas da árvore em largura
                queue<int> filaArvoreLargura = bfsArvore(n, LA);
                imprimirFilaInt(filaArvoreLargura);
                break;
            }

            case 10:
            {
                if(conexidade == -1) {
                    conexidade = conexidadeGrafo(n, tipo, LA);
                }
                // (10) Retornando o valor da AGM
                int agm = agmPrim(n, tipo, conexidade, LA);
                cout << agm << endl;
                break;
            }

            case 11:
            {
                if(possuiCiclo == -1) {
                    possuiCiclo = ciclo(n, LA);
                }
                // (11) Retornando os vértices em ordem topológica
                queue<int> ordemTopologica = ordenacaoTopologicaKahn(n, tipo, possuiCiclo, LA);
                imprimirFilaInt(ordemTopologica);
                break;
            }

            case 12:
            {
                // (12) Retornando o valor do caminho mínimo entre 0 e n-1
                int caminhoMinimo = caminhoMinimoBellmanFord(n, 0, n-1, LA);
                cout << caminhoMinimo << endl;
                break;
            }

            case 13:
            {
                // (13) Retornando o fluxo máximo de 0 a n-1
                int fluxoMaximo = fluxoMaximoEdmondsKarp(n, 0, n-1, tipo, LA);
                cout << fluxoMaximo << endl;
                break;
            }

            case 14:
            {
                // (14) Retornando o fecho transitivo de 0
                queue<int> fechoTransitivo = fechoWarshall(n, 0, tipo, LA);
                imprimirFilaInt(fechoTransitivo);
                break;
            }

            default:
                break;
        }
    }

    delete[] LA;

    return 0;
}