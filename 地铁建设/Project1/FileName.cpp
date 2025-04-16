#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <climits>
#include <fstream>
#include<conio.h>

const int INF = INT_MAX;

// �ߵĽṹ��
struct Edge {
    int u;
    int v;
    int weight;  // Ȩ��
    int cost;    // ����ɱ�
    int priority; // ���ȼ�����ֵԽ�����ȼ�Խ��
};

// ȷ������Ϊ������
int getPositiveInteger(const std::string& prompt) {
    int num;
    while (true) {
        std::cout << prompt;
        if (std::cin >> num && num > 0) {
            std::cin.ignore(); // ������뻺�����Ļ��з�
            return num;
        }
        else {
            std::cout << "����ӦΪ�����������������롣" << std::endl;
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n');
        }
    }
}

// ȷ������Ϊ�Ǹ�����
int getNonNegativeInteger(const std::string& prompt) {
    int num;
    while (true) {
        std::cout << prompt;
        if (std::cin >> num && num >= 0) {
            std::cin.ignore(); // ������뻺�����Ļ��з�
            return num;
        }
        else {
            std::cout << "����ӦΪ�Ǹ����������������롣" << std::endl;
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n');
        }
    }
}

// ��ȡ������Ϣ
std::vector<std::pair<std::string, std::string> > readDistricts(int n, std::map<std::string, int>& code_to_idx) {
    std::vector<std::pair<std::string, std::string> > districts(n);
    std::cout << "���������ر�ź����ƣ��ÿո�ָ�\n";
    for (int i = 0; i < n; ++i) {
        std::string line;
        std::getline(std::cin, line);
        std::istringstream iss(line);
        std::string code, name;
        if (iss >> code) {
            std::getline(iss, name); // ��ȡʣ�ಿ����Ϊ����
            name = name.substr(1); // ȥ��ǰ��Ŀո�
            districts[i] = std::make_pair(code, name);
            code_to_idx[code] = i;
        }
        else {
            std::cout << "�����������Ϣ��ʽ��������������������Ϣ��" << std::endl;
            --i;
        }
    }
    return districts;
}

// ��ȡ����Ϣ
void readEdges(int m, std::vector<std::vector<Edge> >& adj_matrix, const std::map<std::string, int>& code_to_idx) {
    std::cout << "������һ���ߵ��������ر�ţ������ߵ�Ȩ�أ�����ɱ������ȼ����ÿո�ָ���\n";
    for (int i = 0; i < m; ++i) {
        std::string line;
        std::getline(std::cin, line);
        std::istringstream iss(line);
        std::string u_code, v_code;
        int weight, cost, priority;
        if (iss >> u_code >> v_code >> weight >> cost >> priority) {
            if (code_to_idx.find(u_code) != code_to_idx.end() &&
                code_to_idx.find(v_code) != code_to_idx.end()) {
                int u = code_to_idx.find(u_code)->second;
                int v = code_to_idx.find(v_code)->second;
                adj_matrix[u][v].u = u;
                adj_matrix[u][v].v = v;
                adj_matrix[u][v].weight = weight;
                adj_matrix[u][v].cost = cost;
                adj_matrix[u][v].priority = priority;
                adj_matrix[v][u] = adj_matrix[u][v];
            }
            else {
                std::cout << "��������ر�Ų����ڣ��������������Ϣ��" << std::endl;
                --i;
            }
        }
        else {
            std::cout << "����ı���Ϣ��ʽ�����������������Ϣ��" << std::endl;
            --i;
        }
    }
}

// ��ӡ�ڽӱ�
void printAdjacencyList(const std::vector<std::pair<std::string, std::string> >& districts, const std::vector<std::vector<Edge> >& adj_matrix) {
    std::cout << "\n���ɵ��ڽӱ�����" << std::endl;
    int n = districts.size();
    for (int i = 0; i < n; ++i) {
        std::cout << districts[i].second << "(" << districts[i].first << ") -> ";
        bool first = true;
        for (int j = 0; j < n; ++j) {
            if (i != j && adj_matrix[i][j].weight != INF) {
                if (!first) std::cout << " -> ";
                std::cout << districts[j].second << "(" << districts[j].first << ", "
                    << "Ȩ��: " << adj_matrix[i][j].weight << ", "
                    << "�ɱ�: " << adj_matrix[i][j].cost << ", "
                    << "���ȼ�: " << adj_matrix[i][j].priority << ")";
                first = false;
            }
        }
        std::cout << std::endl;
    }
}

// Prim�㷨
std::vector<Edge> prim(const std::vector<std::vector<Edge> >& adj_matrix, int start_idx, int& totalCost) {
    int n = adj_matrix.size();
    std::vector<bool> selected(n, false);
    std::vector<int> key(n, INF);
    std::vector<int> parent(n, -1);
    std::vector<int> costSum(n, INF);
    std::vector<int> prioritySum(n, 0);
    key[start_idx] = 0;
    costSum[start_idx] = 0;
    int count = 0;
    for (; count < n; count++) {
        // Ѱ����Сkey��ͬʱ�������ȼ�
        int u = -1;
        int min_key = INF;
        for (int i = 0; i < n; ++i) {
            if (!selected[i] && key[i] < min_key) {
                min_key = key[i];
                u = i;
            }
        }

        if (u == -1) break; // ͼ����ͨ
        selected[u] = true;

        // �����ڽӽڵ�
        for (int v = 0; v < n; ++v) {
            if (adj_matrix[u][v].weight != INF && !selected[v]) {
                int newWeight = key[u] + adj_matrix[u][v].weight;
                int newCost = costSum[u] + adj_matrix[u][v].cost;
                int newPriority = prioritySum[u] + adj_matrix[u][v].priority;
                if (newWeight < key[v] ||
                    (newWeight == key[v] && newPriority > prioritySum[v])) {
                    key[v] = newWeight;
                    parent[v] = u;
                    costSum[v] = newCost;
                    prioritySum[v] = newPriority;
                }
            }
        }
    }

    if (count < n) {
        std::cout << "ͼ����ͨ���޷�������С��������" << std::endl;
        return std::vector<Edge>();
    }

    // �ռ����
    std::vector<Edge> result;
    totalCost = 0;
    for (int v = 0; v < n; ++v) {
        if (v != start_idx && parent[v] != -1) {
            result.push_back(adj_matrix[parent[v]][v]);
            totalCost += adj_matrix[parent[v]][v].cost;
        }
    }
    return result;
}

// ��ӡ���
void printResult(const std::vector<std::pair<std::string, std::string> >& districts, const std::vector<Edge>& result, int totalCost) {
    std::cout << "\nӦ�޽�����С������·�����£�" << std::endl;
    for (std::vector<Edge>::const_iterator it = result.begin(); it != result.end(); ++it) {
        std::cout << districts[it->u].second << "(" << districts[it->u].first << ") - "
            << districts[it->v].second << "(" << districts[it->v].first << ") "
            << "Ȩ�أ�" << it->weight << ", �ɱ���" << it->cost << ", ���ȼ���" << it->priority << std::endl;
    }
    std::cout << "�ܽ���ɱ�Ϊ��" << totalCost << std::endl;
}

// ������ʷ��¼
void saveHistory(const std::vector<std::pair<std::string, std::string> >& districts, const std::vector<Edge>& result, int totalCost) {
    std::ofstream historyFile("history.txt", std::ios::app);
    if (historyFile.is_open()) {
        historyFile << "Ӧ�޽�����С������·�����£�" << std::endl;
        for (std::vector<Edge>::const_iterator it = result.begin(); it != result.end(); ++it) {
            historyFile << districts[it->u].second << "(" << districts[it->u].first << ") - "
                << districts[it->v].second << "(" << districts[it->v].first << ") "
                << "Ȩ�أ�" << it->weight << ", �ɱ���" << it->cost << ", ���ȼ���" << it->priority << std::endl;
        }
        historyFile << "�ܽ���ɱ�Ϊ��" << totalCost << std::endl;
        historyFile << "------------------------" << std::endl;
        historyFile.close();
    }
    else {
        std::cout << "�޷�����ʷ��¼�ļ���" << std::endl;
    }
}

// �鿴��ʷ��¼
void viewHistory() {
    std::ifstream historyFile("history.txt");
    if (historyFile.is_open()) {
        std::string line;
        while (std::getline(historyFile, line)) {
            std::cout << line << std::endl;
        }
        historyFile.close();
    }
    else {
        std::cout << "û����ʷ��¼���޷�����ʷ��¼�ļ���" << std::endl;
    }
}

// ��ʾ�˵�
void showMenu() {
    std::cout << "\n\n\n";
    std::cout << "\t\t\t\t    ******************************************************\n";
    std::cout << "\t\t\t\t    *----------------------�˵�------------------------*\n";
    std::cout << "\t\t\t\t    *----------------------------------------------------*\n";
    std::cout << "\t\t\t\t    *----------------1. �����µ����غͱ���Ϣ-----------*\n";
    std::cout << "\t\t\t\t    *----------------------------------------------------*\n";
    std::cout << "\t\t\t\t    *----------------2. ������С������·��-------------*\n";
    std::cout << "\t\t\t\t    *----------------------------------------------------*\n";
    std::cout << "\t\t\t\t    *----------------3. �鿴��ʷ��¼-------------------*\n";
    std::cout << "\t\t\t\t    *----------------------------------------------------*\n";
    std::cout << "\t\t\t\t    *----------------4. �˳�---------------------------*\n";
    std::cout << "\t\t\t\t    *----------------------------------------------------*\n";
    std::cout << "\t\t\t\t    ******************************************************\n";
    std::cout << "���������ѡ��";
}

int main() {
    // ���ÿ���̨���ڴ�С
    system("mode con cols=140 lines=200");

    std::vector<std::pair<std::string, std::string> > districts;
    std::map<std::string, int> code_to_idx;
    std::vector<std::vector<Edge> > adj_matrix;
    int start_idx = -1;

    int choice;
    while (true) {
   
        showMenu();
        while(true)
        if (_kbhit() != 0) {
            choice = _getch();
            if (choice >= 49 && choice <= 52)
            {
                std::cout << choice - 48<<std::endl;
                break;
            }
            else std::cout << "������Ч��������1 - 4֮���������" << std::endl;

            
        }
        if (choice >= 49 && choice <= 52) {
            switch (choice-48) {
            case 1: {
                // �����µ����غͱ���Ϣ
         
                int n = getPositiveInteger("���������ص�������");
                districts = readDistricts(n, code_to_idx);

                // ��ʼ���ڽӾ���
                adj_matrix = std::vector<std::vector<Edge> >(n, std::vector<Edge>(n));
                for (int i = 0; i < n; ++i) {
                    for (int j = 0; j < n; ++j) {
                        adj_matrix[i][j].u = 0;
                        adj_matrix[i][j].v = 0;
                        adj_matrix[i][j].weight = INF;
                        adj_matrix[i][j].cost = INF;
                        adj_matrix[i][j].priority = 0;
                    }
                }

                int m = getNonNegativeInteger("������ߵ�������");
                readEdges(m, adj_matrix, code_to_idx);

                std::string start_code;
                while (true) {
                    std::cout << "��������ʼ���ر�ţ�";
                    std::cin >> start_code;
                    std::cin.ignore(); // ������뻺�����Ļ��з�
                    if (code_to_idx.find(start_code) != code_to_idx.end()) {
                        start_idx = code_to_idx.find(start_code)->second;
                        break;
                    }
                    else {
                        std::cout << "�������ʼ���ر�Ų����ڣ����������롣" << std::endl;
                    }
                }
                printAdjacencyList(districts, adj_matrix);
                int totalCost;
                std::vector<Edge> result = prim(adj_matrix, start_idx, totalCost);
                if (!result.empty()) {
                    printResult(districts, result, totalCost);
                    saveHistory(districts, result, totalCost);
                }



                break;
            }
            case 2: {
                // ������С������·��
                if (start_idx == -1) {
                    std::cout << "�����������غͱ���Ϣ��" << std::endl;
                }
                else {
                    printAdjacencyList(districts, adj_matrix);
                    int totalCost;
                    std::vector<Edge> result = prim(adj_matrix, start_idx, totalCost);
                    if (!result.empty()) {
                        printResult(districts, result, totalCost);
                        saveHistory(districts, result, totalCost);
                    }
                }
                break;
            }
            case 3: {
                // �鿴��ʷ��¼
                viewHistory();
                break;
            }
            case 4: {
                // �˳�
                return 0;
            }
            default:
                std::cout << "��Ч��ѡ�����������롣" << std::endl;
            }
        }
    }

    return 0;
}
