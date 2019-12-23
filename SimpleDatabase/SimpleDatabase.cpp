#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <sstream>
#include <stdint.h>
//#include <time.h>
#include <windows.h>
#include <iomanip>
#include <vector>
#include <algorithm>
#include "CDataBase.h"

using namespace std;

struct Time
{
    uint8_t day, month;
    uint16_t year;
    Time() {}
    Time(uint8_t day, uint8_t month, uint16_t year)
        : day(day), month(month), year(year) {}
};

/*
struct BasicFileRecord
{
    virtual int Size() = 0;
};//*/

struct FileRecordOperation
{
    Time date;
    int id;
    char str[40];
    int code, score, sum;
    friend ostream& operator<<(ostream& out, const FileRecordOperation& record)
    {
        return out.write((char*)(&record), sizeof(record));
    }
    friend istream& operator>>(istream& in, FileRecordOperation& record)
    {
        return in.read((char*)(&record), sizeof(record));
    }
    bool operator<(const FileRecordOperation& other)
    {
        return code < other.code;
    }
};

struct FileRecordCustomer
{
    int code;
    char str[30];
    friend ostream& operator<<(ostream& out, const FileRecordCustomer& record)
    {
        return out.write((char*)(&record), sizeof(record));
    }
    friend istream& operator>>(istream& in, FileRecordCustomer& record)
    {
        return in.read((char*)(&record), sizeof(record));
    }
    bool operator<(const FileRecordCustomer& other)
    {
        return code < other.code;
    }
};

struct FileRecordMixed
{
    Time date;
    int id;
    char str[40], customer[30];
    int score, sum;
    FileRecordMixed(){}
    FileRecordMixed(const FileRecordOperation& operation, const FileRecordCustomer& customer)
        : date(operation.date), id(operation.id), score(operation.score), sum(operation.sum)
    {
        memcpy(str, operation.str, 40);
        memcpy(this->customer, customer.str, 30);
    }
    friend ostream& operator<<(ostream& out, const FileRecordMixed& record)
    {
        return out.write((char*)(&record), sizeof(record));
    }
    friend istream& operator>>(istream& in, FileRecordMixed& record)
    {
        return in.read((char*)(&record), sizeof(record));
    }
};

bool ConvertToBinaryOperation(const char* source, const char* result, char delim = ';')
{
    ifstream fin(source);
    ofstream fout(result, ios::binary);
    stringstream str, strdate;
    string line, s;
    char c;
    int d, m;
    FileRecordOperation record;

    while (getline(fin, line))
    {
        str.clear();
        str.str("");
        str << line;
        if (!getline(str, s, delim))
            return false;
        /// pasre data in str;
        strdate.clear();
        strdate.str(s);
        if (!(strdate >> d >> c >> m >> c >> record.date.year))
            return false;
        record.date.day = d;
        record.date.month = m;
        ///
        if (!getline(str, s, delim))
            return false;
        /// parse int (id)
        record.id = stoi(s);
        ///
        if (!getline(str, s, delim))
            return false;
        /// parse string
        while (s.length() > sizeof(record.str) - 1)
            s.pop_back();
        strcpy(record.str, s.data());
        ///
        if (!getline(str, s, delim))
            return false;
        /// parse int (code)
        record.code = stoi(s);
        ///
        if (!getline(str, s, delim))
            return false;
        /// parse int (score)
        record.score = stoi(s);
        ///
        if (!getline(str, s, delim))
            return false;
        /// parse int (sum)
        record.sum = stoi(s);
        ///

        ///
        fout << record;
        cout << record.id << ";" << record.str << ";" << record.score << ";" << record.sum << "\n";
    }
    return true;
}

bool ConvertToBinaryCustomer(const char* source, const char* result, char delim = ';')
{
    ifstream fin(source);
    ofstream fout(result, ios::binary);
    stringstream str;
    string line, s;
    FileRecordCustomer record;

    while (getline(fin, line))
    {
        str.clear();
        str.str("");
        str << line;
        if (!getline(str, s, delim))
            return false;
        /// parse int (code)
        record.code = stoi(s);
        ///
        if (!getline(str, s, delim))
            return false;
        /// parse string
        while (s.length() > sizeof(record.str) - 1)
            s.pop_back();
        strcpy(record.str, s.data());
        ///
//        fout.write( (char*)(&record), sizeof(record) );
        fout << record;
        cout << record.code << ";" << record.str << "\n";
    }
    return true;
}

bool ConvertFromBinaryOperation(const char* source, const char* result, char delim = ';')
{
    ifstream fin(source, ios::binary);
    ofstream fout(result);
    string line, s;
    FileRecordOperation record;

    //    while (fin.read((char*)(&record), sizeof(FileRecord)))
    while (fin >> record)
    {
        fout << setw(2) << setfill('0') << (int)record.date.day << "." << setw(2) << (int)record.date.month << "." << setw(4) << record.date.year << setw(0) << ";"
            << record.id << ";" << record.str << ";" << setw(2) << record.code << setw(0) << ";" << record.score << ";" << record.sum << "\n";
    }
    return true;
}

bool ConvertFromBinaryCustomer(const char* source, const char* result, char delim = ';')
{
    ifstream fin(source, ios::binary);
    ofstream fout(result);
    string line, s;
    FileRecordCustomer record;

    //    while (fin.read((char*)(&record), sizeof(FileRecord)))
    while (fin >> record)
    {
        fout << record.code << ";" << record.str << "\n";
    }
    return true;
}

bool ConvertFromBinaryMixed(const char* source, const char* result, char delim = ';')
{
    ifstream fin(source, ios::binary);
    ofstream fout(result);
    string line, s;
    FileRecordMixed record;

    //    while (fin.read((char*)(&record), sizeof(FileRecord)))
    while (fin >> record)
    {
        fout << setw(2) << setfill('0') << (int)record.date.day << "." << setw(2) << (int)record.date.month << "." << setw(4) << record.date.year << setw(0) << ";"
            << record.id << ";" << record.str << ";" << setw(2) << record.customer << setw(0) << ";" << record.score << ";" << record.sum << "\n";
    }
    return true;
}

void InFileSort(const char* source, const char* result) /// TODO: NOT READY IN MEM SORT REALIZED
{
    cerr << "NOT IMPLEMENTED InFileSort!\n";
    //    fstream file(source, ios::binary);
    ifstream fin(source, ios::binary);
    vector<FileRecordOperation> v;
    FileRecordOperation record;
    while (fin >> record)
        v.push_back(record);
    sort(v.begin(), v.end());
    ofstream fout(result, ios::binary);
    for (int i = 0; i < v.size(); i++)
        fout << v[i];
}

void InMemSort(const char* source, const char* result)
{
    ifstream fin(source, ios::binary);
    vector<FileRecordCustomer> v;
    FileRecordCustomer record;
    while (fin >> record)
        v.push_back(record);
    sort(v.begin(), v.end());
    ofstream fout(result, ios::binary);
    for (int i = 0; i < v.size(); i++)
        fout << v[i];
}

void MergeFiles(const char* source, const char* customers, const char* result)
{
    ifstream fin(source, ios::binary);
    ifstream cIn(customers, ios::binary);
    ofstream fout(result, ios::binary);
    FileRecordOperation record;
    FileRecordCustomer customer;
    cIn >> customer;
    while (fin >> record)
    {
        while (customer.code < record.code)
            cIn >> customer;
        fout << FileRecordMixed(record, customer);
    }
}

template <typename T>
vector<T> ReadBinaryRecords(const char* source)
{
    ifstream fin(source, ios::binary);
    vector<T> v;
    T record;
    while (fin >> record)
        v.push_back(record);
    return v;
}

template <typename T>
void WriteBinaryRecords(const char* result, const vector<T>& v)
{
    ofstream fout(result, ios::binary);
    for (const T& element : v)
        fout << element;
}

template<typename T, typename Pred>
vector<T> CopyVectorIF(const vector<T>& v, Pred pred)
{
    vector<T> ans;
    for (const T& element : v)
        if (pred(element))
            ans.push_back(element);
    return ans;
}

vector<FileRecordMixed> FindCustomerContains(const vector<FileRecordMixed>& v, const char* pattern)
{
    return CopyVectorIF(v, [pattern](const FileRecordMixed& record)
        {
            return strstr(record.customer, pattern) != NULL;
        });
}

vector<FileRecordMixed> FindSumInRange(const vector<FileRecordMixed>& v, int l, int r)
{
    return CopyVectorIF(v, [l, r](const FileRecordMixed& record)
        {
            return l <= record.sum && record.sum <= r;
        });
}

char COemToAnsi(char a)
{
    unsigned char c = a;
    if (c >= 128 && c < 176)
        c += 64;
    else if (c >= 224 && c < 240)
        c += 16;
    return c;
}

int main()
{
    //    int a,b;
    //    cin >> a >> b;
    //    cout << a << " " << b;
    //    return 0;
    setlocale(LC_ALL, "Russian");
    //    SetConsoleCP(1251);
    //    SetConsoleOutputCP(1251);
    ConvertToBinaryOperation("input.txt", "input.bin", '\t');
    ConvertToBinaryCustomer("customers.txt", "customers.bin", '\t');
    //InFileSort("input.bin", "input.sorted.bin");
    DBExternalSort<FileRecordOperation>("input.bin");
    //InMemSort("customers.bin", "customers.sorted.bin");
    DBInMemSort<FileRecordCustomer>("customers.bin");
    //MergeFiles("input.sorted.bin", "customers.sorted.bin", "mixed.bin");
    DBJoinSorted<FileRecordOperation, FileRecordCustomer, FileRecordMixed>("input.bin", "customers.bin", "mixed.bin", [](const FileRecordOperation& o, const FileRecordCustomer& c)
        {
            return o.code == c.code;
        });

    //vector<FileRecordMixed> v = ReadBinaryRecords<FileRecordMixed>("mixed.bin");
    char s[40];
    cout << "ВВедите фильтр substr customer.str: ";
    cin >> s;
    transform(s, s + 40, s, COemToAnsi);
    //WriteBinaryRecords("request.customer.bin", FindCustomerContains(v, s));
    ConvertFromBinaryMixed("request.customer.bin", "request.customer.txt", ';');
    DBSelect<FileRecordMixed>("mixed.bin", "request.customer.bin", [s](const FileRecordMixed& record)
        {
            return strstr(record.customer, s) != NULL;
        });
    int l, r;
    cout << "ВВедите фильтр l, r records.sum: ";
    cin >> l >> r;
    //WriteBinaryRecords("request.sum.bin", FindSumInRange(v, l, r));
    ConvertFromBinaryMixed("request.sum.bin", "request.sum.txt", ';');
    DBSelect<FileRecordMixed>("mixed.bin", "request.sum.bin", [l, r](const FileRecordMixed& record)
        {
            return l <= record.sum && record.sum <= r;
        });

    ConvertFromBinaryOperation("input.bin", "output.txt", ';');
    ConvertFromBinaryCustomer("customers.bin", "customers.out.txt", ';');
    ConvertFromBinaryMixed("mixed.bin", "mixed.txt", ';');
    return 0;
}