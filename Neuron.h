#pragma once
#include<thread>

#include<afxwin.h>
#include<vector>
#include<stack>
using namespace std;

#define PERFECT_SCORE 27.
#define DRAW_SCORE 9
#define MS_INCRBAR WM_USER + 5

#define ROTATELEFT		0
#define ROTATERIGHT		1
#define REFLECTH		2
#define REFLECTV		3
struct neuron
{
	double out = 0;

	vector<double> w;
	double fi = 0;

	neuron() {}
	neuron(int size);
};


class NW
{
private:
	vector<vector<neuron>>Layer;
	vector<int>LayerSize;

	vector<int>ls;
	const double MutationVal = 0.5;
protected:
	inline void ProcessNeuron(neuron& n, vector<double>& in, int len);
	inline double f(double x);
	
	template<typename T> inline void RotateLeft(vector<T>& data);
	template<typename T> inline void RotateRight(vector<T>& data);
	template<typename T> inline void ReflectV(vector<T>& data);
	template<typename T> inline void ReflectH(vector<T>& data);
	template<typename T> inline void Swap(vector<T>& data, int i, int j);
public:
	NW::NW(vector<int>& LS, int Enters);
	NW& NW::operator =(const NW& nw);
	template<typename T>
	inline double Calc(vector<T>& in);
	inline void NW::MakeChild(NW& left, NW& right);
	inline void NW::Mutate();
	inline int MakePredictions(vector<int>& in, int turn);
	inline bool TryToFind(vector<int>& data, stack<int>& actions);
	inline void RestoreData(vector<int>& data, stack<int>& actions);
	inline void RestoreMax(vector<double>& data, stack<int>actions);
	double score;
	double avscore;
	bool killme = false;
	int wins = 0;
};


const double MutationVaraity = 0.5;
const double KillVaraity = 0.8;

typedef  NW* link;
class Trainer
{
private:
	vector<NW>P1;
	vector<NW>P2;
	static const int _size = 30;
	static const int _gamesvsrand = 10;
	static const int _ForbidToStop = 5;
	vector<int>LS = { 12,3, 1};
	int ForbidCtr;
protected:
	void Trainer::scoreVSrandom(NW& p, int turn);
	thread* makethread(int left, int right);
	void Trainer::scorefirst(NW& p1, NW& p2);
	void Trainer::scoresecond(NW& p1, NW& p2);

public:
	int TrainLimit = 100000;
	Trainer();
	HWND parent;
	void GetBest(link& p1, link& p2);
	inline void score(NW& p1, NW& p2);
	void train();
	bool stop = false;
};

#define DRAW 1000

inline int FindWinner(std::vector<int>& data, int zeros);
inline int Max(std::vector<double>& p);


struct TrainerInfo
{
	double av1 = 0;
	double av2 = 0;
	int win1 = 0;
	int win2 = 0;
};


class MyTree
{
private:

	struct Node
	{
		int val = 0;
		Node* next[3] = { nullptr,nullptr,nullptr };
	};

	Node start;
	UINT64 count = 0;
protected:

public:
	void Add(vector<int>& data);
	bool Seek(vector<int>& data);

};