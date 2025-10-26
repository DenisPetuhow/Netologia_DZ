#include <iostream>
#include <chrono>
#include <thread>

int max_count { 10 };
int save { 0 };

void sumelement() {
	int i_count = 0;
	while (i_count < max_count) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
		i_count++;
		save++;
		std::cout << "Добавлен в очередь, сейчас стоит " << save << "посетителей, всего пришло" << i_count << std::endl;
	}
}

void downelement() {
	int i_count = 0;
	while (i_count < max_count) {
		std::this_thread::sleep_for(std::chrono::seconds(2));  //важно добавить до while
		if (save > 0) {
			
			i_count++;
			save--;
			std::cout << "убран из очередь, сейчас стоит " << save << "посетителей, всего обслужено" << i_count << std::endl;
		}
		else {
			std::cout << "Все посетители обслужены" << std::endl;
		}

	}
}

int main() {
	setlocale(LC_ALL, "Russian");

	std::cout << "=== Симуляция очереди клиентов ===" << std::endl;
	std::thread t1(sumelement);
	std::thread t2(downelement);
	t1.join();
	t2.join();
	std::cout << "коец программы" << std::endl;

}