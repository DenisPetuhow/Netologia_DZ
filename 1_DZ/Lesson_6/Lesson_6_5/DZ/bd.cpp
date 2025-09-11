#include <iostream>
#include <pqxx/pqxx>

int main() {
	std::cout << "hello" << std::endl;

	try {
		pqxx::connection bd(
			"host=localhost"
			"port=5432"
			"dname=DZ_6_5"
			"user=DZ_6_5_user"
			"password=my_password_12345678"
		);
	}
	catch (pqxx::sql_error e) {
		std::cout << e.what() << std::endl;
	}


	return 0;
}