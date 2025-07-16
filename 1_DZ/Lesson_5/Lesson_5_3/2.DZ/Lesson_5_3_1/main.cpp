#pragma once
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_session.hpp>
#include <iostream>

// Структура узла двусвязного списка
struct ListNode
{
public:
    ListNode(int value, ListNode* prev = nullptr, ListNode* next = nullptr)
        : value(value), prev(prev), next(next)
    {
        if (prev != nullptr) prev->next = this;
        if (next != nullptr) next->prev = this;
    }

public:
    int value;
    ListNode* prev;
    ListNode* next;
};

// Класс двусвязного списка
class List
{
public:
    List()
        : m_head(new ListNode(static_cast<int>(0))), m_size(0),
        m_tail(new ListNode(0, m_head))
    {
    }

    virtual ~List()
    {
        Clear();
        delete m_head;
        delete m_tail;
    }

    bool Empty() { return m_size == 0; }

    unsigned long Size() { return m_size; }

    void PushFront(int value)
    {
        new ListNode(value, m_head, m_head->next);
        ++m_size;
    }

    void PushBack(int value)
    {
        new ListNode(value, m_tail->prev, m_tail);
        ++m_size;
    }

    int PopFront()
    {
        if (Empty()) throw std::runtime_error("list is empty");
        auto node = extractPrev(m_head->next->next);
        int ret = node->value;
        delete node;
        return ret;
    }

    int PopBack()
    {
        if (Empty()) throw std::runtime_error("list is empty");
        auto node = extractPrev(m_tail);
        int ret = node->value;
        delete node;
        return ret;
    }

    void Clear()
    {
        auto current = m_head->next;
        while (current != m_tail)
        {
            current = current->next;
            delete extractPrev(current);
        }
    }

private:
    ListNode* extractPrev(ListNode* node)
    {
        auto target = node->prev;
        target->prev->next = target->next;
        target->next->prev = target->prev;
        --m_size;
        return target;
    }

private:
    ListNode* m_head;
    ListNode* m_tail;
    unsigned long m_size;
};

// Тесты для базовых функций двусвязного списка
TEST_CASE("Test basic List functions", "[List]")
{
    List list;

    SECTION("Empty function tests")
    {
        INFO("Testing Empty function on newly created list");

        // Проверяем, что свежесозданный список пустой
        REQUIRE(list.Empty() == true);

        // Добавляем элемент
        list.PushFront(5);
        INFO("After adding element to the front");
        REQUIRE(list.Empty() == false);

        // Удаляем элемент
        list.PopFront();
        INFO("After removing element from the front");
        REQUIRE(list.Empty() == true);
    }

    SECTION("Size function tests")
    {
        INFO("Testing Size function with various operations");

        // Проверяем размер пустого списка
        REQUIRE(list.Size() == 0);

        // Добавляем элементы в начало
        list.PushFront(1);
        INFO("After adding first element to front");
        REQUIRE(list.Size() == 1);

        list.PushFront(2);
        INFO("After adding second element to front");
        REQUIRE(list.Size() == 2);

        // Добавляем элементы в конец
        list.PushBack(3);
        INFO("After adding first element to back");
        REQUIRE(list.Size() == 3);

        list.PushBack(4);
        INFO("After adding second element to back");
        REQUIRE(list.Size() == 4);

        // Удаляем элементы
        list.PopFront();
        INFO("After removing element from front");
        REQUIRE(list.Size() == 3);

        list.PopBack();
        INFO("After removing element from back");
        REQUIRE(list.Size() == 2);
    }

    SECTION("Clear function tests")
    {
        INFO("Testing Clear function behavior");

        // Добавляем несколько элементов
        list.PushFront(1);
        list.PushFront(2);
        list.PushBack(3);
        list.PushBack(4);

        INFO("After adding 4 elements to the list");
        REQUIRE(list.Size() == 4);
        REQUIRE(list.Empty() == false);

        // Очищаем список
        list.Clear();

        INFO("After clearing the list");
        REQUIRE(list.Size() == 0);
        REQUIRE(list.Empty() == true);

        // Проверяем, что можно добавлять элементы после очистки
        list.PushFront(10);
        INFO("After adding element to cleared list");
        REQUIRE(list.Size() == 1);
        REQUIRE(list.Empty() == false);
    }
}

// Главная функция - запуск тестов
int main(int argc, char* argv[])
{
    return Catch::Session().run(argc, argv);
}