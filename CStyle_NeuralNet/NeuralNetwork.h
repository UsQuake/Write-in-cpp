#pragma once
#include "LayerSet.h"
using namespace std;
template <typename T3>
class neuralNetwork {
public:
	neuralNetwork(int NeuronCounts[], int NeuronCountsCount, T3 BiasGroup[], int BiasGroupCount, T3(*ActFuncMass[])(T3 x), int ActFuncCount, T3(*ActFuncGradMass[])(T3 x), int ActFuncGradientCount, T3 answer[], int AnswerCount, T3 input[], int InputCount, int layerCount, T3 _Learningconst) {
		InitHiddenLayers(layerCount, NeuronCounts);
		SetHiddenLayersNeuronCounts(NeuronCounts, NeuronCountsCount);
		InitInputLayer(input, InputCount);
		InitAnswerLayer(answer, AnswerCount);
		SetInputLayer(input);
		SetAnswerLayer(answer);
		SetOutputLayer();
		ConnectHiddenLayersAndIOLayers();
		SetBiasGroup(BiasGroup, BiasGroupCount);
		SetLearningConst(_Learningconst);
		SetActivationFunc(ActFuncMass, ActFuncCount);
		SetActivationFuncGradient(ActFuncGradMass, ActFuncGradientCount);
		InitInputsFromIn();
		InitWeighsFromIn();
	}
	~neuralNetwork() {
		TerminateHiddenLayers();
	}

	//For Initialization
	void InitInputsFromIn() {
		for (int i = 0; i < HiddenLayerCount; ++i) HiddenLayers[i].InitInputsFromIn();
		OutputLayer.InitInputsFromIn();
	}
	void SetInputsFromIn() {
		for (int i = 0; i < HiddenLayerCount; ++i) HiddenLayers[i].SetInputsFromIn();
		OutputLayer.SetInputsFromIn();
	}
	void InitHiddenLayers(int _layerCount, int NeuronCount[]) {
		HiddenLayerCount = _layerCount;
		HiddenLayers = new Layer<T3>[HiddenLayerCount];
		for (int i = 0; i < HiddenLayerCount; ++i) HiddenLayers[i].InitNeuron(NeuronCount[i]);
	}
	inline void TerminateHiddenLayers() { delete[] HiddenLayers; } //������ �Ҵ� ����

	void InitInputLayer(T3 input[], int count) {
		InputLayerNeuronCount = count;
		InputLayer = new Layer<T3>();
		InputLayer->InitNeuron(count);

	}
	inline void TerminateInputLayer() { delete InputLayer; }

	void InitAnswerLayer(T3 answer[], int count) {
		AnswerLayerNeuronCount = count;
		AnswerLayer = new Layer<T3>();
		AnswerLayer->InitNeuron(count);
	}
	inline void TerminateAnswerLayer() { delete AnswerLayer; }

	inline void SetInputLayer(T3 input[]) {
		InputLayer->pNextLayer = &HiddenLayers[0];
		for (int i = 0; i < InputLayerNeuronCount; ++i) {
			InputLayer->neurons[i].Output = input[i];
		}
	}
	inline void SetAnswerLayer(T3 answer[]) {
		AnswerLayer->pPrevLayer = &OutputLayer;
		for (int i = 0; i < AnswerLayerNeuronCount; ++i) {
			AnswerLayer->neurons[i].Output = answer[i];
		}
	}
	void SetOutputLayer() {
		OutputLayerNeuronCount = AnswerLayerNeuronCount; //���䰳�� == ����� ���� ����
		OutputLayer.InitNeuron(OutputLayerNeuronCount);
		OutputLayer.pPrevLayer = &HiddenLayers[HiddenLayerCount - 1];
		OutputLayer.pNextLayer = AnswerLayer;
		OutputLayer.SetIsOutputOrNot(true);
		OutputLayer.neurons = new Neuron<T3>[OutputLayerNeuronCount];
	}
	void SetHiddenLayersNeuronCounts(int NeuronCount[], int count) {
		if (HiddenLayerCount != count) {
			cout << "������ �迭 ������ ���̾� ���� �ٸ��ϴ�. " << endl;
			for (int i = 0; i < count; i++) {
				cout << NeuronCount[i] << endl;
			}
			return;
		}

		for (int i = 0; i < HiddenLayerCount; i++) {
			HiddenLayers[i].InitNeuron(NeuronCount[i]);
		}
	}
	void SetBiasGroup(T3 BiasGroup[], int count) {
		if (HiddenLayerCount + 1 != count) {
			cout << "bias ������ ���̾� ���� �ٸ��ϴ�. " << count << "��" << endl;
			for (int i = 0; i < count; i++) {
				cout << BiasGroup[i] << endl;
			}
			return;
		}
		cout << "bias ���� ���Դϴ�." << endl;
		for (int i = 0; i < HiddenLayerCount; i++) {
			HiddenLayers[i].SetBias(BiasGroup[i]);
		}
		OutputLayer.SetBias(BiasGroup[HiddenLayerCount]);
		cout << "bias ������ �Ϸ�Ǿ����ϴ�." << endl;
	}
	void SetLearningConst(T3 _learningConst) {
		cout << "���� ��� ���� ���Դϴ�." << endl;
		LearningConst = _learningConst;
		for (int i = 0; i < HiddenLayerCount; i++) {
			HiddenLayers[i].SetLearningConst(LearningConst);
		}
		OutputLayer.SetLearningConst(LearningConst);
		cout << "���� ��� ������ �Ϸ�Ǿ����ϴ�." << endl;
	}
	void SetActivationFunc(T3(*ActFuncMass[])(T3 x), int count) {
		if (HiddenLayerCount + 1 != count) {
			cout << "Ȱ��ȭ �Լ� ������ ���̾� ���� �ٸ��ϴ�. " << endl;
			return;
		}
		cout << "Ȱ��ȭ �Լ� ���� ���Դϴ�." << endl;
		for (int i = 0; i < HiddenLayerCount; i++) {
			HiddenLayers[i].SetActiveFunc(ActFuncMass[i]);
		}
		OutputLayer.SetActiveFunc(ActFuncMass[HiddenLayerCount]);
		cout << "Ȱ��ȭ �Լ� ������ �Ϸ�Ǿ����ϴ�." << endl;
	}
	void SetActivationFuncGradient(T3(*ActFuncGradMass[])(T3 x), int count) {
		if (HiddenLayerCount + 1 != count) {
			cout << "Ȱ��ȭ �Լ��� ���Լ� ������ ���̾� ���� �ٸ��ϴ�. " << endl;
			return;
		}
		cout << "Ȱ��ȭ �Լ��� ���Լ��� ���� ���Դϴ�." << endl;
		for (int i = 0; i < HiddenLayerCount; i++) {
			HiddenLayers[i].SetActiveFuncGradient(ActFuncGradMass[i]);
		}
		OutputLayer.SetActiveFuncGradient(ActFuncGradMass[HiddenLayerCount]);
		cout << "Ȱ��ȭ �Լ��� ���Լ��� ���� �Ϸ�Ǿ����ϴ�." << endl;
	}
	void ConnectHiddenLayersAndIOLayers() {
		Layer<T3>* pLayerConnector;
		for (int i = 0; i < HiddenLayerCount; i++) {
			pLayerConnector = &HiddenLayers[i];
			if (i == 0) {
				pLayerConnector->pPrevLayer = InputLayer;
				pLayerConnector->pNextLayer = &HiddenLayers[i + 1];
			}
			else if (i == HiddenLayerCount - 1) {
				pLayerConnector->pPrevLayer = &HiddenLayers[i - 1];
				pLayerConnector->pNextLayer = &OutputLayer;
			}
			else {
				pLayerConnector->pPrevLayer = &HiddenLayers[i - 1];
				pLayerConnector->pNextLayer = &HiddenLayers[i + 1];
			}

		}
	}
	void InitWeighsFromIn() {
		for (int i = 0; i < HiddenLayerCount; i++) {
			HiddenLayers[i].InitWeighsFromIn();
		}
		OutputLayer.InitWeighsFromIn();
	}
	void FeedForward() {
		for (int i = 0; i < HiddenLayerCount; i++) {
			HiddenLayers[i].SetInputsFromIn();
			HiddenLayers[i].FeedForward();
		}
		OutputLayer.SetInputsFromIn();
		OutputLayer.FeedForward();
	}
	void BackPropogation() {
		Layer<T3>* temp = &OutputLayer;
		while (temp->pPrevLayer != nullptr) {
			temp->BackPropogation();
			temp = temp->pPrevLayer;
		}

	}
	void PrintAllLayer() {
		Layer<T3>* _layer_iterator = InputLayer;
		for (int i = 0; i < HiddenLayerCount + 3; i++) {
			for (int k = 0; k < _layer_iterator->GetNeuronCount(); k++) {
				cout << _layer_iterator->neurons[k].Output << " ";
			}
			cout << endl;
			_layer_iterator = _layer_iterator->pNextLayer;
		}
		system("cls");
	}
private:
	Layer<T3>* HiddenLayers;
	Layer<T3>* InputLayer, * AnswerLayer, OutputLayer;
	int* HiddenLayerNeuronCounts;
	int HiddenLayerCount;
	int InputLayerNeuronCount, OutputLayerNeuronCount, AnswerLayerNeuronCount;

