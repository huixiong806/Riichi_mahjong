#pragma once
#include<vector>
#include"Rules.h"
#include"Single.h"
#include"Group.h"
#include"Yaku.h"
struct AgariResult
{
	bool zimo;
	int fangchongID;//�����ID 
	int hupaiID;//������ID 
	int fan;//-1=������-2=����������-3=�����������Դ�����,13���ۼ�����
	int fu;
	int dora;
	int akadora;
	int uradora;
	int scoreAdd;//��������ֱ���ͱ������ļ���
	int scoreDecFangchong;//������Ч 
	int scoreDecZhuang;//�ٺ���Ч 
	int scoreDecXian;//�ٺ���Ч 
	//int nukidora;
	AgariResult()
	{
		zimo = fan = fu = dora = akadora = uradora = scoreAdd = scoreDecFangchong = scoreDecZhuang = scoreDecXian = 0;
	}
	std::vector<Yaku> yaku;
	bool operator <(const AgariResult& rhs)const
	{
		return scoreAdd < rhs.scoreAdd;
	}
	bool operator ==(const AgariResult& rhs)const
	{
		return scoreAdd == rhs.scoreAdd;
	}
};
enum class AgariFaildReason
{
	Null,//��
	ShapeWrong,//�����������
	NoYaku,//����
	NotEnoughFan,//����
	CantQiangAnGang//�ǹ�ʿ��˫����������
};
struct TryToAgariResult
{
	AgariResult result;
	bool success;
	AgariFaildReason reason;
	bool operator <(const TryToAgariResult& rhs)const
	{

		if (!success && !rhs.success)
			return reason < rhs.reason;
		if (success && !rhs.success)
			return false;
		if (!success && rhs.success)
			return true;
		return result < rhs.result;
	}
	TryToAgariResult()
	{
		success = false;
		result = AgariResult();
	}
	TryToAgariResult(AgariFaildReason reason_)
	{
		success = false;
		reason = reason_;
	}
	TryToAgariResult(const AgariResult& result_)
	{
		success = true;
		result = result_;
	}
};
/*type
	����=0
	�ٺ�=1
	����=2
	������=3
	state
	����=0
	��غ�=1
	�ӵ�/����=2
*/
struct AgariParameters
{
	WindType selfWind;
	WindType prevailingWind;
	int lizhiXunmu;
	bool yifa;
	int state;
	int type;
	Single target;
	std::vector<Single> handTile;
	std::vector<Group> groupTile;
	std::vector<Single> dora;
	std::vector<Single> ura;
	AgariParameters(
		WindType selfWind_,
		WindType prevailingWind_,
		int lizhiXunmu_,
		bool yifa_,
		int state_,
		int type_,
		const Single& target_,
		const std::vector<Single>& handTile_,
		const std::vector<Group>& groupTile_,
		const std::vector<Single>& dora_,
		const std::vector<Single>& ura_
	)
	{
		selfWind = selfWind_;
		prevailingWind = prevailingWind_;
		lizhiXunmu = lizhiXunmu_;
		yifa = yifa_;
		state = state_;
		type = type_;
		target = target_;
		handTile = handTile_;
		groupTile = groupTile_;
		dora = dora_;
		ura = ura_;
	}
};