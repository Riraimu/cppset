#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <array>
#include <memory>
#include <map>

using namespace std;

//定数-------------------------------------------------
namespace GAME
{
	constexpr int BOARD_SIZE { 3 };
	constexpr int ERROR_DATA { 999 };
	constexpr bool GAME_LOOP { true };
}

namespace FRAME
{
	constexpr int NONE		{ 0 };
	constexpr int CIRCLE	{ 1 };
	constexpr int CROSS		{ 2 };
}

//型---------------------------------------------------
namespace TYPE
{
	template <typename T>
	using LINE = array<T, GAME::BOARD_SIZE>;
	using MAP = array < LINE<int>, GAME::BOARD_SIZE>;

	template <typename T>
	using VEC_LAN = map<int, T>;

	template <typename T>
	using UN_VEC_LAN = unique_ptr<VEC_LAN<T>>;
}

//クラス：位置------------------------------------------
class CPOS
{
	public:
		int x;
		int y;
};
using CVEC = CPOS;

//クラス：ゲームシステム--------------------------------
template <typename T>
class CGame
{
public:
	CGame(T* board);
	bool Input(int stone);
private:
	T * m_board;
	int m_in_num;
};

//クラス：ボード情報用-----
class CBoard
{
public:
	CBoard();
	bool MakeNoPos(int id, int&x, int& y);
	bool CheckIfAreaOutside(int x, int y);
	void SetStone(int x, int y, int id);
	void Draw();
	//int  GetStoneCount(int id) { return m_stone[id]; }
private:
	inline void COUNT_DRAW(int x) { cout << "[" << setw(2) << x << "]"; }
	TYPE::MAP m_board;
};

//ボード作成
CBoard::CBoard()
{
	//ボードの初期化
	int board[GAME::BOARD_SIZE][GAME::BOARD_SIZE]
	{
		{ 0,0,0 },
		{ 0,0,0 },
		{ 0,0,0 },
	};
	memcpy(&m_board, board, sizeof(board));
}

//ボードの領域を超えているかどうか
bool CBoard::CheckIfAreaOutside(int x, int y)
{
	if (x >= GAME::BOARD_SIZE)	return true;
	if (y >= GAME::BOARD_SIZE)	return true;
	if (x < 0)					return true;
	if (y < 0)					return true;
	return false;
}

//IDから配列の要素番号を求める
bool CBoard::MakeNoPos(int id, int&x, int& y)
{
	x = id % GAME::BOARD_SIZE;
	y = id / GAME::BOARD_SIZE;

	if (CheckIfAreaOutside(x, y) == true)
	{
		x = GAME::ERROR_DATA;
		y = GAME::ERROR_DATA;
		return false;
	}

	return true;
}

//設置
void CBoard::SetStone(int x, int y, int id)
{
	if (CheckIfAreaOutside(x, y) == false)
	{
		m_board[y][x] = id;
	}
}

//描画
void CBoard::Draw()
{
	int count = 0;

	system("cls");
	for (auto &y : m_board)
	{
		for (int &x : y)
		{
			switch (x)
			{
				case FRAME::NONE: COUNT_DRAW(count); break;
				case FRAME::CIRCLE: cout << "[〇]"; break;
				case FRAME::CROSS: cout << "[×]"; break;
				default: break;
			}
			count++;
		}
		cout << "\n";
	}
}

//ゲームシステムメソッド群
template <typename T = CBoard>
CGame<T>::CGame(T* board)
{
	m_board = board;
}

//プレイヤーの入力
template <typename T = CBoard>
bool CGame<T>::Input(int stone)
{
	int x, y;
	bool is_outside = false;
	
	while (is_outside == false)
	{
		//入力にミスがある場合はループ
		if (stone == FRAME::CIRCLE) cout << "\n[〇]";
		if (stone == FRAME::CROSS)	cout << "\n[×]";

		cout << "駒を置く位置を番号で入力\n";
		cin >> m_in_num;

		is_outside = m_board->MakeNoPos(m_in_num, x, y);

		//ボードのクリアと再描画
		m_board->Draw();
	}

	return true;
}

//メイン-----------------------------------------
int main()
{
	unique_ptr<CBoard> Board(new CBoard());
	unique_ptr<CGame<CBoard>>Game(new CGame<CBoard>(Board.get()));

	while (GAME::GAME_LOOP)
	{
		bool w, b;
		Board->Draw();
		w = Game->Input(FRAME::CIRCLE);

		Board->Draw();
		b = Game->Input(FRAME::CROSS);

		if (w == false && b == false) break;
	}

	Game.release();
	Board.release();

	cin.clear();
	int a;
	cin >> a;
	return 0;
}