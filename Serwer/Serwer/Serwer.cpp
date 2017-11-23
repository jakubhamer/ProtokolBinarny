#include "stdafx.h"
#include <boost/dynamic_bitset.hpp>


#include <winsock.h>
#include <cstdio>
#include <iostream>

#define MY_PORT 9333   // port, z którym bêd¹ siê ³¹czyli u¿ytkownicy
#define BACK_LOG 10     //jak du¿o mo¿e byæ oczekuj¹cych po³¹czeñ w kolejce
#define MAX_DATA_SIZE 100 

void dodawanie();
void odejmowanie();
void mnozenie();
void dzielenie();
void modulo();
void rownosc();
void potegowanie();
void wieksza();
std::string decimalToBinary(int liczba);
int binaryToDecimal(long long n);

int nextID = 1;
char komunikat[] = "\0";

int convertBinaryToDecimal(long long n);
int main()
{
	boost::dynamic_bitset<> x(5);
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
		std::cout << "server: got connection from: " << inet_ntoa(theirAddr.sin_addr);
		
		//nadanie ID klientowi
		/*std::string nextIDS;
		int sizeID = decimalToBinary(nextID).size();
		while (sizeID++ < 3)  nextIDS += "0";
		nextIDS += decimalToBinary(nextID++);
		
		std::string ID_message = "0000000" + nextIDS + "00000000000000000000000000000000";
		std::bitset<42> byte(ID_message);
		std::string a = byte.to_string();
		const char * c = a.c_str();*/
		
		//otrzymanie operacji
		char bufor[100];
		int numBytes;
		if((numBytes = recv(new_fd, bufor, strlen(bufor),0)) == -1)
		{
			perror("recv");
			exit(1);
		}
		std::cout << std::endl;
		std::string odebrane;
		for (int i = 0; i < numBytes; i++) odebrane+= bufor[i];// usuniêcie nieznacz¹cych bitów

		std::string operacja;
		operacja += odebrane[0];
		operacja += odebrane[1];
		operacja += odebrane[2];

		std::string dlugoscDanych;
		for (int i = 7; i < 39; i++) dlugoscDanych += odebrane[i];
		std::string::size_type sz = 0;
		long long ll = std::stoll(dlugoscDanych, &sz, 2); // string to long long, gdzie podstawa to system binarny
		int dlugoscDanychInt = binaryToDecimal(ll);

		std::string dane;
		for (int i = 39; i < dlugoscDanychInt + 39 - 3; i++)
		{
			std::cout << "i: " << i << std::endl;
			dane += odebrane[i];
		}


		std::string ID;
		for (int i = dlugoscDanychInt + 39 - 3; i < dlugoscDanychInt + 39; i++) ID += odebrane[i];

		
		std::cin.get();
		/*if(send(new_fd, c, strlen(c), 0)== -1)
		{
			perror("send ID");
		}*/


		
		std::cin.get();
		closesocket(new_fd);
	}
}

void dodawanie();
void odejmowanie();
void mnozenie();
void dzielenie();
void modulo();
void rownosc();
void potegowanie();
void wieksza();

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