#include<iostream>
#include<cmath>
#include<windows.h>
#include<conio.h>
#pragma comment(lib, "ws2_32.lib")  
#include<WinSock2.h>
using namespace std;
BOOL RecvLine(SOCKET s, char* buf);
struct food{
	int xpos = 0;
	int ypos = 0;
	int fAlive = 1;
};
struct pos{
	int x = 10;
	int y = 10;
};
struct snakehead{
	pos p[500] {};
	int alive = 1;
	int way = 0;
	int num = 1;
};
food Food;
snakehead ahead;
snakehead bhead;
void createFood();
void print();
int move();
void print2();
int move2();
void createFood2();
int main(){
	cout << "贪吃蛇2.0" << endl;
	cout << "press 'a' to start in 1p" << endl;
	cout << "press other to start in 2p" << endl;
	char a;
	cin >> a;
	if (a != 'a'){//多人模式
		food fstore = Food;
		snakehead hstore = ahead;
		snakehead pstore = bhead;
	begin2:	Food = fstore;
		ahead = hstore;
		bhead = pstore;
		ahead.p[0].y = 5;
		bhead.p[0].y = 15;
		const int BUF_SIZE = 64;



		WSADATA wsd; //WSADATA变量  
		SOCKET sHost; //服务器套接字  
		SOCKADDR_IN servAddr; //服务器地址  
		char buf[BUF_SIZE]; //接收数据缓冲区  
		char bufRecv[BUF_SIZE];
		int retVal; //返回值  
		//初始化套结字动态库  
		if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
		{
			cout << "WSAStartup failed!" << endl;
			return -1;
		}
		//创建套接字  
		sHost = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (INVALID_SOCKET == sHost)
		{
			cout << "socket failed!" << endl;
			WSACleanup();//释放套接字资源  
			return  -1;
		}

		//设置服务器地址  
		servAddr.sin_family = AF_INET;
		servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
		servAddr.sin_port = htons((short)4999);
		int nServAddlen = sizeof(servAddr);

		//连接服务器  
		retVal = connect(sHost, (LPSOCKADDR)&servAddr, sizeof(servAddr));
		if (SOCKET_ERROR == retVal)
		{
			cout << "connect failed!" << endl;
			closesocket(sHost); //关闭套接字  
			WSACleanup(); //释放套接字资源  
			return -1;
		}
		while (true)
		{
			//向服务器发送数据  
			ZeroMemory(buf, BUF_SIZE);
			cin >> buf;
			retVal = send(sHost, buf, strlen(buf), 0);
			if (SOCKET_ERROR == retVal)
			{
				cout << "send failed!" << endl;
				closesocket(sHost); //关闭套接字  
				WSACleanup(); //释放套接字资源  
				return -1;
			}
			//RecvLine(sHost, bufRecv);  
			ZeroMemory(bufRecv, BUF_SIZE);
			recv(sHost, bufRecv, BUF_SIZE, 0); // 接收服务器端的数据， 只接收5个字符  
			cout << endl << "从服务器接收数据：" << bufRecv;
			cout << "\n";
		}
		//退出  
		closesocket(sHost); //关闭套接字  
		WSACleanup(); //释放套接字资源  



		print2();
		while (ahead.alive && bhead.alive){
			if (!move2()){
				break;
			}
			if (!Food.fAlive)
				createFood2();
			system("cls");
			print2();
			Sleep(500);
			ZeroMemory(bufRecv, BUF_SIZE);
			if (recv(sHost, bufRecv, BUF_SIZE, 0)){
				char key = bufRecv[0];
				if (key == 'd')
					bhead.way = 1;
				else if (key == 'w')
					bhead.way = 0;
				else if (key == 'a')
					bhead.way = 3;
				else if (key == 's')
					bhead.way = 2;
			}
			if (_kbhit()){
				ZeroMemory(buf, BUF_SIZE);
				char key = _getch();
				buf[0] = key;
				retVal = send(sHost, buf, strlen(buf), 0);
				if (SOCKET_ERROR == retVal)
				{
					cout << "send failed!" << endl;
					closesocket(sHost); //关闭套接字  
					WSACleanup(); //释放套接字资源  
					return -1;
				}

				if (key == 'd')
					ahead.way = 1;
				else if (key == 'w')
					ahead.way = 0;
				else if (key == 'a')
					ahead.way = 3;
				else if (key == 's')
					ahead.way = 2;
			}
			Sleep(2000);
			cout << endl;
			if (ahead.alive)
				cout << "A:大吉大利，今晚吃鸡" << endl;
			else
				cout << "B:大吉大利，今晚吃鸡" << endl;
			cout << "TRY AGAIN Y/N";
			char a;
			cin >> a;
			if (a == 'y' || a == 'Y')
				goto begin2;
		}
	}
	else{//单人模式
		food fstore = Food;
		snakehead hstore = ahead;
begin:	Food = fstore;
		ahead = hstore;
		createFood();
		print();
		while (ahead.alive){
			if (!move()){
				break;
			}
			if (!Food.fAlive)
				createFood();
			system("cls");
			print();
			Sleep(500);
			if (_kbhit()){
				char key = _getch();
				if (key == 'd')
					ahead.way = 1;
				else if (key == 'w')
					ahead.way = 0;
				else if (key == 'a')
					ahead.way = 3;
				else if (key == 's')
					ahead.way = 2;
			}
		}
		Sleep(2000);
		cout << endl;
		cout << "请重头来过" << endl;
		cout << "TRY AGAIN Y/N";
		char a;
		cin >> a;
		if (a == 'y' || a == 'Y')
			goto begin;
	}
}
void createFood(){
	Food.fAlive = 1;
	while (true){
		int a = rand() % 20;
		int b = rand() % 20;
		int flag = 0;
		for (int i = 0; i < ahead.num; i++){
			if (a == ahead.p[i].x && b == ahead.p[i].y)
				flag = 1;
		}
		if (flag == 0){
			Food.xpos = a;
			Food.ypos = b;
			return;
		}
	}
}
void createFood2(){
	Food.fAlive = 1;
	while (true){
		int a = rand() % 20;
		int b = rand() % 20;
		int flag = 0;
		for (int i = 0; i < ahead.num; i++){
			if (a == ahead.p[i].x && b == ahead.p[i].y){
				flag = 1;
				break;
			}
			if (a == bhead.p[i].x && b == bhead.p[i].y){
				flag = 1;
				break;
			}
		}
		if (flag == 0){
			Food.xpos = a;
			Food.ypos = b;
			return;
		}
	}
}
int move(){
	pos temp = ahead.p[0];

	switch (ahead.way){
	case 0:ahead.p[0].x--;
		break;
	case 1:ahead.p[0].y++;
		break;
	case 2:ahead.p[0].x++;
		break;
	case 3:ahead.p[0].y--;
		break;
	}
	if (ahead.p[0].x == Food.xpos && ahead.p[0].y == Food.ypos){
		for (int i = ahead.num - 1; i > 0; i--){
			ahead.p[i + 1] = ahead.p[i];
		}
		ahead.p[1] = temp;
		Food.fAlive = 0;
		ahead.num++;
	}
	else{
		for (int i = ahead.num - 2; i > 0; i--){
			ahead.p[i + 1] = ahead.p[i];
		}
		ahead.p[1] = temp;
	}
	if (ahead.p[0].x == -1 || ahead.p[0].x == 20 || ahead.p[0].y == -1 || ahead.p[0].y == 20)
		return 0;
	return 1;
}
int move2(){
	pos temp = ahead.p[0];
	pos tempb = bhead.p[0];
	switch (ahead.way){
	case 0:ahead.p[0].x--;
		break;
	case 1:ahead.p[0].y++;
		break;
	case 2:ahead.p[0].x++;
		break;
	case 3:ahead.p[0].y--;
		break;
	}
	switch (bhead.way){
	case 0:bhead.p[0].x--;
		break;
	case 1:bhead.p[0].y++;
		break;
	case 2:bhead.p[0].x++;
		break;
	case 3:bhead.p[0].y--;
		break;
	}
	if (ahead.p[0].x == Food.xpos && ahead.p[0].y == Food.ypos){
		for (int i = ahead.num - 1; i > 0; i--){
			ahead.p[i + 1] = ahead.p[i];
		}
		ahead.p[1] = temp;
		Food.fAlive = 0;
		ahead.num++;
	}
	else{
		for (int i = ahead.num - 2; i > 0; i--){
			ahead.p[i + 1] = ahead.p[i];
		}
		ahead.p[1] = temp;
	}
	if (ahead.p[0].x == -1 || ahead.p[0].x == 20 || ahead.p[0].y == -1 || ahead.p[0].y == 20){
		ahead.alive = 0;
		return 0;
	}
	if (bhead.p[0].x == Food.xpos && bhead.p[0].y == Food.ypos){
		for (int i = bhead.num - 1; i > 0; i--){
			bhead.p[i + 1] = bhead.p[i];
		}
		bhead.p[1] = temp;
		Food.fAlive = 0;
		bhead.num++;
	}
	else{
		for (int i = bhead.num - 2; i > 0; i--){
			bhead.p[i + 1] = bhead.p[i];
		}
		bhead.p[1] = temp;
	}
	if (bhead.p[0].x == -1 || bhead.p[0].x == 20 || bhead.p[0].y == -1 || bhead.p[0].y == 20){
		bhead.alive = 0;
		return 0;
	}
	for (int i = 1; i < ahead.num; i++){
		if (bhead.p[0].x == ahead.p[i].x && bhead.p[0].y == ahead.p[i].y){
			bhead.alive = 0;
			return 0;
		}
	}
	for (int i = 1; i < bhead.num; i++){
		if (ahead.p[0].x == bhead.p[i].x && ahead.p[0].y == bhead.p[i].y){
			ahead.alive = 0;
			return 0;
		}
	}
	return 1;
}
void print(){
	for (int i = 0; i < 22; i++)
		cout << "-";
	cout << endl;
	for (int i = 0; i < 20; i++){
		cout << '|';
		for (int j = 0; j < 20; j++){
			for (int k = 0; k < ahead.num; k++){
				if (ahead.p[k].x == i && ahead.p[k].y == j){
					cout << '*';
					goto k;
				}
			}
			if ((Food.xpos == i && Food.ypos == j))
				cout << '$';
			else
				cout << ' ';
		k:;
		}
		cout << '|';
		cout << endl;
	}
	for (int i = 0; i < 22; i++)
		cout << "-";
}
void print2(){
	for (int i = 0; i < 22; i++)
		cout << "-";
	cout << endl;
	for (int i = 0; i < 20; i++){
		cout << '|';
		for (int j = 0; j < 20; j++){
			for (int k = 0; k < ahead.num; k++){
				if (ahead.p[k].x == i && ahead.p[k].y == j){
					cout << '*';
					goto k;
				}
				if (bhead.p[k].x == i && bhead.p[k].y == j){
					cout << '#';
					goto k;
				}
			}
			if ((Food.xpos == i && Food.ypos == j))
				cout << '$';
			else
				cout << ' ';
		k:;
		}
		cout << '|';
		cout << endl;
	}
	for (int i = 0; i < 22; i++)
		cout << "-";
}
