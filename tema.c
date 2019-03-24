#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct list
{
	int timestamp;
	double value;
	struct list* prev;
	struct list* next;
}*List;

// initializeaza lista 
List initList(int ts, double val)
{
	List newList = (List) malloc(sizeof(struct list));
	if (newList == NULL)
	{
		printf("Alocarea dinamica a esuat");
		exit(-1);
	}
	newList->timestamp = ts;
	newList->value = val;
	newList->next = NULL;
	newList->prev = NULL;
	return newList;
}

// adauga un element la inceputul listei 
List addFirst(List l, int ts, double val) {
	List new_element = initList(ts, val);
	new_element->next = l;
	new_element->prev = NULL;
	return new_element;
}

// adauga un element la sfarsitul listei
List addLast(List l, int ts, double val)
{
	List newElement = initList(ts, val);
	List temp;

	if (l == NULL)
	{
		l = newElement;
		return l;
	}
	else
	{
		temp = l;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = newElement;
		newElement->prev = temp;
		return l;
	}
}

// adauga un element in interiorul listei
List addItem(List l, int ts, double val) {
	List temp;

	temp = l;
	while (temp != NULL && val > temp->next->value)
		temp = temp->next;

	List new_element = initList(ts, val);
	new_element->next = temp->next;
	temp->next->prev = new_element;
	new_element->prev = temp; 
	temp->next = new_element;

	//free(temp);
	return l;
}

// cauta elementul cu valoarea val in lista; daca nu-l gaseste, returneaza NULL
List search(List l, double val)
{
	List temp;

	temp = l;
	while (temp != NULL && val != temp->value)
		temp = temp->next;

	return temp;
}

// sterge un element din lista
List deleteItem(List l, double val) {
	List temp, prev;


	// daca e primul element
	if (l->value == val)
	{
		temp = l;
		l = l->next;
		free(temp);
	}

	temp = l;
	while (temp != NULL && temp->value != val)
		temp = temp->next;

	prev = l;
	while (prev != NULL && prev->next->value != val)
		prev = prev->next;
	
	if (temp != NULL)
	{
		prev->next = temp->next;
		if (temp->next != NULL)
			temp->next->prev = prev;
		free(temp);
	}

	return l;
}

// sorteaza lista dupa campul value
List sortare(List l)
{
	double aux1;
	int aux2;
	List tmp1, tmp2;
	tmp1 = l;
	while (tmp1 != NULL)
	{
		tmp2 = tmp1->next;
		while (tmp2 != NULL)
		{
			if (tmp1->value > tmp2->value)
			{
				aux1 = tmp1->value;
				tmp1->value = tmp2->value;
				tmp2->value = aux1;

				aux2 = tmp1->timestamp;
				tmp1->timestamp = tmp2->timestamp;
				tmp2->timestamp = aux2;
			}
			tmp2 = tmp2->next;
		}
		tmp1 = tmp1->next;
	}

	//free(tmp1);
	//free(tmp2);
	return l;
}

// sterge toata lista
List deleteList(List l) {
	List temp;
	while (l != NULL)
	{
		temp = l;
		l = l->next;
		free(temp);
	}
	while (l != NULL)
	{
		temp = l;
		l = l->prev;
		free(temp);
	}
}

// afisarea listei
void printList(List l, int nrPerechi)
{
	printf("%d\n", nrPerechi);
	while (l != NULL)
	{
		printf("%d %.2lf\n", l->timestamp, l->value);
		l = l->next;
	}
}

List eliminare_exceptii(List l, int k, int *nrPerechi)
{
	int n, i;
	double avg, dev;

	List lst, tmp;
	tmp = l;
	lst = initList(tmp->timestamp, tmp->value);
	tmp = tmp->next;
	for (i = 1; i < *nrPerechi; i++)
	{
		lst = addLast(lst, tmp->timestamp, tmp->value);
		tmp = tmp->next;
	}

	lst = lst->next->next;
	n = *nrPerechi;
	for (i = 0; i < n - k + 1; i++)
	{
	
		avg = (lst->prev->prev->value + lst->prev->value + lst->value + 
				lst->next->value + lst->next->next->value) / k ;
		dev = sqrt(((lst->prev->prev->value - avg) * 
			(lst->prev->prev->value - avg) + (lst->prev->value - avg) *
			(lst->prev->value - avg) + (lst->value - avg) * (lst->value - avg) +
			(lst->next->value - avg) * (lst->next->value - avg) +
			(lst->next->next->value - avg) * (lst->next->next->value - avg))/k);
		//printf("%lf %lf\n", avg, dev);
		if (lst->value < avg - dev || lst->value > avg + dev)
		{
			l = deleteItem(l, lst->value);
			*nrPerechi = *nrPerechi - 1;
		}
		lst = lst->next;
	}

	return l;
}

List filtrare_medie_aritmetica(List l, int k, int *nrPerechi)
{
	List lst, tmp;
	int n, i, ok;
	double avg;
	ok = 0; // noua lista nu are niciun element in ea
	tmp = l->next->next;

	for (i = k/2; i < *nrPerechi - k/2 ; i++)
	{
		avg = (tmp->prev->prev->value + tmp->prev->value + tmp->value +
			tmp->next->value + tmp->next->next->value) / k;
		if (ok == 0)
		{
			ok = 1; // lista contine un element
			lst = initList(tmp->timestamp, avg);
		}
		else
			lst = addLast(lst, tmp->timestamp, avg);		
		tmp = tmp->next;
	}
	*nrPerechi = *nrPerechi - k + 1;
	return lst;
}

List filtrare_mediana(List l, int k, int* nrPerechi)
{
	List lst, new_list;
	List tmp, aux; // pentru parcurgere
	int n, i, j, ok;
	ok = 0; // noua lista nu contine niciun element
	double mid;

	tmp = l;
	for (i = 0; i < *nrPerechi - k + 1; i++)
	{
		aux = tmp;
		new_list = initList(aux->timestamp, aux->value);
		aux = aux->next;
		for (j = 0 ; j < 4; j++)
		{
			new_list = addLast(new_list, aux->timestamp, aux->value);
			aux = aux->next;
		}
		new_list = sortare(new_list);
		mid = new_list->next->next->value;

		if (ok == 0)
		{
			ok = 1; // lista contine un element
			lst = initList(tmp->next->next->timestamp, mid);
		}
		else
			lst = addLast(lst, tmp->next->next->timestamp, mid);		
		tmp = tmp->next;
		new_list = deleteList(new_list);
	}

	*nrPerechi = *nrPerechi - k + 1;
	return lst;
}

List uniformizare(List l)
{
	List tmp;
	int i;

	tmp = l->next;
	while (tmp != NULL)
	{
		if (tmp->timestamp - tmp->prev->timestamp >= 100 && 
				tmp->timestamp - tmp->prev->timestamp <=1000)
		{	
			tmp->value = (tmp->prev->value + tmp->value) / 2;
			tmp->timestamp = (tmp->prev->timestamp + tmp->timestamp) / 2;
		}
		tmp = tmp->next;
	}
	return l;
}

List completare_date(List l, int *nrPerechi)
{
	// cele mai apropiate elemente de marginile intervalului se afla spre
	// finalul listelor right si left
	List tmp; // pentru parcurgerea listei l
	List list; // noua lista cu valorile completate
	int listPerechi = 0, listTimestamp, k = 3, i;
	double w0, w1, w2, sum = 0, listValue, C, sumleft, sumright;
	for (i = 0; i < 3; i++)
		sum = sum + (double)i / (k - 1)* i / (k - 1) * 0.9 + 0.1;
	w0 = (double)0.1 / sum;
	w1 = ((double) 1 / (k - 1) * 1 / (k - 1) * 0.9 + 0.1) / sum;
	w2 = ((double) 2 / (k - 1) * 2 / (k - 1) * 0.9 + 0.1) / sum;

	// initializarea listei list cu primele valori din lista l
	tmp = l;
	list = initList(tmp->timestamp, tmp->value);
	listPerechi++;
	tmp = tmp->next;
	while (tmp != NULL)
	{
		if (tmp->timestamp - tmp->prev->timestamp <= 1000)
		{
			//printf("%lf %ld\n", tmp->value, tmp->prev->value);
			list = addLast(list, tmp->timestamp, tmp->value);
			listPerechi++;
		}
		else
		{
			listTimestamp = tmp->prev->timestamp + 200;

			while (listTimestamp < tmp->timestamp)
			{
				C = (double)(listTimestamp - tmp->prev->timestamp) / (tmp->timestamp - 
					tmp->prev->timestamp);
				sumleft =  w0 * tmp->prev->prev->prev->value +
				 	w1 * tmp->prev->prev->value + w2 * tmp->prev->value;
				sumright = w0 * tmp->next->next->value + w1 * tmp->next->value + 
					w2 * tmp->value;
 				listValue = (1 - C) * sumleft + C * sumright;

				list = addLast(list, listTimestamp, listValue);
				listPerechi++;
				listTimestamp = listTimestamp + 200;
			}
			list = addLast(list, tmp->timestamp, tmp->value);
			listPerechi++;
		}
		tmp = tmp->next;
	}
	*nrPerechi = listPerechi;
	return list;
}


void bonus(List l, char text[])
{
	// extragerea intervalului de timp din argument
	char p[30];
	int i, x, k, aux;
	List tmp, end; // pentru parcurgerea listelor
	List lst, list;
	strcpy(p, text + 4);
	x = atoi(p); // intervalul de timp

	tmp = l;
	if (tmp->value > 0)
	{
		aux = tmp->value / x;
		aux = aux * x;
	}
	else
	{
		aux = tmp->value / x;
		aux = aux * x - x;
	}
	//printf("%d ", aux);
	// lista lst va retine in campul timestamp numarul de valori din intervalul
	// respectiv, iar in campul value, limita inferioara a intervalului
	lst = initList(1, (double)aux);
	tmp = tmp->next;

	while (tmp != NULL)
	{
		//printf("%d ", aux);
		if (tmp->value > 0)
		{
			aux = tmp->value / x;
			aux = aux * x;
		}
		else
		{
			aux = tmp->value / x;
			aux = aux * x - x;

		}
		// caut intervalul in care se afla tmp->value in lista lst
		list = search(lst, (double)aux);
	
		// gasesc sfarsitul listei
		end = lst;
		while(end->next != NULL)
			end = end->next;
		// cazul in care in intervalul in care se afla tmp->value
		// se gaseste deja o valoare
		if (list != NULL)
			list->timestamp = list->timestamp + 1;

		// daca intervalul in care se afla tmp->value este mai mic decat oricare
		// alt interval din lista lst, deci mai mic decat primul interval
		else 
			if (aux < lst->value)
				lst = addFirst(lst, 1, (double)(aux));

		// daca intervalul in care se afla tmp->value este mai mare decat 
		// oricare alt interval din lista lst, deci mai mare decat ultimul
			else 
				if (aux > end->value)
					lst = addLast(lst, 1, (double)(aux));
			else
					lst = addItem(lst, 1, (double)(aux));
				
		tmp = tmp->next;
	}
	while (lst != NULL)
	{
		printf("[%d, %d] %d\n", (int)lst->value, (int)lst->value + x, 
					lst->timestamp);
		lst = lst->next;
	}
}

int main(int args, char **argv)
{
	// declaratii
	int nrPerechi, i, ts, k = 5;
	double val;
	List l;
	
	// citirea datelor si crearea listei 
	scanf("%d %d %lf", &nrPerechi, &ts, &val);
	l = initList(ts, val);
	for (i = 1; i < nrPerechi; i++)
	{
		scanf("%d %lf", &ts, &val);
		l = addLast(l, ts, val);
	}

	for (i = 1; i < args; i++)
	{
		if (strcmp(argv[i], "--e1") == 0)
		{
			l = eliminare_exceptii(l, k, &nrPerechi);
			if (i == args - 1)
				printList(l, nrPerechi);
		}
		else
			if (strcmp(argv[i], "--e2") == 0)
			{
				l = filtrare_mediana(l, k, &nrPerechi);
				if (i == args - 1)
					printList(l, nrPerechi);
			}
			else
				if (strcmp(argv[i], "--e3") == 0)
				{
					l = filtrare_medie_aritmetica(l, k, &nrPerechi);
					if (i == args -1)
						printList(l, nrPerechi);
				}
			else
				if (strcmp(argv[i], "--u") == 0)
				{
					l = uniformizare(l);
					if (i == args - 1)
						printList(l, nrPerechi);
				}
				else
					if (strcmp(argv[i], "--c") == 0)
					{
						l = completare_date(l, &nrPerechi);
						if (i == args - 1)
							printList(l, nrPerechi);
					}
					else
						bonus(l, argv[i]);
	}

	//free(l);
	return 0;
}
