/* A fénykard továbbfejlesztése vektorokkal, így nem szükséges változtatni a programot változó adatbázis méretekre,
futtatás argumentumként a bemeneti adatbázis, a szülőkönyvtárnak tartalmaznia kell a beolvasandó könyvtárakat,
illetve adatbázist. A program létrehoz egy out.txt file-t a könyvtárak kivonatát (pontok feladat). Kimenetként csökkenően rendezve kirja
a pontokkal rendelkező hallgatók nevét, illetve képes név szerint hallgatóra keresni, javaslatot adni, ha van hasonló név.
A --trad-lab-mark kapcsolóval a védéssel rendelkező hallgatők nevét, jegyeinek átlagát és védéseinek átlagát írja ki.

Fordítás: g++ jediv3.cpp -o [amitszeretnél] -std=c++11(opcionális)

Futtatás: ./jediv3.cpp [adatbázis_neve] --trad-lab-mark(opcionális)*/

#include <boost/sort/spreadsort/string_sort.hpp>
#include <iostream>
#include <dirent.h>
#include <vector>
#include <string>
#include <sys/stat.h>
#include <fstream>
#include <numeric>
#include <math.h>
#include <map>



using namespace std;

using namespace boost::sort::spreadsort;

int vegosszeg=0;

int inditas = 0;

vector<string> nevlista;

vector<string> feladat;


vector<string> adat_nev;

vector<string> adat_feladat;

vector<int> nev_pont;

vector<string> fel_pont;

std::vector<double> eredmenyv;

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

void leker( string nev, std::vector<string> &nevlista, std::vector<int> &pont ){

	int szam = 0;

	bool flag = false;

	bool talalat = false;

	vector<int> javaslat;

	for ( int i = 0; i < nevlista.size(); i++){

		if ( pont[i] != 0 )

			szam++;

			if ( nev == nevlista[i]){
				
				talalat = true;

				cout<< szam<<".\t"<<nevlista[i] <<" "<< pont[i]<<endl;
			}
			
				else if ( nevlista[i].find(nev) != 18446744073709551615  ){

					javaslat.push_back(i);
					
				}	
	
				
			}
		
		int elem = 0;

		if ( talalat == false ){
			
			if( javaslat.size() > 0 ){
				for (std::vector<int>::iterator i = javaslat.begin(); i != javaslat.end(); ++i)
				{
					if( flag == false ){
						cout<<"Nem találató ilyen nevű hallgató.\nEsetleg rá gondoltál?\n"<<nevlista[*i]<<endl;
						
						flag = true;
						}
					else
						cout<<nevlista[*i]<<endl;
				}
			
			cout<<"Írd be a keresett hallgató nevét, vagy 'exit' a kilépéshez!"<<endl;
			
			getline(cin, nev);

				if( nev != "exit")

					leker(nev,nevlista, nev_pont);

			}
			else cout<<"Nem találató ilyen nevű hallgató."<<endl;
		}
}

void rendez(vector<string>& nevlista, vector<double>& eredmenyv, vector<short int>& teljesitett){

	for( int i = 0; i < (nevlista.size()-1) ; i++){
				int csere_jegy = 0;

				string csere_nev;

				int csere_telj = 0;
					
				for(int j = i+1; j < nevlista.size(); j++){											//szimpla min. kiválasztásos rendezés
						
					if (eredmenyv[j] > eredmenyv[i]){
							
						csere_jegy = eredmenyv[j];

						eredmenyv[j] = eredmenyv[i];

						eredmenyv[i] = csere_jegy;

						csere_nev = nevlista[j];

						nevlista[j] = nevlista[i];

						nevlista[i] = csere_nev;

						csere_telj = teljesitett[j];

						teljesitett[j] = teljesitett[i];

						teljesitett[i] = csere_telj;

					}
				}
			}

	}


void nevlista_rendez(std::vector<string> &nevlista){

	string_sort ( nevlista.begin(), nevlista.end());			//boost spread sort fv.

}

int oszto(int a, int b, int c){

	int oszt = 0;

	if (a != 0 )

		oszt++;

	if (b != 0)

		oszt++;

	if (c != 0)
		oszt++;

	if (oszt != 0)

		return oszt;
	
	else return 1;
}

int kerekit( double szam ){

	int intszam = szam;

	if( szam < 3 ){

		if (szam - intszam >= 0.5){
			intszam  = szam+1;
			
			return intszam;
		}
		else return intszam; 
	}

	if ( szam > 3 ){

		if ( szam - intszam > 0.5){
			intszam  = szam+1;
			
			return intszam;
		}
		else return intszam; 
	}

	
}


void vedes(char* argv){

	int oszt;

	std::vector<int> elso;

	std::vector<int> masodik;

	std::vector<int> harmadik;

	std::vector<short int> teljesitett;

	bejar_db(argv);

	string sorszam;

	nevlista_rendez(nevlista);

	for ( int i = 0; i < nevlista.size(); i++){
		elso.push_back(0);

		masodik.push_back(0);

		harmadik.push_back(0);

		eredmenyv.push_back(0);

		teljesitett.push_back(0);
	}


	for( int i = 0; i < adat_feladat.size(); i++){

		if ( adat_feladat[i].find("Védés/") != 18446744073709551615 ){

			string x = adat_feladat[i].substr(adat_feladat[i].find("Védés/")+8);

			string sorszam;

			if ( x.find_first_of("/") != 18446744073709551615 ){
				sorszam = x.substr(0, x.find_first_of("/"));
			}
	


			string jegy = x.substr(x.find_first_of("/")+1 );
				
				jegy = jegy.substr(0, jegy.length());
				
				int szam = 0;

				
				if ( jegy == "elégtelen")
					szam = 1;

				else if( jegy == "elégséges")
					szam = 2;

				else if ( jegy == "közepes" )
					szam = 3;

				else if ( jegy == "jó")
					szam = 4;

				else if (jegy == "jeles")
					szam = 5;

				else szam = 0; 
			
			

				for (int j = 0 ; j < nevlista.size(); j++)
				{
					if( nevlista[j] == adat_nev[i] ){
						

						if ( sorszam == "I"){
							elso[j] = szam;

						}

						else if( sorszam == "II" )
							masodik[j] = szam;

						else if ( sorszam == "III")
							harmadik[j] = szam;
				
						oszt = oszto(elso[j], masodik[j], harmadik[j]);

						teljesitett[j] = oszt;

						eredmenyv[j] = kerekit(  (elso[j] + masodik[j] + harmadik[j]) / oszt  );


					}
				}		
		
		}
		
	}




	/*
	std::map<string, int> m;

	for (size_t i = 0; i < nevlista.size(); ++i)
   		m[nevlista[i]] = eredmenyv[i];

	for ( const auto &p : m){
		
		if ( p.second != 0 )
			cout<<p.first<<"\t"<<p.second<<endl;
	}

	*/

	cout<<"FUTURE LABORJEGY LISTA \n\nNév\t\t |Érdemjegy  |Teljesített\n----------------------------------------------------------\n";

	rendez( nevlista, eredmenyv, teljesitett);

	for ( int j = 0 ; j < nevlista.size(); j++){

		if ( eredmenyv[j] != 0 ){


			cout<<" "<<nevlista[j];

			if (nevlista[j].length() < 16 )

				cout<<'\t';

			cout<<" |"<<eredmenyv[j]<<"\t     |#"<<teljesitett[j]<<endl;
			cout<<"----------------------------------------------------------\n";
		}
	}


}


int main(int argc, char *argv[]){

														//cout<<"Usage: "<<argv[0]<< " db_name --trad-lab-mark(optional)" <<endl;

	bool flag = false;	//--trad-lab-mark esetén true

	for(int it = 0 ; it < argc; it++){
		
		if ( string(argv[it]) == "--trad-lab-mark"){
			
			flag = true;
			}

		}

	if (flag == false ){
			
				//bejar(argv[1]);		//ebben az esetben futtatási argumentumként adjuk meg a kezdő dir-t

				//----------------------------------------------------------

				//out<<"\nCity/Debrecen/Oktatás/Informatika/Programozás/DEIK/Prog1/Példák/Előadás\n"<<endl;

			bejar((char*)"City/Debrecen/Oktatás/Informatika/Programozás/DEIK/Prog1/Példák/Előadás");

				//out<<"\nCity/Debrecen/Szórakozás/Könyv/Ismeretterjesztő/Informatika\n"<<endl;

			bejar((char*)"City/Debrecen/Szórakozás/Könyv/Ismeretterjesztő/Informatika");

				//out<<"\nCity/Debrecen/Oktatás/Informatika/Programozás/Tankönyv olvasás\n"<<endl;

			bejar((char*)"City/Debrecen/Oktatás/Informatika/Programozás/Tankönyv olvasás");

			bejar((char*)"City/Debrecen/Oktatás/Informatika/Programozás/UDPROG");

			bejar((char*)"City/Debrecen/Szórakozás/Előadás/Ismeretterjesztő/Informatika");

			bejar((char*)"City/Debrecen/Oktatás/Informatika/Programozás/Forráskód olvasás");

			bejar ((char*)"City/Debrecen/Oktatás/Informatika/Programozás/Forráskód írás");

			bejar ((char*)"City/Debrecen/Oktatás/Informatika/Programozás/API doksi olvasás");

			bejar ((char*) "City/Debrecen/Oktatás/Informatika/Programozás/DEIK/Prog1/Labor/Védés");

			bejar ((char*) "City/Debrecen/Szórakozás/Film");

				//out<<"Összesen: "<<vegosszeg<<endl;

			out.close();

			bejar_db(argv[1]);

			extr_feladat();

			for(int i = 0; i < nevlista.size(); i++){

				nev_pont.push_back(0);    //fel kell töltenünk a vektort, hogy azonos méretű legyen a névlistával, különben a pontok betöltésekor szegmentálási hibát kapunk
			}


			//nevlista_rendez(nevlista);

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

			cout<<"Írd be egy hallgató nevét, vagy 'all'-t a teljes listához!"<<endl;

			string nev;

			getline(cin, nev);

			if ( nev == "all" ){


				int szam = 0;

				cout<<"FUTURE PONTLISTA:\n\n";

				for(int i = 0; i < nevlista.size(); i++){

					if(nev_pont[i] != 0 ){														//névlista pontokkal, mivel még random méretű tömb, így addig megyünk,
						
						szam++;

						cout<<szam<<".\t"<<nevlista[i]<<" "<<nev_pont[i]<<endl;									//amíg nem üres, hogy kerüljük az üres köröket.
					
						/*if ( szam == 30 )
							cout<< "\n ----------- JEGYMEGAJANLAS VEGE-------------- \n"<<endl;*/
															}
				}
				
				cout<<"\nÖsszesen elérhető: "<<vegosszeg<<endl;

					
				}
				
				else if (nev == "exit")

					return 0;

				else {
					leker(nev, nevlista, nev_pont);

					return 0;
				}

		}	
	
	else
		vedes(argv[1]);
			

	
	

	return 0;
}
