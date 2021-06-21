#include <iostream>
#include <cstdlib>
#include <cmath>
#include <stack>
#include <string>
#include <fstream>

using namespace std;



int prioridad(char op) {
    switch (op) {
    case '^': return 3;
    case '*':
    case '/': return 2;
    case '+':
    case '-': return 1;
    case ')': return -1;
    default: return 0;
    }
}

string convertir(string in) {
    stack<char> pila;
    string posf = "";
    for (int i = 0; i < in.size(); i++) {
        switch (in[i]) {
        case '(':
            pila.push('(');
            break;
        case ')':
            while (!pila.empty() && pila.top() != '(') {
                posf += string(1, pila.top()) + " ";
                pila.pop();
            }
            pila.pop();
            break;
        case '+':
        case '-':
        case '*':
        case '/':
        case '^':
            while (!pila.empty() && prioridad(in[i]) <= prioridad(pila.top())) {
                posf += string(1, pila.top()) + " ";
                pila.pop();
            }
            pila.push(in[i]);
            break;
        default:
            while (isdigit(in[i]) || in[i] == '.')
                posf += string(1, in[i++]);
            posf += " ";
            i--;
        }
    }
    while (!pila.empty()) {
        posf += string(1, pila.top()) + " ";
        pila.pop();
    }
    return posf;
}

double evaluar(string p) {
    stack<double> pila;
    double op1, op2;
    for (int i = 0; i < p.size(); i += 2) {
        switch (p[i]) {
        case '^':
            op2 = pila.top(); pila.pop();
            op1 = pila.top(); pila.pop();
            pila.push(pow(op1, op2));
            break;
        case '*':
            op2 = pila.top(); pila.pop();
            op1 = pila.top(); pila.pop();
            pila.push(op1 * op2);
            break;
        case '/':
            op2 = pila.top(); pila.pop();
            op1 = pila.top(); pila.pop();
            pila.push(op1 / op2);
            break;
        case '+':
             op2 = pila.top(); pila.pop();
            op1 = pila.top(); pila.pop();
            pila.push(op1 + op2);
            break;
        case '-':
            op2 = pila.top(); pila.pop();
            op1 = pila.top(); pila.pop();
            pila.push(op1 - op2);
            break;
        default:
            string aux = "";
            while (p[i] != ' ')
                aux += string(1, p[i++]);
            pila.push(atof(aux.c_str()));
            i--;
        }
    }
    return pila.top();
}


bool ExpCorrecta(string str) {
    //voy a agarrar el string y si la ultima posicion del string es alguno de los operadores retorno false, y si no retorno true
    int tam = str.size();
    char ultimo = str[tam-1];

    //si solo se ingresa un dato
    if (tam == 1) {
        return false;
    }


    for (int i = 0; i < tam; i++)
    {
        switch (ultimo) {
            case '^': return false;
                break;
            case '*': return false;
                break;
            case '/': return false;
                break;
            case '+': return false;
                break;
            case '-': return false;
                break;
            case '%': return false;
                break;
            default: return true;
                break;
        }
    }
}

string ValidarVariables(string str) {
    char valor;
    for (int i = 0; i < str.size(); i++)
    {
        switch (str[i]) {
        case 'a':
            cout << "Ingrese valor de variable 'a': ";
            cin >> valor;

            str[i] = valor;
            break;

        case 'b':
            cout << "Ingrese valor de variable 'b': ";
            cin >> valor;

            str[i] = valor;
            break;

        case 'c':
            cout << "Ingrese valor de variable 'c': ";
            cin >> valor;

            str[i] = valor;
            break;
            
        default:
            break;
        }
    }

    //cout << "Expresion nueva: " << str;
    return str;
}

string BuscarEnFile(string exp) {

    //crear arreglos de variable y valor con los datos que tiene el archivo de txt
    ifstream file("variables.txt");
    string str;

    string arrVariable[20];
    float arrValor[20];

    string delimiter = "=";
    int i = 0;

    while (getline(file, str)) {
        size_t pos = 0;
        string token;
        while ((pos = str.find(delimiter)) != string::npos) {
            token = str.substr(0, pos);
            str.erase(0, pos + delimiter.length());

            //Agrega Variables
            arrVariable[i] = token;
            //Agrega Valores
            arrValor[i] = stof(str);

            i++;
        }
    }


    //buscar la variable en el arrVariable y asiganrle el valor a la posicion
    int x = 0;
    int arrSize = sizeof(arrVariable) / sizeof(arrVariable[0]);
    int cambio = 0;

    string valor;
    do
    {
        while (x <= exp.size()) {

            string cmp(1, exp[x]);

            if (cmp == arrVariable[cambio]) {

                valor = to_string(arrValor[cambio]);
                exp.replace(x, valor.size(), valor);
            }
            x++;
        }
        cambio++;
        x = 0;
    } while (cambio != arrSize);


    return exp;
}

int main() {

    string s, p;
    int tam = s.size();

    while (true) {
        cout << "\n";
        cout << "Ingrese expresion: ";
        cin >> s;
        

        //Resivar variables (constantes) en el archivo si estan reemplazarlos
        s = BuscarEnFile(s);


        //Darle valor a las variables que no estan en el archivo de constantes
        s = ValidarVariables(s);
       

        if (ExpCorrecta(s)) {
            p = convertir(s);
            cout << "Posfija: " << p << endl;
            cout << "Evaluada: " << evaluar(p) << endl;
        }
        else {
            cout << "Expresion ingresada incorrectamente :(" << endl;
            cout << "\n";
        }
    }

    
}