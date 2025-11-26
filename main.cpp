#include <iostream>
#include <chrono>
#include "src/indexer/Indexer.hpp"
#include "src/query/QueryProcessor.hpp"

using namespace std;
using namespace std::chrono;

int main() {
    cout << "Мой поисковый движок v1.0 (с BM25)\n";
    cout << "=====================================\n\n";

    Indexer indexer;
    QueryProcessor* qp = nullptr;

    const string indexDir = "data/index";
    const string docsDir = "data/documents";

    if (IndexSerializer::loadFromJson(indexer.getInvertedIndex(), indexDir)) {
        cout << "Индекс успешно загружен за ";
        timer.printElapsed();
        cout << "Документов: " << indexer.getTotalDocuments() << "\n";
        cout << "Терминов: " << indexer.getTotalTerms() << "\n\n";
    } else {
        cout << "Индекс не найден. Начинаем индексацию папки:\n   " << docsDir << "\n\n";

        timer.start();
        indexer.buildIndexFromDirectory(docsDir);
        cout << "Индексация завершена за ";

        cout << "Сохраняем индекс в " << indexDir << "... ";
        if (IndexSerializer::saveToJson(indexer.getInvertedIndex(), indexDir)) {
            cout << "OK!\n\n";
        } else {
            cout << "ОШИБКА сохранения!\n\n";
        }
    }

    qp = new QueryProcessor(indexer.getInvertedIndex());

    cout << "Готов к поиску! Вводите запросы (или 'exit' для выхода):\n";
    cout << "Поддерживается: кот AND собака, машина OR дом, просто \"кот\"\n\n";

    string query;
    while (true) {
        cout << "> ";
        if (!getline(cin, query) || query == "exit" || query == "quit") {
            break;
        }

        if (query.empty()) continue;

        timer.start();
        auto results = qp->search(query, 10);

        cout << "Найдено: " << results.size() << " результатов за ";
        timer.printElapsed();

        if (results.empty()) {
            cout << "   Ничего не найдено.\n\n";
            continue;
        }

        cout << "\n";
        for (size_t i = 0; i < results.size(); ++i) {
            const auto& r = results[i];
            cout << setw(2) << (i+1) << ". ";
            cout << fixed << setprecision(4) << r.score << " | ";
            cout << r.title;
            if (r.path != r.title) {
                cout << "  (" << r.path << ")";
            }
            cout << "\n";
        }
        cout << "\n";
    }

    delete qp;
    cout << "До свидания!\n";
    return 0;
}