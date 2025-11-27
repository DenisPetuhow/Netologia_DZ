#include <iostream>
#include <string>

// ≈динственности ответственнойсти 
using namespace std;

class MyData // ƒанные которые должны быть сохранены 
{
    string data;
public:
    MyData(string _data) : data(_data) {}
    string GetName() { return data; }
};

class SaveDataDB
{
    MyData* m_data;
public:
    SaveDataDB(MyData* data) : m_data(data) {}
    void Save()
    {
        cout << "Save to Data Base" << endl;
    }
};

class SaveDataJSON
{
    MyData* m_data;
public:
    SaveDataJSON(MyData* data) : m_data(data) {}
    void Save()
    {
        cout << "Save to JSON file " << m_data->GetName() << endl;
    }
};

int main1()
{
    MyData data("My Data");
    SaveDataDB saver1(&data);
    saver1.Save();
    return 0;
}