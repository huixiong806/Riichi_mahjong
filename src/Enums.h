#pragma once
enum WindType {
	EAST,
	SOUTH,
	WEST,
	NORTH
};
enum class BonusYakuState {
	Normal,//��ͨ
	FirstTurn,//��غ͡�W��
	LastTurn,//�ӵס�����
};
enum class AgariWays {
	Tsumo,
	Ron,
	Chankan,
	ChanAnkan
};
enum class ActionType {
	Null,//��ָ��
	Skip,
	DiscardTile,//��������
	Chii,
	Pon,
	Kan,//��
	Ron,
	Riichi,
	Tsumo,
	Ryuukyoku,
	Peinuki,
};

enum class ErrorType {
	None,//�ɹ��ˣ�û��error
	NotYourTurn,
	AlreadyActed,
	ActionRejected,
	TileNotExist,//����ʱ������Ҫ��Ķ���
	CantRiichi,//������ֱ
	CannotChooseNull,//��׼����ָ��
};

enum class GameState :bool
{
	OneAct,
	WaitingForNaki
};

enum class AgariFaildReason {
	Null,//��
	ShapeWrong,//�����������
	NoYaku,//����
	NotEnoughHan,//����
	AgariTypeWrong //�ǹ�ʿ��˫����������
};