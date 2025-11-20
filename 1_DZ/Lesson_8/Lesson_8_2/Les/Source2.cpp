#include <map>
#include <string>
#include <iostream>
#include <memory>

class VeryHeavyDatabase {
public:
    virtual std::string GetData(const std::string& key) {
        return "Very Big Data String: " + key;
    }
};

class CacheProxyDB : VeryHeavyDatabase {
public:
    explicit CacheProxyDB(std::unique_ptr<VeryHeavyDatabase> real_object) : real_db_(move(real_object)) {}
    std::string GetData(const std::string& key)  override {
        if (cache_.find(key) == cache_.end()) { // если нет ключа перебрасывает в конец списка
            std::cout << "Get from real object\n";
            cache_[key] = real_db_->GetData(key);
        }
        else {
            std::cout << "Get from cache\n";
        }
        return cache_.at(key); // вернуть результат
    }
private:
    std::map<std::string, std::string> cache_;
    std::unique_ptr<VeryHeavyDatabase> real_db_; //уникальное владение, возможно только переместить, чтобы забрать владение к себе 
};


class TestDB : VeryHeavyDatabase {
public:
    explicit TestDB(std::unique_ptr<VeryHeavyDatabase> real_object) : real_db_(move(real_object)) {}
    std::string GetData(const std::string& key) override {
        return "test_data\n";
    }
private:
    std::unique_ptr<VeryHeavyDatabase> real_db_;
};



std::unique_ptr< CacheProxyDB> init_cach() {
    auto original_db = std::make_unique<VeryHeavyDatabase>();
    return std::make_unique<CacheProxyDB>(move(original_db));//соединение кеш Ѕƒ с основной
}

int main1() {

    auto DB = init_cach();
    std::cout << DB->GetData("key1") << std::endl;
    std::cout << DB->GetData("key1") << std::endl;
    std::cout << DB->GetData("key1") << std::endl;


    auto test_db = std::make_unique<TestDB>(move(std::make_unique<VeryHeavyDatabase>()));
    std::cout << test_db->GetData("key1") << std::endl;
    std::cout << test_db->GetData("key1") << std::endl;
    return 0;
}