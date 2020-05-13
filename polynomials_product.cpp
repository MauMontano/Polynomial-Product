#include <bits/stdc++.h>
using namespace std;

typedef struct Queue{
    int coefficient;
    int priority;
    struct Queue *ant;
    struct Queue *next;
}Coefi;

Coefi *front_1 = NULL;//First element 1
Coefi *front_2 = NULL;//First element 2
Coefi *front_prod = NULL;//First element product
Coefi *back_1 = NULL;//Last element 1
Coefi *back_2 = NULL;//Last element 2
Coefi *back_prod = NULL;//Last element prod

bool regex_pol(string &, char);
void data_controller(string &,int,char);
string search_coeficcient(int, string &);
string search_potence(int, string &);
void eval_potence(int, int, int);
void push_Front(Coefi *,int);
void push_Back(Coefi *,int);
void push_sw(Coefi *,int);
void display(char);
void product(void);
void sum(void);



int main(){
    bool flag = false;
    int cont = 1;
    char var;
    string polym1, polym2;
    cout << "Ingresa la variable de los polinomios(x,y,a,b): " << endl;
    cin >> var;
    while(!flag){
        cout << "Ingresa el primer polinomio con la variable elegida "<< "(" << var << ")" << ": ";
        cin >> polym1;
        flag = regex_pol(polym1,var);
    }
    data_controller(polym1,1,var);
    flag = false;//changing flag
    while(!flag){
        cout << "Ingresa el segundo polinomio con la variable elegida "<< "(" << var << ")" << ": ";
        cin >> polym2;
        flag = regex_pol(polym2,var);
    }
    data_controller(polym2,2,var);
    product();
    display(var);
    return 0;
}
bool regex_pol(string &polym, char var){
    int conta = 0;
    for(int i = 0; i<polym.size();i++){
        if(polym[i] == '^' || polym[i] == '+' || polym[i] == '-' || polym[i] == var || isdigit(polym[i]))
            conta++;
    }
    if(conta == polym.size())
        return true;
    else
        return false;

}
void data_controller(string &polym,int id_polym, char var){
    int i,j, icoef, ipot, temp;
    bool flag;
    string coef, pot;
    if(polym[0] != '-')
        polym.insert(0,"+");
    for(i = 0;i<polym.size();i++){
        flag = true;
        coef.clear();
        if(polym[i] == var){
            if(polym[i+1] == '^'){
                pot = search_potence(i+2,polym);
                istringstream(pot) >> ipot;
                if(isdigit(polym[i-1])){
                    //j = i-1;
                    coef = search_coeficcient(i-1, polym);
                    //cout << "Coef: " << coef << endl;
                    istringstream(coef) >> icoef;
                    //cout << "ICoef: " << icoef << endl;
                    eval_potence(icoef,ipot,id_polym);
                    //coef.clear();
                }
                else{
                    if(polym[i-1] == '-'){
                        eval_potence(-1,ipot,id_polym);
                    }
                    else{
                        eval_potence(1,ipot,id_polym);
                    }
                }
            }
            else{
                if(isdigit(polym[i-1])){
                    j = i-1;
                    coef = search_coeficcient(j, polym);
                    istringstream(coef) >> icoef;
                    eval_potence(icoef,1,id_polym);
                }
                else{
                    if(polym[i-1] == '-'){
                        eval_potence(-1,1,id_polym);
                    }
                    else{
                        eval_potence(1,1,id_polym);
                    }
                }
            }
        }
        else if(polym[i] == '+' || polym[i] == '-'){
            //cout << "Signo encontrado: " << polym[i] << endl;
            if(isdigit(polym[i+1])){//validating -5555
                j = i+1;
                coef.push_back(polym[i]);
                while(isdigit(polym[j])){
                    coef.push_back(polym[j]);
                    j++;
                    if(polym[j] == '^' || polym[j] == var){
                        //cout << "Encontrado(^, var)" << endl;
                        flag = false;
                    }
                }
                if(flag){
                    //cout << "Coef: " << coef << endl;
                    istringstream(coef) >> icoef;
                    //cout << "ICoef: " << coef << endl;
                    eval_potence(icoef,0,id_polym);
                }
                //flag = true;
            }
        }
    }
}
string search_coeficcient(int j, string &polym){
    string coef = "";
    int temp = j;
    int i;
    while(j>=0 && isdigit(polym[j])){//validating enter all the coefficient
        j--;
    }
    if(polym[j] != '-'){
        coef.insert(0,"+");
        j++;
    }
    for(i = j; i<= temp; i++){
        coef.push_back(polym[i]);
    }
    return coef;
}
string search_potence(int j, string &polym){//finding the potence
    string pot = "";
    while(j<polym.size() && isdigit(polym[j])){//validating enter all the coefficient
        pot.push_back(polym[j]);
        j++;
    }
    return pot;
}

void eval_potence(int coef, int priority, int id_polym){
    Coefi *new_element;
    new_element = (Coefi*)malloc(sizeof(Coefi));//Each time that we call the function a new struct is created and added to the stack;
    new_element->coefficient = coef;//I think stoi is for parse string to int
    new_element->priority = priority;
    int i = 0,caso;
    if(id_polym == 1){
        if(front_1 == NULL && back_1 == NULL){//If the queue is void
                front_1 = new_element;
                back_1 = new_element;
                new_element->ant = NULL;
                new_element->next = NULL;
        }
        else{//If the queue has at least one element
            if(front_1 == back_1){// Case 1: we have just one element
                /*Here they can happen two things, Suppose the first coeficcient is 2:
                    1.- Coeficcient will be pushed at the begin
                        3 2
                    2.- Word will be pushed at the end
                        2 1
                */
                if(new_element->priority > front_1->priority){//word will be pushed at the begin
                    push_Front(new_element,1);
                }
                else{//word will be pushed at the end
                    push_Back(new_element,1);
                }
            }
            else {//Case 2: We have at least two elements
                /*Here they can happen three things, Suppose we have 3 and 1
                    1.- Coeficcient will be pushed at the begin
                        4 3 1
                    2.- Coeficcient will be pushed at the end
                        3 1 0
                    3.- Coeficcient will be pushed between the elements
                        3 2 1
                    If I get 3 make all the things here
                    */
                if(new_element->priority > front_1->priority){//word coeficcient be pushed at the begin
                    push_Front(new_element,1);
                }
                else{//Word is higher than the firstone
                    if(new_element->priority < back_1->priority){//coefficient will be pushed at the end
                        push_Back(new_element,1);
                    }
                    else{
                        push_sw(new_element,1);
                    }
                }
            }
        }
    }
    else if(id_polym == 2){//id_polym == 2
        if(front_2 == NULL && back_2 == NULL){//If the queue is void
                front_2 = new_element;
                back_2 = new_element;
                new_element->ant = NULL;
                new_element->next = NULL;
        }
        else{//If the queue has at least one element
            if(front_2 == back_2){// Case 1: we have just one element
                /*Here they can happen two things, Suppose the first coeficcient is 2:
                    1.- Coeficcient will be pushed at the begin
                        3 2
                    2.- Word will be pushed at the end
                        2 1
                */
                if(new_element->priority > front_2->priority){//word will be pushed at the begin
                    push_Front(new_element,2);
                }
                else{//word will be pushed at the end
                    push_Back(new_element,2);
                }
            }
            else {//Case 2: We have at least two elements
                /*Here they can happen three things, Suppose we have 3 and 1
                    1.- Coeficcient will be pushed at the begin
                        4 3 1
                    2.- Coeficcient will be pushed at the end
                        3 1 0
                    3.- Coeficcient will be pushed between the elements
                        3 2 1
                    If I get 3 make all the things here
                    */
                if(new_element->priority > front_2->priority){//coeficcient be pushed at the begin
                    push_Front(new_element,2);
                }
                else{//Word is higher than the firstone
                    if(new_element->priority < back_2->priority){//coefficient will be pushed at the end
                        push_Back(new_element,2);
                    }
                    else{
                        push_sw(new_element,2);
                    }
                }
            }
        }
    }
    else{//product
        if(front_prod == NULL && back_prod == NULL){//If the queue is void
                front_prod = new_element;
                back_prod = new_element;
                new_element->ant = NULL;
                new_element->next = NULL;
        }
        else{//If the queue has at least one element
            if(front_prod == back_prod){// Case 1: we have just one element
                /*Here they can happen two things, Suppose the first coeficcient is 2:
                    1.- Coeficcient will be pushed at the begin
                        3 2
                    2.- Word will be pushed at the end
                        2 1
                */
                if(new_element->priority > front_prod->priority){//word will be pushed at the begin
                    push_Front(new_element,3);
                }
                else{//word will be pushed at the end
                    push_Back(new_element,3);
                }
            }
            else {//Case 2: We have at least two elements
                /*Here they can happen three things, Suppose we have 3 and 1
                    1.- Coeficcient will be pushed at the begin
                        4 3 1
                    2.- Coeficcient will be pushed at the end
                        3 1 0
                    3.- Coeficcient will be pushed between the elements
                        3 2 1
                    If I get 3 make all the things here
                    */
                if(new_element->priority > front_prod->priority){//coeficcient be pushed at the begin
                    push_Front(new_element,3);
                }
                else{//Word is higher than the firstone
                    if(new_element->priority < back_prod->priority){//coefficient will be pushed at the end
                        push_Back(new_element,3);
                    }
                    else{
                        push_sw(new_element,3);
                    }
                }
            }
        }

    }
}
void push_Front(Coefi *new_element,int id_polym){
    Coefi *temp;
    if(id_polym == 1){
        temp = front_1;
        front_1 = new_element;
        front_1->ant = NULL;
        front_1->next = temp;
        temp->ant = front_1;
    }
    else if(id_polym == 2){
        temp = front_2;
        front_2 = new_element;
        front_2->ant = NULL;
        front_2->next = temp;
        temp->ant = front_2;
    }
    else{//prod polym
        temp = front_prod;
        front_prod = new_element;
        front_prod->ant = NULL;
        front_prod->next = temp;
        temp->ant = front_prod;
    }
}
void push_Back(Coefi *new_element, int id_polym){
    Coefi *temp;
    if(id_polym == 1){
        temp = back_1;
        back_1 = new_element;
        temp->next = back_1;
        back_1->ant = temp;
        back_1->next = NULL;
    }
    else if(id_polym == 2){//id_polym == 2
        temp = back_2;
        back_2 = new_element;
        temp->next = back_2;
        back_2->ant = temp;
        back_2->next = NULL;
    }
    else{//prod pol
        temp = back_prod;
        back_prod = new_element;
        temp->next = back_prod;
        back_prod->ant = temp;
        back_prod->next = NULL;
    }
}
void push_sw(Coefi *new_element,int id_polym){
    int flag = 0,i;
    Coefi *buscador;
    if(id_polym == 1)
        buscador = front_1;
    else if(id_polym == 2)
        buscador = front_2;
    else//prod pol
        buscador = front_prod;
    while(flag == 0 && buscador != NULL){//while space don´t be found and buscador don´t get the end
        if(buscador->priority >= new_element->priority){
            if(buscador->next->priority <= new_element->priority)
                flag = 1;
            else
                buscador = buscador->next;
        }
    }
    //When while ends we have the space where we have to insert the word given
    new_element->ant = buscador;
    new_element->next = buscador->next;
    buscador->next = new_element;
    new_element->next->ant = new_element;
}
void product(){
    Coefi *iter1 = front_1;
    Coefi *iter2 = front_2;
    while(iter1 != NULL){
        iter2 = front_2;
        while(iter2 != NULL){
            eval_potence(((iter1->coefficient)*(iter2->coefficient)),((iter1->priority)+(iter2->priority)),3);
            iter2 = iter2->next;
        }
        iter1 = iter1->next;
    }
    sum();
}
void sum(){
    int suma;
    Coefi *new_element;
    Coefi *pivote = front_prod;
    Coefi *temp = pivote->next;
    suma = 0;
    while(pivote != NULL && temp->next != NULL){
        if(pivote->priority == temp->priority){
            new_element = (Coefi*)malloc(sizeof(Coefi));
            while(pivote->priority == temp->priority){
                suma = suma+((temp->coefficient)+(pivote->coefficient));
                temp = temp->next;
            }
            new_element->coefficient = suma;
            new_element->priority = pivote->priority;
            pivote->ant->next = new_element;
            new_element->ant = pivote->ant;
            new_element->next = temp;
            temp->ant = new_element;
        }
        else{
            temp = temp->next;
            pivote = pivote->next;
        }
    }

}
void display(char var){
    Coefi *iter1 = front_1;
    Coefi *iter2 = front_2;
    Coefi *iter_prod = front_prod;
    cout << endl << "Polinomio 1: " << endl << endl;
    while(iter1 != NULL){
        if(iter1->priority > 1){
            if(iter1->coefficient > 1){
                cout << "+" << iter1->coefficient;
                cout << var << "^" << iter1->priority;
            }
            else if(iter1->coefficient == 1){
                cout << "+" << var << "^" << iter1->priority;
            }
            else{
                cout << iter1->coefficient << var << "^" << iter1->priority;
            }
        }
        else{
            if(iter1->priority == 1){
                if(iter1->coefficient > 1){
                    cout << "+" << iter1->coefficient << var;
                }
                else if(iter1->coefficient == 1){
                    cout << "+" << var;
                }
                else{//coeficcient <=0
                    cout << iter1->coefficient << var;
                }
            }
            else{//priority == 0
                if(iter1->coefficient > 1){
                    cout << "+" << iter1->coefficient;
                }
                else{
                    cout << iter1->coefficient;
                }
            }
        }
        iter1 = iter1->next;
    }
    cout << endl << endl << "Polinomio 2: " << endl << endl;
    while(iter2 != NULL){
        if(iter2->priority > 1){
            if(iter2->coefficient > 1){
                cout << "+" << iter2->coefficient;
                cout << var << "^" << iter2->priority;
            }
            else if(iter2->coefficient == 1){
                cout << "+" << var << "^" << iter2->priority;
            }
            else{
                cout << iter2->coefficient << var << "^" << iter2->priority;
            }
        }
        else{
            if(iter2->priority == 1){
                if(iter2->coefficient > 1){
                    cout << "+" << iter2->coefficient << var;
                }
                else if(iter2->coefficient == 1){
                    cout << "+" << var;
                }
                else{//coeficcient <=0
                    cout << iter2->coefficient << var;
                }
            }
            else{//priority == 0
                if(iter2->coefficient > 1){
                    cout << "+" << iter2->coefficient;
                }
                else{
                    cout << iter2->coefficient;
                }
            }
        }
        iter2 = iter2->next;
    }
    cout << endl << endl << "Producto: " << endl << endl;
    while(iter_prod != NULL){
        if(iter_prod->priority > 1){
            if(iter_prod->coefficient > 1){
                cout << "+" << iter_prod->coefficient;
                cout << var << "^" << iter_prod->priority;
            }
            else if(iter_prod->coefficient == 1){
                cout << "+" << var << "^" << iter_prod->priority;
            }
            else{
                cout << iter_prod->coefficient << var << "^" << iter_prod->priority;
            }
        }
        else{
            if(iter_prod->priority == 1){
                if(iter_prod->coefficient > 1){
                    cout << "+" << iter_prod->coefficient << var;
                }
                else if(iter_prod->coefficient == 1){
                    cout << "+" << var;
                }
                else{//coeficcient <=0
                    cout << iter_prod->coefficient << var;
                }
            }
            else{//priority == 0
                if(iter_prod->coefficient > 1){
                    cout << "+" << iter_prod->coefficient;
                }
                else{
                    cout << iter_prod->coefficient;
                }
            }
        }
        iter_prod = iter_prod->next;
    }
    cout << endl << endl;
}
