/**
 * Лабораторная работа 4
 *
 * Вариант 4.
 * Разработать классы для описанных ниже объектов. Включить в класс методы
 * set (...), get (...), show (...). Определить другие методы.
 * Phone: Фамилия, Имя, Отчество, Адрес, Номер, Время внутригородских разговоров,\
 *
 * Время междугородних разговоров. Создать массив объектов. Вывести:
 * а) сведения об абонентах, время внутригородских разговоров которых превышает заданное;
 * б) сведения об абонентах, воспользовавшихся междугородней связью;
 * в) сведения об абонентах, выведенные в алфавитном порядке.
 */

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <locale>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

constexpr const char* RED = "\033[31m";
constexpr const char* GREEN = "\033[32m";
constexpr const char* BLUE = "\033[34m";
constexpr const char* CYAN = "\033[36m";
constexpr const char* RESET = "\033[0m";

constexpr const char* LABEL_LASTNAME = "Фамилия:";
constexpr const char* LABEL_FIRSTNAME = "Имя:";
constexpr const char* LABEL_MIDDLENAME = "Отчество:";
constexpr const char* LABEL_ADDRESS = "Адрес:";
constexpr const char* LABEL_PHONE = "Телефон:";
constexpr const char* LABEL_INTRACITY = "Внутрегородские разговоры:";
constexpr const char* LABEL_LONGDIST = "Междугородние разговоры:";

class Phone {
   private:
    string lastName{};
    string firstName{};
    string middleName{};
    string address{};
    string phoneNumber{};
    double intracityCalltime = 0.0;
    double longDistCalltime = 0.0;

   public:
    Phone() = default;

    Phone(const string& ln, const string& fn, const string& mn, const string& addr,
          const string& phone, double intra, double ld)
        : lastName(ln),
          firstName(fn),
          middleName(mn),
          address(addr),
          phoneNumber(phone),
          intracityCalltime(intra),
          longDistCalltime(ld) {}

    const string& getLastName() const { return lastName; }
    const string& getFirstName() const { return firstName; }
    const string& getMiddleName() const { return middleName; }
    const string& getAddress() const { return address; }
    const string& getPhoneNumber() const { return phoneNumber; }
    double getIntracityCalltime() const { return intracityCalltime; }
    double getLongDistCalltime() const { return longDistCalltime; }

    void setLastName(const string& ln) { lastName = ln; }
    void setFirstName(const string& fn) { firstName = fn; }
    void setMiddleName(const string& mn) { middleName = mn; }
    void setAddress(const string& addr) { address = addr; }
    void setPhoneNumber(const string& p) { phoneNumber = p; }
    void setIntracityCalltime(double t) { intracityCalltime = t; }
    void setLongDistCalltime(double t) { longDistCalltime = t; }

    friend ostream& operator<<(ostream& os, const Phone& p);
};

ostream& operator<<(ostream& os, const Phone& p) {
    os << left << LABEL_LASTNAME << " " << p.lastName << '\n'
       << LABEL_FIRSTNAME << " " << p.firstName << '\n'
       << LABEL_MIDDLENAME << " " << p.middleName << '\n'
       << LABEL_ADDRESS << " " << p.address << '\n'
       << LABEL_PHONE << " " << p.phoneNumber << '\n'
       << LABEL_INTRACITY << " " << p.intracityCalltime << '\n'
       << LABEL_LONGDIST << " " << p.longDistCalltime << endl;

    return os;
};

class FormattedPhoneList {
   private:
    vector<Phone> phones;

   public:
    FormattedPhoneList() = default;

    FormattedPhoneList(const vector<Phone>& phones_) : phones(phones_) {}

    void show() const {
        for (const auto& p : phones) {
            cout << p << "\n";
        }
    }
};

enum SortOrdering { ASC = 1, DESC = 2 };

class Phones {
   private:
    vector<Phone> phones;

   public:
    Phones(const vector<Phone>& _phones) : phones(_phones) {}

    void setSourcePhonesList(vector<Phone>& _phones) { phones = _phones; }

    FormattedPhoneList getPhonesByIntracityCalltime(double val = 0.0) const {
        vector<Phone> filtered;

        copy_if(phones.begin(), phones.end(), back_inserter(filtered),
                [val](const Phone& p) { return p.getIntracityCalltime() > val; });

        return FormattedPhoneList(filtered);
    }

    FormattedPhoneList getPhonesByLongDistCalltime(double val = 0.0) const {
        vector<Phone> filtered;

        copy_if(phones.begin(), phones.end(), back_inserter(filtered),
                [val](const Phone& p) { return p.getLongDistCalltime() > val; });

        return FormattedPhoneList(filtered);
    }

    FormattedPhoneList getSortedPhonesList(SortOrdering order = SortOrdering::ASC) const {
        vector<Phone> _phones = phones;

        sort(_phones.begin(), _phones.end(), [order](const Phone& p1, const Phone& p2) {
            auto t1 = tie(p1.getLastName(), p1.getFirstName(), p1.getMiddleName());
            auto t2 = tie(p2.getLastName(), p2.getFirstName(), p2.getMiddleName());

            if (order == SortOrdering::ASC) {
                return t1 < t2;
            } else {
                return t1 > t2;
            }
        });

        return FormattedPhoneList(_phones);
    }

    ~Phones() = default;
};

int main(void) {
    setlocale(LC_ALL, "Russian");

    Phones p(
        {Phone("Vadim", "Nizamov", "Filaritovich", "Ufa, Bekhtereva", "+7 (999) 123-45-67", 123.45,
               0),
         Phone({"Petrov", "Peter", "Petrovich", "Saint Petersburg", "+7 (998) 123-45-67", 95.0, 0}),
         Phone({"Sidorov", "Sidr", "Sidorovich", "Moscow", "+7 (997) 123-45-67", 88.8, 63.1})});

    /*
        [А]. Сначала получаем сведения об абонентах, время внутригородских
        разговоров которых превышает заданное, а потом формируем список PhoneList
    */
    cout << GREEN << "Абоненты с длительностью внутрегородских вызвов > 94.56:" << RESET << "\n\n";
    p.getPhonesByIntracityCalltime(94.56).show();

    /*
        [B]. Сначала получаем сведения об абонентах, воспользовавшихся междугородней
        связью, а потом формируем список PhoneList
    */
    cout << GREEN << "Абоненты, которые воспользовались междугородней связью:" << RESET << "\n\n";
    p.getPhonesByLongDistCalltime().show();

    /*
        [C]. Сначала получаем сведения об абонентах, выведенные в алфавитном порядке,
        а потом формируем список PhoneList
    */
    cout << GREEN << "Все абоненты в алфавитном порядке:" << RESET << "\n\n";
    p.getSortedPhonesList().show();

    return 0;
}
