#pragma once
enum BonusYakuState {
	Normal,//��ͨ
	FirstTurn,//��غ͡�W��
	LastTurn,//�ӵס�����
};
enum AgariWays {
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

enum GameState :bool
{
	GAME_STATE_NORMAL,
	GAME_STATE_WAITING_FOR_MINGPAI
};

enum class AgariFaildReason {
	Null,//��
	ShapeWrong,//�����������
	NoYaku,//����
	NotEnoughHan,//����
	AgariTypeWrong //�ǹ�ʿ��˫����������
};