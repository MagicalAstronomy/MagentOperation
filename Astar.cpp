#include "Astar.h"

/*Point类的实现****************************/

Point::Point(int x, int y)
{
	this->x = x;
	this->y = y;
	this->F = 0;
	this->G = 0;
	this->H = 0;
	this->parent = nullptr;

}

void Point::updateG(Point* new_parent)
{
	this->parent = new_parent;
	//更新G和F的值
	this->calcG(this->parent);
	this->calcF();
}

void Point::setOpen(Point* parent)
{
	this->parent = parent;
}


void Point::calcF()
{
	this->F = this->G + this->H;
}

void Point::calcG(Point* temp_parent)
{
	int distance = (abs(this->getX() - temp_parent->getX()) + abs(this->getY() - temp_parent->getY())) == 1 ? COST_PARA : COST_DIA;
	this->G = distance + temp_parent->getG();

}


void Point::calcH_Manh(Point* endPoint)
{
	int block = abs(this->getX() - endPoint->getX()) + abs(this->getY() - endPoint->getY());
	this->H = block * COST_PARA;

}


int Point::getF()
{
	this->calcF();	//防止忘了计算F
	return this->F;
}

int Point::getG()
{
	return this->G;
}

int Point::getH()
{
	return this->H;
}

int Point::getX()
{
	return this->x;
}

int Point::getY()
{
	return this->y;
}


Point* Point::getParent()
{
	return this->parent;
}

/*Astar类的实现*************************************/

Astar::Astar(cv::Mat map, Point* start, Point* end,int expandPixels)
{
	this->map = map;
	this->start = start;
	this->end = end;
	this->preAiming = start;
	preAimLocate = 0;

	borderExpand(expandPixels);
}

std::list<Point*> Astar::findPath()
{

	

	/*在进入循环之前，对起始点进行操作*/
	//把起始点加入开启列表
	N = this->start;
	N->setOpen(nullptr);	//起始点没有父节点
	openList.push_back(N);
	//把与起始点相邻的可到达方格加入开启列表，起始点作为父节点
	for (int j = (N->getY() - 1); j < (N->getY() + 2); j++)		//二重循环，判断当前点八方的点，先X（内循环）再y（外循环）（x和y在二维vector中是反过来表示的）
	{
		for (int i = (N->getX() - 1); i < (N->getX() + 2); i++)
		{
			if (i == N->getX() && j == N->getY())
			{
				continue;
			}
			else
			{
				if (i == end->getX() && j == end->getY())	//如果直接找到目标点 则一步完成，跳转到路径复原部分
				{
					end->setOpen(N);
					openList.push_back(end);
					goto finish;
				}
				else if (!reachAble(N, i, j))	//如果此坐标点不可到达 则忽略
				{
					continue;
				}
				else
				{
					Point* temp = new Point(i, j);		//把此坐标点加入开启列表,并计算F,G,H
					temp->setOpen(N);
					temp->calcG(N);
					temp->calcH_Manh(end);
					temp->calcF();
					openList.push_back(temp);
				}
			}
		}
	}
	//把起始点加入关闭列表
	openList.remove(N);
	closeList.push_back(N);


	/*正式开始循环*/

	do
	{
		//在开启列表中找到F值最小的坐标N
		N = openList.front();
		for (auto i : openList)
		{
			if (N->getF() > i->getF())
			{
				N = i;
			}
		}

		//将N从开启列表取出，加入关闭列表
		openList.remove(N);
		closeList.push_back(N);

		for (int j = (N->getY() - 1); j < (N->getY() + 2); j++)		//判断N八方的点状态
		{
			for (int i = (N->getX() - 1); i < (N->getX() + 2); i++)
			{
				if (i == N->getX() && j == N->getY())
				{
					continue;
				}
				else
				{
					if (i == end->getX() && j == end->getY())	//如果找到目标点 放入开启列表，跳转到路径复原部分
					{
						end->setOpen(N);
						openList.push_back(end);
						goto finish;
					}
					else if (!reachAble(N, i, j) || isInList(closeList, i, j))	//如果此坐标点不可到达或在关闭列表里 则忽略
					{
						continue;
					}
					else if (!isInList(openList, i, j))	//如果此坐标点尚未在列表中，把父节点设为N，放入开启列表，计算F,G,H
					{
						Point* temp = new Point(i, j);
						temp->setOpen(N);
						temp->calcG(N);
						temp->calcH_Manh(end);
						temp->calcF();
						openList.push_back(temp);
					}
					else	//如果此坐标点在开启列表中
					{
						Point* origin = isInList(openList, i, j);
						Point* temp = new Point(i, j);
						temp->calcG(N);
						if (temp->getG() < origin->getG())	//如果新路径G更小则更新父节点，重新计算G和F
						{
							origin->setOpen(N);	//设置父节点
							origin->calcG(N);	//计算新G
							origin->calcF();	//计算新F
						}
					}

				}
			}
		}


	} while (!isInList(openList, end->getX(), end->getY()));		//当未在开启列表中找到目标点时 继续循环

finish:


	Point* result = end;
	while (result)
	{
		path.push_front(result);
		result = result->getParent();
	}

	return path;

}

bool Astar::reachAble(Point* N, int x, int y)
{
	if ((int)map.ptr<uchar>(x)[y] == 0)		//该坐标点是障碍物或墙壁
	{
		return false;
	}
	else if (abs(x - N->getX()) + abs(y - N->getY()) == 2 && ((int)map.ptr<uchar>(x)[N->getY()] == 0 || (int)map.ptr<uchar>(N->getX())[y] == 0))		//坐标被障碍物绊住时，需要坐标点在当前操作点对角线方向，且此对角线方向的两个正向中存在障碍物
	{
		return false;
	}
	else
	{
		return true;
	}
}

Point* Astar::isInList(std::list<Point*> list, int x, int y)
{
	for (auto i : list)
	{
		if (i->getX() == x && i->getY() == y)
		{
			return i;
		}
	}
	return nullptr;
}

void Astar::borderExpand(int expandPixels)
{
	if (expandPixels > 0)
	{
		cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(expandPixels, expandPixels));
		cv::erode(map, map,  element);
	}

}

Point* Astar::findPreAimingPoint(int len)
{
	int count = 0;
	for (auto i : path)
	{
		if (count == len + preAimLocate - 1)
		{
			preAiming = i;
			preAimLocate = count;
			break;
		}
		
		count++;
	}

	return preAiming;
}