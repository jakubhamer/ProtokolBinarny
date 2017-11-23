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

#define MY_PORT 2500
void dodawanie();
void odejmowanie();
void mnozenie();
void dzielenie();
void modulo();
void rownosc();
void potegowanie();
void wieksza();
std::string decimalToBinary(int liczba);
void odwrocenieStringa(std::string name);
void dodajLiczbyDoBitset(int liczba1, int liczba2);
void dodajWartosciDoBitset(int liczba);
void dodajKomunikat();

boost::dynamic_bitset<> bits(7);

int dlugoscDanych = 3;

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
	
	std::cout << wiadomosc;
	std::cin.get();*/
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
	char *DEST_IP;
	std::cout << "Podaj adres IP: \n";
	//std::cin >> DEST_IP;
	DEST_IP = "192.168.0.20";

	//wczytywanie portu serwera
	u_short DEST_PORT;
	std::cout << "Podaj port: \n";
	//std::cin >> DEST_PORT;
	DEST_PORT = 9333;

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
		std::cout << "6. Rownosc\n"; //101
		std::cout << "7. Potegowanie\n"; //110
		std::cout << "8. Wieksza\n"; //111
		std::cout << "9. Rozlaczenie z serwerem\n"; //111
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
			rownosc();
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
			return 0;
		}
		default:
		{
			std::cout << "Podaj inna liczbe.\n";
			continue;
		}
		}
	
		//bity identyfikatora sesji w polu danych...
		bits.push_back(0);
		bits.push_back(0);
		bits.push_back(0);

		std::string komunikat;
		to_string(bits, komunikat);
		reverse(komunikat.begin(), komunikat.end());
		const char * msg = komunikat.c_str();
		std::cout << msg;
		if(send(sockfd, msg, strlen(msg), 0)==-1)
		{
			perror("send");
			exit(1);
		}
		char bufor[100];
		if (recv(sockfd, bufor, strlen(bufor)-1, 0) == -1)
		{
			perror("recv");
			exit(1);
		}
		std::cout << bufor << std::endl;
		boost::dynamic_bitset<> bits2(7);
		bits = bits2;
		dlugoscDanych = 3;
	}


}


void dodawanie()
{
	bits[0] = 0;
	bits[1] = 0;
	bits[2] = 0;
	dodajKomunikat();
	std::cout << "Podaj dwie liczby do dodania: ";
	int l1, l2;
	std::cin >> l1 >> l2;
	while ((l1 || l2) < -2147483648LL || (l1 || l2) > 2147483647)
	{
		std::cout << "Liczba zbyt duza lub zbyt mala. Podaj inne dwie liczby: ";
		std::cin >> l1 >> l2;
	}
	dodajLiczbyDoBitset(l1, l2);
}
void odejmowanie()
{
	bits[0] = 0;
	bits[1] = 0;
	bits[2] = 1;
	dodajKomunikat();
	std::cout << "Podaj odjemna i odjemnik: (max 65535): ";
	int l1, l2;
	std::cin >> l1 >> l2;
	while ((l1 || l2) < -2147483648LL || (l1 || l2) > 2147483647)
	{
		std::cout << "Liczba zbyt duza lub zbyt mala. Podaj inne dwie liczby: ";
		std::cin >> l1 >> l2;
	}
	dodajLiczbyDoBitset(l1, l2);
}
void mnozenie()
{
	bits[0] = 0;
	bits[1] = 1;
	bits[2] = 0;
	dodajKomunikat();
	std::cout << "Podaj mnozna i mnoznik (max 65535): ";
	int l1, l2;
	std::cin >> l1 >> l2;
	while ((l1 || l2) < -2147483648LL || (l1 || l2) > 2147483647)
	{
		std::cout << "Liczba zbyt duza lub zbyt mala. Podaj inne dwie liczby: ";
		std::cin >> l1 >> l2;
	}
	dodajLiczbyDoBitset(l1, l2);
}
void dzielenie()
{
	bits[0] = 0;
	bits[1] = 1;
	bits[2] = 1;
	dodajKomunikat();
	std::cout << "Podaj dzielna i dzielnik (max 65535): ";
	int l1, l2;
	std::cin >> l1 >> l2;
	while ((l1 || l2) < -2147483648LL || (l1 || l2) > 2147483647)
	{
		std::cout << "Liczba zbyt duza lub zbyt mala. Podaj inne dwie liczby: ";
		std::cin >> l1 >> l2;
	}
	dodajLiczbyDoBitset(l1, l2);
}
void modulo()
{
	bits[0] = 1;
	bits[1] = 0;
	bits[2] = 0;
	std::cout << "Podaj dzielna i dzielnik do dzielenia z reszta (max 65535): ";
	int l1, l2;
	std::cin >> l1 >> l2;
	while ((l1 || l2) < -2147483648LL || (l1 || l2) > 2147483647)
	{
		std::cout << "Liczba zbyt duza lub zbyt mala. Podaj inne dwie liczby: ";
		std::cin >> l1 >> l2;
	}
	dodajLiczbyDoBitset(l1, l2);
}
void rownosc()
{
	bits[0] = 1;
	bits[1] = 0;
	bits[2] = 1;
	dodajKomunikat();
	std::cout << "Podaj dwie liczby do porownania (max 65535): ";
	int l1, l2;
	std::cin >> l1 >> l2;
	while ((l1 || l2) < -2147483648LL || (l1 || l2) > 2147483647)
	{
		std::cout << "Liczba zbyt duza lub zbyt mala. Podaj inne dwie liczby: ";
		std::cin >> l1 >> l2;
	}
	dodajLiczbyDoBitset(l1, l2);
}
void potegowanie()
{
	bits[0] = 1;
	bits[1] = 1;
	bits[2] = 0;
	dodajKomunikat();
	std::cout << "Podaj dwie liczby do potegowania (podstawa i wykladnik): ";
	int l1, l2;
	std::cin >> l1 >> l2;
	while ((l1 || l2) < -2147483648LL || (l1 || l2) > 2147483647)
	{
		std::cout << "Liczba zbyt duza lub zbyt mala. Podaj inne dwie liczby: ";
		std::cin >> l1 >> l2;
	}
	dodajLiczbyDoBitset(l1, l2);
}
void wieksza()
{
	bits[0] = 1;
	bits[1] = 1;
	bits[2] = 1;
	dodajKomunikat();

	std::cout << "Podaj liczbe, aby dowiedziec sie, ktora z nich jest wieksza: ";
	int l1, l2;
	std::cin >> l1 >> l2;
	while ((l1 || l2) < -2147483648LL || (l1 || l2) > 2147483647)
	{
		std::cout << "Liczba zbyt duza lub zbyt mala. Podaj inne dwie liczby: ";
		std::cin >> l1 >> l2;
	}
	dodajLiczbyDoBitset(l1, l2);
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
void dodajLiczbyDoBitset(int liczba1, int liczba2) // dodaje pole dłufości danych oraz liczby do bitsetu 
{
	std::string temp1 = decimalToBinary(liczba1);
	std::string temp2 = decimalToBinary(liczba2);
	dlugoscDanych += temp1.size();
	dlugoscDanych += temp2.size();
	std::cout << "Dlugosc danych: " << dlugoscDanych << std::endl;

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
	
	//dodanie liczb do bitsetu
	for(int i = 0; i < temp1.size(); i++)
	{
		if (temp1[i] == '1') bits.push_back(1);
		else bits.push_back(0);
	}
	for (int i = 0; i < temp2.size(); i++)
	{
		if (temp2[i] == '1') bits.push_back(1);
		else bits.push_back(0);
	}
}
void dodajWartosciDoBitset(int liczba) // dodaje wartość int do bitsetu
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
