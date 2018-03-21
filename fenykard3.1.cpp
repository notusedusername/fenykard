/* A fénykard továbbfejlesztése vektorokkal, így nem szükséges változtatni a programot változó adatbázis méretekre,
futtatás argumentumként a bemeneti adatbázis, a szülőkönyvtárnak tartalmaznia kell a beolvasandó könyvtárakat,
illetve adatbázist. A program létrehoz egy out.txt file-t a könyvtárak kivonatát (pontok feladat). Kimenetként csökkenően rendezve kirja
a pontokkal rendelkező hallgatók nevét. */

#include <iostream>
#include <dirent.h>
#include <vector>
#include <string>
#include <sys/stat.h>
#include <fstream>
#include <numeric>
#include <math.h>



using namespace std;

int vegosszeg=0;

int inditas = 0;

vector<string> nevlista;

vector<string> feladat;


vector<string> adat_nev;

vector<string> adat_feladat;

vector<int> nev_pont;

vector<string> fel_pont;

ofstream out("out.txt");




void extr_feladat(){

	

	ifstream pontlista("out.txt");

	if( !pontlista.is_open() ){
		cerr<<"Hiba!!"<<endl;
		
	}

	string feladat_path;
	string pont;

	



	while(pontlista.good())
	{

		getline(pontlista, pont, '\t');

		fel_pont.push_back(pont);
			
			
		getline(pontlista, feladat_path, '\n');	
		
		if(feladat_path != "")

			feladat_path=feladat_path.erase(feladat_path.find_last_of("/"));

		feladat.push_back(feladat_path);
		

		feladat_path = "";

		pont = "";

		
	}
	

	

}



void extr_nev(string nev){
	
	
	
	inditas++;
	
	if (inditas == 1)
			nevlista.push_back(nev);
			
		

	int i=-1;

	do	
		{
			
			i++;
			
		}
	while  (nev != nevlista[i] && i <= nevlista.size() && nevlista[i] != "");

	if ( nev != nevlista[i])
			
		nevlista.push_back(nev);
			
	


	
}




void bejar_db( char *db_nev){

	
	ifstream db(db_nev);	

	if(!db.is_open()){
		cerr<<"Nem jött össze!";
	}
	


	
	string nev, feladat, ido;

	

	

	while(db.good()){

		getline(db, nev, ',');

		if(nev.size() != 0)
			nev = nev.substr(1,(nev.length()-2));

		extr_nev(nev);
		
		adat_nev.push_back(nev);
		
		
		getline(db, feladat, ',');

		if(feladat.size() != 0)
			
			feladat = feladat.substr(3,(feladat.length()-4)); // a négy az első három + az utolsó karakterből jön össze ami "./ +" 
 
		adat_feladat.push_back( feladat );		//adatok mátrixban 
		
		getline(db, ido, '\n');
		
		//adat[szamlalo][2] = ido;
		
		
		ido = "";
		feladat = "";
		
	
	
	
	
		
	}

}



int kivon( string line){
	
	int number=0;
	
	int counter=0;
	
	std::vector<int> szamjegyek;			// A beolvasott sorból megkeressi a számjegyeket	
											//vektorba rakja, és helyiértékenként összerakja egy számmá.		

	for(int i=line.length(); i>0; i--){
		
		if(isdigit (line[i])){
			
			int szamjegy=line[i]-'0';			// - '0' , hogy ne az ASCII értéket adja vissza.
			
			szamjegyek.push_back(szamjegy);
		}
	}
	
	while(counter<szamjegyek.size()){
		
		number = number+szamjegyek[counter]*pow(10,counter);		//decimális szám a 10 hatványaival
		counter++;
	}

	return number;
}

int pontszam(string path){

	string line;

	//string pont;
	
	int pont;

	vector<int> pontok;

	ifstream file(path);



	while (file.good()){

		getline(file,line);

		if (line != "\0"){

			/*pont=(line.substr(line.rfind(" ")+1)); //a sor végi szám egy " " után található,								1.0-ás módszer, a .props fileokba a számok után beletrollkodott space
													//levágjuk a sor végét, és integer típusú vátozóvá konvertáljuk.		megfogja.
			int intpont = stoi(pont);*/
		
			pont = kivon(line);

			pontok.push_back(pont);				//a kész változót vektorba mentjük.

			

		}

	}
	
	int sum = std::accumulate(pontok.begin(), pontok.end(), 0);	//a vektor elemeinek összege az adott file pontszáma.
																
	
	vegosszeg += sum;

	file.close();

	return sum;

}

void bejar(char *nev){
	DIR *dir;	

	struct dirent *entry;
	struct stat info;
	int meret;

	dir = opendir(nev);
		
	/*if(!dir){
		cout<<"Nem találató megnyitható mappa!"<<endl;
		return;
	} */

	while ((entry = readdir(dir)) != NULL){
		
		if (entry->d_name[0] != '.'){
			
			string path = string (nev) + "/" + string(entry->d_name); // a path előállítása
			

			meret=string(entry->d_name).length(); // a tömb mérete
			
			if (entry->d_name[meret-6] == '.'){ // *.????? formátumú sorok keresése
				
				out<< pontszam(path)<<"\t"<<path << endl;
				
				


			}
			stat(path.c_str(), &info);

			if(S_ISDIR(info.st_mode)){
				bejar((char*)path.c_str());
			}
		}

	}

closedir(dir);
}



int main(int argc, char *argv[]){

//bejar(argv[1]);		//ebben az esetben futtatási argumentumként adjuk meg a kezdő dir-t

//----------------------------------------------------------

//out<<"\nCity/Debrecen/Oktatás/Informatika/Programozás/DEIK/Prog1/Példák/Előadás\n"<<endl;

bejar((char*)"City/Debrecen/Oktatás/Informatika/Programozás/DEIK/Prog1/Példák/Előadás");

//out<<"\nCity/Debrecen/Szórakozás/Könyv/Ismeretterjesztő/Informatika\n"<<endl;

bejar((char*)"City/Debrecen/Szórakozás/Könyv/Ismeretterjesztő/Informatika");

//out<<"\nCity/Debrecen/Oktatás/Informatika/Programozás/Tankönyv olvasás\n"<<endl;

bejar((char*)"City/Debrecen/Oktatás/Informatika/Programozás/Tankönyv olvasás");

//out<<"Összesen: "<<vegosszeg<<endl;

out.close();

bejar_db(argv[1]);

extr_feladat();

for(int i = 0; i < nevlista.size(); i++){

	nev_pont.push_back(0);    //fel kell töltenünk a vektort, hogy azonos méretű legyen a névlistával, különben a pontok betöltésekor szegmentálási hibát kapunk
}

for(int adatsz = 1; adatsz < adat_nev.size(); adatsz++){
	
	for(int feladatsz = 1; feladatsz <= feladat.size() ; feladatsz++){
																						//a tömbökben megkeresni a feladatot és az ahhoz tartozó pontot
																						//majd a megoldó nevét megkeresi a névlistában, és hozzáadja a pontjaihoz						
		if ( adat_feladat[adatsz] == feladat[feladatsz]){									//a feladat pontszámát.
			
			for(int nevsz = 1; nevsz <= nevlista.size(); nevsz++){
				
				if ( adat_nev[adatsz] == nevlista[nevsz])
					
					if(fel_pont[feladatsz] != "")
						
						nev_pont[nevsz] += stoi(fel_pont[feladatsz]);				//konvertálni kell, mert stringben van tárolva
				
				
		
			}
		}
	}
}	



//csökkenő rendezés

for( int i = 0; i < (nevlista.size()-1) ; i++){
	int csere_pont = 0;

	string csere_nev;
	
	for(int j = i+1; j < nevlista.size(); j++){											//szimpla min. kiválasztásos rendezés
		
		if (nev_pont[j] > nev_pont[i]){
			
			csere_pont = nev_pont[j];

			nev_pont[j] = nev_pont[i];

			nev_pont[i] = csere_pont;

			csere_nev = nevlista[j];

			nevlista[j] = nevlista[i];

			nevlista[i] = csere_nev;

		}
	}
}

cout<<"FUTURE PONTLISTA:\n\n";

for(int i = 0; i < nevlista.size(); i++){

	if(nev_pont[i] != 0 )															//névlista pontokkal, mivel még random méretű tömb, így addig megyünk,

		cout<<nevlista[i]<<" "<<nev_pont[i]<<endl;									//amíg nem üres, hogy kerüljük az üres köröket.

}
cout<<"\nÖsszesen elérhető: "<<vegosszeg<<endl;




return 0;
}
