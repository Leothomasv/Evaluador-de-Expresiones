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

void tests(string exp, double respuesta) {
   
    string expresion;
    auto ans=0.0; //feature v11 c++

    exp = BuscarEnFile(exp);

    if (ExpCorrecta(exp)) {
        expresion = convertir(exp);
        ans = evaluar(expresion);

        if (ans == respuesta) {
            cout << exp << ":" << endl;
            printf("\x1B[32mTest Passed\033[0m");
            ans = 0;
            cout << endl;
        }else if (respuesta != ans) {
            cout << exp << ":" << endl;
            printf("\x1B[31mTest Failed \033[0m");
            cout << endl;
            ans = 0;
        }
    
    }
    
    
    if (ExpCorrecta(exp) == false) {
        cout << exp << ":" << endl;
        printf("\x1B[31mTest Failed \033[0m");
        cout << endl;
        ans = 0;
    }

}

void unit_Tests() {
    //buenas
    tests("3^2", 9.0);
    tests("100+x", 500.0);
    tests("(7)+3+e", 12.71828);
    tests("50.0+y", 350.0);
    tests("1+2+3+4+5+6+7+8+9", 45.0);
    cout << endl;

    //malas
    tests("1+10", 2.0);
    tests("10+e", 16.0);
    tests("500+x", 3.0);
    tests("2^2", 2.0);
    tests("1+", 3.0);



}

int main() {

    string s, p;
    int tam = s.size();
    int Opcion;

    do {
        cout << endl;
        cout << "## Menu Evaluador de Expresiones ##" << endl;
        cout << "==================================================" << endl;
        cout << "1. Ingresar Expresion (Manualmente)" << endl;
        cout << "2. Unit Tests" << endl;
        cout << "3. Salir" << endl;
        cout << "==================================================" << endl;
        cout << "Ingresar Opcion: ";
        cin >> Opcion;
        cout << endl;

        switch (Opcion) {
        case 1:
            do {
                cout << "\n";
                cout << "0 para salir" << endl;
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
            } while (s != "0");
            break;

        case 2:
            unit_Tests();
            break; 

        case 3:
            cout << "Saliendo del sistema :)" << endl;
            break;
        }
       
    } while (Opcion != 3);



    
}