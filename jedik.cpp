# fenykard 

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
				
				cout<< pontszam(path)<<"\t"<<path << endl;
				
				


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

cout<<"\nCity/Debrecen/Oktatás/Informatika/Programozás/DEIK/Prog1/Példák/Előadás\n"<<endl;

bejar((char*)"City/Debrecen/Oktatás/Informatika/Programozás/DEIK/Prog1/Példák/Előadás");

cout<<"\nCity/Debrecen/Szórakozás/Könyv/Ismeretterjesztő/Informatika\n"<<endl;

bejar((char*)"City/Debrecen/Szórakozás/Könyv/Ismeretterjesztő/Informatika");

cout<<"\nCity/Debrecen/Oktatás/Informatika/Programozás/Tankönyv olvasás\n"<<endl;

bejar((char*)"City/Debrecen/Oktatás/Informatika/Programozás/Tankönyv olvasás");

cout<<"Összesen: "<<vegosszeg<<endl;

return 0;
}
