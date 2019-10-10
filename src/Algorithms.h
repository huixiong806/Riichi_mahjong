#pragma once
#include "ResultInspector.hpp"
#include<set>
#include<map>
#include<cassert>
#include<queue>
#include<algorithm>
#include<cstring>
class Algorithms
{
private:
	//�ж��Ƿ�Ϊ��ʿ��˫������
	static bool guoshiWithoutYaku(const Single& target, const std::vector<Single>& handTile);
	//�ж��Ƿ�Ϊ�߶Ժ�����
	static bool qiduiWithoutYaku(const Single& target, const std::vector<Single>& handTile);
	//�ж��Ƿ�Ϊ��׼������,*************��bug�����ã���������getdistance�����������Ƿ�Ϊ-1*****************
	static bool agariSearchWithoutYaku(std::vector<int> pool);
	//�ж��Ƿ���ʽ����,��������
	static bool agariWithoutYaku(const Single& target, const std::vector<Single>& handTile);
	//��ͼ���ƣ��������ʿ��˫�ͺ��Ƶ���
	static TryToAgariResult guoshiwushuang(const AgariParameters& par);
	//��ͼ���ƣ��������׼�ͺ��Ƶ���������(��������)���,depth=0��ʾö��ȸͷ��dep=4Ϊ�����
	static TryToAgariResult agariSearch(const AgariParameters& par, int depth, std::vector<Single> remainTiles, std::vector<Group> mianzi);
	//��ͼ���ƣ��������߶��ͺ��Ƶ���
	static TryToAgariResult qidui(const AgariParameters& par);
	//�������������ȸͷ�����ӵ����ͣ��浽queue��
	static void constructTarget(int quetou, int mianzi, std::queue<int>& q,int shape,int target,bool z);
	//5����
	static const int pow5[10];
	//������Ŀ��ľ��룬�����ֱ�Ϊԭ��״��Ŀ��ȸͷ��*5+Ŀ��������(ȸͷ���ֻ����һ��) ԭ��״Ϊ9λ5���������ӵ͵��߱�ʾ����������1~9�м���
	static uint8_t distanceToTarget[1953125][10];
	//������Ŀ��ľ���(����)�������ֱ�Ϊԭ��״��Ŀ��ȸͷ��*5+Ŀ��������(ȸͷ���ֻ����һ��) ԭ��״Ϊ9λ5���������ӵ͵��߱�ʾ�����ж��������׷����м���
	static uint8_t distanceToTargetZ[78125][10];
	//��׼�������ж�
	static TryToAgariResult YakuCheckForStandard(const AgariParameters& par, std::vector<Group>& mianzi);
public:
	
	//��ͼ���ƣ����ص������Ľ��
	static TryToAgariResult agari(const AgariParameters par);
	/*type
	����=0
	�ٺ�=1
	����=2
	������=3
	*/
	//���ݺ��ƽ��������
	static AgariResult getScore(WindType selfWind, AgariResult inp);
	//0~8 ����,9~17 ����,18~26 ����,27~33 ���������׷���
	static std::vector<int> getPool(const std::vector<Single>& tiles);
	//�����ж��Ƿ���ʽ���ơ����������������������������������
	static std::vector<Single> tingpai(std::vector<Single> handTile);
	//shapeΪѹ��������Ͳ�����valueΪҪ��ȡ���������֡������Ե���ɫ�Ƶ�ѹ���ͽ��в�����
	static int getNumberCount(int shape, int value);
	static void setNumberCount(int& shape, int value, int count);
	static void addNumberCount(int& shape, int value, int count);
	//Ԥ�������㵥��ɫ��Ŀ��ľ���(����������ɾ����������)
	static void preprocessDistance();
	//����14�����Ƶı�׼��������(0Ϊһ������-1Ϊ����)
	static int getDistanceStandard(const std::vector<Single>& handTile);
	//����14�����Ƶ��߶���������(0Ϊһ������-1Ϊ����)
	static int getDistanceQidui(const std::vector<Single>& handTile);
	//��õ���ɫ������
	static int getDistanceSingle(int shape,int mianzi,int quetou,bool z);
}; 