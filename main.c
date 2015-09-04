/**
 * Who's approved?
 *
 * @author      Fernando Barbosa Neto   
 * @author      Jeferson de Oliveira Batista   
 * 
 */


#include<stdio.h>
#define PES_POR 2.5
#define PES_LOG 1.5
#define PES_ETI 1.0
#define PES_CES 3.5


void load_gabarito(int gab[]);
int load_candidatos(int cand[][81]);
void calcula_notas(int cand[][81], float notas[][5], int gab[], int insc);
int pontos(int *resp_materia, int *gab_materia, int questoes);
void nota_final(float notas[][5], int insc);
void print_eliminados(int cand[][81], float notas[][5], int insc);
void print_habilitados(int cand[][81], float notas[][5], int insc);
int comparar(int cand[][81], float notas[][5], int hab, int i);


int main()
{	
	int gabarito[20]; //20 questoes (optamos por um gabarito numérico)
	int candidatos[100][81]; //100 candidatos (máximo), 1 matricula + 80 alternativas
	int inscritos=0; //quantidade de inscritos
	
	load_gabarito(gabarito);
	inscritos = load_candidatos(candidatos);
	
	float notas[inscritos][5]; //inscritos (entre 0 e 100), acertos de por([0]), log([1]), eti([2]), ces([3]) e nota final([4])
	
	calcula_notas(candidatos, notas, gabarito, inscritos);
	
	print_eliminados(candidatos, notas, inscritos);
	print_habilitados(candidatos, notas, inscritos);
	
	return 0;
}


/* 
 * Função para carregar
 * respostas do gabarito 
 */
void load_gabarito(int gab[])
{	
   char letra;
   int i;
   
   //recebe o gabarito
   for(i=0; i<20; i++)
   {
      scanf("%c\n", &letra);
      gab[i] = (int) (letra - 'a'); //converte pra versão numérica
   }
}


/*
 * Função para carregar o
 * números de matrícula e 
 * respostas do candidatos 
 */
int load_candidatos(int cand[][81])
{	
	int i=0, j=0;
	
	while(1)
	{	
		//registra número de matrícula
		scanf("%d", &cand[i][0]);
		
		//testa se está no fim da lista
		if(cand[i][0] == -1)
			break;
		
		//armazena respostas dos candidatos
		for(j=1; j<81; j++)
		{		
			scanf("%d", &cand[i][j]);
		}
		
		i++;
		
	}
	
	//retorna quantidade de inscritos
	return i;
}


/*
 * Função para calcular
 * as pontuações de 
 * cada candidato 
 */
void calcula_notas(int cand[][81], float notas[][5], int gab[], int insc)
{	
	int i;
	
	for(i=0; i<insc; i++)
	{	
		//checa respostas de português
		notas[i][0] = PES_POR*pontos(&cand[i][1+4*0], &gab[0], 5);
		
		//checa respostas de lógica
		notas[i][1] = PES_LOG*pontos(&cand[i][1+4*5], &gab[5], 3);
		
		//checa respostas de ética
		notas[i][2] = PES_ETI*pontos(&cand[i][1+4*8], &gab[8], 2);
		
		//checa respostas de conhecimentos específicos
		notas[i][3] = PES_CES*pontos(&cand[i][1+4*10], &gab[10], 10);
	}
	
	//calcula nota final
	nota_final(notas, insc);
}

int pontos(int *resp_materia, int *gab_materia, int questoes)
{
	int acertos = 0;
	int c; //variável que conta o número de alternativas marcadas
	int i,j;
   	for(i=0; i<questoes; i++) //loop que verifica cada questão da matéria
   	{
   		c = 0;
   	    for(j=0; j<4; j++)
   	    {
   	       c += resp_materia[4*i+j];
   	    }

   	    if(c == 1 && resp_materia[4*i + gab_materia[i]] == 1)
   	    	acertos++;
   	}
   	return acertos;
}


/* 
 * Calcula nota final
 */
void nota_final(float notas[][5], int insc)
{
	int i;
	
	for(i=0; i<insc; i++)
	{
		if(notas[i][0] < 5.0 || notas[i][1] < 1.5 || notas[i][2] < 1.0 || notas[i][3] < 14.0)
			notas[i][4]=-1.0;
		else
			notas[i][4] = notas[i][0] + notas[i][1] + notas[i][2] + notas[i][3];
	}	
}


/* 
 * Mostra os candidatos
 * eliminados
 */
void print_eliminados(int cand[][81], float notas[][5], int insc)
{	
	int i;
	
	for(i=0;i<insc;i++)
		if(notas[i][4]==-1.0)
			printf("Candidato Eliminado\nMATRICULA=%d\n\n", cand[i][0]);
	
}


/* 
 * Mostra os candidatos
 * habilitados
 */
void print_habilitados(int cand[][81], float notas[][5], int insc)
{	
	int i, hab1, hab2;
	
	if(comparar(cand, notas, 1, 0))
	{
		hab1 = 0;
		hab2 = 1; 
	}
	else
	{
		hab1 = 1;
		hab2 = 0;
	}
	
	for(i=2; i<insc; i++)
	{		
		if(comparar(cand, notas, hab1, i))
		{
			hab2 = hab1;
			hab1 = i;
		}
		else if(comparar(cand, notas, hab2, i))
		{
			hab2 = i;	
		}
	}
	
	printf("Habilitado 1\nMATRICULA=%d\nPONTUACAO=%.1f\n\n", cand[hab1][0], notas[hab1][4]);
	printf("Habilitado 2\nMATRICULA=%d\nPONTUACAO=%.1f\n\n", cand[hab2][0], notas[hab2][4]);
}


/*
 * Compara e resolve o empate
 */
int comparar(int cand[][81], float notas[][5], int hab, int i)
{	
	if(notas[i][4] > notas[hab][4])
		return 1;
	else if(notas[i][4] < notas[hab][4])
		return 0;
	else if(notas[i][3] > notas[hab][3])
		return 1;
	else if(notas[i][3] < notas[hab][3])
		return 0;
	else if(notas[i][0] > notas[hab][0])
		return 1;
	else if(notas[i][0] < notas[hab][0])
		return 0;
	else if(notas[i][1] > notas[hab][1])
		return 1;
	else if(notas[i][1] < notas[hab][1])
		return 0;
	else if(notas[i][2] > notas[hab][2])
		return 1;
	else if(notas[i][2] < notas[hab][2])
		return 0;
	else if(cand[i][0] < cand[hab][0])
		return 1;
	else
		return 0;
}
