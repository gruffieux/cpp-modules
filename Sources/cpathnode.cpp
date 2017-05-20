#include <cpathnode.h>

PathNode::PathNode()
{
	NodeID = ParentID = -1;
	FScore = GScore = HScore = 0;
	angle = -1;
}

PathNode::PathNode(PathNode &model)
{
	NodeID = model.GetNodeID();
	ParentID = model.GetParentID();
	FScore = model.GetFScore();
	GScore = model.GetGScore();
	HScore = model.GetHScore();
	angle = model.GetAngle();
	Zone = *model.GetZone();
}

bool PathNode::compare(Object *pObject)
{
	PathNode *pPathNode = dynamic_cast<PathNode*>(pObject);
	if (pPathNode) return NodeID == pPathNode->GetNodeID();
	return false;
}

int PathNode::GenerateGScore(int ParentID, int ParentGScore, int IDInterval)
{
	if ((NodeID == ParentID + IDInterval) | (NodeID == ParentID - IDInterval) | (NodeID == ParentID + 1) | (NodeID == ParentID - 1))
		return ParentGScore + 10;
	else
		return ParentGScore + 14;
}

void PathNode::GenerateHScore(int TargetID, int IDInterval)
{
	int i;
	float j, k;

	j = (float)(TargetID - NodeID);

	if (j < 0)
		i = -IDInterval;
	else
		i = IDInterval;

	j /= i;
	k = j * IDInterval;
	k += 0.9f;
	j += 0.9f;
	Zone.x = (int)j;
	Zone.y = Zone.x * IDInterval - (int)k;

	if (Zone.y < 0)
		Zone.y *= -1;

	HScore = (Zone.x + Zone.y) * 10;
}

void PathNode::GenerateAngle(int ParentID, int IDInterval)
{
	if (NodeID == ParentID + IDInterval)
		angle = 0;
	if (NodeID == ParentID - IDInterval)
		angle = PI;
	if (NodeID == ParentID + 1)
		angle = PI / 2;
	if (NodeID == ParentID - 1)
		angle = 3 * PI / 2;
	if (NodeID == ParentID + IDInterval + 1)
		angle = PI / 2 - PI / 4;
	if (NodeID == ParentID + IDInterval - 1)
		angle = PI * 2 - PI / 4;
	if (NodeID == ParentID - IDInterval + 1)
		angle = PI / 2 + PI / 4;
	if (NodeID == ParentID - IDInterval - 1)
		angle = PI + PI / 4;
}

void PathNode::GenerateTileCoord(int IDInterval)
{
	int temp;

	temp = HScore;

	GenerateHScore(0, IDInterval);

	HScore = temp;
}
