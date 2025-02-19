#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <random>
#include <algorithm>

using namespace std;

// 定义个体结构
struct Individual
{
    double a, b, c;
};

const int popsize = 90;
const int generations = 1000;
const double error_threshold = 1.63068;     //最低是1.63067

// 随机数引擎（random）
random_device rd;
mt19937 generator(rd());
uniform_real_distribution<> dist_real(-10.0, 10.0); // 扩大初始种群的范围

// 生成高斯噪声
inline double generateGaussianNoise()
{
    normal_distribution<> d(0, 0.5);
    return d(generator);
}

// 判断是否有概率被选中有机会迭代变异
inline bool judging1()
{
    uniform_int_distribution<> dist(0, 2);
    return dist(generator) == 0 ? true : false;
}

// 判断是否变异
inline bool judging2()
{
    uniform_int_distribution<> dist(0, 1);
    return dist(generator) == 0 ? false : true;
}

// 迭代变异函数
void mutate(Individual& individual)
{
    uniform_real_distribution<> dist(-0.01, 0.01); // 小的变异范围
    if (judging1())
    {
        if (judging2())
        {
            individual.a += generateGaussianNoise();
        }
    }
    if (judging1())
    {
        if (judging2())
        {
            individual.b += generateGaussianNoise();
        }
    }
    if (judging1())
    {
        if (judging2())
        {
            individual.c += generateGaussianNoise();
        }
    }
}

// 计算个体的均方误差
inline double calculateMSE(const Individual& individual, const double* x_values, const double* y_values, int sample_size)
{
    double sum_squared_error = 0.0;
    for (int i = 0; i < sample_size; ++i)
    {
        double y_predicted = individual.a * x_values[i] * x_values[i] + individual.b * x_values[i] + individual.c;
        sum_squared_error += (y_predicted - y_values[i]) * (y_predicted - y_values[i]);
    }
    return sum_squared_error / sample_size;
}

// 交叉函数（算数交叉）（线性插值）
void crossover(const Individual& parent1, const Individual& parent2, Individual& offspring1, Individual& offspring2)
{
    uniform_real_distribution<> alpha_dist(0.0, 1.0); // 使用均匀分布的alpha值
    double alpha = alpha_dist(generator);
    offspring1.a = alpha * parent1.a + (1 - alpha) * parent2.a;
    offspring1.b = alpha * parent1.b + (1 - alpha) * parent2.b;
    offspring1.c = alpha * parent1.c + (1 - alpha) * parent2.c;
    offspring2.a = (1 - alpha) * parent1.a + alpha * parent2.a;
    offspring2.b = (1 - alpha) * parent1.b + alpha * parent2.b;
    offspring2.c = (1 - alpha) * parent1.c + alpha * parent2.c;
}

// 非线性函数
double f(double x, double a, double b, double c)
{
    return a * x * x + b * x + c;
}

void readData(const string& filename, double* x_values, double* y_values, int& sample_size)
{
    ifstream data(filename);
    string line;
    sample_size = 0;
    while (getline(data, line)) {
        istringstream iss(line);
        double x, y;
        if (iss >> x >> y) {
            x_values[sample_size] = x;
            y_values[sample_size++] = y;
        }
    }
    data.close();
}

int main()
{
    int max_sample_size = 100; // 最大数据样本大小
    double* x_values = new double[max_sample_size];
    double* y_values = new double[max_sample_size];
    int sample_size = 0;

    // 从文件读取数据
    readData("sampledata3.txt", x_values, y_values, sample_size);

    // 初始化种群
    Individual population[popsize];
    for (int i = 0; i < popsize; i++)
    {
        population[i].a = dist_real(generator);
        population[i].b = dist_real(generator);
        population[i].c = dist_real(generator);
    }

    // 临时数组用于存储适应度和个体
    Individual fitness_population[popsize];
    double fitness_scores[popsize];

    for (int generation = 1; generation <= generations; ++generation)
    {
        // 计算每个个体的适应度（均方误差）
        for (int i = 0; i < popsize; i++)
        {
            fitness_scores[i] = calculateMSE(population[i], x_values, y_values, sample_size);
            fitness_population[i] = population[i];
        }

        // 按适应度升序排序（冒泡排序）
        for (int i = 0; i < popsize - 1; i++)
        {
            for (int j = 0; j < popsize - i - 1; j++)
            {
                if (fitness_scores[j] > fitness_scores[j + 1])
                {
                    swap(fitness_scores[j], fitness_scores[j + 1]);
                    swap(fitness_population[j], fitness_population[j + 1]);
                }
            }
        }

        // 打印当前最优解的适应度
        cout << "Generation " << generation << ": Best Fitness = " << fitness_scores[0] << endl;

        // 检查是否达到误差阈值
        if (fitness_scores[0] <= error_threshold)
        {
            cout << "在误差范围内找到解 " << generation << " generations." << endl;
            cout << "最优解： a = " << fitness_population[0].a
                << ", b = " << fitness_population[0].b
                << ", c = " << fitness_population[0].c << endl;
            delete[] x_values;
            delete[] y_values;
            return 0;
        }

        // 选择前 popsize*2/3 个个体作为父代，并保留精英个体
        int selected_size = popsize * 2 / 3;
        const int elite_count = 5; // 可以根据需要调整精英个体数量
        Individual elite_population[elite_count];
        for (int i = 0; i < elite_count; i++)
        {
            elite_population[i] = fitness_population[i];
        }

        // 交叉产生新的子代，并处理可能的未配对个体
        Individual new_population[popsize];
        int new_pop_index = 0;

        // 添加精英个体到新种群
        for (int i = 0; i < elite_count; i++)
        {
            new_population[new_pop_index++] = elite_population[i];
        }

        // 进行交叉
        for (int i = 0; i < selected_size - elite_count; i += 2)
        {
            int parent1_idx = i;
            int parent2_idx = (i + 1) % (selected_size - elite_count);
            Individual offspring1, offspring2;
            crossover(fitness_population[parent1_idx], fitness_population[parent2_idx], offspring1, offspring2);
            if (new_pop_index < popsize)
            {
                new_population[new_pop_index++] = offspring1;
            }
            if (new_pop_index < popsize)
            {
                new_population[new_pop_index++] = offspring2;
            }
        }

        // 如果新种群大小小于popsize，用随机生成的个体补足（这里也可以考虑用其他策略）
        while (new_pop_index < popsize)
        {
            Individual new_individual;
            new_individual.a = dist_real(generator);
            new_individual.b = dist_real(generator);
            new_individual.c = dist_real(generator);
            new_population[new_pop_index++] = new_individual;
        }

        // 对非精英个体进行变异
        for (int i = elite_count; i < popsize; i++)
        {
            mutate(new_population[i]);
        }

        // 更新种群
        for (int i = 0; i < popsize; i++)
        {
            population[i] = new_population[i];
        }
    }

    // 达到最大迭代次数，输出最优解
    cout << "达到最大迭代次数却未能在误差范围内找到解" << endl;
    cout << "最优解： a = " << fitness_population[0].a
        << ", b = " << fitness_population[0].b
        << ", c = " << fitness_population[0].c << endl;

    delete[] x_values;
    delete[] y_values;
    return 0;
}