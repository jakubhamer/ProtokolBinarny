// KOMUNIKATY
// 0000 - brak b³êdów, wykonuj dzia³ania na dwóch liczbach
// 0001 - wynik wykracza poza zakres
// 0010 - licz silnie
// 0011 - dzielnik nie mo¿e byæ równy 0
// 0100 - argument silni musi byæ wiekszy b¹dŸ równy 0.
// 1111 - roz³¹czenie
//

#include "stdafx.h"
#include <boost/dynamic_bitset.hpp>


#include <winsock.h>
#include <cstdio>
#include <iostream>
#include <bitset>
#define MY_PORT 9333   // port, z którym bêd¹ siê ³¹czyli u¿ytkownicy
#define BACK_LOG 10     //jak du¿o mo¿e byæ oczekuj¹cych po³¹czeñ w kolejce
#define MAX_DATA_SIZE 100 

void dodawanie(long long l1, long long l2);
void odejmowanie(long long l1, long long l2);
void mnozenie(long long l1, long long l2);
void dzielenie(long long l1, long long l2);
void modulo(long long l1, long long l2);
void NWD(long long l1, long long l2);
void potegowanie(long long l1, long long l2);
void wieksza(long long l1, long long l2);
void liczenieSilni(int l1);

long long silnia(int x);
std::string decimalToBinary(long long liczba);
int binaryToDecimal(long long n);
void dodajID(int ID);
void dodajLiczbeDoBitset(long long liczba1);
int byteToInt(std::string byte);

int nextID = -1;
boost::dynamic_bitset<> bits(7);
int dlugoscDanych = 35;


int main()
{
	// konfiguracja pod system windows
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0)
	{
		fprintf(stderr, "WSAStartup failed.\n");
		exit(1);
	}

	int sockfd; //deskryptor gniazda
	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) // zwraca deskryptor gniazda
	{
		perror("socket");
		exit(1);
	}
	int new_fd; // adres przychodz¹cy - deskryptor
	sockaddr_in myAddr; // mój adres
	sockaddr_in theirAddr; // adres przychodz¹cy
	myAddr.sin_family = PF_INET;
	myAddr.sin_port = htons(MY_PORT); // numer portu
	myAddr.sin_addr.s_addr = htonl(INADDR_ANY); //inet_addr("192.168.43.121"); //mój adres
	memset(&myAddr.sin_zero, '\0', 8);

	if (bind(sockfd, (struct sockaddr *) &myAddr, sizeof(struct sockaddr)) == -1) // przypisanie gniazda do portu na maszynie lokalnej
	{
		perror("bind");
		exit(1);
	}
	if (listen(sockfd, BACK_LOG) == -1) // nas³uchiwanie przychodz¹cych po³¹czeñ
	{
		perror("listen");
		exit(1);
	}


	int sin_size;
	while (1)
	{;
		sin_size = sizeof(struct sockaddr_in);
		std::cout << "Waiting for connection...\n";
		if ((new_fd = accept(sockfd, (struct sockaddr*)&theirAddr, &sin_size)) == -1) // akceptacja przychodz¹cego po³¹czenia
		{
			perror("accept");
			continue;
		}
		system("cls");
		std::cout << "Server: got connection from: " << inet_ntoa(theirAddr.sin_addr) << std::endl;
		nextID++;	
		while (1)
		{
			// otrzymanie operacji
			char bufor[100];
			int numBytes; // iloœæ otrzymanych bajtów
			if ((numBytes = recv(new_fd, bufor, 25, 0)) == -1)
			{
				perror("recv");
				exit(1);
			}
			std::string odebrane;
			//zamiana char[] na string
			for (int i = 0; i < numBytes; i++)
			{
				for (int j = 0; j < 256; j++)
				{
					if (bufor[i] == (char)j)
					{
						std::string temp = decimalToBinary(j);
						while (temp.size() != 8)
						{
							std::string temp2 = temp;
							temp = "0";
							temp += temp2;
						}
						odebrane += temp;
						
					}
				}
			}

			std::cout << "Recived string: " << odebrane << std::endl;
			std::cout << "Recived bytes: " << numBytes << std::endl;
			std::string operacja;
			operacja += odebrane[0];
			operacja += odebrane[1];
			operacja += odebrane[2];

			std::string poleStatusu;
			poleStatusu += odebrane[3];
			poleStatusu += odebrane[4];
			poleStatusu += odebrane[5];
			poleStatusu += odebrane[6];


			if (poleStatusu == "0000")
			{

				std::string dlugoscDanychS;

				for (int i = 7; i < 39; i++) dlugoscDanychS += odebrane[i];

				long long dlugoscDanychLong = stoll(dlugoscDanychS, 0, 2); // string to long long, gdzie podstawa to system binarny

				dlugoscDanychLong -= 35; // 32 bity miejsca drugiej liczby, 3 bity ID

				std::string poczatekDrugiejLiczbyS;
				for (int i = 39; i < 71; i++) poczatekDrugiejLiczbyS += odebrane[i];
				long long poczatekDrugiejLiczby = stoll(poczatekDrugiejLiczbyS, 0, 2); // string to long long, gdzie podstawa to system binarny

				std::string ID;
				for (int i = 71; i < 74; i++) ID += odebrane[i];

				std::string znakL1;
				znakL1 += odebrane[74];

				std::string l1S;
				for (int i = 75; i < poczatekDrugiejLiczby; i++) l1S += odebrane[i];
				long long l1 = stoll(l1S, 0, 2);
				if (znakL1 == "0") l1 *= -1;

				std::string znakL2;
				znakL2 += odebrane[poczatekDrugiejLiczby];

				std::string l2S;
				for (int i = poczatekDrugiejLiczby + 1; i < dlugoscDanychLong + 74; i++) l2S += odebrane[i];
				long long l2 = stoll(l2S, 0, 2);
				if (znakL2 == "0") l2 *= -1;
	
				switch (stoll(operacja, 0, 2))
					{
					case 0:
					{
						dodawanie(l1, l2);
						break;
					}
					case 1:
					{
						odejmowanie(l1, l2);
						break;
					}
					case 2:
					{
						mnozenie(l1, l2);
						break;
					}
					case 3:
					{
						dzielenie(l1, l2);
						break;
					}
					case 4:
					{
						modulo(l1, l2);
						break;
					}
					case 5:
					{
						NWD(l1, l2);
						break;
					}
					case 6:
					{
						potegowanie(l1, l2);
						break;
					}
					case 7:
					{
						wieksza(l1, l2);
						break;
					}

					}

				// uzupe³nienie komunikatu do ca³ych bajtów
				if (bits.size() % 8 != 0)
				{
					int temp = bits.size() % 8;
					for (int i = 0; i < 8 - temp; i++) bits.push_back(0);
				}

				std::string komunikat;
				to_string(bits, komunikat);
				reverse(komunikat.begin(), komunikat.end());

				char msg[25];
				
				for (int i = 0; i<komunikat.size() / 8; i++) msg[i] = byteToInt(komunikat.substr(i * 8, 8)); // zamiana ka¿dego bajtu na integer
				
				int bytesSent;

				bytesSent = (send(new_fd, msg, komunikat.size() / 8, 0));
				if (bytesSent < 0)
				{
					perror("send");
					exit(1);
				}
				std::cout << "Send string: " << komunikat << std::endl;
				std::cout << "Send bytes: " << bytesSent << std::endl;

				// resetowanie zmiennych
				boost::dynamic_bitset<> bits2(7);
				bits = bits2;
				dlugoscDanych = 35;
			}
			else if (poleStatusu == "0010")
			{
				std::cout << "Liczenie silni." << std::endl;

				std::string dlugoscDanychS;
				for (int i = 7; i < 39; i++) dlugoscDanychS += odebrane[i];
				long long dlugoscDanychLong = stoll(dlugoscDanychS, 0, 2); // string to long long, gdzie podstawa to system binarny
				dlugoscDanychLong -= 35; // 32 bity miejsca drugiej liczby, 3 bity ID

				std::string poczatekDrugiejLiczbyS;
				for (int i = 39; i < 71; i++) poczatekDrugiejLiczbyS += odebrane[i];
				long long poczatekDrugiejLiczby = stoll(poczatekDrugiejLiczbyS, 0, 2); // string to long long, gdzie podstawa to system binarny

				std::string ID;
				for (int i = 71; i < 74; i++) ID += odebrane[i];

				std::string znakL1;
				znakL1 += odebrane[74];
				std::string l1S;
				for (int i = 75; i < poczatekDrugiejLiczby; i++) l1S += odebrane[i];
				long long l1 = stoll(l1S, 0, 2);
				if (znakL1 == "0") l1 *= -1;


				liczenieSilni(l1);

				// uzupe³nienie komunikatu do ca³ych bajtów
				if (bits.size() % 8 != 0)
				{
					int temp = bits.size() % 8;
					for (int i = 0; i < 8 - temp; i++) bits.push_back(0);
				}

				std::string komunikat;
				to_string(bits, komunikat);
				reverse(komunikat.begin(), komunikat.end());

				char msg[25];

				for (int i = 0; i<komunikat.size() / 8; i++) msg[i] = byteToInt(komunikat.substr(i * 8, 8)); // zamiana ka¿dego bajtu na integer

				int bytesSent;

				bytesSent = (send(new_fd, msg, komunikat.size() / 8, 0));
				if (bytesSent < 0)
				{
					perror("send");
					exit(1);
				}
				std::cout << "Send string: " << komunikat << std::endl;
				std::cout << "Send bytes: " << bytesSent << std::endl;

				boost::dynamic_bitset<> bits2(7);
				bits = bits2;
				dlugoscDanych = 35;


			}
			else if(poleStatusu == "1111")
			{
				std::cout << "Disconnecting." << std::endl;
				shutdown(new_fd, 2);
				closesocket(sockfd);
				return 0;
			}
		}
		

	}
}

void dodawanie(long long l1, long long l2)
{
	// ustawienie bitów operacji
	bits[0] = 0;
	bits[1] = 0;
	bits[2] = 0;
	
	long long wynik = (long long)l1 + (long long)l2;

	if(wynik < -2147483648LL || wynik > 2147483647)
	{
		// ustawienie bitów pola statusu - wynik wykracza poza zakres
		bits[3] = 0;
		bits[4] = 0;
		bits[5] = 0;
		bits[6] = 1;
	}
	else
	{	
		// ustawienie bitów pola statusu - brak b³êdów
		bits[3] = 0;
		bits[4] = 0;
		bits[5] = 0;
		bits[6] = 0;
	}

	dodajLiczbeDoBitset(wynik);
}
void odejmowanie(long long l1, long long l2)
{
	bits[0] = 0;
	bits[1] = 0;
	bits[2] = 1;

	long long wynik = (long long)l1 - (long long)l2;

	if (wynik < -2147483648LL || wynik > 2147483647)
	{
		// ustawienie bitów pola statusu - wynik wykracza poza zakres
		bits[3] = 0;
		bits[4] = 0;
		bits[5] = 0;
		bits[6] = 1;
	}
	else
	{
		bits[3] = 0;
		bits[4] = 0;
		bits[5] = 0;
		bits[6] = 0;
	}

	dodajLiczbeDoBitset(wynik);
}
void mnozenie(long long l1, long long l2)
{
	bits[0] = 0;
	bits[1] = 1;
	bits[2] = 0;

	long long wynik = (long long)l1 * (long long)l2;

	if (wynik < -2147483648LL || wynik > 2147483647LL)
	{
		// ustawienie bitów pola statusu - wynik wykracza poza zakres
		bits[3] = 0;
		bits[4] = 0;
		bits[5] = 0;
		bits[6] = 1;
	}
	else
	{
		bits[3] = 0;
		bits[4] = 0;
		bits[5] = 0;
		bits[6] = 0;
	}

	dodajLiczbeDoBitset(wynik);
}
void dzielenie(long long l1, long long l2)
{
	bits[0] = 0;
	bits[1] = 1;
	bits[2] = 1;
	if (l2 == 0)
	{
		// ustawienie bitów pola statusu - dzielnik nie mo¿e byæ równy 0
		bits[3] = 0;
		bits[4] = 0;
		bits[5] = 1;
		bits[6] = 1;

		dodajLiczbeDoBitset(0);
		return;
	}
	long long wynik = (long long)l1 / (long long)l2;
	if (wynik < -2147483648LL || wynik > 2147483647)
	{
		// ustawienie bitów pola statusu - wynik wykracza poza zakres
		bits[3] = 0;
		bits[4] = 0;
		bits[5] = 0;
		bits[6] = 1;
	}
	else
	{
		bits[3] = 0;
		bits[4] = 0;
		bits[5] = 0;
		bits[6] = 0;
	}

	dodajLiczbeDoBitset(wynik);
}
void modulo(long long l1, long long l2)
{
	bits[0] = 1;
	bits[1] = 0;
	bits[2] = 0;

	if (l2 == 0)
	{
		// ustawienie bitów pola statusu - dzielnik nie mo¿e byæ równy 0
		bits[3] = 0;
		bits[4] = 0;
		bits[5] = 1;
		bits[6] = 1;

		dodajLiczbeDoBitset(0);
		return;
	}
	long long wynik = (long long)l1 % (long long)l2;
	if (wynik < -2147483648LL || wynik > 2147483647)
	{
		// ustawienie bitów pola statusu - wynik wykracza poza zakres
		bits[3] = 0;
		bits[4] = 0;
		bits[5] = 0;
		bits[6] = 1;
	}
	else
	{
		bits[3] = 0;
		bits[4] = 0;
		bits[5] = 0;
		bits[6] = 0;
	}

	dodajLiczbeDoBitset(wynik);
}
void NWD(long long l1, long long l2)
{
	bits[0] = 1;
	bits[1] = 0;
	bits[2] = 1;

	l1 = abs(l1);
	l2 = abs(l2);

	while (l1 != l2) // l1 staje siê wynikiem
	{
		if (l1 < l2) l2 -= l1;
		else l1 -= l2;
	}

	bits[3] = 0;
	bits[4] = 0;
	bits[5] = 0;
	bits[6] = 0;


	dodajLiczbeDoBitset(l1);
}
void potegowanie(long long l1, long long l2)
{
	bits[0] = 1;
	bits[1] = 1;
	bits[2] = 0;

	long long wynik = pow((long long)l1, (long long)l2);
	if (wynik < -2147483648LL || wynik > 2147483647)
	{
		// ustawienie bitów pola statusu - wynik wykracza poza zakres
		bits[3] = 0;
		bits[4] = 0;
		bits[5] = 0;
		bits[6] = 1;
	}
	else
	{
		bits[3] = 0;
		bits[4] = 0;
		bits[5] = 0;
		bits[6] = 0;
	}

	dodajLiczbeDoBitset(wynik);
}
void wieksza(long long l1, long long l2)
{
	bits[0] = 1;
	bits[1] = 1;
	bits[2] = 1;

	long long wynik;
	if (l1 > l2) wynik = l1;
	else wynik = l2;

	bits[3] = 0;
	bits[4] = 0;
	bits[5] = 0;
	bits[6] = 0;
	

	dodajLiczbeDoBitset(wynik);
}
void liczenieSilni(int l1)
{
	// bity niewa¿ne
	bits[0] = 0;
	bits[1] = 0;
	bits[2] = 0;


	if (l1 < 0)
	{
		// ustawienie bitów pola statusu - argument silni musi byæ wiekszy b¹dŸ równy 0
		bits[3] = 0;
		bits[4] = 1;
		bits[5] = 0;
		bits[6] = 0;

		dodajLiczbeDoBitset(0);
		return;
	}
	long long wynik = silnia((long long)l1);
	
	if (wynik  < -2147483648LL || wynik > 2147483647)
	{
		// ustawienie bitów pola statusu - wynik wykracza poza zakres
		bits[3] = 0;
		bits[4] = 0;
		bits[5] = 0;
		bits[6] = 1;
	}
	else
	{
		bits[3] = 0;
		bits[4] = 0;
		bits[5] = 0;
		bits[6] = 0;
	}
	dodajLiczbeDoBitset(wynik);
}

long long silnia(int x)
{
	if (x == 0 || x == 1) return 1;
	else return x*silnia(x - 1);
}

std::string decimalToBinary(long long liczba) // konwersja z liczby dziesietnej do binarnej w postaci stringa
{
	if (liczba == 0) return "0";
	if (liczba == 1) return "1";

	if (liczba % 2 == 0)
		return decimalToBinary(liczba / 2) + "0";
	else
		return decimalToBinary(liczba / 2) + "1";
}
void dodajLiczbeDoBitset(long long liczba1) // dodaje pole d³ufoœci danych oraz liczby do bitsetu 
{
	std::string temp1 = decimalToBinary(liczba1);
	dlugoscDanych += temp1.size();

	if (liczba1 >= 0) dlugoscDanych++;


	//dodawanie pola d³ugoœci danych - musi byæ oddzielnie, poniewa¿ append dodaje liczbê i resztê wype³nia zerami do 32 bitów (dla nas to z³a kolejnoœæ)
	boost::dynamic_bitset<> poleDlugosci(0);
	poleDlugosci.append(dlugoscDanych);
	std::string ciagDlugosciDanych;
	to_string(poleDlugosci, ciagDlugosciDanych);
	for (int i = 0; i < ciagDlugosciDanych.size(); i++)
	{
		if (ciagDlugosciDanych[i] == '1') bits.push_back(1);
		else bits.push_back(0);
	}

	//dodanie ID sesji
	dodajID(nextID);
	
	// znak liczby i uwzglêdnienie czy liczba jest ujemna lub dodatnia, ujemna ma dodatkowo znak 0 na pocz¹tku, trzeba sie go pozbyæ

	int i;
	if (liczba1 > 0)
	{
		bits.push_back(1);
		i = 0;
	}
	else
	{
		bits.push_back(0);
		i = 1;
	}

	//dodanie liczby do bitsetu
	for (; i < temp1.size(); i++)
	{
		if (temp1[i] == '1') bits.push_back(1);
		else bits.push_back(0);
	}

}
void dodajID(int ID)
{
	std::string IDS = decimalToBinary(ID);
	for (int i = 0; i < 3 - IDS.size(); i++) bits.push_back(0); // dope³nienie do 3 bitów
	for (int i = 0; i < IDS.size(); i++)
	{
		if (IDS[i] == '1') bits.push_back(1);
		else bits.push_back(0);
	}
}
int byteToInt(std::string byte) 
{
	int temp = 0;
	for (int i = byte.size() - 1, p = 1; i >= 0; i--, p *= 2)
	{
		if (byte[i] == '1')
			temp += p;
	}
	return temp;
}
