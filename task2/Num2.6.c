#include <stdio.h>
#include <stdlib.h>
#define N 20
typedef struct Node *link; 
typedef int elemtype;
typedef struct Node { 
	elemtype elem;
	struct Node *left; 
	struct Node *right;
}node;  //описываем структкуру дерева
typedef link tree;  //описываем указатель на корень дерева

void print(tree t){
	if(t->left) print(t->left); 
	printf("%d\n",t->elem);
	if(t->right) print(t->right); 
} //печатает дерево (функция для отладки)

tree minEl(tree t) {
    if (t->left == NULL) return t;
    return minEl(t->left);
}	//выдает ссылку на наименьший элемент введенного поддерева


tree delEl(elemtype el, tree t) {
    tree tr;
    if (t != NULL) {
        if (el < t->elem)
            t->left = delEl(el, t->left);
        else if (el > t->elem)
            t->right = delEl(el, t->right);
        else if ((t->left != NULL) && (t->right != NULL)) {
            t->elem = minEl(t->right)->elem;
            t->right = delEl(t->elem, t->right);
        } //если у узла есть два поддерева, то меняем значение элемента в узле на наименьшее из правого поддерева и удаляем 
        else if (t->left != NULL) { //только одно поддерево
            tr= t;
            t = t->left;
            free(tr);
        }
        else if (t->right != NULL) { //только одно поддерево
            tr = t;
            t = t->right;
            free(tr);
        }
        else if ((t->left == NULL) && (t->right == NULL)){	//у элемента нет поддеревьев
            if (t->elem == el) {
                t = NULL;
                free(t);
            }
        }
        
    }
    return t;
}


tree makenode (elemtype el, tree t){
	if (t==NULL){
		t = malloc(sizeof(node));
		t->left=t->right=NULL;
		t->elem = el;
	}
	if (t->elem > el){
		t->left = makenode(el, t->left);
	}
	else if (t->elem < el){
		t->right = makenode(el, t->right);
	}
	return t;
} //создает новое звено с заданным элементом

int find(elemtype el, tree t){
	if (t==NULL) return 0;
	if (el==t->elem) {
		return 1;
	}
	else return find(el,t->left)||find(el,t->right);
}  //ищет в дереве заданное число, выдает 0/1 

void delete(tree p){
	if (p!=NULL){
		delete(p->left);
		delete(p->right);
		free(p);
	}
} //удаляет дерево полностью (используем перед завершением работы)

int strtoint(char str[N]){
	int i=1;
	int sum=0;
	while (str[i] != '\0'){
		sum= sum*10+(str[i]-'0');
		i++;
	}				
	return sum;
} //переводит введенную строку в число

int main(){
	elemtype sum;
	int sign;
	char str[N];
	printf("%s\n","Program started ");
	tree t= NULL;
	while (scanf("%s", str)!=EOF){
		sum = strtoint(str);
		if (str[0]=='+'){
			t=makenode(sum, t);
		}
		else if (str[0]=='-'){
			if (find(sum, t)!=1){
				printf("%s\n","Такого элемента в дереве нет");
			} 
			t=delEl(sum, t);
		}
		else if (str[0]=='?'){
			sign = find(sum, t);
			if (sign==1){
				printf("%d\n YES", sum);
			}
			else {
				printf("%d\n NO", sum);
			}
		}
	}
	delete(t);
	return 0;
}