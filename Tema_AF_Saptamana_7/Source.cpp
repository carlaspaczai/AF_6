/*
* Student: Spaczai Carla Noemi
* Grupa: 30229
* Am implementat 2 algoritmi de complexitate liniara (O(n)) pentru transformarea arborilor multicai intre:
* Reprezentare Parinte -> Reprezentare Arbore Multicai (T1)
* Reprezentare Arbore Multicai -> Reprezentare Binara (T2)
* 
* Reprezentare Parinte:
* Parcurgem vectorul de parinti, cautam si afisam copiii (prin intermediul parintilor) in functie de nivelul la care se afla si apelam in mod recursiv aceasta functie (copilul
* actual va fi parintele din urmatoarea executie a functiei).
* 
* Transformare: Reprezentare Parinte -> Reprezentare Arbore Multicai:
* Parcurgem vectorul de parinti si pentru fiecare element cream parintele (daca nu exista deja) si adaugam copilul sau. Aceste legaturi se pastreaza prin intermediul nodurilor
* si a unui vector de noduri.
* 
* Reprezentare Arbore Multicai:
* Parcurgem arborele cu ajutorul radacinii (si a legaturilor) returnate de transformarea anterioara si afisarea se face in functie de nivelul la care se afla nodurile. Functia
* se apeleaza in mod recursiv astfel incat viitorul parinte este copilul parintelui actual.
* 
* Transformare: Reprezentare Arbore Multicai -> Reprezentare Binara:
* Parcurgem arborele creat la transformarea anterioara si pentru fiecare nod o sa 'retinem' primul copil (de pe prima pozitie) si fratii sai. Functia se apeleaza in mod recursiv
* astfel incat sa fie 'retinute' aceste legaturi pentru fiecare nod (unde este cazul) cu ajutorul a doua noduri.
* 
* Reprezentare Binara:
* Parcurgerea realizata este similara unei parcurgeri in preordine si presupune afisarea fiecarui nod (cu ajutorul apelurilor functiei (de tip recursiv)) in functie de nivelul
* la care se afla.
*/

#include <stdio.h>
#include <stdlib.h>

void prettyPrintR1(int v[], int n, int p, int numarSpatii) // Reprezentare Parinte
{
	int q = 0;
	for (int i = 0; i < n; i++)
	{
		if (v[i] == p)
		{
			if (q != i + 1 && q != 0)
			{
				numarSpatii--;
			}
			for (int j = 0; j < numarSpatii; j++)
			{
				printf("	");
			}
			numarSpatii++;
			q = i + 1;
			printf("%d\n", q);
			prettyPrintR1(v, n, q, numarSpatii);
		}
	}
}

typedef struct NodeMWayTree
{
	int Data;
	int numberPopulated;
	struct NodeMWayTree* Children[100];
} NodeMWayTree;

NodeMWayTree* createNodeMWT(int Data)
{
	NodeMWayTree* NMW = (NodeMWayTree*)malloc(sizeof(NodeMWayTree));
	NMW->Data = Data;
	NMW->numberPopulated = NULL;
	return NMW;
}

NodeMWayTree* T1(int v[], int n) // Transformare: Reprezentare Parinte -> Reprezentare Arbore Multicai
{
	NodeMWayTree* ROOT = createNodeMWT(NULL);
	NodeMWayTree** VN = (NodeMWayTree**)calloc(n, sizeof(NodeMWayTree*));
	for (int i = 0; i < n; i++) // Parcurgem vectorul de parinti
	{
		if (v[i] == -1) // Daca e radacina
		{
			if (VN[i] == NULL)
			{
				ROOT = createNodeMWT(i + 1);
			}
			else
			{
				ROOT = VN[i];
			}
		}
		else // Daca nu e radacina
		{
			NodeMWayTree* P = VN[v[i] - 1]; // Punem parintele in vector
			if (P == NULL)
			{
				P = createNodeMWT(v[i]);
				VN[v[i] - 1] = P;
			}
			int numberChildren = P->numberPopulated; // Numarul actual de copii
			if (VN[i] == NULL) // Verificam copilul parintelui actual si facem legatura
			{
				P->Children[numberChildren] = createNodeMWT(i + 1);
			}
			else
			{
				P->Children[numberChildren] = VN[i];
			}
			VN[i] = P->Children[numberChildren];
			P->numberPopulated = numberChildren + 1;
		}
	}
	return ROOT;
}

void prettyPrintR2(NodeMWayTree* ROOT, int numarSpatii) // Reprezentare Arbore Multicai
{
	if (ROOT != NULL)
	{	
		for (int j = 0; j < numarSpatii; j++)
		{
			printf("	");
		}
		numarSpatii++;
		printf("%d\n", ROOT->Data);
		for (int i = 0; i < ROOT->numberPopulated; i++)
		{
			prettyPrintR2(ROOT->Children[i], numarSpatii);
		}
	}
	else
	{
		printf("Radacina e NULL!");
	}
}

typedef struct NodeBinaryTree
{
	int Data;
	struct NodeBinaryTree* Child;
	struct NodeBinaryTree* Sibling;
} NodeBinaryTree;

NodeBinaryTree* createNodeBT(int Data)
{
	NodeBinaryTree* NBT = (NodeBinaryTree*)malloc(sizeof(NodeBinaryTree));
	NBT->Data = Data;
	NBT->Child = NULL;
	NBT->Sibling = NULL;
	return NBT;
}

NodeBinaryTree* T2(NodeMWayTree* ROOT) // Transformare: Reprezentare Arbore Multicai -> Reprezentare Binara
{
	NodeBinaryTree* NBT = createNodeBT(ROOT->Data); // Noua radacina
	NodeBinaryTree* NBTN = NULL; // Nod intermediar
	if (ROOT->numberPopulated != 0)
	{
		NBT->Child = T2(ROOT->Children[0]); // Legaturi pentru copii
	}
	NBTN = NBT->Child;
	for (int i = 1; i < ROOT->numberPopulated; i++) // Legaturi pentru frati
	{
		NBTN->Sibling = T2(ROOT->Children[i]);
		NBTN = NBTN->Sibling;
	}
	return NBT;
}

void prettyPrintR3(NodeBinaryTree* ROOOOT, int numarSpatii) // Reprezentare Binara (Preordine)
{
	if (ROOOOT != NULL)
	{
		for (int j = 0; j < numarSpatii; j++)
		{
			printf("	");
		}
		numarSpatii++;
		printf("%d\n", ROOOOT->Data);
		prettyPrintR3(ROOOOT->Child, numarSpatii);
		prettyPrintR3(ROOOOT->Sibling, numarSpatii - 1);
	}
}

void demo()
{
	int v[] = {2, 7, 5, 2, 7, 7, -1, 5, 2};
	int n = sizeof(v) / sizeof(int);

	printf("Pretty-Print R1:\n");
	prettyPrintR1(v, n, -1, 0); // -1 = Parinte, 0 = Numar de Spatii
	printf("\n");
	
	printf("Pretty-Print R2:\n");
	NodeMWayTree* ROOT = createNodeMWT(NULL);
	ROOT = T1(v, n);
	prettyPrintR2(ROOT, 0); // 0 = Numar de Spatii
	printf("\n");

	printf("Pretty-Print R3:\n");
	NodeBinaryTree* ROOOOT = createNodeBT(NULL);
	ROOOOT = T2(ROOT);
	prettyPrintR3(ROOOOT, 0); // 0 = Numar de Spatii
}

int main()
{
	demo();
	return 0;
}