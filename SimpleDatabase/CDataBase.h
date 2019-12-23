#pragma once
#include <fstream>
#include <vector>

template<typename TSource>
void DBInMemSort(const char* sourceFile)
{
    std::ifstream fin(sourceFile, std::ios::binary);
    std::vector<TSource> v;
    TSource record;
    while (fin >> record)
        v.push_back(record);
    fin.close();
    sort(v.begin(), v.end());
    std::ofstream fout(sourceFile, std::ios::binary);
    for (int i = 0; i < v.size(); i++)
        fout << v[i];
}

template<typename TSource>
void DBExternalSort(const char* sourceFile) // TODO: ExternalSort
{
    //cerr << "DBExternalSort not implemented, using DBInMemSort instead";
    //DBInMemSort<TSource>(sourceFile);
    std::fstream file(sourceFile, std::fstream::in | std::fstream::out | std::ios::binary);
    std::cout << file.is_open();
    file.seekg(0, file.end);
    int size = file.tellg() / sizeof(TSource);
    file.clear();
    file.seekg(0, file.beg);
    TSource a, b;
    for (int i = 0; i < size; i++)
    {
        file.seekg((long long)i * sizeof(TSource));
        for (int j = i + 1; j < size; j++)
        {
            file.seekg(((long long)j - 1) * sizeof(TSource));
            file >> a >> b;
            if (b < a)
            {
                file.seekg(((long long)j - 1) * sizeof(TSource));
                file << b << a;
            }
        }
    }
}

template<typename TSource, typename TJoin, typename TResult, typename TCmp>
void DBJoinSorted(const char* sourceFile, const char* joinFile, const char* resultFile, const TCmp& eq = std::equal<TSource>())
{
    std::ifstream fS(sourceFile, std::ios::binary);
    std::ifstream fJ(joinFile, std::ios::binary);
    std::ofstream fout(resultFile, std::ios::binary);
    TSource source;
    TJoin join;
    fJ >> join;
    while (fS >> source)
    {
        while (!eq(source, join))
            fJ >> join;
        fout << TResult(source, join);
    }
}

template<typename TSource, typename TPred>
void DBSelect(const char* sourceFile, const char* resultFile, TPred pred)
{
    std::ifstream fin(sourceFile, std::ios::binary);
    std::ofstream fout(resultFile, std::ios::binary);
    TSource record;
    while (fin >> record)
    {
        if (pred(record))
            fout << record;
    }
}

template<typename TSource, typename TPred>
std::vector<TSource> DBSelectVec(const char* sourceFile, const char* resultFile, TPred pred)
{
    std::ifstream fin(sourceFile, std::ios::binary);
    std::vector<TSource> v;
    TSource record;
    while (fin >> record)
    {
        if (pred(record))
            v.push_back(record);
    }
    return v;
}