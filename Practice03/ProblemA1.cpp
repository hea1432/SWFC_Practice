
#include<stdio.h>
#include<malloc.h>

typedef struct _node{
	int diff;
	int index;
}node;

int N, coupon_cnt;
long long M;
int original[50000][2];
int normal_arr[50000][2];
int discount_arr[50000][2];
node heap[50001];
int heap_last;
char V[50000];

void quickSort(int first, int last, int arr[50000][2])
{
	int pivot;
	int i;
	int j;
	int temp1, temp2;

	if (first < last)
	{
		pivot = first;
		i = first;
		j = last;

		while (i < j)
		{
			while (arr[i][0] <= arr[pivot][0] && i < last)
			{
				i++;
			}
			while (arr[j][0] > arr[pivot][0])
			{
				j--;
			}
			if (i < j)
			{
				temp1 = arr[i][0];
				temp2 = arr[i][1];
				arr[i][0] = arr[j][0];
				arr[i][1] = arr[j][1];
				arr[j][0] = temp1;
				arr[j][1] = temp2;

			}
		}

		temp1 = arr[pivot][0];
		temp2 = arr[pivot][1];
		arr[pivot][0] = arr[j][0];
		arr[pivot][1] = arr[j][1];
		arr[j][0] = temp1;
		arr[j][1] = temp2;

		quickSort(first, j - 1, arr);
		quickSort(j + 1, last, arr);
	}
}
void swap(node *a, node *b) {
	node temp = *a;
	*a = *b;
	*b = temp;
}

void insert_heap(int index) {
	node temp;
	temp.diff = original[index][0] - original[index][1];
	temp.index = index;
	heap[++heap_last] = temp;
	index = heap_last;
	while (index != 1) {
		if (heap[index / 2].diff < heap[index].diff) {
			swap(&heap[index / 2], &heap[index]);
			index /= 2;
		}
		else {
			break;
		}
	}
}
int top_heap(){
	return heap[1].diff;
}

node pop_heap() {
	node ret = heap[1];
	if (heap_last == 0) {
		printf("warning heap empty\n");
		return ret;
	}

	heap[1] = heap[heap_last--];
	int index = 1;
	while (index*2 + 1 <= heap_last) {
		if (heap[index].diff < heap[index * 2 + 1].diff || heap[index].diff < heap[index * 2].diff) {
			if (heap[index * 2].diff < heap[index * 2 + 1].diff) {
				swap(&heap[index], &heap[index * 2 + 1]);
				index = index * 2 + 1 ;
			}
			else {
				swap(&heap[index],&heap[index*2]);
				index = index * 2;
			}
		}
		else {
			break;
		}
	}
	return ret;


}


int add_up_until_full() {
	long long sum = 0;
	int i;
	int j = 0;
	int comp_val;
	int res=0;
	node node_temp;
	for (i = 0; sum <= M; i++) {
		sum += normal_arr[i][0];
		insert_heap(normal_arr[i][1]);
		V[normal_arr[i][1]] = 1;
	}
	res = i;
	i--;

	while (coupon_cnt > 0) {
		while (V[discount_arr[j][1]] != 0 && j < N) { // 쿠폰을 적용하기 전, 이미 목록안에 있는 값이라면, 패스
			j++;
		}
		while (V[normal_arr[i][1]] == -1 && i > 0) { // 쿠폰이 이미 먹혀 있다면 패스 
			i--;
		}
		while (top_heap() == 0 && heap_last) {
			pop_heap();
		}
		comp_val = normal_arr[i][0] - discount_arr[j][0];
		if (comp_val < 0) {
			break;
		}
		//printf("i: %d  j: %d res : %d\n", i, j, res);
		//printf("comp_val : %d, top_heap : %d\n", comp_val, top_heap());

		if (heap_last && j < N) {
			if (comp_val > top_heap()) { // 쿠폰을 적용하기 전의 값 중 가장 큰 값 대신, 쿠폰 값 중 가장 싼 값을 넣었을때가 
				V[normal_arr[i][1]] = 0; // 현재 있는 값 중에서 가장 크게 할인되는 값에다가 쿠폰을 먹이는 것 보다 이득이라면
				V[discount_arr[j][1]] = -1; // 쿠폰 값 중 가장 싼 값을 가장 큰 값에 대체
				i--;
				j++;
				coupon_cnt--;
				sum -= comp_val;
				//printf("coupon replace original %d replaced : %d \n", normal_arr[i + 1][0], discount_arr[j - 1][0]);
			}
			else {
				node_temp = pop_heap();
				V[node_temp.index] = -1;
				sum -= node_temp.diff;
				coupon_cnt--;
				//printf("coupon existing original %d replaced : %d \n", original[node_temp.index][0], original[node_temp.index][1]);
			}
		}
		else if (heap_last && j >=N) { // 이미 가능한 모든 상품을 넣은 상황이라면
			node_temp = pop_heap(); // 이미 리스트에 있는 품목중 가장 차이가 큰 상품에 쿠폰 적용
			V[node_temp.index] = -1;
			sum -= node_temp.diff;
			coupon_cnt--;
			//printf("coupon existing original %d replaced : %d \n", original[node_temp.index][0], original[node_temp.index][1]);
		}
		else if(heap_last == 0 && j<N){ // 리스트에 있는 모든 상품에 쿠폰이 적용됐을때
			if (comp_val > 0) {
				V[normal_arr[i][1]] = 0;
				V[discount_arr[j][1]] = -1;
				i--;
				j++;
				sum -= comp_val;
				//printf("heap empty coupon existing original %d replaced : %d \n", original[node_temp.index][0], original[node_temp.index][1]);
			}
			else {
				break;
			}
		}
		else {
			//printf("exception call at i : %d j : %d heap_last : %d res : %d\n",i,j, heap_last,res);
			break;
		}
		
		if (sum + normal_arr[i+1][0] <= M && i+1 < N) {
			res++;
			sum += normal_arr[++i][0];
			V[normal_arr[i][1]] = 1;
			insert_heap(normal_arr[i][1]);
			//printf("add val : %d\n", normal_arr[last][0]);
		}

		//printf("sum : %d\n", sum);
	}
	
	if (sum > M) {
		res--;
	
	}

	return res;
}


int main() {
	freopen("input.txt","r",stdin);
	scanf("%d %d %lld", &N, &coupon_cnt, &M);
	for (int i = 0; i < N; i++) {
		scanf("%d %d", &normal_arr[i][0], &discount_arr[i][0]);
		normal_arr[i][1] = i;
		discount_arr[i][1] = i;
		original[i][0] = normal_arr[i][0];
		original[i][1] = discount_arr[i][0];
	}
	quickSort(0, N-1 , normal_arr);
	quickSort(0, N-1, discount_arr);
	//for (int i = 0; i < N; i++)
		//printf("%d %d   %d  %d\n", normal_arr[i][0], discount_arr[i][0], normal_arr[i][1], discount_arr[i][1]);
	printf("%d\n", add_up_until_full());
}
