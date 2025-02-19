#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <random>
#include <algorithm>

using namespace std;

// �������ṹ
struct Individual
{
    double a, b, c;
};

const int popsize = 90;
const int generations = 1000;
const double error_threshold = 1.63068;     //�����1.63067

// ��������棨random��
random_device rd;
mt19937 generator(rd());
uniform_real_distribution<> dist_real(-10.0, 10.0); // �����ʼ��Ⱥ�ķ�Χ

// ���ɸ�˹����
inline double generateGaussianNoise()
{
    normal_distribution<> d(0, 0.5);
    return d(generator);
}

// �ж��Ƿ��и��ʱ�ѡ���л����������
inline bool judging1()
{
    uniform_int_distribution<> dist(0, 2);
    return dist(generator) == 0 ? true : false;
}

// �ж��Ƿ����
inline bool judging2()
{
    uniform_int_distribution<> dist(0, 1);
    return dist(generator) == 0 ? false : true;
}

// �������캯��
void mutate(Individual& individual)
{
    uniform_real_distribution<> dist(-0.01, 0.01); // С�ı��췶Χ
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

// �������ľ������
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

// ���溯�����������棩�����Բ�ֵ��
void crossover(const Individual& parent1, const Individual& parent2, Individual& offspring1, Individual& offspring2)
{
    uniform_real_distribution<> alpha_dist(0.0, 1.0); // ʹ�þ��ȷֲ���alphaֵ
    double alpha = alpha_dist(generator);
    offspring1.a = alpha * parent1.a + (1 - alpha) * parent2.a;
    offspring1.b = alpha * parent1.b + (1 - alpha) * parent2.b;
    offspring1.c = alpha * parent1.c + (1 - alpha) * parent2.c;
    offspring2.a = (1 - alpha) * parent1.a + alpha * parent2.a;
    offspring2.b = (1 - alpha) * parent1.b + alpha * parent2.b;
    offspring2.c = (1 - alpha) * parent1.c + alpha * parent2.c;
}

// �����Ժ���
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
    int max_sample_size = 100; // �������������С
    double* x_values = new double[max_sample_size];
    double* y_values = new double[max_sample_size];
    int sample_size = 0;

    // ���ļ���ȡ����
    readData("sampledata3.txt", x_values, y_values, sample_size);

    // ��ʼ����Ⱥ
    Individual population[popsize];
    for (int i = 0; i < popsize; i++)
    {
        population[i].a = dist_real(generator);
        population[i].b = dist_real(generator);
        population[i].c = dist_real(generator);
    }

    // ��ʱ�������ڴ洢��Ӧ�Ⱥ͸���
    Individual fitness_population[popsize];
    double fitness_scores[popsize];

    for (int generation = 1; generation <= generations; ++generation)
    {
        // ����ÿ���������Ӧ�ȣ�������
        for (int i = 0; i < popsize; i++)
        {
            fitness_scores[i] = calculateMSE(population[i], x_values, y_values, sample_size);
            fitness_population[i] = population[i];
        }

        // ����Ӧ����������ð������
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

        // ��ӡ��ǰ���Ž����Ӧ��
        cout << "Generation " << generation << ": Best Fitness = " << fitness_scores[0] << endl;

        // ����Ƿ�ﵽ�����ֵ
        if (fitness_scores[0] <= error_threshold)
        {
            cout << "����Χ���ҵ��� " << generation << " generations." << endl;
            cout << "���Ž⣺ a = " << fitness_population[0].a
                << ", b = " << fitness_population[0].b
                << ", c = " << fitness_population[0].c << endl;
            delete[] x_values;
            delete[] y_values;
            return 0;
        }

        // ѡ��ǰ popsize*2/3 ��������Ϊ��������������Ӣ����
        int selected_size = popsize * 2 / 3;
        const int elite_count = 5; // ���Ը�����Ҫ������Ӣ��������
        Individual elite_population[elite_count];
        for (int i = 0; i < elite_count; i++)
        {
            elite_population[i] = fitness_population[i];
        }

        // ��������µ��Ӵ�����������ܵ�δ��Ը���
        Individual new_population[popsize];
        int new_pop_index = 0;

        // ��Ӿ�Ӣ���嵽����Ⱥ
        for (int i = 0; i < elite_count; i++)
        {
            new_population[new_pop_index++] = elite_population[i];
        }

        // ���н���
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

        // �������Ⱥ��СС��popsize����������ɵĸ��岹�㣨����Ҳ���Կ������������ԣ�
        while (new_pop_index < popsize)
        {
            Individual new_individual;
            new_individual.a = dist_real(generator);
            new_individual.b = dist_real(generator);
            new_individual.c = dist_real(generator);
            new_population[new_pop_index++] = new_individual;
        }

        // �ԷǾ�Ӣ������б���
        for (int i = elite_count; i < popsize; i++)
        {
            mutate(new_population[i]);
        }

        // ������Ⱥ
        for (int i = 0; i < popsize; i++)
        {
            population[i] = new_population[i];
        }
    }

    // �ﵽ������������������Ž�
    cout << "�ﵽ����������ȴδ������Χ���ҵ���" << endl;
    cout << "���Ž⣺ a = " << fitness_population[0].a
        << ", b = " << fitness_population[0].b
        << ", c = " << fitness_population[0].c << endl;

    delete[] x_values;
    delete[] y_values;
    return 0;
}