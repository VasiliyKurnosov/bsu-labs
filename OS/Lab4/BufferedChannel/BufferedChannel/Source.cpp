#include <iostream>
#include <thread>
#include <chrono>
#include "BufferedChannel.h"

BufferedChannel<int> buffered_channel(3);

void SendToChannel(int value);
void RecieveFromChannel();
void CloseChannel();
void ThreadFunction1();
void ThreadFunction2();
void ThreadFunction3();

int main()
{
	std::thread thread1(ThreadFunction1);
	std::thread thread2(ThreadFunction2);
	std::thread thread3(ThreadFunction3);

	thread1.join();
	thread2.join();
	thread3.join();

	return 0;
}

void SendToChannel(int value)
{
	try
	{
		buffered_channel.Send(value);
		std::cout << "Added " << value << std::endl;
	}
	catch (std::runtime_error e)
	{
		std::cout << e.what() << std::endl;
	}
}

void RecieveFromChannel()
{
	auto element = buffered_channel.Recv();
	if (element.second == true)
	{
		std::cout << "Removed " << element.first << std::endl;
	}
	else
	{
		std::cout << "Channel is empty" << std::endl;
	}
}

void CloseChannel()
{
	buffered_channel.Close();
	std::cout << "Closed" << std::endl;
}

void ThreadFunction1()
{
	SendToChannel(1);
	SendToChannel(2);
	SendToChannel(3);
	SendToChannel(4);
	SendToChannel(5);
	std::this_thread::sleep_for(std::chrono::seconds(7));
	SendToChannel(10);
	RecieveFromChannel();
	RecieveFromChannel();
	RecieveFromChannel();
	RecieveFromChannel();
	RecieveFromChannel();
}

void ThreadFunction2()
{
	std::this_thread::sleep_for(std::chrono::seconds(1));
	RecieveFromChannel();
	std::this_thread::sleep_for(std::chrono::seconds(1));
	RecieveFromChannel();
	RecieveFromChannel();
	SendToChannel(6);
	SendToChannel(7);
	SendToChannel(8);
}

void ThreadFunction3()
{
	std::this_thread::sleep_for(std::chrono::seconds(5));
	CloseChannel();
	SendToChannel(9);
}
