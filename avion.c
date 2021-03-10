#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define INT_MAX 999		

typedef struct Resursa
{
    char *nume;
    int cantitate;
}Resursa;

typedef struct Island
{
    char *nume;
    int nrResurse;
    Resursa *inventarResurse;
    int nrAvioane;
    int *avioane;
    int tolAvioane;
}Island;

typedef struct 		
{
	int V, E;        //nr. varfuri si muchii 
	int** a;  //matricea de adiacenta
}Graph;

typedef struct Nod_arbore	//structura de date pentru arbore
{
    Resursa* val;			//in arbore sunt stocate valori de tip Resursa
    struct Nod_arbore *left, *right;			//legatura catre subarborii din stanga si dreapta
} Nod_arbore;

typedef struct elem stackNode;  //structura pentru stiva

struct elem
{
    Nod_arbore *val; 	//datele memorate de tip nod
    struct elem *next; 	//legatura catre urmatorul element
};

char* setsir(char*x,FILE*f)		//functie care aloca memorie pentru un sir de caractere citit dintr-un fisier binar
{
    char aux[100];	//variabila locala ce va "expira" dupa "incheierea" functiei

    fscanf(f, "%s", aux);		//citesc sirul din fisier
    x = malloc((strlen(aux) + 1) * sizeof(char));		//aloc spatiu pentru variabila ce va retine sirul de caractere
    return strcpy(x,aux);		//copiez valoarea din fisier in variabila ce va primi sirul si returnez valoarea
}

void citire(Island *v, int nr, FILE *f)		//functia citeste structurile de insule din fisierul binar
{
    int i, j, ct;	//variabile locale

    for(i = 0; i < nr; i++)		//parcurg fiecare insula; nr - reprezinta numarul de insule
    {
        (v+i)->nume = setsir((v+i)->nume,f);	//citesc numele insulei
        fscanf(f, "%d", &(v+i)->nrResurse);		//aflu cate resurse are insula citita

        ct = (v+i)->nrResurse;	
        (v+i)->inventarResurse = malloc(sizeof(Resursa) * ct);	//aloc spatiu pentru resursele insulei citite

        for(j = 0; j < ct; j++)
        {
            (v+i)->inventarResurse[j].nume = setsir((v+i)->inventarResurse[j].nume,f);		//citesc resursa
            fscanf(f, "%d", &(v+i)->inventarResurse[j].cantitate);		//citesc cantitatea
        }
    }
}

Graph* alocGraph(int val)
{               // functie pentru alocat spatiu pentru un graf, 
	int i;	//fara initializare arce 

	Graph *g = malloc(sizeof(Graph)); 
	if (g == NULL) return NULL;

	(g->V) = val;
	//aloc spatiu pentru matricea cu care se reprezinta graful
	g->a = malloc(sizeof(int*) * g->V);
	
	for (i = 0; i < g->V; i++) g->a[i] = calloc(g->V,sizeof(int));
		
	return g;
}

void creare_matrice(FILE*date, int nr_conexiuni,Graph*g,Island* insula, int nr_insule)	//creez matricea de costuri aferenta grafului
{
	int cost;
	char c;
    for(int k = 0; k < nr_conexiuni; k++)	//pana se epuizeaza numarul conexiunilor
    {
    	//citesc conetiunile intre insule
        char *insula1 = setsir(insula1, date);   
        fscanf(date, " %c ", &c);
        char *insula2 = setsir(insula2, date);
        fscanf(date, " %d", &cost);

        for(int i = 0; i < nr_insule; i++)
        {
            for(int j = 0; j < nr_insule; j++)
                if(strcmp((insula+i)->nume, insula1) == 0 && strcmp((insula+j)->nume, insula2) == 0 )
                {
                	//pun costul - durata zborului in matrice
                    g->a[i][j] = cost;
                    g->a[j][i] = cost;
                }
        }

  		//eliberez spatiul ocupat de cele doua insule citite pentru aceasta linie a fisierului
        free(insula1);
        free(insula2);  
    }
     
}

void elib_matr_graph(Graph **g)
{
	int i, j, k=(*g)->V;
	do	//sterg cate o linie pana cand "numarul de noduri"(copia) ajunge sa fie 0
	{
		for(i = 0; i < (*g)->V-1; i++)	
		{
			for(j = 0; j < (*g)->V; j++)
				(*g)->a[i][j] = (*g)->a[i+1][j];	//sterg linia i (prin suprascriere)
		}
		k --;	//scade dimensiunea(am sters o linie)
	}while(k!=0);

	free(*g);	//eliberez momoria ocupata de graf
	*g=NULL;	//acum va pointa catre NULL
}

int conversie_island(FILE* date)
{
	int x = 0;
	char *insula = malloc(sizeof(char));
	insula = setsir(insula, date);	//citesc numele insulei din fisierul de date

	for(int i = 6; i < strlen(insula); i++)		//nu parcurg primele 6 litere intrucat acestea sinbolizeaza sirul "Island"
	{											//asa ca voi citi doar continuarea sirului, acela care reprezinta numarul insulei
		x = x * 10 + insula[i] - '0';	//compun numarul si convertesc fiecare caracter in int
	}

	x = x - 1;		//scad 1 intrucat in fisier numaratoarea incepe de la 1, insa in program pornesc de pe pozitia 0

	return x;	//returnez "indicele" insulei
}

void conexiune(FILE* date, Graph *g, FILE* rezultate)
{
	int i, j, ok = 0, insula1, insula2;	//ok = 0, adica initial nu exista muchie

	insula1=conversie_island(date);		//aflu care este indicele fiecarei insule
	insula2=conversie_island(date);

	for (i = 0; i < g->V; i++) 	//parcurg matricea in functie de linie
	{
		if(i == insula1)	//daca am gasit primul varf continui cautarea pe coloane pentru a afla daca exita muchie
		{
			for (j = 0; j < g->V; j++)

				if(j == insula2) 
				{
					if(g->a[i][j] != 0)
					ok = 1;	//daca am gasit si cel de-al doilea varf inseamna ca exista muchie
							//asa ca schimb valoarea inregistrata in ok
				}
		}
	}

	if(ok == 1) fprintf(rezultate,"OK\n");
		else fprintf(rezultate,"NO\n");
}

void legatura(FILE* date,FILE* rezultate, Graph* g)
{
	int i, j, insula;		//initial nu avem muchii

	insula=conversie_island(date);		//aflu pozitia insulei in matrice

	for (i = 0; i < g->V; i++) 	//parcurg dupa linii
	{
		if(i == insula)	//daca gasesc nodul dat caut pe coloane varfurile cu care formeaza muchie
		{
			for (j = 0; j < g->V; j++)
			{
				if(g->a[i][j] != 0)	//daca nu e 0 inseamna ca exita muchie
				{
					fprintf(rezultate,"Island%d ", j+1);	//afisez muchia caracteristica pozitiei curente
				}
			}

			fprintf(rezultate, "\n");
		}
	}
}

void adauga_zbor(FILE* date, FILE* rezultate, Graph** g)
{
	int nod1, nod2, i, j, cost;

	nod1=conversie_island(date);	//citesc insulele intre care vreau sa adaug un zbor
	nod2=conversie_island(date);	//si le aflu pozitia in matricea de costuri

	fscanf(date,"%d",&cost);	//citesc durata zborului

	for (i = 0; i < (*g)->V; i++) 	//parcurg matricea in functie de linie
	{
		if(i == nod1)	//daca am gasit primul varf continui cautarea pe coloane pentru a afla daca exita muchie
		{
			for (j = 0; j < (*g)->V; j++)

				if(j == nod2) 	//daca am gasit si nodul celei de-a doua insule, inseamna ca pot adauga zborul
				{
					(*g)->a[i][j]=cost;
					(*g)->a[j][i]=cost;
				}
		}
	}
}

void anulare_zbor(FILE* date, FILE* rezultate, Graph* g)
{
	int nod1, nod2, i, j;

	nod1 = conversie_island(date);	//citesc insulele si le aflu pozitia in matrice
	nod2 = conversie_island(date);

	for (i = 0; i < (g)->V; i++) 	//parcurg matricea in functie de linie
	{
		if(i == nod1)	//daca am gasit primul varf continui cautarea pe coloane pentru a afla daca exita muchie
		{
			for (j = 0; j < (g)->V; j++)

				if(j == nod2) 	//daca am gasit si cealalta insula
				{
					(g)->a[i][j]=0;		//durata(costul) devine 0(minute)
					(g)->a[j][i]=0;
				}
		}
	}
	
}


Nod_arbore* newNode(Resursa* data)		//functia creeaza un nod nou pentru arbore
{
    Nod_arbore* node = malloc(sizeof(Nod_arbore));		//alocam memorie pentru noul nod
    node->val = data;				//stocam in el valoarea data prin parametru
    node->left = node->right = NULL;		//copiii pointeaza catre NULL

    return node;		//returnam nodul creat
}

Nod_arbore* insert(Nod_arbore *node, Resursa* key_res)			//functia insereaza resursele unei insele in arbore
{
    // daca (sub)arborele e gol – se creaza un nod si se returneaza adresa
    if (node == NULL)
        return newNode(key_res);

    //introducem valorile alfabetic in arbore
    if(strcmp(key_res->nume,node->val->nume) > 0)	
        node->left  = insert(node->left, key_res);

    else if (strcmp(key_res->nume,node->val->nume) < 0)	
        node->right = insert(node->right, key_res);
    //este o functie recursiva,deci se proceseaza fiecare nod radacina pana e introdusa resursa care trebuie :)

    return node;	//returnam noul arbore creat cu resursele date ca parametru
}

void push(stackNode** top, Nod_arbore* v)  //functie ce adauga un element in stiva
{
    stackNode* newNode = malloc(sizeof(Nod_arbore));  //aloc memorie pentru noul nod din stiva
    newNode->val = v; //valoarea stocata in noul nod al stivei va fi un nod al arborelui
    newNode->next = *top;  //cream legatura
    *top = newNode;  //acum noul nod este varful stivei
}

int isEmpty(stackNode* top)
{
    return top == NULL;  //functia verifica daca stiva e goala
}

Nod_arbore* pop(stackNode** top)  //functie pentru scoaterea unui element din stiva
{
    if (isEmpty(*top))  //verific daca stiva e goala, adica daca am elemente de scos
        return NULL;

    stackNode *temp = (*top);   //variabila locala,temporara
    Nod_arbore* d = temp->val; //in stiva se afla elemente de tip nod_arbore, variabila d salveaza valoarea memorata in varful curent al stivei
    *top = (*top)->next; //varful stivei trece la urmatorul element

    free(temp); //eliberam memoria ocupata de variabila locala, adica stergem acel nod 

    return d; // returnam valoarea memorata in varful stivei
}

void deleteStack(stackNode** top)   //functie pentru stergerea stivei
{
    stackNode* topCopy= *top, *temp;  //variabile locale

    while (topCopy != NULL)  //cat timp stiva nu e goala afisam valorile memorate in ea si le stergem pe rand
    {
        temp = topCopy;
        topCopy = topCopy->next;
        printf("%s", temp->val->val->nume);

        free(temp);
    }

    *top = NULL;  //dupa ce operatiunea anterioara s-a terminat varful stivei va pointa catre null
}


void inorderNRec(Nod_arbore* root, FILE* rezultate)  //functie ce parcurge arborele inordine(SRD) iterativ
{
	int nr = 0;
	
    stackNode *S = NULL, *VEC = NULL; //creez stivele

    while (1)   //cat timp mai am elemente de procesat
    {
        while (root)   //daca exista nod radacina neprocesat
        {
            push(&S, root);  //adaug nodul radacina in stiva
            root = root->left;  //trec la urmatorul element din subarborele stang
        }

        if (isEmpty(S))  //verific daca stiva e goala
            break;

        root = pop(&S);   //scot un element din stiva

        push(&VEC, root);	//il adaug in stiva VEC
        
        nr++;	//creste numarul de resurse diferite

        root = root ->right; //si ma deplasez la dreapta
    }  // procedeul se repeta 

    fprintf(rezultate, "%d ",nr );	//afisez numarul de resurse diferite

    while(!isEmpty(VEC))  //verific daca stiva e goala
    {
        root = pop(&VEC);   //scot un element din stiva in care am salvat resursele
        fprintf (rezultate,"%s ", root->val->nume);  //afisez numele resursei
    }

    fprintf(rezultate, "\n" );

    deleteStack(&S);  //sterg stiva dupa ce am terminat
    deleteStack(&VEC);  //sterg stiva dupa ce am terminat
}

void postorder_elib(Nod_arbore* root) //functie ce elibereaza spatiul ocupat de un arbore binar
{
	if (root)  //daca radacina nu e nula
	{
		postorder_elib(root->left);  //parcurg subarborele stang
		postorder_elib(root->right);  //parcurg subarborele drept
		free(root);  //sterg valorile stocate si eliberez memoria
	}
	root=NULL;  //radacina e acum null
}

void max_resurse(Island* insula,FILE* rezultate ,int nr_insule)
{
	int i,j;
	
	Nod_arbore *root = NULL;

	for(i = 0; i <nr_insule; i++)		//parcurg insulele
	{
		for(j = 0;j < (insula+i)->nrResurse; j++)	//parcurg resursele fiecarei insule
		{
			root = insert(root, &(insula+i)->inventarResurse[j]);	//introduc in arbore resursele
		}
	}

	inorderNRec(root,rezultate);	//afisez in fisier numarul de resurse distincte de pe tot arhipelagul, urmat de
									//numele resurselor separate printr-un spatiu in ordine lexicografica
}

void max_cantitate(FILE* date, FILE* rezultate, Island* insula, int nr_insule)
{
	int maxi = 0, i, j;
	char*resursa = malloc(sizeof(char));
	resursa = setsir(resursa,date);		//citesc resursa
	
	for(i = 0;i < nr_insule; i++)
	{
		for(j = 0; j < (insula+i)->nrResurse; j++)
		{
			if(strcmp((insula+i)->inventarResurse[j].nume,resursa) == 0)
			{
				if((insula+i)->inventarResurse[j].cantitate > maxi) 
				{
					maxi = (insula+i)->inventarResurse[j].cantitate;	//aflu care este cantitatea cea mai mare de resursa disponibila
				}
			}
		}
	}

	for(i = 0; i < nr_insule; i++)
	{
		for(j = 0; j < (insula+i)->nrResurse; j++)
		{
			if(strcmp((insula+i)->inventarResurse[j].nume,resursa) == 0)
			{
				if((insula+i)->inventarResurse[j].cantitate == maxi) 
				{
					fprintf(rezultate, "%s ",(insula+i)->nume );	//afisez insula (respectiv insulele) cu cantitatea cea mai
																	//mare de resursa disponibila
				}
			}
		}
	}
	
	fprintf(rezultate,"\n");
	
	free(resursa);
}

int minDistance(int dist[], int sps[], int V)
{
	int  mini = INT_MAX;
	int v, min_index;

	for ( v = 0; v < V; v++)	//parcurg nodurile
	if (sps[v]==0 && dist[v] <= mini) 	//daca nu avem element pe pozitia v in sps
	{					// si costul varfului e mai mic decat cel al minimului
		mini = dist[v];		//actualizam valoarea stocata in min
		min_index = v;		//retinem pozitia varfului cu cost minim
	}

	return min_index;		//returnam varful minim ce va fi pus in sps
}

int timp(int dist[], int n, int destinatie)		//functia determina "timpul" parcurs dimineata
{
	for (int i = 0; i < n; i++)
	{
		if(i == destinatie) return dist[i];		//in dist[i] e stocat costul, durata corespunzatoare zborului
		else if(i == INT_MAX) return INT_MAX;
	}
}

int dijkstra(Graph *g, int s, int pred[], int k)
{
	int i, j, u;
	int dist[g->V];     	//  vectorul cu distantele de la sursa la destinatie 
	int sps[g->V]; 	    	// sps[i]  e 1 daca varful i e inclus in cea mai scurta cale
				// initial toate distantele sunt infinit si niciun element nu e in sps 
	for (  i = 0; i < g->V; i++)
	{    				//initializam vectorii
		pred[i] = -2;		
		dist[i] = INT_MAX; 
		sps[i] = 0;   
	}	// distanta de la sursa la ea insasi e  0 si adaugam sursa in sps 

	dist[s] = 0;    //calculez pentru toate varfurile calea de cost minim 
	pred[s]=-1;

	for (j = 0; j < g->V-1; j++)
	{	
 		// iau varful cu distanta minima pana la sursa dintre varfurile care nu sunt in sps
		u = minDistance(dist, sps, g->V); 
					// il adaug in sps 
		sps[u] = 1;	//incep sa il procesez

		// Updatez valoarea distantei pentru fiecare varf adiacent cu u
		for (i = 0; i < g->V; i++)
		{	
			// Updatez dist[i] daca varful i nu e in sps, daca u si i adiacente si
			//daca distanta pe noua ruta e mai mica decat cea stocata anterior 
			if (sps[i] == 0 && g->a[u][i] != 0  && dist[u] != INT_MAX &&  dist[u] + g->a[u][i] < dist[i])
			{			
				dist[i] = dist[u] + g->a[u][i];
				pred[i]=u;	//actualizez valoarea stocata in vectorul de predecesori pentru nodul corespunzator
						//ea este valoarea ultimului nod adaugat in sps care respecta conditiile
			}
		}
	}	// afisaza vectorul cu distantele de la sursa la fiecare varf
	
	return timp(dist, g->V, k);		//returneaza durata minima in conditii de distribuire instanta(dimineata)
} 

int dijkstra_alt(Graph *g, int s, int pred[], int k)
{
	int i, j, u;
	int dist[g->V];     	//  vectorul cu distantele de la sursa la destinatie 
	int sps[g->V]; 	    	// sps[i]  e 1 daca varful i e inclus in cea mai scurta cale
				// initial toate distantele sunt infinit si niciun element nu e in sps 
	for (  i = 0; i < g->V; i++)
	{    				//initializam vectorii
		pred[i] = -2;		
		dist[i] = INT_MAX; 
		sps[i] = 0;   
	}	// distanta de la sursa la ea insasi e  0 si adaugam sursa in sps 

	dist[s] = 0;    //calculez pentru toate varfurile calea de cost minim 
	pred[s]=-1;

	for (j = 0; j < g->V-1; j++)
	{	
 		// iau varful cu distanta minima pana la sursa dintre varfurile care nu sunt in sps
		u = minDistance(dist, sps, g->V); 
		printf("%d\n",u);	// il adaug in sps 
		sps[u] = 1;	//incep sa il procesez

		// Updatez valoarea distantei pentru fiecare varf adiacent cu u
		for (i = 0; i < g->V; i++)
		{	
			// Updatez dist[i] daca varful i nu e in sps, daca u si i adiacente si
			//daca distanta pe noua ruta e mai mica decat cea stocata anterior 
			// +15 reprezinta durata stationarii pe fiecare insula pentru a evita supraincarcarea aeroporturilor insulelor
			if (sps[i] == 0 && g->a[u][i] != 0  && dist[u] != INT_MAX &&  dist[u] + g->a[u][i] + 15 < dist[i] )
			{			
	
				dist[i] = dist[u] + g->a[u][i] + 15 ;
				pred[i]=u;	//actualizez valoarea stocata in vectorul de predecesori pentru nodul corespunzator
						//ea este valoarea ultimului nod adaugat in sps care respecta conditiile
			}
		}
	}	

	return timp(dist,g->V,k);
} 


int drum(Graph* g, int pred[], int j, int k, FILE* rezultate, int init, int* nr, char* cod)	
{		//functia afiseaza drumul de la un nod dat pana la destinatie
	int a, i;
	
	for(i = 0; i < g->V; i++)
	{
		if(i == k ) a = pred[i];	//in a stocam pozitia valorii varfului predecesor
		
	}

		//stim ca varful sursa va avea intotdeauna pozitia -1 in vectorul de predecesori
		//procesul recursiv se opreste in momentul in care se ajunge la varful sursa
		//apoi se afiseaza valorile "recursiv"(de la sursa pana la ultimul varf)
	if(a < -1) return -1;	//inseamna ca nu am gasit drum

	for(i = 0; i < g->V; i++)
	{
		if(i == a) drum(g,pred,j,i,rezultate,init,nr,cod);		
	}
		
	if(k!=init)		//nodul e diferit de sursa
 	{
 		if(strcmp(cod,"drum_zbor")==0)
 			fprintf(rezultate,"Island%d ",k+1);	//afisez insulele
   	}
	
}

void drum_zbor(FILE* date, FILE* rezultate, Graph* g, char* cod)
{
	int nr = 0;
	int tara1 = conversie_island(date);
	int tara2 = conversie_island(date);		//citesc cele 2 tari si le aflu pozitia in matrice
	int exista_drum;
	int* pred = malloc(sizeof(int)*g->V);	//aloc memorie pentru vectorul de predecesori

	dijkstra(g,tara1,pred,tara2);	//aplic algoritmul lui dijkstra pentru a afla care distanta minima si predecesorii
	exista_drum=drum(g,pred,tara1,tara2,rezultate,tara1,&nr,cod);	//aflu si afisez calea dintre cele doua insule citite

	if(exista_drum == -1) fprintf(rezultate, "NO\n" );
	else fprintf(rezultate,"\n");
	
	free(pred);
}

void timp_zbor(FILE* date, FILE* rezultate, Graph* g)
{
	int tara1 = conversie_island(date);
	int tara2 = conversie_island(date);
	int* pred = malloc(sizeof(int));

	if(dijkstra(g,tara1,pred,tara2) == INT_MAX) fprintf(rezultate, "INF\n" );
	else fprintf(rezultate, "%d\n", dijkstra(g,tara1,pred,tara2));		//functia dijkstra returneaza durata minima
	
	free(pred);
}

void min_zbor(FILE* date, FILE* rezultate, Graph* g, char* cod)
{
	int cost_initial, cost_final, nr = 0;
	int tara1 = conversie_island(date);
	int tara2 = conversie_island(date);
	int exista_drum;
	int* pred = malloc(sizeof(int)*g->V);

	cost_initial = dijkstra_alt(g, tara1, pred, tara2);	//retine durata minima in conditiile stationarii pe insule
	
	fprintf(rezultate, "%d\n", cost_initial - 15 );	//scad 15 intrucat algoritmul adauga 15 minute si insulei finale

	free(pred);
}

void oprire(int* mini,int* maxi)	//functia determina minimul si maximul "binar" al cerintelor
{									//ea va fi utilizata pentru a reusi "sa oprim" procesul citirii elementelor 
									//ce nu trebuie sa indeplineasca cerintele

	char cerinte[9][13]={"conexiune","legatura","adauga_zbor","max_resurse","drum_zbor","anulare_zbor","timp_zbor","max_cantitate","min_zbor"};
	int i, j, *s = calloc(9, sizeof(int)), min, max;	//vectorul de sume e initializat cu 0
	for(i=0;i<9;i++)
	{
		for(j=0;j<strlen(cerinte[i])+1;j++)
		{
			s[i] = s[i] + cerinte[i][j]-'0';	//calculam suma pentru fiecare cuvant din vectorul de cerinte
												//transform in int fiecare caracter si calculez suma
		}
	}

	min = s[0];
	max = s[0];		//initial minimul si maximul se afla pe prima pozitie

	for(i = 0; i < 9; i++)	//parcurg vectorul de cerinte si determin care este "cuvantul" minim, respectiv maxim
	{
		if(s[i] < min) min = s[i];
		if(s[i] > max) max = s[i];
	}

	*mini = min;
	*maxi = max;	//transmit valorile prin referinta
	
	free(s);	//eliberez memoria ocupata de vectorul de suma
}

void cerinte(FILE*date,Graph*g,FILE*rezultate,int nr_insule,Island*insula,int mini,int maxi)
{
	char *x = malloc(sizeof(char));		//aloc spatiu pentru numele cerintei
	int i, s;

	x = setsir(x, date);	//citesc numele cerintei

    do
    {
    	s = 0;

    	for(i = 0; i < strlen(x)+1; i++)
		{
			s = s + x[i]-'0';		//calculez valoarea intreaga a cuvantului citit

		}
        
	        if(s >= mini && s <= maxi)		//daca valoarea intreaga se afla in intervalul de cerinte atunci le rezolv
	        {
		        if(strcmp(x, "conexiune") == 0) conexiune(date, g, rezultate);
		        if(strcmp(x, "legatura") == 0) legatura(date, rezultate, g);
		        if(strcmp(x, "adauga_zbor") == 0) adauga_zbor(date, rezultate, &g);
		        if(strcmp(x, "max_resurse") == 0) max_resurse(insula, rezultate, nr_insule);
		        if(strcmp(x, "drum_zbor") == 0) drum_zbor(date, rezultate, g, "drum_zbor");	
		        if(strcmp(x, "anulare_zbor") == 0) anulare_zbor(date, rezultate, g);
		        if(strcmp(x, "timp_zbor") == 0) timp_zbor(date, rezultate, g);
		        if(strcmp(x, "max_cantitate") == 0) max_cantitate(date, rezultate, insula, nr_insule);  
		        if(strcmp(x, "min_zbor") == 0) min_zbor(date, rezultate, g, "min_zbor");	
	       	}
	       	else 	//altfel s primeste valoarea -1
	       		{
	       			s = -1;
	       		}
       	if(s == -1) break;	//daca s=-1 atunci va iesi din ciclul while

		}while((fscanf(date, "%s", x)) != EOF);	//pana la sfarsitul fisierului
    
        insula->nrAvioane = atoi(x);	//convertesc in int sirul de caractere ce nu apartine intervalului de cerinte
        								//deoarece el reprinza un numar si anume cel de insule corespunzator etapei de gestiune

        free(x);  //eliberez spatiul ocupat de vectorul care retine numele cerintei
}

int* vector_insule(FILE* date, Island* insula)
{
	int i, j;
	int* nr_av_insula = malloc(sizeof(int) * insula->nrAvioane);	//aloc memorie pentru vectorul de total avioane/insula

	for(i = 0; i < insula->nrAvioane; i++)
	{
		fscanf(date, "%d ", &nr_av_insula[i]);	//citesc valorile din fisier
	}
	
	for(i = 0; i < insula->nrAvioane; i++)
	{
		(insula+i)->avioane = malloc(sizeof(int) * nr_av_insula[i]);	//aloc memorie pentru pentru vectorul de avioane

		for(j = 0; j < nr_av_insula[i]; j++)
		{
			fscanf(date,"%d ",&(insula+i)->avioane[j]);		//citesc avioanele fiecarei insule
		}
	}
	return nr_av_insula;	//returnez vectorul ce contine numarul de avioane pentru fiecare insula
}

Graph* citire_gestiune(FILE* date, Island* insula, int* nr_avi_insula)
{
	Graph* situatie_zbor;
	int i, j;
	
	situatie_zbor = alocGraph(insula->nrAvioane);	//aloc memorie pentru noul graf

	for(i=0;i<insula->nrAvioane;i++)	//parcurg matricea de adiacenta specifica grafului si ii atribui valorile citite
	{
		for(j=0;j<insula->nrAvioane;j++)
		{
			fscanf(date,"%d",&situatie_zbor->a[i][j]);
		}
	}
	
	return situatie_zbor;	//returnez graful creat
}

int existenta_avioane(Graph* situatie_zbor, Island* insula,int *nr_av_insula, FILE* rezultate)
{
	int ok = 0, i, j, toleranta, stack_over = 0;	

	toleranta = insula->tolAvioane;

	for(i = 0; i < insula->nrAvioane; i++)	//parcurg vectorul de total avioane/insula
	{
		if(nr_av_insula[i] > toleranta)		//inseamna ca trebuie distribuit macar un avion
		{
			ok = 1;	//daca i se schimba valoarea in 1 inseamna ca avem avioane de distribuit
			stack_over++;	//creste valoarea variabilei
		}
	}

	if(ok == 0)	//daca nu s-a modificat valoarea stocata in ok
	{			//inseamna ca nicio insula nu trebuie sa distribuie avioane
		for(i = 0; i < insula->nrAvioane; i++)
		{
			fprintf(rezultate, "Island%d\n", i+1 );

			for(j = 0; j < nr_av_insula[i]; j++)
			{
				fprintf(rezultate, "%d ", (insula+i)->avioane[j]);	//afisam valorile initiale
			}
			fprintf(rezultate, "\n");
		}
	}

	if(stack_over == insula->nrAvioane) //inseamna ca toate insulele trebuie sa-si distribuie avioanele
	{									//ceea ce inseamna ca nu e suficient spatiu
		fprintf(rezultate, "Stack overflow!\n");
		return -1;	//returnam daca nu e suficient spatiu pentru a afla daca mai are rost sa continuam rezolvarea anumitor cerinte
	}
}

int crescator(const void *nr_1, const void *nr_2)
{
     int avion1 = *(const int *)nr_1;
     int avion2 = *(const int *)nr_2;

     return (avion1 - avion2);
}

Graph* graf_distribuire_insula (int n)
{  // functia aloca spatiu pentru un graf si il initializeaza cu valori
	int i, j, k, ct=0, orientat, **a;

	Graph *g = malloc(sizeof(Graph));	//alocam memorie pentru graf

	if (g==NULL) 
	{ 
		printf( " no memory for this graph");
		return NULL;
	}

	g->V = n;

	g->a = malloc((g->V)*sizeof(int*));	//alocam memorie pentru matricea de adiacenta

	for (i=0; i < g->V; i++) g->a[i] = calloc(g->V, sizeof(int)); //o initializez cu 0

	if (g->a == NULL) 
	{ 
		printf( " no memory for this matrix");  
		return NULL;
	}

	g->E = 0;

	for (i=0; i < g->V; i++) 
	{			//parcurgem matricea
		for (j = g->V; j > i; j--)		//pentru matricea inferior triunghiulara
		{
			//g->a[j][i]=1;
			g->a[i][j] = 1; 		
			if(g->a[i][j]!=0) g->E+=2;	//numarul de arce creste cu 2 (matrice simetrica)
		}
	}
	return g;
} 

int numar(int* avioane, int* val_numar, int n_av, int k, int n_val)
{
	int i, j, ok = 0;

	for(j = 0; j < n_av; j++)
	{
		ok = 0;
		for(i = 0; i < n_val; i++)
		{
			if(j == k)	ok = 1;
			if(avioane[j] == val_numar[i]) ok=1; 
		}

		if(ok == 0) return avioane[j];
	}
}

void distribuire_2_avioane(Island* insula, FILE* rezultate, int* avioane_de_distribuit, int i, int n, int final, int k)
{

	for(int indice2 = 0; indice2 < final; indice2++)
	{
		fprintf(rezultate, "%d ", (insula+k)->avioane[indice2]);
	
		printf( "%d ", (insula+k)->avioane[indice2]);
	}

	if(i == n) 
	{
		fprintf(rezultate, "%d \n",avioane_de_distribuit[0] );
	
		for(int indice2=0;indice2<final;indice2++)
		{
			 fprintf(rezultate, "%d ", (insula+k)->avioane[indice2]);
		}

		fprintf(rezultate, "\n" );
		
		return;
	}

	for(int j = i; j < n; j++) 
	{
		fprintf(rezultate, "%d ",avioane_de_distribuit[j] );
	}

	fprintf(rezultate, "\n" );

	distribuire_2_avioane(insula, rezultate, avioane_de_distribuit, i+1, n, final, k);
}

void redistribuire_avioane(int avioane_distribuite,Island*insula,int insula_primitoare, int indice_parinte, int* nr_av_insula,FILE*rezultate )
{
	int i;
	int*vect_insule=malloc(sizeof(int)*avioane_distribuite);
	int copie_avioane_distribuite = avioane_distribuite,p=0;
	int k,total=nr_av_insula[insula_primitoare]+avioane_distribuite,inceput_adaugare=nr_av_insula[insula_primitoare];
	
	for(i=nr_av_insula[indice_parinte];i>0;i--)
	{
		if(copie_avioane_distribuite!=0)
		{
			vect_insule[p]=(insula+indice_parinte)->avioane[i-1];
			p++;
			copie_avioane_distribuite--;
		}
	}

	(insula+insula_primitoare)->avioane=malloc(sizeof(int)*total);
	//creeaza un vector in care sa retii valorile vechi pt insula si in care sa le adaugi pe cele noi
	//apoi realaca spatiu pt insula si copiaza valorile din vector in insula
	//mai trebuie sa scazi dimensiunea parintelui pentru a putea primi noile valori
	//transmiti prin parametru noua valoare

	for(i=0;i<p;i++)
	for(k = inceput_adaugare; k < total; k++)
	{
		(insula+insula_primitoare)->avioane[k] = vect_insule[i];
	}

	for(k=0;k<total;k++)
	{
		fprintf(rezultate , "%d ",(insula+insula_primitoare)->avioane[k] );
	}

}

void rezolvare_zboruri_problematice(Island*insula,Graph*situatie_zbor,int i,int avioane_distribuite,int* nr_av_insula,FILE*rezultate )
{
	int*toleranta=malloc(sizeof(int)), j;
	int suma_toleranta_avioane=0;

	for(j=0;j<insula->nrAvioane;j++)
	{
		(toleranta[j]) = insula->tolAvioane - nr_av_insula[j];

		
		if(avioane_distribuite > 0 && situatie_zbor->a[i][j]==1)
		{
			suma_toleranta_avioane=suma_toleranta_avioane+toleranta[j];
		}
	}

	for(j=0;j<insula->nrAvioane;j++)
	if(suma_toleranta_avioane < avioane_distribuite && situatie_zbor->a[i][j]==1) 
		rezolvare_zboruri_problematice(insula,situatie_zbor,j,avioane_distribuite,nr_av_insula,rezultate );

	if(suma_toleranta_avioane >= avioane_distribuite &&  situatie_zbor->a[i][j]==1) 
		redistribuire_avioane(avioane_distribuite,insula,j,i,nr_av_insula,rezultate );

	free(toleranta);
}

void distribuire_avioane(Island* insula, Graph* situatie_zbor, int i, int* nr_av_insula, FILE* rezultate)
{
	
	int avioane_distribuite, k = 0, j, *toleranta = malloc(sizeof(int)), indice1, indice2, n, frunze, nr_frunze = 0;
	Graph* g;

	avioane_distribuite = nr_av_insula[i] - insula->tolAvioane;

	int copie_distribuire = avioane_distribuite, *val_nivel = malloc(sizeof(int));
	int* vect_insule = malloc(sizeof(int) * avioane_distribuite), *copie_vect_insule = malloc(sizeof(int) * avioane_distribuite);
	int copie_avioane_distribuite = avioane_distribuite, p = 0, ok, *vector_frunze = malloc(sizeof(int)), total_frunze = 0;
	int suma_toleranta_avioane = 0;
	
	for(j = nr_av_insula[i]; j > 0; j--)
	{
		if(copie_distribuire != 0)
		{
			vect_insule[p] = (insula+i)->avioane[j-1];
			p++;
			copie_distribuire--;
		}
	}
	
	for(j = 0; j < p; j++)
	{
		copie_vect_insule[j] = vect_insule[j];
	}

	qsort(copie_vect_insule, p, sizeof(int), crescator);

	k=0;	

	for(j = 0; j < insula->nrAvioane; j++)
	{
		(toleranta[j]) = insula->tolAvioane - nr_av_insula[j];

		if(avioane_distribuite > 0 && situatie_zbor->a[i][j] == 1)
		{
			suma_toleranta_avioane = suma_toleranta_avioane + toleranta[j];
		}
	}

	for(j = 0; j < insula->nrAvioane; j++)
	{
		if(suma_toleranta_avioane < avioane_distribuite && situatie_zbor->a[i][j] == 1) 
		{
			rezolvare_zboruri_problematice(insula,situatie_zbor,j,avioane_distribuite,nr_av_insula,rezultate );
		}
	}

	for(j = 0; j < insula->nrAvioane; j++)
	{
		if(j == i)
		{
			fprintf(rezultate, "Island%d\n", j+1);

			for(indice1 = 0; indice1 < insula->tolAvioane; indice1++)
			{
				fprintf(rezultate , "%d ", (insula+i)->avioane[indice1]);
			}

			fprintf(rezultate , "\n" );
		}

		else if(situatie_zbor->a[i][j] == 1) 
		{	
			if(toleranta[j] == 0)
			{
				fprintf(rezultate, "Island%d\n", j+1);

				for(indice2 = 0;indice2 < nr_av_insula[j]; indice2++)
				{
					fprintf(rezultate , "%d ", (insula+j)->avioane[indice2]);
				}

				fprintf(rezultate , "\n" );
			}
			else fprintf(rezultate, "Island%d\n", j+1 );

			if(j == i) ++j;

			int n;
			
			n = avioane_distribuite;

			if(n == 2 && toleranta[j] <= insula->tolAvioane && toleranta[j] != 0)
			{
				distribuire_2_avioane(insula, rezultate, copie_vect_insule, 0, n, nr_av_insula[j], j);
			}

			g = alocGraph(n);

			if(n > 2 && toleranta[j] <= insula->tolAvioane && toleranta[j] != 0)
			{
				g = graf_distribuire_insula (n);

				int ct = 0;
			
				int n_tol = avioane_distribuite;
				int nr = n;
				total_frunze = 0;
				do
				{	
					for (indice1 = 0; indice1 < n; indice1++) 
					{		
						
						for(indice2 = 0;indice2 < nr_av_insula[j]; indice2++)
						{
							if(n <= toleranta[j]) fprintf(rezultate, "%d ", (insula+j)->avioane[indice2]);
						}

						if(n <= toleranta[j]) fprintf(rezultate, "%d ", copie_vect_insule[indice1]); 
					
						if(n == 1 && (n <= toleranta[j])) fprintf(rezultate, "%d ", vect_insule[indice1+1]);

						for (indice2 = n; indice2 > i; indice2--)		//pentru matricea inferior triunghiulara
						{	
							if(g->a[indice1][indice2] == 1)
							{
								if(n <= toleranta[j]) fprintf(rezultate, "%d ", vect_insule[indice2-2]);
								ct++;

								if(ct == nr-2)
								{
									val_nivel[nr_frunze] = vect_insule[indice2-2];
									nr_frunze++;
								}
							}
						}

						if(n <= toleranta[j]) fprintf(rezultate, "\n" );

						if(n != 1)
						{
							frunze = numar(copie_vect_insule,val_nivel,nr,indice1,nr_frunze);
							vector_frunze[total_frunze] = frunze;
							total_frunze++;
						}
						
						n--;
						nr_frunze=0;
						ct=0;
					}
				
					copie_avioane_distribuite--;
				
				}while(n != 0 );

				free(val_nivel);

			if(n == 0)
			{
				for(indice1 = 0;indice1 < total_frunze ; indice1++)
				{
					for(indice2 = 0; indice2 < nr_av_insula[j]; indice2++)
					{
						if(n <= toleranta[j] && toleranta[j] != 0) fprintf(rezultate, "%d ", (insula+j)->avioane[indice2]);
					}

					if(n <= toleranta[j] && toleranta[j]!=0) fprintf(rezultate, "%d\n", vector_frunze[indice1]); 
				}	
			}

			if(toleranta[j] != 0 && toleranta[j] < avioane_distribuite)
			{
				for(indice2 = 0; indice2 < nr_av_insula[j]; indice2++)
				{
					if(n <= toleranta[j] ) fprintf(rezultate, "%d ", (insula+j)->avioane[indice2]);
				}

				fprintf(rezultate, "\n" );
			}
		}
	}

	else if(situatie_zbor->a[i][j] == 0)
	{
		fprintf(rezultate, "Island%d\n", j+1);

		for(indice1=0;indice1<nr_av_insula[j];indice1++)
		{
			fprintf(rezultate , "%d ", (insula+j)->avioane[indice1]);
		}

		fprintf(rezultate , "\n" );
	}
}
	elib_matr_graph(&g);
	free(toleranta);
	free(vect_insule);
	free(copie_vect_insule);
	free(vector_frunze);	
}

int main(int nr_fisiere,char**fisier)
{
	Graph *g, *situatie_zbor;
    Island* insula;
    int nr_insule, nr_conexiuni;
    int mini, maxi, verdict_distribuire;

    FILE* date = fopen(fisier[1], "rb"), *rezultate = fopen(fisier[2], "wb");	

    if(date == NULL)
    {
        printf("Fisierul aferent datelor nu s-a putut deschide!");
        exit(1);
    }
    if(rezultate == NULL)
    {
        printf("Fisierul aferent rezultatelor nu s-a putut deschide!");
        exit(1);
    }

    fscanf(date, "%d", &nr_insule);		//citesc numarul de insule
    insula = malloc(sizeof(Island) * nr_insule);	//aloc spatiu pentru structura de date de tip Island
    citire(insula, nr_insule, date);		//citesc din fisier datele aferente structurii
    

    g = alocGraph(nr_insule);	//aloc spatiu pentru graf

    fscanf(date, "%d" , &nr_conexiuni);		//citesc numarul de conexiuni dintre insule

    creare_matrice(date, nr_conexiuni, g, insula, nr_insule);	//creez matricea de costuri aferenta grafului de avioane

	oprire( &mini, &maxi);		//determin intervalul pentru cerinte

    cerinte(date, g, rezultate, nr_insule, insula, mini, maxi);	//rezolv cerintele destinate partilor de proiectare si analiza

    if(insula->nrAvioane != 0)	//daca avem avioane pentru etapa de gestiune
    {
    	int *nr_av_insula;
	  
	    fscanf(date,"%d ",&insula->tolAvioane);		//citesc toleranta maxima
	    
	    nr_av_insula=malloc(sizeof(int)*insula->nrAvioane);		//aloc spatiu pentru vectorul in care este stocat 
	    														//numarul de avioane pentru fiecare insula
	    situatie_zbor = alocGraph(insula->nrAvioane);		//aloc spatiu pentru matricea de adiacenta 
	    													//care precizeaza existenta conexiunilor directe intre insule
	    
	    nr_av_insula = vector_insule(date,insula);	//populez vectorul
	    situatie_zbor = citire_gestiune(date,insula,nr_av_insula);	//citesc valorile pentru graf
	   
	    int i, j;
	    
	    elib_matr_graph(&g);	//eliberez momoria ocupata de graful utilizat pentru rezolvarea cerintelor anterioare
	    
	    //aflu daca continui sa rezolv urmatoarele cerinte
	    //si afisez valorile input-ului in cazul in care nu se distribuie niciun avion
	    verdict_distribuire = existenta_avioane(situatie_zbor,insula,nr_av_insula,rezultate);	

	    if(verdict_distribuire != -1)	//daca se pot distribui avioane
	    {
		    for(i = 0; i < insula->nrAvioane; i++)	//pentru fiecare insula
		    {
		    	if(nr_av_insula[i] > insula->tolAvioane)	//verific daca depaseste toleranta maxima 
		    	{											//=>trebuie sa-si distribuie avioanele in plus

		    		distribuire_avioane(insula,situatie_zbor,i,nr_av_insula,rezultate);	//incep procesul distribuirii pentru insula 
		    																			//care depaseste toleranta maxima
		    	}
		    }
		}
		
		free(nr_av_insula);
		elib_matr_graph(&situatie_zbor);

		fclose(date);
	    fclose(rezultate);
   }
   else  elib_matr_graph(&g);
   free(insula);
   
   return 0;
}
