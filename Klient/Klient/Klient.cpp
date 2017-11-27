#include "stdafx.h"
//Autorski protokół internetowy
//Wszelkie prawa zastrzeżone
//Napisany przez Marcina Hradowicza i Jakuba Hamerlińskiego
#include <winsock.h>
#include <cstdio>
#include <iostream>
#include <string>
#include <boost/dynamic_bitset.hpp>
#include <algorithm>
//
void dodawanie();
void odejmowanie();
void mnozenie();
void dzielenie();
void modulo();
void NWD();
void potegowanie();
void wieksza();
void liczenieSilni();

std::string decimalToBinary(long long liczba);
void odwrocenieStringa(std::string name);
void dodajLiczbyDoBitset(long long liczba1, long long liczba2);
void dodajWartosciDoBitset(long long liczba);
void dodajKomunikat();
void dodajID(int ID);

long long ID = 0;
boost::dynamic_bitset<> bits(7);
int dlugoscDanych = 35; // 32 bity miejsca drugiej liczby, 3 bity ID

int main(int argc, char *argv[])
{	
	
	/*bits[0] = 1;	bits[1] = 1; 	bits[2] = 1;	bits[3] = 0;	bits[4] = 1;	bits[5] = 1;
	bits[6] =0;	bits[7] = 0;	bits[8] = 0;	bits[9] = 0;	bits[10] = 1;	bits[11] = 1;
	
	bits2[0] = 1;	bits2[1] = 1; 	bits2[2] = 1;	bits2[3] = 1;
	for(int i =0 ; i<45 ; i++)
	{
		std::cout << bits[i];
	}
	std::cout << std::endl;
	
	bits.append(500);
	std::string wiadomosc;
	to_string(bits, wiadomosc);
	//std::reverse(wiadomosc.begin(),wiadomosc.end());
	
	std::cout << wiadomosc;*/


	/*long long q = 5;
	long long q1 = -5;
	std::cout << q + q1;*/


	//std::cin.get();
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0)
	{
		fprintf(stderr, "WSAStartup failed.\n");
		exit(1);
	}


	int sockfd; //deskryptor gniazda klienta
	if((sockfd = socket(PF_INET, SOCK_STREAM, 0))==-1)
	{
		perror("socket");
		exit(1);
	}

	char yes = '1';
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
	{
		perror("setsockopt");
		exit(1);
	}
	
	//wczytywanie IP serwera
	char DEST_IP[16];
	std::cout << "Podaj adres IP: \n";
	std::cin >> DEST_IP;
	//DEST_IP = "127.0.0.1";

	//wczytywanie portu serwera
	u_short DEST_PORT;
	std::cout << "Podaj port (9333): \n";
	std::cin >> DEST_PORT;
	//DEST_PORT = 9333;

	sockaddr_in theirAddr;
	theirAddr.sin_family = PF_INET;
	theirAddr.sin_port = htons(DEST_PORT);
	theirAddr.sin_addr.s_addr = inet_addr(DEST_IP);
	memset(&theirAddr.sin_zero, '\0', 8);

	//łączenie z serwerem
	if (connect(sockfd, (struct sockaddr *) & theirAddr, sizeof(struct sockaddr)) == -1)
	{
		perror("connect");
		exit(1);
	}
	printf("client: got connection to %s\n", inet_ntoa(theirAddr.sin_addr));
	
	int wybor;
	while (1)
	{
		std::cout << "Operacje:\n";
		std::cout << "1. Dodawanie\n"; //000
		std::cout << "2. Odejmowanie\n"; //001
		std::cout << "3. Mnozenie\n"; //010
		std::cout << "4. Dzielenie\n"; //011
		std::cout << "5. Modulo\n"; //100
		std::cout << "6. NWD\n"; //101
		std::cout << "7. Potegowanie\n"; //110
		std::cout << "8. Ktora wieksza?\n"; //111
		std::cout << "9. Rozlaczenie z serwerem\n"; //111
		std::cout << "10. Silnia\n"; //pole statusu - licz silnie  0010
		std::cin >> wybor;
		switch (wybor)
		{
		case 1:
		{
			dodawanie();
			break;
		}
		case 2:
		{
			odejmowanie();
			break;
		}
		case 3:
		{
			mnozenie();
			break;
		}
		case 4:
		{
			dzielenie();
			break;
		}
		case 5:
		{
			modulo();
			break;
		}
		case 6:
		{
			NWD();
			break;
		}
		case 7:
		{
			potegowanie();
			break;
		}
		case 8:
		{
			wieksza();
			break;
		}
		case 9:
		{
			// pole operacji - domyśkne
			bits[0] = 0;
			bits[1] = 0;
			bits[2] = 0;

			// pole statusu - rozłączenie
			bits[3] = 1;
			bits[4] = 1;
			bits[5] = 1;
			bits[6] = 1;

			//dodawanie pola długości danych - musi być oddzielnie, ponieważ append dodaje liczbę i resztę wypełnia zerami do 32 bitów (dla nas to zła kolejność)
			boost::dynamic_bitset<> poleDlugosci(0);
			poleDlugosci.append(dlugoscDanych);
			std::string ciagDlugosciDanych;
			to_string(poleDlugosci, ciagDlugosciDanych);
			for (int i = 0; i < ciagDlugosciDanych.size(); i++)
			{
				if (ciagDlugosciDanych[i] == '1') bits.push_back(1);
				else bits.push_back(0);
			}

			dodajID(ID);


			// dopełnienie do modulo 8 bitów
			if (bits.size() % 8 != 0)
			{
				int temp = bits.size() % 8;
				for (int i = 0; i < 8 - temp; i++) bits.push_back(0);
			}

			//wysłanie żądania o rozłączenie
			std::string komunikat;
			to_string(bits, komunikat);
			reverse(komunikat.begin(), komunikat.end());
			const char * msg = komunikat.c_str();


			if (send(sockfd, msg, strlen(msg), 0) == -1)
			{
				perror("send");
				exit(1);
			}
			std::cout << "Wyslane: " << msg << std::endl;
			std::cin.get();
			closesocket(sockfd);
			return 0;

		}
		case 10:
		{
			liczenieSilni();
			break;
		}
		default:
		{
			std::cout << "Podaj inna liczbe.\n";
			continue;
		}
		}
	
		//bity identyfikatora sesji w polu danych...
		/*bits.push_back(0);
		bits.push_back(0);
		bits.push_back(0);*/

		// dopełnienie do modulo 8 bitów
		if (bits.size() % 8 != 0)
		{
			int temp = bits.size() % 8;
			for (int i = 0; i < 8 - temp; i++) bits.push_back(0);
		}

		std::string komunikat;
		to_string(bits, komunikat);
		reverse(komunikat.begin(), komunikat.end());
		const char * msg = komunikat.c_str();
		

		if(send(sockfd, msg, strlen(msg), 0)==-1)
		{
			perror("send");
			exit(1);
		}
		std::cout << "Wyslane: " << msg << std::endl;
		std::cin.get();

		char bufor[100];
		int numBytes;
		if ((numBytes = recv(sockfd, bufor, strlen(bufor)-1, 0)) == -1)
		{
			perror("recv");
			exit(1);
		}
		std::string odebrane;
		for (int i = 0; i < numBytes; i++) odebrane += bufor[i];// usunięcie nieznaczących bitów
		std::cout << "Odebrane: " << odebrane << std::endl;


		std::string operacja;
		operacja += odebrane[0];
		operacja += odebrane[1];
		operacja += odebrane[2];

		std::string statusSesji;
		statusSesji += odebrane[3];
		statusSesji += odebrane[4];
		statusSesji += odebrane[5];
		statusSesji += odebrane[6];

		std::string dlugoscDanychS;
		for (int i = 7; i < 39; i++) dlugoscDanychS += odebrane[i];
		long long dlugoscDanychLong = stoll(dlugoscDanychS, 0, 2); // string to long long, gdzie podstawa to system binarny
		dlugoscDanychLong -= 35; // 32 bity miejsca drugiej liczby, 3 bity ID

		std::string IDS;
		for (int i = 39; i < 42; i++) IDS += odebrane[i];
		ID = stoll(IDS, 0, 2);

		std::string znak;
		znak += odebrane[42];
		std::string wynikS;
		for (int i = 43; i < dlugoscDanychLong + 42; i++) wynikS += odebrane[i];
		long long wynik = stoll(wynikS, 0, 2);
		if (znak == "0") wynik *= -1;

		//std::cout << odebrane << std::endl;
		//std::cout <<  operacja  << statusSesji << dlugoscDanychS << IDS << std::endl;


		if(statusSesji == "0000") std::cout << "Wynik: " << wynik << std::endl; // wynik
		else if(statusSesji == "0001") std::cout << "Blad. Wynik przekracza rozmiar integer." << std::endl;
		else if (statusSesji == "0011") std::cout << "Blad. Dzielnik nie moze byc rowny 0." << std::endl;
		else if (statusSesji == "0100") std::cout << "Blad. Argument silni musi byc wiekszy badz rowny 0." << std::endl;
		std::cout << "Kliknij aby kontynuowac..." << std::endl;

		std::cin.get();
		boost::dynamic_bitset<> bits2(7);
		bits = bits2;
		dlugoscDanych = 35;
		system("cls");
	}


}

void dodawanie()
{
	bits[0] = 0;
	bits[1] = 0;
	bits[2] = 0;
	dodajKomunikat();
	std::cout << "Podaj dwie liczby do dodania: ";
	std::string l1S, l2S;
	std::cin >> l1S >> l2S;

	/*while ((l1 || l2) < -2147483648LL || (l1 || l2) > 2147483647)
	{
		std::cout << "Liczba zbyt duza lub zbyt mala. Podaj inne dwie liczby: ";
		std::cin >> l1 >> l2;
	}*/
	while (l1S.size() > 19 || l2S.size() > 19)
	{
		std::cout << "Liczba zbyt duza lub zbyt mala. Podaj inne dwie liczby: ";
		std::cin >> l1S >> l2S;
	}
	long long l1, l2;
	l1 = stoll(l1S, 0, 10);
	l2 = stoll(l2S, 0, 10);
	dodajLiczbyDoBitset(l1, l2);
}
void odejmowanie()
{
	bits[0] = 0;
	bits[1] = 0;
	bits[2] = 1;
	dodajKomunikat();
	std::cout << "Podaj odjemna i odjemnik: ";
	std::string l1S, l2S;
	std::cin >> l1S >> l2S;

	/*while ((l1 || l2) < -2147483648LL || (l1 || l2) > 2147483647)
	{
	std::cout << "Liczba zbyt duza lub zbyt mala. Podaj inne dwie liczby: ";
	std::cin >> l1 >> l2;
	}*/
	while (l1S.size() > 19 || l2S.size() > 19)
	{
		std::cout << "Liczba zbyt duza lub zbyt mala. Podaj inne dwie liczby: ";
		std::cin >> l1S >> l2S;
	}
	long long l1, l2;
	l1 = stoll(l1S, 0, 10);
	l2 = stoll(l2S, 0, 10);
	std::cout << l1 << std::endl;
	std::cout << l2 << std::endl;
	dodajLiczbyDoBitset(l1, l2);
}
void mnozenie()
{
	bits[0] = 0;
	bits[1] = 1;
	bits[2] = 0;
	dodajKomunikat();
	std::cout << "Podaj mnozna i mnoznik: ";
	std::string l1S, l2S;
	std::cin >> l1S >> l2S;

	/*while ((l1 || l2) < -2147483648LL || (l1 || l2) > 2147483647)
	{
	std::cout << "Liczba zbyt duza lub zbyt mala. Podaj inne dwie liczby: ";
	std::cin >> l1 >> l2;
	}*/
	while (l1S.size() > 19 || l2S.size() > 19)
	{
		std::cout << "Liczba zbyt duza lub zbyt mala. Podaj inne dwie liczby: ";
		std::cin >> l1S >> l2S;
	}
	long long l1, l2;
	l1 = stoll(l1S, 0, 10);
	l2 = stoll(l2S, 0, 10);
	dodajLiczbyDoBitset(l1, l2);
}
void dzielenie()
{
	bits[0] = 0;
	bits[1] = 1;
	bits[2] = 1;
	dodajKomunikat();
	std::cout << "Podaj dzielna i dzielnik: ";
	std::string l1S, l2S;
	std::cin >> l1S >> l2S;

	/*while ((l1 || l2) < -2147483648LL || (l1 || l2) > 2147483647)
	{
	std::cout << "Liczba zbyt duza lub zbyt mala. Podaj inne dwie liczby: ";
	std::cin >> l1 >> l2;
	}*/
	while (l1S.size() > 19 || l2S.size() > 19)
	{
		std::cout << "Liczba zbyt duza lub zbyt mala. Podaj inne dwie liczby: ";
		std::cin >> l1S >> l2S;
	}
	long long l1, l2;
	l1 = stoll(l1S, 0, 10);
	l2 = stoll(l2S, 0, 10);
	dodajLiczbyDoBitset(l1, l2);
}
void modulo()
{
	bits[0] = 1;
	bits[1] = 0;
	bits[2] = 0;
	std::cout << "Podaj dzielna i dzielnik do dzielenia z reszta: ";
	std::string l1S, l2S;
	std::cin >> l1S >> l2S;

	/*while ((l1 || l2) < -2147483648LL || (l1 || l2) > 2147483647)
	{
	std::cout << "Liczba zbyt duza lub zbyt mala. Podaj inne dwie liczby: ";
	std::cin >> l1 >> l2;
	}*/
	while (l1S.size() > 19 || l2S.size() > 19)
	{
		std::cout << "Liczba zbyt duza lub zbyt mala. Podaj inne dwie liczby: ";
		std::cin >> l1S >> l2S;
	}
	long long l1, l2;
	l1 = stoll(l1S, 0, 10);
	l2 = stoll(l2S, 0, 10);
	dodajLiczbyDoBitset(l1, l2);
}
void NWD()
{
	bits[0] = 1;
	bits[1] = 0;
	bits[2] = 1;
	dodajKomunikat();
	std::cout << "Podaj dwie liczby: ";
	std::string l1S, l2S;
	std::cin >> l1S >> l2S;

	/*while ((l1 || l2) < -2147483648LL || (l1 || l2) > 2147483647)
	{
	std::cout << "Liczba zbyt duza lub zbyt mala. Podaj inne dwie liczby: ";
	std::cin >> l1 >> l2;
	}*/
	while (l1S.size() > 19 || l2S.size() > 19)
	{
		std::cout << "Liczba zbyt duza lub zbyt mala. Podaj inne dwie liczby: ";
		std::cin >> l1S >> l2S;
	}
	long long l1, l2;
	l1 = stoll(l1S, 0, 10);
	l2 = stoll(l2S, 0, 10);
	dodajLiczbyDoBitset(l1, l2);
}
void potegowanie()
{
	bits[0] = 1;
	bits[1] = 1;
	bits[2] = 0;
	dodajKomunikat();
	std::cout << "Podaj dwie liczby do potegowania (podstawa i wykladnik): ";
	std::string l1S, l2S;
	std::cin >> l1S >> l2S;

	/*while ((l1 || l2) < -2147483648LL || (l1 || l2) > 2147483647)
	{
	std::cout << "Liczba zbyt duza lub zbyt mala. Podaj inne dwie liczby: ";
	std::cin >> l1 >> l2;
	}*/
	while (l1S.size() > 19 || l2S.size() > 19)
	{
		std::cout << "Liczba zbyt duza lub zbyt mala. Podaj inne dwie liczby: ";
		std::cin >> l1S >> l2S;
	}
	long long l1, l2;
	l1 = stoll(l1S, 0, 10);
	l2 = stoll(l2S, 0, 10);
	dodajLiczbyDoBitset(l1, l2);
}
void wieksza()
{
	bits[0] = 1;
	bits[1] = 1;
	bits[2] = 1;
	dodajKomunikat();

	std::cout << "Podaj liczbe, aby dowiedziec sie, ktora z nich jest wieksza: ";
	std::string l1S, l2S;
	std::cin >> l1S >> l2S;

	/*while ((l1 || l2) < -2147483648LL || (l1 || l2) > 2147483647)
	{
	std::cout << "Liczba zbyt duza lub zbyt mala. Podaj inne dwie liczby: ";
	std::cin >> l1 >> l2;
	}*/
	while (l1S.size() > 19 || l2S.size() > 19)
	{
		std::cout << "Liczba zbyt duza lub zbyt mala. Podaj inne dwie liczby: ";
		std::cin >> l1S >> l2S;
	}
	long long l1, l2;
	l1 = stoll(l1S, 0, 10);
	l2 = stoll(l2S, 0, 10);
	dodajLiczbyDoBitset(l1, l2);
}
void liczenieSilni()
{
	bits[0] = 0;
	bits[1] = 0;
	bits[2] = 0;

	bits[3] = 0;
	bits[4] = 0;
	bits[5] = 1;
	bits[6] = 0;

	std::cout << "Podaj liczbe: ";
	std::string l1S;
	std::cin >> l1S;
	while (l1S.size() > 19)
	{
		std::cout << "Liczba zbyt duza lub zbyt mala. Podaj inna liczbe: ";
		std::cin >> l1S;
	}
	int l1, l2 = 0;
	l1 = stoll(l1S, 0, 10);

	/*while (l1 < 0 || l1 > 2147483647)
	{
		std::cout << "Liczba zbyt duza lub mniejsza od zera. Podaj inna liczbe: ";
		std::cin >> l1;
	}*/
	dodajLiczbyDoBitset(l1, l2);
}

std::string decimalToBinary(long long liczba)
{
	if (liczba == 0) return "0";
	if (liczba == 1) return "1";

	if (liczba % 2 == 0)
		return decimalToBinary(liczba / 2) + "0";
	else
		return decimalToBinary(liczba / 2) + "1";
}
void dodajLiczbyDoBitset(long long liczba1, long long liczba2) // dodaje pole dłufości danych oraz liczby do bitsetu 
{
	std::string temp1 = decimalToBinary(liczba1);
	std::string temp2 = decimalToBinary(liczba2);
	dlugoscDanych += temp1.size();
	dlugoscDanych += temp2.size();
	int miejscePierwszegoBituDrugiejLiczby = 70 + temp1.size() + 3 + 1; //70 - dane do pola zawierajacego miejsce rozpoczęcia drugiej liczby, 3 bity ID, +1 bo zaczyna się dopiero na kolejnym bicie
	if (liczba1 >= 0)
	{
		miejscePierwszegoBituDrugiejLiczby++; // +1 bo znak liczby pierwszej nie jest dodany (tylko do ujemnych jest dodany)
		dlugoscDanych++;// +1 znak jeśli jest dodatnia, bo ujemne z góry ma znak 0
	}
	if (liczba2 >= 0) dlugoscDanych++;// +1 znak jeśli jest dodatnia, bo ujemne z góry ma znak 0


	//std::cout << "Dlugosc danych: " << dlugoscDanych << std::endl;

	//dodawanie pola długości danych - musi być oddzielnie, ponieważ append dodaje liczbę i resztę wypełnia zerami do 32 bitów (dla nas to zła kolejność)
	boost::dynamic_bitset<> poleDlugosci(0);
	poleDlugosci.append(dlugoscDanych);
	std::string ciagDlugosciDanych;
	to_string(poleDlugosci, ciagDlugosciDanych);
	for (int i = 0; i < ciagDlugosciDanych.size(); i++)
	{
		if (ciagDlugosciDanych[i] == '1') bits.push_back(1);
		else bits.push_back(0);
	}

	boost::dynamic_bitset<> poczatekDrugiejLiczby(0);
	poczatekDrugiejLiczby.append(miejscePierwszegoBituDrugiejLiczby); 
	std::string poczatekDrugiejLiczbyS;
	to_string(poczatekDrugiejLiczby, poczatekDrugiejLiczbyS);
	for (int i = 0; i < poczatekDrugiejLiczbyS.size(); i++)
	{
		if (poczatekDrugiejLiczbyS[i] == '1') bits.push_back(1);
		else bits.push_back(0);
	}

	//dodanie ID sesji od serwera
	dodajID(ID);



	//dodanie liczb do bitsetu
	int i;
	// znak 1 liczby i uwzględnienie czy liczba jest ujemna lub dodatnia, ujemna ma dodatkowo znak 0 na początku, trzeba sie go pozbyć
	if (liczba1 >= 0)
	{
		bits.push_back(1);
		i = 0;
	}
	else
	{
		bits.push_back(0);
		i = 1;
	}
	for(; i < temp1.size(); i++)
	{
		if (temp1[i] == '1') bits.push_back(1);
		else bits.push_back(0);
	}

	// znak 2 liczby i uwzględnienie czy liczba jest ujemna lub dodatnia, ujemna ma dodatkowo znak 0 na początku, trzeba sie go pozbyć
	if (liczba2 >= 0)
	{
		bits.push_back(1);
		i = 0;
	}
	else
	{
		bits.push_back(0);
		i = 1;
	}

	for (; i < temp2.size(); i++)
	{
		if (temp2[i] == '1') bits.push_back(1);
		else bits.push_back(0);
	}
}
void dodajWartosciDoBitset(long long liczba) // dodaje wartość int do bitsetu
{
	std::string temp = decimalToBinary(liczba);
	for (int i = 0; i < temp.size(); i++)
	{
		if (temp[i] == '1') bits.push_back(1);
		else bits.push_back(0);
	}
}
void dodajKomunikat()
{
	//bity komunikatu - 0000 brak przeciwskazań
	bits[3] = 0;
	bits[4] = 0;
	bits[5] = 0;
	bits[6] = 0;
}
void dodajID(int ID)
{
	std::string IDS = decimalToBinary(ID);
	for(int i = 0; i < 3 - IDS.size(); i++) bits.push_back(0); // dopełnienie do 3 bitów
	for (int i = 0; i < IDS.size(); i++)
	{
		if (IDS[i] == '1') bits.push_back(1);
		else bits.push_back(0);
	}
}