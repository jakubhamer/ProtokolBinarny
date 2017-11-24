// KOMUNIKATY
// 0000 - brak b³êdów
// 0001 - wynik przekracza maksymaln¹ wartoœæ

#include "stdafx.h"
#include <boost/dynamic_bitset.hpp>


#include <winsock.h>
#include <cstdio>
#include <iostream>

#define MY_PORT 9333   // port, z którym bêd¹ siê ³¹czyli u¿ytkownicy
#define BACK_LOG 10     //jak du¿o mo¿e byæ oczekuj¹cych po³¹czeñ w kolejce
#define MAX_DATA_SIZE 100 

void dodawanie(int l1, int l2);
void odejmowanie(int l1, int l2);
void mnozenie(int l1, int l2);
void dzielenie(int l1, int l2);
void modulo(int l1, int l2);
void NWD(int l1, int l2);
void potegowanie(int l1, int l2);
void wieksza(int l1, int l2);
std::string decimalToBinary(int liczba);
int binaryToDecimal(long long n);
void dodajID(int ID);
void dodajLiczbeDoBitset(int liczba1);

int nextID = 1;
char komunikat[] = "\0";
boost::dynamic_bitset<> bits(7);
int dlugoscDanych = 0;


int main()
{
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0)
	{
		fprintf(stderr, "WSAStartup failed.\n");
		exit(1);
	}

	int sockfd; //deskryptor gniazda
	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
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

	if (bind(sockfd, (struct sockaddr *) &myAddr, sizeof(struct sockaddr)) == -1)
	{
		perror("bind");
		exit(1);
	}
	if (listen(sockfd, BACK_LOG) == -1)
	{
		perror("listen");
		exit(1);
	}


	int sin_size;
	//datagram
	while (1)
	{;
		sin_size = sizeof(struct sockaddr_in);
		if ((new_fd = accept(sockfd, (struct sockaddr*)&theirAddr, &sin_size)) == -1)
		{
			perror("accept");
			continue;
		}
		std::cout << "server: got connection from: " << inet_ntoa(theirAddr.sin_addr) << std::endl;
		
		//nadanie ID klientowi
		/*std::string nextIDS;
		int sizeID = decimalToBinary(nextID).size();
		while (sizeID++ < 3)  nextIDS += "0";
		nextIDS += decimalToBinary(nextID++);
		
		std::string ID_message = "0000000" + nextIDS + "00000000000000000000000000000000";
		std::bitset<42> byte(ID_message);
		std::string a = byte.to_string();
		const char * c = a.c_str();*/
		
		while(1)
		{
			//otrzymanie operacji
			char bufor[100];
			int numBytes;
			if ((numBytes = recv(new_fd, bufor, strlen(bufor), 0)) == -1)
			{
				perror("recv");
				exit(1);
			}
			std::cout << std::endl;
			std::string odebrane;
			for (int i = 0; i < numBytes; i++) odebrane += bufor[i];// usuniêcie nieznacz¹cych bitów
			
			std::string operacja;
			operacja += odebrane[0];
			operacja += odebrane[1];
			operacja += odebrane[2];

			std::string dlugoscDanychS;
			for (int i = 7; i < 39; i++) dlugoscDanychS += odebrane[i];
			std::string::size_type sz = 0;
			long long ll = std::stoll(dlugoscDanychS, &sz, 2); // string to long long, gdzie podstawa to system binarny
			int dlugoscDanychInt = binaryToDecimal(ll);

			std::string ID;
			for (int i = 39; i < 42; i++) ID += odebrane[i];

			std::string dane;
			for (int i = 42; i < dlugoscDanychInt + 42; i++) dane += odebrane[i];


			int l1 = 5, l2 = 3; //liczby
			std::cout << odebrane << std::endl;
			std::cout << operacja << "----" << dlugoscDanychS << ID << dane << std::endl;

			std::cout << "przed switchem: " << binaryToDecimal(stoll(operacja, &sz, 2)) << std::endl;
			switch (binaryToDecimal(stoll(operacja, &sz, 2)))
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
			
			if (bits.size() % 8 != 0)
			{
				int temp = bits.size() % 8;
				for (int i = 0; i < 8 - temp; i++) bits.push_back(0);
			}

			std::string komunikat;
			to_string(bits, komunikat);
			reverse(komunikat.begin(), komunikat.end());
			const char * msg = komunikat.c_str();

			std::cout << komunikat << std::endl;
			std::cout << msg << std::endl;
			if((send(new_fd, msg, strlen(msg),0)) == -1)
			{
				perror("send");
				exit(1);
			}

			boost::dynamic_bitset<> bits2(7);
			bits = bits2;
			dlugoscDanych = 0;
		}



		
		std::cin.get();
		closesocket(new_fd);
	}
}

void dodawanie(int l1, int l2)
{
	bits[0] = 0;
	bits[1] = 0;
	bits[2] = 0;
	
	long long wynik;
	if((wynik = l1+l2) < -2147483648LL || (l1 || l2) > 2147483647)
	{
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
void odejmowanie(int l1, int l2)
{
	bits[0] = 0;
	bits[1] = 0;
	bits[2] = 1;

	long long wynik;
	if ((wynik = l1 - l2) < -2147483648LL || (l1 || l2) > 2147483647)
	{
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
void mnozenie(int l1, int l2)
{
	bits[0] = 0;
	bits[1] = 1;
	bits[2] = 0;

	long long wynik;
	if ((wynik = l1 * l2) < -2147483648LL || (l1 || l2) > 2147483647)
	{
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
void dzielenie(int l1, int l2)
{
	bits[0] = 0;
	bits[1] = 1;
	bits[2] = 1;

	long long wynik;
	if ((wynik = l1 / l2) < -2147483648LL || (l1 || l2) > 2147483647)
	{
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
void modulo(int l1, int l2)
{
	bits[0] = 1;
	bits[1] = 0;
	bits[2] = 0;

	long long wynik;
	if ((wynik = l1 % l2) < -2147483648LL || (l1 || l2) > 2147483647)
	{
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
void NWD(int l1, int l2)
{
	bits[0] = 1;
	bits[1] = 0;
	bits[2] = 1;

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
void potegowanie(int l1, int l2)
{
	bits[0] = 1;
	bits[1] = 1;
	bits[2] = 0;

	long long wynik;
	if ((wynik = pow(l1,l2)) < -2147483648LL || (l1 || l2) > 2147483647)
	{
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
void wieksza(int l1, int l2)
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

std::string decimalToBinary(int liczba)
{
	/*std::stringstream a(liczba);
	int liczbaInt;
	a >> liczbaInt;*/

	if (liczba == 0) return "0";
	if (liczba == 1) return "1";

	if (liczba % 2 == 0)
		return decimalToBinary(liczba / 2) + "0";
	else
		return decimalToBinary(liczba / 2) + "1";
}
int binaryToDecimal(long long n)
{
	int decimalNumber = 0, i = 0, remainder;
	while (n != 0)
	{
		remainder = n % 10;
		n /= 10;
		decimalNumber += remainder*pow(2, i);
		++i;
	}
	return decimalNumber;
}
void dodajLiczbeDoBitset(int liczba1) // dodaje pole d³ufoœci danych oraz liczby do bitsetu 
{
	std::string temp1 = decimalToBinary(liczba1);
	dlugoscDanych += temp1.size();

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
	int id = 0; // TUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU SKOÑCZYÆ
	dodajID(id);

	//dodanie liczby do bitsetu
	for (int i = 0; i < temp1.size(); i++)
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