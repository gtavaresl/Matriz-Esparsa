#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

// LISTA ENCADEADA

typedef struct testruct {
    float valor;
    int coluna;
    struct testruct* prox;
} estruct;

typedef estruct* dado;

typedef struct {
    dado* vetor;
    int linhas;
    int colunas;
} matriz;

void Inicializa(matriz* m, int lin, int col) {
    int i;
    m->linhas = lin;
    m->colunas = col;
    m->vetor = (dado*) malloc(lin*sizeof(dado));
    for (i=0;i<lin;i++) m->vetor[i] = NULL;
}

void Exclui(matriz* m){
    m->colunas=0;
    m->linhas=0;
    free(m->vetor);
    printf("Matriz excluida!\n");
}

int MudaDado(matriz* m,int lin, int col,float val) {
    if (lin<0 || lin >= m->linhas || col<0 || col >= m->colunas) return 0;
    dado ant = NULL;
    dado atual = m->vetor[lin];
    while (atual != NULL && atual->coluna<col) {
    ant = atual;
    atual = atual->prox;
    }
    if (atual != NULL && atual->coluna == col) {
        if (val == 0) {
            if (ant==NULL) m->vetor[lin] = atual->prox;
            else ant->prox = atual->prox;
            free(atual);
        }
        else atual->valor = val;
    }
    else if (val != 0) {
    dado novo = (dado) malloc(sizeof(estruct));
    novo->coluna = col;
    novo->valor = val;
    novo->prox = atual;
    if (ant == NULL) m->vetor[lin] = novo;
    else ant->prox = novo;
    }
    return 1;
}

float ConsultaDado(matriz* m, int lin, int col) {
    if (lin<0 || lin>=m->linhas || col<0 || col >= m->colunas) return 0;
    dado atual = m->vetor[lin];
    while (atual != NULL && atual->coluna < col) atual = atual->prox;
    if (atual !=NULL && atual->coluna == col) return atual->valor;
    return 0;
}

float SomaLinha(matriz* m, int lin){
    float soma=0;
    if(lin < 0 || lin >= m->linhas ){
        return 0;
    }
    dado atual = m-> vetor[lin];
    while(atual != NULL && atual->coluna < m->colunas){
        soma += atual->valor;
        atual = atual->prox;
    }
    return soma;
}

float SomaColuna(matriz* m, int col){
    float soma=0;
    if( col < 0 || col >= m->colunas){
        return 0;
    }
    dado atual;
    for(int lin=0;lin < m->linhas; lin++){
        atual = m->vetor[lin];
        while(atual != NULL && atual->coluna <col){
            atual = atual->prox;
        }
        if(atual != NULL && atual->coluna == col){
            soma += atual->valor;
        }
    }
    return soma;
}

int LinhaNula(matriz* m){
    int lin = m->linhas;
    int col = m->colunas;
    int i,j,contador;
    for(i=0;i<lin;i++){
        if(m->vetor[i]==NULL){
            return 1;
        }
    }
    for(i=0;i<col;i++){
        contador=0;
        for(j=0;j<lin;j++){
            if(ConsultaDado(m,j,i)==0){
                contador++;
            }
        }
        if(contador==lin){
            return 1;
        }
    }
    return 0;
}
float Determinante(float *m, int ordem);
float detLaplace(float *m, float *Cofator, int lin, int col, int ordem);

float Determinante(float *m, int ordem){
  float detCofator = 0;
  float *Cofator;
  int c;
  if(ordem == 0) return 0;
  if(ordem == 1) return *m;
  Cofator = calloc(ordem*ordem, sizeof(float));
  for(c = 0; c < ordem; c++){
    detLaplace(m, Cofator, 0, c, ordem);
    if(!(c%2)) detCofator+= Determinante(Cofator, ordem-1) * *(m+c);
    else detCofator-= Determinante(Cofator, ordem-1) * *(m+c);
  }
  free(Cofator);
  return detCofator;
}

float detLaplace(float *m, float *Cofator, int lin, int col, int ordem){
    int cont,i=0,f=0;
    for(cont = 0; cont < ordem*ordem; cont++) *(Cofator+cont) = *(m+cont);
    for(cont = 0; cont < ordem*(ordem-1); cont++)
        if(cont>=(lin*ordem))
            *(Cofator+cont) = *(Cofator+cont+ordem);
    for(cont = 0; cont < ordem*(ordem-1); cont++)
        if(((cont+f)%ordem)==col){
            i=cont;
            while(i<ordem*(ordem-1)){
                *(Cofator+i) = *(Cofator+i+1);
                i++;
            }
            f+=1;
        }
    return 0;
}

void menu(void){
    printf("\nMenu Principal\n");
    printf("1. Criar uma matriz\n");
    printf("2. Excluir uma matriz\n");
    printf("3. Consulta do valor de uma posicao da matriz\n");
    printf("4. Consulta da soma dos valores de uma linha da matriz\n");
    printf("5. Consulta da soma dos valores de uma coluna da matriz\n");
    printf("6. Atribuicao de um valor na posicao (i,j) da matriz\n");
    printf("7. Calcular o determinante da matriz\n");
    printf("8. Sair\n");
    printf("<?>: ");
}

int main()
{
    matriz* m=(matriz* )malloc(sizeof(matriz));
    m->colunas=0;
    m->linhas=0;
    m->vetor=NULL;
    int lin,col;
    int op=0;
    float v;
    while(op!=8){
        menu();
        scanf("%d",&op);
        while(op<1 || op>8){
            printf("Entrada invalida!\n");
            printf("<?>: ");
            scanf("%d",&op);
        }
        if(op==1){
            printf("\nModo: Criar Matriz\n");
            if(m->linhas!=0 && m->colunas!=0 && m->vetor!=NULL){
                printf("Eh preciso excluir a matriz primeiro!\n");
            }else{
                printf("Entre com a quantidade de linhas: ");
                scanf("%d",&lin);
                printf("Entre com a quantidade de colunas: ");
                scanf("%d",&col);
                Inicializa(m,lin,col);
            }
        }
        if(op==2){
            printf("\nModo: Excluir Matriz\n");
            if(m->linhas!=0 && m->colunas!=0 && m->vetor!=NULL){
                Exclui(m);
            }else{
                printf("Nao ha matriz p/ ser excluida!\n");
            }
        }
        if(op==3){
            printf("\nModo: Consulta do Valor de uma posicao\n");
            if(m->linhas!=0 && m->colunas!=0 && m->vetor!=NULL){
                printf("Entre com a linha: ");
                scanf("%d",&lin);
                while(lin < 0 || lin >= m->linhas){
                    printf("Linha invalida!\n");
                    printf("Entre com a linha: ");
                    scanf("%d",&lin);
                }
                printf("Entre com a coluna: ");
                scanf("%d",&col);
                while(col < 0 || col >= m->colunas){
                    printf("Coluna invalida!\n");
                    printf("Entre com a coluna: ");
                    scanf("%d",&col);
                }
                v=ConsultaDado(m,lin,col);
                printf("M[%d][%d]=%.2f\n",lin,col,v);
            }else{
                printf("Matriz nao incializada!\n");
            }
        }
        if(op==4){
            printf("\nModo: Consultar a soma das linhas da matriz\n");
            if(m->linhas!=0 && m->colunas!=0 && m->vetor!=NULL){
                printf("Entre com a linha: ");
                scanf("%d",&lin);
                while(lin < 0 || lin >= m->linhas){
                    printf("Linha invalida!\n");
                    printf("Entre com a linha: ");
                    scanf("%d",&lin);
                }
                v=SomaLinha(m,lin);
                printf("Soma da linha %d = %.2f\n",lin,v);
            }else{
                printf("Matriz nao inicializada!\n");
            }
        }
        if(op==5){
            printf("\nModo: Consultar a soma das colunas da matriz\n");
            if(m->linhas!=0 && m->colunas!=0 && m->vetor!=NULL){
                printf("Entre com a coluna: ");
                scanf("%d",&col);
                while(col < 0 || col >= m->colunas){
                    printf("Coluna invalida!\n");
                    printf("Entre com a coluna: ");
                    scanf("%d",&col);
                }
                v=SomaColuna(m,col);
                printf("Soma da coluna %d = %.2f\n",col,v);
            }else{
                printf("Matriz nao inicializada!\n");
            }
        }
        if(op==6){
            printf("\nModo: Atribuir um valor na posicao (i,j) da matriz\n");
            if(m->linhas!=0 && m->colunas!=0 && m->vetor!=NULL){
                printf("Entre com a linha: ");
                scanf("%d",&lin);
                while(lin < 0 || lin >= m->linhas){
                    printf("Linha invalida!\n");
                    printf("Entre com a linha: ");
                    scanf("%d",&lin);
                }
                printf("Entre com a coluna: ");
                scanf("%d",&col);
                while(col < 0 || col >= m->colunas){
                    printf("Coluna invalida!\n");
                    printf("Entre com a coluna: ");
                    scanf("%d",&col);
                }
                printf("Entre com o valor: ");
                scanf("%f",&v);
                MudaDado(m,lin,col,v);
                printf("M[%d][%d]=%.2f\n",lin,col,v);
            }else{
                printf("Matriz nao inicializada!\n");
            }
        }
        if(op==7){
            printf("\nModo: Calcular determinante\n");
            if((m->linhas == m->colunas) && m->linhas!=0 && m->colunas!=0 && m->vetor!=NULL){
                if(LinhaNula(m)){
                    printf("Determinante[M]= 0\n");
                }else{
                    float *fordet;
                    int p=0;
                    int quadrada;
                    lin=m->linhas;
                    col=m->colunas;
                    quadrada=lin;
                    fordet=malloc(lin*col*sizeof(float));
                    if(fordet!=NULL){
                        for(lin=0;lin<m->linhas;lin++){
                            for(col=0;col<m->colunas;col++){
                                fordet[p]=ConsultaDado(m,lin,col);
                                p++;
                            }
                        }
                        v=Determinante(fordet,quadrada);
                        printf("Determinante[M]= %.2f\n",v);
                        free(fordet);
                    }else{
                        printf("Erro ao calcular determinante!\n");
                        fordet=NULL;
                    }
                }
            }else{
                printf("Eh preciso criar uma matriz quadrada!\n");
            }
        }
    }
    free(m);
    return 0;
}
