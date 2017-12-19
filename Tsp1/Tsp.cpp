#include <iostream>
#include <vector>
#include <ctime>
#include <iomanip>
#include <list>
using namespace std;

const int N = 50;//种群个体数
const int number = 8;//城市数目
const int farest = 50;//最远距离
int distances[number][number];//城市距离矩阵,设定城市距离在50以内
const float pc = 1.0;//交叉率
const float pm = 0.05;//变异率

class individual {
public:
	int sequence[number];//城市排列
	float fitness;//适应度,跟书上区别一下，此处不采用距离的倒数，采用距离，适应度越小越好

	individual();
	void calcuf();
	void mutant();
};

class routines {
public:
	individual group[N];//种群个数为N
	individual best;//记录最短路径
	int best_gen;//记录最短路径的在第几代
	float sums;//总的适配之和

	routines();
	void cross(individual &father, individual &mother);
	~routines() {}
	individual getOne();
	void work();//种群进化
	void calcuAll()//计算所有的适应度
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

	int a[number];//交叉的两块
	int b[number];

	for (int i = 0; i < number; i++)
	{
		a[i] = father.sequence[i];
		b[i] = mother.sequence[i];
	}

	for (int i = l; i <= r; i++)//初步交换，此时还有冲突
	{
		a[i] = mother.sequence[i];
		b[i] = father.sequence[i];
	}

	bool flags1[number] = {};//冲突处理记录
	bool flags2[number] = {};

	for (int i = 0; i < number; i++)
	{
		if (i == l) {
			i = r;
			continue;
		}
		for (int j = l; j <= r; j++)
		{
			if ((!flags1[j]) && a[i] == a[j])//是否有冲突
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
	cout << "选择错误" << endl;
	return individual();
}

void routines::work()//种群进化一代
{
	individual father, mother;
	individual children[N];
	int n = 0;//记录当前子代的个数
	while (n != N) {
		father = getOne();
		mother = getOne();
		int r = (rand() % 101) / 101;//随机交叉概率
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
	cout << "设置每次的最大循环次数：" << endl;
	int maxtimes;
	cin >> maxtimes;
	cout << "设置理想最小距离" << endl;
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
	for (int i = 0; i < 10; i++)//一共计算十次
	{
		routines r = routines();
		for (int j = 0; j < maxtimes; j++)
		{
			r.work();
			if (r.best.fitness < float(1 / ideal))
				break;
		}
		cout << "第 " << i << "次求解:   ";
		cout << "解序列为: " << endl;
		for (int k = 0; k < number; k++)
		{
			cout << r.best.sequence[k] << " ";
		}
		cout << "最短距离为：" << int(1 / r.best.fitness)<<endl;
	}
	system("pause");
}