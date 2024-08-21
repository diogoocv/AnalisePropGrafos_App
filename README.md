# AnalisePropGrafos_App
Algoritmo modular para análise de propriedades de Grafos - Algoritmos em Grafos - 2024/01 - UFLA
DIOGO OLIVEIRA CARVALHO – 202120533
LUCAS SILVA MEIRA - 202120807
BRUNO DE ALMEIDA DE PAULA – 201920350
-----------------------------------------------------------------------------------------------------------

Orientações ao usuário
    Para utilizar o programa, basta escolher as opções de funções que deseja analisar no grafo (digite uma lista de valores de 0 a 16), indicar a quantidade de vértices e arestas, respectivamente, o tipo do grafo (orientado ou nao_orientado, escritos exatamente desta maneira). Em seguida informar cada uma das arestas contendo 4 informações, sendo elas o id da aresta, o vértice de origem, o vértice de destino e, por fim, seu peso.
    
    Exemplo de entrada válida:
    0 1 2 3 4 5             -> Funções escolhidas  
    4 3                     -> Número de vértices e arestas, respectivamente
    nao_direcionado         -> Tipo do grafo
    0 0 1 1                 -> Arestas do grafo
    1 0 2 1
    2 2 3 1

    Funções presentes no programa:
        (0)  - Conexidade: Retorna 1 se for conexo (nao_direcionado) ou fracamente conexo (direcionado) ou 0 se for desconexo.
        (1)  - Bipartido: Apenas para grafos nao_direcionados. Retorna 1 se um grafo nao_direcionado é bipartido ou 0 caso contrário.
            Se o grafo for direcionado, retorna 0.
        (2)  - Euleriano: Retorna 1 se o grafo for euleriano ou 0 caso contrário.
        (3)  - Ciclo: Retorna 1 caso o grafo possua ciclo ou 0 caso contrário.
        (4)  - Componentes Conexas: Apenas grafos nao_direcionados. Retorna a quantidade de componentes conexas ou -1 caso o grafo seja direcionado.
        (5)  - Componentes Fortemente Conexas: Apenas grafos direcionados. Retorna a quantidade de componentes fortemente conexas ou -1 caso nao_direcionado.
        (6)  - Vértices articulação: Retorna uma lista com os vértices de articulação do grafo ou -1 caso não exista nenhum.
        (7)  - Arestas ponte: Retorna uma lista com o id as arestas ponte do grafo ou -1 caso não exista nenhuma.
        (8)  - Árvore profundidade: Retorna uma lista com o id das arestas presentes em uma árvore de uma busca profundidade.
        (9)  - Árvore largura: Retorna uma lista com o id das arestas presentes em uma árvore de uma busca em largura.
        (10) - Árvore geradora mínima: Retorna uma lista com o id das arestas presentes na AGM ou -1 caso o grafo seja direcionado ou desconexo.
        (11) - Ordenação Topológica: Apenas direcionados. Retorna uma lista com os vértices em ordem topológica, ou -1 caso não haja uma ordem topológica.
        (12) - Caminho mínimo: Retorna o valor do caminho mínimo do vértice 0 ao vértice n-1 (sendo n o número de vértices) ou -1 caso não exista um caminho.
        (13) - Fluxo máximo: Apenas grafos direcionados. Retorna o valor do fluxo máximo entre os vértices 0 e n-1 ou -1 caso o grafo seja nao_direcionado.
        (14) - Fecho transitivo: Apenas grafos direcionados. Retorna uma lista com o fecho transitivo do vértice 0 ou retorna -1 caso o grafo seja nao_direcionado ou o fecho seja vazio.