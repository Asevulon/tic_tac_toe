#pragma once


#include<vector>
using namespace std;

#define PERFECT_SCORE 10

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
	void ProcessNeuron(neuron& n, vector<double>& in);
	inline double f(double x);
public:
	NW::NW(vector<int>& LS, int Enters);

	int NW::Calc(vector<int>& in);
	int Calc(vector<double>& in);
	NW NW::MakeChild(NW& nw);
	void NW::Mutate();

	double score;
};



class Trainer
{
private:
	static const int _size;
	vector<NW>P1;
	vector<NW>P2;


protected:

public:
	Trainer();

	inline void score(NW& p1, NW& p2);
};