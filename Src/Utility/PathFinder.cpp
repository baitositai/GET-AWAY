#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <climits>
#include "PathFinder.h"

PathFinder::PathFinder()
{
}

void PathFinder::SetMap(const std::vector<std::vector<int>>& map)
{
    //�}�b�v���R�s�[
    mapData_ = map;

	//�}�b�v�̃T�C�Y���擾
    int height = map.size();
    int width = map[0].size();

	//�����f�[�^�ƑO�̃f�[�^��������
    distData_.assign(height, std::vector<int>(width, INT_MAX));
    prevData_.assign(height, std::vector<Vector2>(width, { -1, -1 }));
}

bool PathFinder::FindPath(const Vector2& start, const Vector2& goal, std::vector<Vector2>& path)
{
    //�D��x�t���L���[��p��
    std::priority_queue<Node> pq;

    //�}�b�v�T�C�Y���擾
    int height = static_cast<int>(mapData_.size());
    if (height == 0) return false;  //�������Ȃ��ꍇ�I��
    int width = static_cast<int>(mapData_[0].size());

    //�����f�[�^�ƑO�̃f�[�^��������
    distData_.assign(height, std::vector<int>(width, INT_MAX));
    prevData_.assign(height, std::vector<Vector2>(width, { -1, -1 }));

    //�n�_�̋�����0�ɐݒ肵�A�L���[�ɒǉ�
    distData_[start.y][start.x] = 0;
    pq.push({ 0, start });

	//�T�����[�v
    while (!pq.empty()) 
    {
        Node current = pq.top(); pq.pop();

        //���݃m�[�h�̍��W���S�[���Ȃ�T���I��
        if (current.pos.x == goal.x && current.pos.y == goal.y) { break; }

        //�S���������ɒT��
        for (int i = 0; i < DIR_NUM; ++i) 
        {
            Vector2 newPos = { current.pos.x + DX[i], current.pos.y + DY[i] };

			//�͈͊O�`�F�b�N
            if (!IsInside(newPos.x, newPos.y)) { continue; }

            //�ǂ̏ꍇ�͒ʂ�Ȃ����߃X�L�b�v
            if (mapData_[newPos.y][newPos.x] == BLOCK) continue;
            
			//�V���ɒʂ�R�X�g�͌��݃m�[�h�̃R�X�g+1
            int newCost = distData_[current.pos.y][current.pos.x] + 1;

            //�����L�^����Ă���R�X�g��菬������΍X�V
            if (newCost < distData_[newPos.y][newPos.x])
            {
				distData_[newPos.y][newPos.x] = newCost;    //�ŒZ�������X�V
				prevData_[newPos.y][newPos.x] = { current.pos.x, current.pos.y }; //�O�̃m�[�h���X�V
				pq.push({ newCost, newPos }); //�L���[�ɒǉ�(���̒T�����Ƃ��Ēǉ�)
            }
        }
    }

    //�S�[���܂ł̋������X�V����Ă��Ȃ���Όo�H�Ȃ�
    if (distData_[goal.y][goal.x] == INT_MAX)
    {
        return false;   //�T�����s
    }

    //�o�H����
	//�S�[�����N�_�ɂ��āA�O�̃m�[�h��H���Ă���
    path.clear();
    Vector2 cur = goal;
    int loopCount = 0;
    while (cur.x != -1 && cur.y != -1) 
    {
        if (loopCount++ > MAX_PATH_LENGTH)
        {
            // �������[�v�h�~
            break;
        }

        path.push_back(cur);
        cur = prevData_[cur.y][cur.x];

        //// �������[�v�h�~�i�������g���w���Ă���j
        //if (Vector2::IsSameVector2(goal, cur)) break;

        //cur = goal;
    }

	//�o�H���t���ɂ���
    std::reverse(path.begin(), path.end());

    //�T���听��
    return true;
}

bool PathFinder::IsInside(int x, int y) const
{
	//�}�b�v�͈͓̔����`�F�b�N
    return y >= 0 && y < static_cast<int>(mapData_.size()) &&
        x >= 0 && x < static_cast<int>(mapData_[0].size());
}

bool PathFinder::RightHandSearch(const Vector2& start, const Vector2& goal, std::vector<Vector2>& pathOut)
{
    pathOut.clear();

    Vector2 pos = start;
    DIR dir = DIR::RIGHT; // ���������i�C�Ӂj

    static constexpr int MAX_STEPS = 1000;
    int step = 0;

    while (step++ < MAX_STEPS)
    {
        pathOut.push_back(pos);

        if (Vector2::IsSameVector2(pos, goal))
        {
            return true;
        }

        // �E������
        DIR rightDir = TurnRight(dir);
        Vector2 rightVec = DirToVector(rightDir);
        Vector2 rightPos = { pos.x + rightVec.x, pos.y + rightVec.y };

        // �O������
        Vector2 forwardVec = DirToVector(dir);
        Vector2 forwardPos = { pos.x + forwardVec.x, pos.y + forwardVec.y };

        // �E���ʂ��Ȃ�E�֋Ȃ���
        if (IsWalkable(rightPos))
        {
            dir = rightDir;
            pos = rightPos;
        }
        // �O���ʂ��Ȃ炻�̂܂ܐi��
        else if (IsWalkable(forwardPos))
        {
            pos = forwardPos;
        }
        // ����ȊO�͍��։��
        else 
        {
            dir = TurnLeft(dir);
        }
    }

    // �X�e�b�v������߁����[�g�����i�������[�v����j
    return false;
}

bool PathFinder::FindPathAStar(const Vector2& start, const Vector2& goal, std::vector<Vector2>& pathOut)
{
    pathOut.clear();

    const int height = static_cast<int>(mapData_.size());
    const int width = static_cast<int>(mapData_[0].size());

    // �ړ������i�㉺���E�j
    const std::vector<Vector2> directions = 
    {
        { 0, -1 },
        { 1, 0 }, 
        { 0, 1 }, 
        { -1, 0 }
    };

    // g(���݂܂ł̃R�X�g)
    std::vector<std::vector<int>> gScore(height, std::vector<int>(width, INT_MAX));
    gScore[start.y][start.x] = 0;

    // �O�̃m�[�h��ۑ�
    prevData_ = std::vector<std::vector<Vector2>>(height, std::vector<Vector2>(width, { -1, -1 }));

    // �D��x�t���L���[�i�ŏ��R�X�g���j
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openList;
    openList.push({ Heuristic(start, goal), start });

    while (!openList.empty())
    {
        Node current = openList.top();
        openList.pop();

        if (Vector2::IsSameVector2(current.pos, goal))
        {
            // �o�H����
            Vector2 cur = goal;
            while (cur.x != -1 && cur.y != -1)
            {
                pathOut.push_back(cur);
                cur = prevData_[cur.y][cur.x];
            }
            std::reverse(pathOut.begin(), pathOut.end());
            return true;
        }

        for (const Vector2& dir : directions)
        {
            Vector2 next = { current.pos.x + dir.x, current.pos.y + dir.y };

            // �͈͊O�`�F�b�N
            if (next.x < 0 || next.y < 0 || next.x >= width || next.y >= height)
            {
                continue;
            }

            // �ǃ`�F�b�N
            if (mapData_[next.y][next.x] != 0)
            {
                continue;
            }

            int tentativeG = gScore[current.pos.y][current.pos.x] + 1;

            if (tentativeG < gScore[next.y][next.x])
            {
                gScore[next.y][next.x] = tentativeG;
                int f = tentativeG + Heuristic(next, goal);
                openList.push({ f,next });
                prevData_[next.y][next.x] = current.pos;
            }
        }
    }

    return false; // �o�H�Ȃ�
}

bool PathFinder::IsWalkable(const Vector2& pos) const
{
    if (pos.x < 0 || pos.y < 0 || pos.y >= static_cast<int>(mapData_.size()) || pos.x >= static_cast<int>(mapData_[0].size()))
        return false;

    return mapData_[pos.y][pos.x] == 0; // 0 = �ʂ��A1 = ��
}

int PathFinder::Heuristic(const Vector2& a, const Vector2& b)
{
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}
