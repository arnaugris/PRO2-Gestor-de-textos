#include "Gestor_de_textos.hh"

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

Gestor_de_textos::Gestor_de_textos() {
	Gestor_de_textos::autors = Conjunt_autors();
	Gestor_de_textos::cites = Conjunt_cites();
	consultes = Consultes();
}
void Gestor_de_textos::afegir(string consulta) {
	istringstream iss(consulta);
	string paraula;
	iss >> paraula;
	if (paraula == "text") afegir_text(consulta.substr(paraula.size()+1));
	else if (paraula == "cita") afegir_cita(consulta.substr(paraula.size()+1));
}

void Gestor_de_textos::afegir_text(string consulta) {
	istringstream iss(consulta);
	string paraula, titol_llegit, autor_llegit, frase;
	//cout << "M'entra: " << consulta << endl;
	vector<string> frases(0);
	bool titol_acabat = false, autor_acabat = false;
	iss >> paraula;
	Text text = Text();
	while(paraula != "****") {
		//cout << "Loop" << endl;
		if (!titol_acabat) {
			titol_llegit += " " + paraula;
			if (paraula[paraula.size()-1] == '\"') {
				titol_acabat = true;
				titol_llegit = titol_llegit.substr(2, titol_llegit.size()-3);
				text.modificar_titol(titol_llegit);
				iss >> paraula; //llegim la paraula "autor".
			}
		} else if (!autor_acabat) {
			autor_llegit += " " + paraula;
			if (paraula[paraula.size()-1] == '\"') {
				autor_acabat = true;
				autor_llegit = autor_llegit.substr(2, autor_llegit.size()-3);
			}
		} else {
			frase += " " + paraula;
			if (paraula[paraula.size()-1] == '.' || paraula[paraula.size()-1] == '?' || paraula[paraula.size()-1] == '!') { //aquesta linia tampoc compila, he intentat mirar perq pero no hi ha manera
				frases.push_back(frase);
				frase = "";
			}
		}
		iss >> paraula;
	}
	text.afegir_contingut(frases);
	/*cout << "Text llegit!" << endl;
	cout << "Titol: " << text.consultar_titol() << endl;
	cout << "Autor: " << autor_llegit << endl;
	cout << "Contingut(" << frases.size() << "): "  << endl;
	for (int i = 0 ; i < frases.size()-1 ; i++) {
		cout << i << ": " << frases[i] << endl;
	}*/
	//cout << "intentem afegir" << endl;
	autors.afegir_text_a_autor(text, autor_llegit);
	//cout << "text afegit" << endl;
}

void Gestor_de_textos::afegir_cita(string consulta) {
	istringstream iss(consulta);
	int x, y;
	iss >> x >> y;
	if(!autors.hi_ha_text_seleccionat() or !cites.afegir_cita(x, y))
		cout << "error" << endl;
	else {
		consulta = "afegint cita";
		cout << consulta << endl;
	}
}

void Gestor_de_textos::eliminar(string consulta) {
	istringstream iss(consulta);
	string paraula;
	iss >> paraula;
	if (paraula == "text") afegir_text(consulta.substr(paraula.size()));
	else if (paraula == "cita") afegir_cita(consulta.substr(paraula.size()));
}

void Gestor_de_textos::eliminar_text(string consulta) {
	istringstream iss(consulta);
	string titol;
	iss >> titol;
	string nom_autor = autors.existeix_titol(titol);
	if (nom_autor != "NULL") {
		if(!autors.eliminar_text_de_autor(titol, nom_autor)) cout << "error" << endl;
		else consulta = "eliminant text";
	}
	else cout << "error" << endl;
	cout << consulta << endl;
}

void Gestor_de_textos::eliminar_cita(string consulta) {
	istringstream iss(consulta);
	string referencia;
	iss >> referencia;
	referencia = referencia.substr(1, referencia.size()-2);
	if (!cites.eliminar_cita(referencia)) cout << "error" << endl;
	else {
		consulta = "eliminant cita";
		cout << consulta << endl;
	}
}
void Gestor_de_textos::triar_text(string consulta) {
	istringstream iss(consulta);
	string paraula;
	iss >> paraula;
	vector<string> paraules(0);
	while(paraula[paraula.size()-1] != '}') {
		cout << "paraula: " << paraula << endl;
			if (paraula[0] == '{') paraula = paraula.substr(1, paraula.size()-1);
			paraules.push_back(paraula);
			iss >> paraula;
	}
	paraula = paraula.substr(1, paraula.size()-2);
	paraules.push_back(paraula);

	if(!autors.triar_text(paraules)) cout << "error" << endl;
	else {
		consulta = "text triat";
		cout << consulta << endl;
	}
}
void Gestor_de_textos::substituir_paraules(string consulta) {
	istringstream iss(consulta);
	string paraula1, paraula2;
	iss >> paraula1;
	iss >> paraula2;
	iss >> paraula2;
	paraula1 = paraula1.substr(1, paraula1.size()-2);
	paraula2 = paraula2.substr(1, paraula2.size()-2);
	Text* text = autors.obtenir_text_seleccionat();
	text->substitueix_paraula(paraula1, paraula2);
	consulta = "";
}

Conjunt_autors Gestor_de_textos::obtenir_conjunt_autors() {
	return Conjunt_autors();
}
Conjunt_cites Gestor_de_textos::obtenir_conjunt_cites() {
	return Conjunt_cites();
}

int main() {
	Gestor_de_textos gestor = Gestor_de_textos();
	string linia, paraula;
	getline(cin,linia);

	while (linia != "sortir"){
	    cout << linia << endl;
	    istringstream iss(linia);
			cout << "Reading first word... ";
	    iss >> paraula;
			cout << ": " << paraula << endl;
	    if (paraula == "afegir"){
			gestor.afegir(linia.substr(paraula.size()));
		} else if (paraula == "eliminar") {
			gestor.eliminar(linia.substr(paraula.size()));
		} else if (paraula == "triar") {
			gestor.triar_text(linia.substr(paraula.size()));
		} else if (paraula == "substitueix") {
			gestor.substituir_paraules(linia.substr(paraula.size()));
		} else {
			gestor.consultes.processar_consulta(paraula, gestor.obtenir_conjunt_autors(), gestor.obtenir_conjunt_cites());
		}
		cout << "Reading new line..." << endl;
		paraula = "";
		linia = "";
		getline(cin, linia);
	}
}
