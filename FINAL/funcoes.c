#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "funcoes.h"

#define INF 1000000

/* Cria matriz [ l ] [ c ] vazia */
int **MATRIZint(int l, int c){
    int i, j;
    int ** m = (int **) malloc(l * sizeof(int *));
    for (i = 0; i < l; i++)
        m[i] = (int *) malloc(c * sizeof(int));
    for (i = 0; i < l; i++)
        for (j = 0; j < c; j++)
            m[i][j] = 0;
    return m;
}

/* Cria vetor [ n ] vazio */
int *VETORint(int n){
    int i;
    int *v = (int *) malloc(n * sizeof(int));
    for (i = 0; i < n; i++)
        v[i] = 0;
    return v;
}

/* Cria um grafo vazio de n vertices */
Grafo GRAFOcria(int n){
    Grafo G = (Grafo) malloc(sizeof(Grafo));
    G->n = n;
    G->m = 0;
    G->adj = MATRIZint(n,n);

    return G;
}

/* Funcao para inserir um arco em um grafo com peso */
void GRAFOinsereA(Grafo G, Arco e, int peso){
    int v = e.v;
    int w = e.w;
    if (G->adj[v][w] == 0) {
        G->m++;
        G->adj[v][w] = peso;
    }
}

/* Funcao para remover um arco de um grafo */
void GRAFOremoveA(Grafo G, Arco e){
    int v = e.v;
    int w = e.w;
    if (G->adj[v][w] > 0) {
        G->m--;
        G->adj[v][w] = 0;
    }
}

/* Funcao para imprimir um grafo */
void GRAFOimprime(Grafo G) {
    int v, w;

    if (G != NULL){
    	printf("%d vertices, %d arestas\n", G->n, G->m);
    	for (v = 0; v < G->n; v++) {
            for (w = 0; w < G->n; w++){
            	printf( " %2d", G->adj[v][w]);
            }
            printf( "\n");
    	}
    }
    else{
	printf("%p\n", G);
    }
}

/* Funcao para destruir um grafo */
void GRAFOdestroi(Grafo G){
    int i;
    for (i = 0; i < G->n; i++) {
        free(G->adj[i]);
    }
    free(G->adj);
    free(G->peso);
    free(G);
}

Grafo GRAFOleitura(const char *nomearq){
    int n,m,t,peso,r,i,flag,x,y;
    Arco a;
    FILE *arquivo;
    arquivo = fopen(nomearq,"r");

    if (!arquivo) {
        printf("O Arquivo %s nao pode ser aberto.\n", nomearq);
        getchar();
        exit(1);
    }

    r = fscanf(arquivo, "%d %d", &n, &m);
    Grafo G = GRAFOcria(n);
		G->peso = VETORint(n);

		r = fscanf(arquivo, "%d", &flag);

		for(i=0; i < n ; i++){
			r = fscanf(arquivo, "%d %d %d", &x, &y, &G->peso[i]);
		}
		r = fscanf(arquivo, "%d", &flag);
		for(i=0; i < m ; i++){
			r = fscanf(arquivo, "%d %d %d", &a.v, &a.w, &peso);
			GRAFOinsereA(G, a, peso);
		}
    fclose(arquivo);
    return G;
}

int cont;
int *ordem, *pai, *menor;

int custoH1(Grafo G, Grafo AGM)
{
	int *grau = malloc(G->n * sizeof(int));
	int i, j, peso=0,cont=0,maior=0;
	int custoA;

	for(i=0; i<G->n; i++)
	{
		grau[i] = 0;
	}

	for(i=0; i<G->n; i++)
	{
		for(j=0; j<G->n; j++)
		{
			if (AGM->adj[i][j] != 0)
			{
				AGM->adj[j][i] = AGM->adj[i][j];
			}
		}
	}

	//Descobrindo grau dos vértices
	for(i=0; i < AGM->n; i++){
		for(j=0; j < AGM->n; j++)
		{
	    	if(AGM->adj[i][j] != 0)
	      	{
	        	grau[i]++;
	      	}
	    }
	}

	//verifica o peso dos vertices internos para adicionar no custo da arvore
	for (i = 0; i < G->n ; i++)
	{
		if(grau[i] > 1)
		{
			peso = peso + G->peso[i];
		}
	}

	//encontra o custo da arvore atual
	for (i = 0; i < G->n ; i++)
	{
		for(j = 0; j < G->n; j++)
		{
			if (AGM->adj[i][j] != 0)
			{
				cont = cont + AGM->adj[i][j];
			}
		}
	}

	custoA = ((cont / 2) + peso);
	free(grau);
  	return custoA;
}

Grafo Kruskal(Grafo G, int *custo){
    int min, k, l, x, y, c;
    int *comp = malloc(G->n * sizeof(int));
    Grafo AGM = GRAFOcria(G->n); 
    Arco a;
    int i,j;

    // TO DO
    *custo=0;
    k = -1;
    x = -1;
    y = -1;

    for(i=0; i< G->n; i++){
      comp[i] = i;
    }

    while (1){
      min = INF;
      for(i=0;i < G->n ; i++){
        for(j=0; j < G->n; j++){
          if(G->adj[i][j] != 0){
            if((comp[i] != comp[j]) && (min > G->adj[i][j])){
              min = G->adj[i][j];
              k = i;
              l = j;
            }
          }
        }
      }
      if(min == INF){
        break;
      }
      a.v = k;
      a.w = l;
      GRAFOinsereA(AGM,a,G->adj[k][l]);
      *custo+= G->adj[k][l];

      x = comp[k];
      y = comp[l];
      for(i=0; i < G->n; i++){
        if(comp[i] == y){
          comp[i] = x;
        }
      }
    }
    c = custoH1(G, AGM);
    printf("** Custo AGM Heuristica 1: %d\n", c);
    return(AGM);
}


int vM, wM;

void ciclo(Grafo G)
{
	int i, j, k;
	int cont = 0;
	int maior = 0;
	for(i=0; i<G->n ; i++)
	{
		for(j=0; j < G->n; j++)
		{
			if(G->adj[i][j] != 0)
			{
				cont++;
			}
		}
		//encontra e retira os vertices que nao estao no ciclo
		if (cont == 1)
		{
			for(k=0; k< G->n ; k++)
			{
				if (G->adj[i][k] != 0)
				{
					G->adj[i][k] = 0;
					G->adj[k][i] = 0;
				}
				
			}
			ciclo(G);
		}
		cont = 0;
	}
	//verifica qual a maior aresta do ciclo
  	for(i=0 ; i< G->n; i++)
  	{
		for(j=0; j<G->n ; j++)
		{
			if (G->adj[i][j] > maior)
			{
				maior = G->adj[i][j];
				vM = i;
				wM = j;
			}
		}
	}
	

}

int verificaArvore(Grafo AGM, Grafo G, int **custo)
{ 
	int *grau = malloc(G->n * sizeof(int));
	int i, j, peso=0,cont=0,maior=0;
	int custoA;

	for(i=0; i<G->n; i++)
	{
		grau[i] = 0;
	}

	//Descobrindo grau dos vértices
	for(i=0; i < AGM->n; i++){
		for(j=0; j < AGM->n; j++)
		{
	    	if(AGM->adj[i][j] != 0)
	      	{
	        	grau[i]++;
	      	}
	    }
	}

	//verifica o peso dos vertices internos para adicionar no custo da arvore
	for (i = 0; i < G->n ; i++)
	{
		if(grau[i] > 1)
		{
			peso = peso + G->peso[i];
		}
	}

	//encontra o custo da arvore atual
	for (i = 0; i < G->n ; i++)
	{
		for(j = 0; j < G->n; j++)
		{
			if (AGM->adj[i][j] != 0)
			{
				cont = cont + AGM->adj[i][j];
			}
		}
	}
	custoA = ((cont / 2) + peso);

  	return custoA;  
}



Grafo Heuristica(Grafo AGM, Grafo G,int *custo)
{
  	int i,j,k,l,peso=0,cont=0,maior=0;
  	int custoA, custoAGM = INF;
  	
  	Arco a;
  	Grafo AGMaux = GRAFOcria(G->n); // cria uma AGM auxiliar
  	
  	for(i=0;i < G->n; i++)
	{
	//copia a AGM

		for(j=0; j < G->n ; j++)
		{
			if (AGM->adj[i][j] != 0)
			{
				AGMaux->adj[i][j] = AGM->adj[i][j];
				AGMaux->adj[j][i] = AGMaux->adj[i][j];	
			}	
		}
	}

  //encontra uma aresta do grafo que não existe na AGM
  for (i = 0; i < G->n; i++)
  {
  	for (j = 0; j < G->n; j++)
  	{
  		if ((G->adj[i][j] != 0) && (AGM->adj[i][j] == 0))
  		{
  			/*adiciona a aresta, formando um ciclo*/
  			AGMaux->adj[i][j] = G->adj[i][j];
  			AGMaux->adj[j][i] = AGMaux->adj[i][j];

  			//detecta o ciclo que foi criado com a adição da aresta
  			ciclo(AGMaux);

  			AGM->adj[i][j] = G->adj[i][j];
  			//retira a maior aresta do ciclo
  			AGM->adj[vM][wM] = 0;
  			AGM->adj[wM][vM] = 0;
  			
  			//zera a AGMaux
  			for(k=0; k<G->n;k++)
  			{
  				for(l=0;l<G->n;l++)
  				{
  					AGMaux->adj[k][l] = 0;
  				}
  			}

  			//copia a AGM para a AGMaux
  			for(k =0;k < G->n ; k++)
  			{
  				for(l = 0; l< G->n; l++)
  				{
  					if (AGM->adj[k][l] != 0)
  					{
  						AGMaux->adj[k][l] = AGM->adj[k][l];
						AGMaux->adj[l][k] = AGMaux->adj[k][l];
  					}
  					
  				}
  			}

  			custoA = verificaArvore(AGMaux, G, &custo);

  			if (custoA < custoAGM)
  			{
  				custoAGM = custoA;
  			}
  		}
  	} 	
  }

  printf("Custo da Heuristica 2:  %d\n", custoAGM);
/**********************************************************************************************************/
  return(AGM);
}
