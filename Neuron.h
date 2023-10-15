#pragma once

#include<afxwin.h>
#include<vector>
using namespace std;

#define PERFECT_SCORE 10.
#define MS_INCRBAR WM_USER + 5
struct neuron
{
	vector<double> in;
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

protected:
	inline void ProcessNeuron(neuron& n, vector<double>& in, int len);
	inline double f(double x);
public:
	NW::NW(vector<int>& LS, int Enters);

	template<typename T>
	inline double Calc(vector<T>& in);
	inline void NW::MakeChild(NW& left, NW& right);
	inline void NW::Mutate();
	inline int MakePredictions(vector<int>& in, int turn);

	double score;
	double avscore;
	bool killme = false;
};



typedef  NW* link;
class Trainer
{
private:
	vector<NW>P1;
	vector<NW>P2;
	static const int _size = 30;
	vector<int>LS = { 9,14,14,1 };


protected:
public:
	int TrainLimit = 1000;
	Trainer();
	HWND parent;
	void GetBest(link& p1, link& p2);
	inline void score(NW& p1, NW& p2);
	void train();
};

#define DRAW 1000

inline int FindWinner(std::vector<int>& data, int zeros);
inline int Max(std::vector<double>& p);
