#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <climits>
#include <fstream>
#include<conio.h>

const int INF = INT_MAX;

// 边的结构体
struct Edge {
    int u;
    int v;
    int weight;  // 权重
    int cost;    // 建设成本
    int priority; // 优先级，数值越大，优先级越高
};

// 确保输入为正整数
int getPositiveInteger(const std::string& prompt) {
    int num;
    while (true) {
        std::cout << prompt;
        if (std::cin >> num && num > 0) {
            std::cin.ignore(); // 清除输入缓冲区的换行符
            return num;
        }
        else {
            std::cout << "输入应为正整数，请重新输入。" << std::endl;
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n');
        }
    }
}

// 确保输入为非负整数
int getNonNegativeInteger(const std::string& prompt) {
    int num;
    while (true) {
        std::cout << prompt;
        if (std::cin >> num && num >= 0) {
            std::cin.ignore(); // 清除输入缓冲区的换行符
            return num;
        }
        else {
            std::cout << "输入应为非负整数，请重新输入。" << std::endl;
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n');
        }
    }
}

// 读取区县信息
std::vector<std::pair<std::string, std::string> > readDistricts(int n, std::map<std::string, int>& code_to_idx) {
    std::vector<std::pair<std::string, std::string> > districts(n);
    std::cout << "请输入区县编号和名称，用空格分隔\n";
    for (int i = 0; i < n; ++i) {
        std::string line;
        std::getline(std::cin, line);
        std::istringstream iss(line);
        std::string code, name;
        if (iss >> code) {
            std::getline(iss, name); // 读取剩余部分作为名称
            name = name.substr(1); // 去除前面的空格
            districts[i] = std::make_pair(code, name);
            code_to_idx[code] = i;
        }
        else {
            std::cout << "输入的区县信息格式错误，请重新输入区县信息。" << std::endl;
            --i;
        }
    }
    return districts;
}

// 读取边信息
void readEdges(int m, std::vector<std::vector<Edge> >& adj_matrix, const std::map<std::string, int>& code_to_idx) {
    std::cout << "请输入一条边的两个区县编号，这条边的权重，建设成本，优先级（用空格分隔）\n";
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
                std::cout << "输入的区县编号不存在，请重新输入边信息。" << std::endl;
                --i;
            }
        }
        else {
            std::cout << "输入的边信息格式错误，请重新输入边信息。" << std::endl;
            --i;
        }
    }
}

// 打印邻接表
void printAdjacencyList(const std::vector<std::pair<std::string, std::string> >& districts, const std::vector<std::vector<Edge> >& adj_matrix) {
    std::cout << "\n生成的邻接表如下" << std::endl;
    int n = districts.size();
    for (int i = 0; i < n; ++i) {
        std::cout << districts[i].second << "(" << districts[i].first << ") -> ";
        bool first = true;
        for (int j = 0; j < n; ++j) {
            if (i != j && adj_matrix[i][j].weight != INF) {
                if (!first) std::cout << " -> ";
                std::cout << districts[j].second << "(" << districts[j].first << ", "
                    << "权重: " << adj_matrix[i][j].weight << ", "
                    << "成本: " << adj_matrix[i][j].cost << ", "
                    << "优先级: " << adj_matrix[i][j].priority << ")";
                first = false;
            }
        }
        std::cout << std::endl;
    }
}

// Prim算法
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
        // 寻找最小key且同时考虑优先级
        int u = -1;
        int min_key = INF;
        for (int i = 0; i < n; ++i) {
            if (!selected[i] && key[i] < min_key) {
                min_key = key[i];
                u = i;
            }
        }

        if (u == -1) break; // 图不连通
        selected[u] = true;

        // 更新邻接节点
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
        std::cout << "图不连通，无法生成最小生成树。" << std::endl;
        return std::vector<Edge>();
    }

    // 收集结果
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

// 打印结果
void printResult(const std::vector<std::pair<std::string, std::string> >& districts, const std::vector<Edge>& result, int totalCost) {
    std::cout << "\n应修建的最小生成树路径如下：" << std::endl;
    for (std::vector<Edge>::const_iterator it = result.begin(); it != result.end(); ++it) {
        std::cout << districts[it->u].second << "(" << districts[it->u].first << ") - "
            << districts[it->v].second << "(" << districts[it->v].first << ") "
            << "权重：" << it->weight << ", 成本：" << it->cost << ", 优先级：" << it->priority << std::endl;
    }
    std::cout << "总建设成本为：" << totalCost << std::endl;
}

// 保存历史记录
void saveHistory(const std::vector<std::pair<std::string, std::string> >& districts, const std::vector<Edge>& result, int totalCost) {
    std::ofstream historyFile("history.txt", std::ios::app);
    if (historyFile.is_open()) {
        historyFile << "应修建的最小生成树路径如下：" << std::endl;
        for (std::vector<Edge>::const_iterator it = result.begin(); it != result.end(); ++it) {
            historyFile << districts[it->u].second << "(" << districts[it->u].first << ") - "
                << districts[it->v].second << "(" << districts[it->v].first << ") "
                << "权重：" << it->weight << ", 成本：" << it->cost << ", 优先级：" << it->priority << std::endl;
        }
        historyFile << "总建设成本为：" << totalCost << std::endl;
        historyFile << "------------------------" << std::endl;
        historyFile.close();
    }
    else {
        std::cout << "无法打开历史记录文件。" << std::endl;
    }
}

// 查看历史记录
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
        std::cout << "没有历史记录，无法打开历史记录文件。" << std::endl;
    }
}

// 显示菜单
void showMenu() {
    std::cout << "\n\n\n";
    std::cout << "\t\t\t\t    ******************************************************\n";
    std::cout << "\t\t\t\t    *----------------------菜单------------------------*\n";
    std::cout << "\t\t\t\t    *----------------------------------------------------*\n";
    std::cout << "\t\t\t\t    *----------------1. 输入新的区县和边信息-----------*\n";
    std::cout << "\t\t\t\t    *----------------------------------------------------*\n";
    std::cout << "\t\t\t\t    *----------------2. 计算最小生成树路径-------------*\n";
    std::cout << "\t\t\t\t    *----------------------------------------------------*\n";
    std::cout << "\t\t\t\t    *----------------3. 查看历史记录-------------------*\n";
    std::cout << "\t\t\t\t    *----------------------------------------------------*\n";
    std::cout << "\t\t\t\t    *----------------4. 退出---------------------------*\n";
    std::cout << "\t\t\t\t    *----------------------------------------------------*\n";
    std::cout << "\t\t\t\t    ******************************************************\n";
    std::cout << "请输入你的选择：";
}

int main() {
    // 设置控制台窗口大小
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
            else std::cout << "输入无效，请输入1 - 4之间的整数。" << std::endl;

            
        }
        if (choice >= 49 && choice <= 52) {
            switch (choice-48) {
            case 1: {
                // 输入新的区县和边信息
         
                int n = getPositiveInteger("请输入区县的数量：");
                districts = readDistricts(n, code_to_idx);

                // 初始化邻接矩阵
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

                int m = getNonNegativeInteger("请输入边的数量：");
                readEdges(m, adj_matrix, code_to_idx);

                std::string start_code;
                while (true) {
                    std::cout << "请输入起始区县编号：";
                    std::cin >> start_code;
                    std::cin.ignore(); // 清除输入缓冲区的换行符
                    if (code_to_idx.find(start_code) != code_to_idx.end()) {
                        start_idx = code_to_idx.find(start_code)->second;
                        break;
                    }
                    else {
                        std::cout << "输入的起始区县编号不存在，请重新输入。" << std::endl;
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
                // 计算最小生成树路径
                if (start_idx == -1) {
                    std::cout << "请先输入区县和边信息。" << std::endl;
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
                // 查看历史记录
                viewHistory();
                break;
            }
            case 4: {
                // 退出
                return 0;
            }
            default:
                std::cout << "无效的选择，请重新输入。" << std::endl;
            }
        }
    }

    return 0;
}
