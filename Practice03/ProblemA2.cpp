
#include<stdio.h>
#include<malloc.h>

typedef struct _invoice {
	int from;
	int to;
	int parcel;
	struct _invoice *prev;
	struct _invoice *next;
}invoice;

typedef struct {
	invoice head;
	invoice tail;
	int cnt;
}list;


int N, C, M;
list *L;
invoice *index;
list compartment;
int comp_arr[100001] = {0};

void init() {
	for (int i = 1; i <= N; i++) {
		L[i].head.next = &(L[i].tail);
		L[i].tail.prev = &(L[i].head);
	}
}

void insert(int from, int to, int parcel) {
	invoice *p = &(L[from].head);
	invoice *get = (invoice *)malloc(sizeof(invoice));
	get->from = from;
	get->to = to;
	get->parcel = parcel;
	L[from].cnt++;
	while (p->next != &(L[from].tail)) {
		if (to < p->next->to) {
			break;
		}
		p = p->next;
	}
	get->next = p->next;
	get->prev = p;
	p->next->prev = get;
	p->next = get;
}

void insert_q(list *q, int from, int to, int parcel) {
	invoice *p = (*q).tail.prev;
	invoice *get = (invoice *)malloc(sizeof(invoice));
	get->from = from;
	get->to = to;
	get->parcel = parcel;

	get->next = p->next;
	get->prev = p;
	p->next->prev = get;
	p->next = get;
}
void insert_insertion(list *q, int from, int to, int parcel) {
	invoice *p = &(q->tail);
	
	while (p->prev != &(q->head)) {
		if (to > p->prev->to) {
			break;
		}
		p = p->prev;
	}
	invoice *get = (invoice *)malloc(sizeof(invoice));
	get->from = from;
	get->to = to;
	get->parcel = parcel;

	get->prev = p->prev;
	get->next = p;
	p->prev->next = get;
	p->prev = get;
	
}
void delete_all() {
	invoice *p;
	for (int i = 1; i <= N; i++) {
		while (L[i].head.next != &(L[i].tail)) {
			p = L[i].head.next;
			L[i].head.next->next->prev = &(L[i].head);
			L[i].head.next = L[i].head.next->next;
			free(p);
		}
	}
	free(L);
}
void prn_L(int i) {
	invoice *p = L[i].head.next;
	printf("L[%d] print\n", i);
	while (p != &(L[i].tail)) {
		printf("from : %d  to : %d  parcel : %d\n", p->from, p->to, p->parcel);
		p = p->next;
	}
	printf("-----------------------------------------\n");
}

void prn_compartment() {
	invoice *p = compartment.head.next;
	printf("compartment print  cnt : %d\n", compartment.cnt);
	while (p != &(compartment.tail)) {
		printf("from : %d  to : %d  parcel : %d\n", p->from, p->to, p->parcel);
		p = p->next;
	}
	printf("-----------------------------------------\n");
}

int calc() {
	int town_index = 1;
	int res = 0;

	compartment.head.next = &(compartment.tail);
	compartment.tail.prev = &(compartment.head);
	compartment.cnt = 0;
	invoice *ep;
	int last_index;
	for (town_index = 1; town_index <= N; town_index++) {
		index = L[town_index].head.next;
		while (compartment.head.next->to <= town_index && compartment.head.next != &(compartment.tail)) { // 적재 물품중 내릴 수 있는건 다 내림
			ep = compartment.head.next;
			res += comp_arr[town_index];
			compartment.cnt -= comp_arr[town_index];
			compartment.head.next->next->prev = &(compartment.head);
			compartment.head.next = compartment.head.next->next;
			free(ep);
		}


		while (index != &(L[town_index].tail)) {
			compartment.cnt += index->parcel;
			if (comp_arr[index->to] == 0) {
				insert_insertion(&compartment, index->from, index->to, 0);
				comp_arr[index->to] += index->parcel;
			}
			else
				comp_arr[index->to] += index->parcel;
			index = index->next;
		}
		while (compartment.cnt > C) {
			if (compartment.cnt - comp_arr[compartment.tail.prev->to] > C) {
				ep = compartment.tail.prev;
				compartment.cnt -= comp_arr[compartment.tail.prev->to];
				comp_arr[compartment.tail.prev->to] = 0;
				ep->prev->next = &(compartment.tail);
				compartment.tail.prev = ep->prev;
				free(ep);
			}
			else {
				int temp;
				temp = C - (compartment.cnt - comp_arr[compartment.tail.prev->to]);
				comp_arr[compartment.tail.prev->to] = temp;
				compartment.cnt = C;
			}
		}


	}
	return res;

}



int main() {
	freopen("input.txt","r",stdin);
	scanf("%d %d", &N, &C);
	scanf("%d", &M);
	L = (list*)calloc(N + 1, sizeof(list));
	init();
	int from, to, parcel;
	for (int i = 0; i < M; i++) {
		scanf("%d %d %d", &from, &to, &parcel);
		//  printf("%d %d %d\n", from, to, parcel);
		insert(from, to, parcel);
		//  printf("insert\n");
	}
	printf("%d\n", calc());

	delete_all();

	return 0;
}
