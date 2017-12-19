#include <iostream>
#include <vector>
#include <ctime>
#include <iomanip>
#include <list>
using namespace std;

const int N = 50;//��Ⱥ������
const int number = 8;//������Ŀ
const int farest = 50;//��Զ����
int distances[number][number];//���о������,�趨���о�����50����
const float pc = 1.0;//������
const float pm = 0.05;//������

class individual {
public:
	int sequence[number];//��������
	float fitness;//��Ӧ��,����������һ�£��˴������þ���ĵ��������þ��룬��Ӧ��ԽСԽ��

	individual();
	void calcuf();
	void mutant();
};

class routines {
public:
	individual group[N];//��Ⱥ����ΪN
	individual best;//��¼���·��
	int best_gen;//��¼���·�����ڵڼ���
	float sums;//�ܵ�����֮��

	routines();
	void cross(individual &father, individual &mother);
	~routines() {}
	individual getOne();
	void work();//��Ⱥ����
	void calcuAll()//�������е���Ӧ��
	{
		float sum = 0.0;
		for (int i = 0; i < N; i++)
		{
			group[i].calcuf();
			sum += group[i].fitness;
		}
		sums = sum;
	}
};

individual::individual() {

}

void individual::calcuf()
{
	float sum = 0.0;
	for (int i = 0; i < number - 1; i++)
	{
		sum += distances[sequence[i]][sequence[i + 1]];
	}
	fitness = 1 / sum;
}

void individual::mutant()
{
	int po1 = rand() % number;
	int po2 = rand() % number;
	while (po1 == po2)
	{
		po2 = rand() % number;
	}
	swap(this->sequence[po1], this->sequence[po2]);
}

routines::routines() {
	best.fitness = 0;
	for (int i = 0; i < N; i++)
	{
		bool flags[number] = {};
		for (int j = 0; j < number; j++)
		{
			int citynum = rand() % number;
			while (flags[citynum])
			{
				citynum = rand() % number;
			}
			flags[citynum] = true;
			group[i].sequence[j] = citynum;
		}
	}
	calcuAll();
}



void routines::cross(individual & father, individual & mother)
{
	int l = rand() % number;
	int r = l + rand() % (number - l);

	int a[number];//���������
	int b[number];

	for (int i = 0; i < number; i++)
	{
		a[i] = father.sequence[i];
		b[i] = mother.sequence[i];
	}

	for (int i = l; i <= r; i++)//������������ʱ���г�ͻ
	{
		a[i] = mother.sequence[i];
		b[i] = father.sequence[i];
	}

	bool flags1[number] = {};//��ͻ�����¼
	bool flags2[number] = {};

	for (int i = 0; i < number; i++)
	{
		if (i == l) {
			i = r;
			continue;
		}
		for (int j = l; j <= r; j++)
		{
			if ((!flags1[j]) && a[i] == a[j])//�Ƿ��г�ͻ
			{
				a[i] = b[j];
				flags1[j] = true;
				continue;
			}
			if ((!flags2[j]) && b[i] == b[j])
			{
				b[i] = a[j];
				flags2[j] = true;
				continue;
			}
		}
	}
	memcpy(father.sequence, a, number * sizeof(int));
	memcpy(mother.sequence, b, number * sizeof(int));
}



individual routines::getOne()
{
	float temp = (rand() % 101) / 101 * sums;

	for (int i = 0; i < N; i++) {
		temp -= group[i].fitness;
		if (temp <= 0)
			return group[i];
	}
	cout << "ѡ�����" << endl;
	return individual();
}

void routines::work()//��Ⱥ����һ��
{
	individual father, mother;
	individual children[N];
	int n = 0;//��¼��ǰ�Ӵ��ĸ���
	while (n != N) {
		father = getOne();
		mother = getOne();
		int r = (rand() % 101) / 101;//����������
		if (r < pc) {
			cross(father, mother);
		}
		int t = (rand() % 101) / 101;
		if (t < pm)
		{
			father.mutant();
			mother.mutant();
		}
		memcpy(&children[n++], &father, sizeof(individual));
		memcpy(&children[n++], &mother, sizeof(individual));
	}
	memcpy(group, children, N * sizeof(individual));
	calcuAll();
	int pos = 0;
	for (int i = 0; i < N; i++) {
		if (best.fitness < group[i].fitness)
		{
			best.fitness = group[i].fitness;
			pos = i;
		}
	}
	memcpy(best.sequence, group[pos].sequence, number * sizeof(int));
}


int main() {
	srand((int)time(0));
	cout << "����ÿ�ε����ѭ��������" << endl;
	int maxtimes;
	cin >> maxtimes;
	cout << "����������С����" << endl;
	int ideal;
	cin >> ideal;
	for (int i = 0; i < number; i++)
		for (int j = i; j < number; j++) {
			distances[j][i] = (distances[i][j] = (i == j ? 0 : int(rand() % farest + 1)));
			/*=distances[i][j];*/
		}
	for (int i = 0; i < number; i++) {
		for (int j = 0; j < number; j++) {
			cout << setw(6) << distances[i][j];
		}
		cout << endl;
	}
	cout << "---------------------------------" << endl;
	for (int i = 0; i < 10; i++)//һ������ʮ��
	{
		routines r = routines();
		for (int j = 0; j < maxtimes; j++)
		{
			r.work();
			if (r.best.fitness < float(1 / ideal))
				break;
		}
		cout << "�� " << i << "�����:   ";
		cout << "������Ϊ: " << endl;
		for (int k = 0; k < number; k++)
		{
			cout << r.best.sequence[k] << " ";
		}
		cout << "��̾���Ϊ��" << int(1 / r.best.fitness)<<endl;
	}
	system("pause");
}